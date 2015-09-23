/*****************************************************************
 * $Id: acc_bankimportcheckdialog.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_bankimportcheckdialog.h"

#include <cmath>
#include <QPrintDialog>
#include <QPrinter>
#include <QStringListModel>
#include <QTableWidgetItem>
#include "acc_bankimport.h"
#include "acc_bankparser.h"
#include "acc_banktransdoc.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_postgltransaction.h"
#include "acc_qachartmaster.h"
#include "acc_salesinvoicehtml.h"
#include "acc_selectallocndialog.h"
#include "acc_selectcustbranchdialog.h"
#include "acc_sqlcommonfunctions.h"
#include "db_actionfileopen.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "db_itemdelegate.h"
#include "rb_mdiwindow.h"
#include "rb_settings.h"
#include "rb_sqlrelationaldelegate.h"
#include "rb_variant.h"


/**
 * Constructor
 */
ACC_BankImportCheckDialog::ACC_BankImportCheckDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    pbPrevious->setEnabled(false);
    pbNext->setEnabled(false);
    pbImport->setEnabled(false);

    mBankImportList = NULL;
    mColCount = 0;
    mColorLightYellow.setNamedColor("lightyellow"); // rgb(255,255,224)
    mIsImportInProgress = false;
    mIsNumbersUpdated = false;
    mBankAccount = "";

    mChartMasterModel = NULL;
    mDocModel = NULL;
    mDocMapper = NULL;
    mItemModel = NULL;
    mItemMapper = NULL;
    mAllocDocModel = NULL; // selection document only

    mGlTransList = new RB_ObjectContainer("", NULL, "ACC_GlTransList",
                                          ACC_OBJECTFACTORY);

    mInitInProgress = false;
    mSaveInProgress = false;
    mValidationMessage = "";

    mBankGlAccId = "";
    mTransDocListRoot = NULL;

    mLastImportedNo = 0;
}

/**
 * Destructor
 */
ACC_BankImportCheckDialog::~ACC_BankImportCheckDialog() {
    // store selected gl number from cbLedgerAccount
    RB_SETTINGS->beginGroup(objectName());
    RB_SETTINGS->setValue("glno", cbLedgerAccount->currentIndex());
    RB_SETTINGS->endGroup();

    // non-shared models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    //    delete mAllocDocModel;
    //    delete mAllocModel;

    delete mChartMasterModel; // shared because of parent of document model
    delete mDocModel;  // OK if not used anywhere else
    delete mItemModel; // OK if not used anywhere else

    delete mBankImportList;
    // delete mTransDocListRoot; deleted with mDocModel
    delete mGlTransList;
    RB_DEBUG->print("ACC_BankImportCheckDialog::~ACC_BankImportCheckDialog() OK");
}

/**
 * Initialize widget and models
 * @param statList bank statement list
 */
void ACC_BankImportCheckDialog::init() {
    setWindowTitle(tr("Bank Import Check and Update[*]"));

    // Stack page 1
    if (!mBankImportList) {
        mBankImportList = new RB_ObjectContainer("", NULL, "ACC_BankImportList");
    }

    tw->setItemDelegate(new DB_ItemDelegate(this));
    connect(tw, SIGNAL(itemChanged(QTableWidgetItem*)),
            this, SLOT(slotUpdateStatementNumbers(QTableWidgetItem*)));

    // Stack page 2 initializes when button Next is clicked
    if (!mTransDocListRoot) {
        mTransDocListRoot = new RB_ObjectContainer("", NULL, "ACC_TransDocListRoot");
    }

    readSettings();
}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave(). Always does a select() for now.
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_BankImportCheckDialog::fileSave(bool /*withSelect*/) {
    // Just to make sure
    if (!isValidTransDoc()) {
        DB_DIALOGFACTORY->requestWarningDialog("Can not save the document,\n"
                                               "document or item amounts not valid.");
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    mSaveInProgress = true;
    beforeFileSave();

    // update transaction document and items including tax
    updateBeforeSave();

    // Add new GL transations, true means selected doc only, false here
    createNewGlTrans(false);

    // Create updated glSum with deleted and new transactions
    ACC_PostGlTransaction oper;
    oper.createSumList(mGlTransList);

    // Start transaction
    //
    ACC_MODELFACTORY->getDatabase().transaction();

    // Insert glTrans items
    bool success = mGlTransList->dbUpdateList(ACC_MODELFACTORY->getDatabase(),
                                                 RB2::ResolveOne);
    // Update glSum to database
    success = success ? oper.postSumList() : false;

    RB_ObjectContainer* transDocList = mTransDocListRoot->getContainer("ACC_TransDocList");
    success = success ? transDocList->dbUpdateList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveAll) : false;

    // Set last and highest used bank statement number in system type list
    ACC_SqlCommonFunctions f;
    success = success ? f.setLastTransNo(ACC2::TransBankCash, mBankGlAccId, mLastImportedNo) : false;

    // TODO: implement
//    success = success ? mHandleAllocn.submitAllAndSelect() : false;

    // Commit or rollback
    //
    if (success && ACC_MODELFACTORY->getDatabase().commit()) {
        setWindowModified(false);
    } else {
        ACC_MODELFACTORY->getDatabase().rollback();
        RB_DEBUG->error("ACC_BankImportCheckDialog::fileSave() transaction ERROR");
        RB_DEBUG->error(ACC_MODELFACTORY->getDatabase().lastError().text());
        fileRevert();
    }

    mSaveInProgress = false;

    // HACK: MySQL on MS Windows does not store the ACC_TransDoc's. I do not know why.
    bool allTransDocExisted = ensureTransDocExisted(transDocList);
    QApplication::restoreOverrideCursor();

    if (!allTransDocExisted) {
        ACC_DIALOGFACTORY->requestWarningDialog("ACC_TransDoc additional save.");
    }

    // TODO: remove, debug and temporary only:
    if (f.isGlTransOrphanExisting()) {
        ACC_DIALOGFACTORY->requestWarningDialog("Orphaned ACC_GlTrans rows,"
                                                "remember what you did\n and "
                                                "inform the development team.");
    }

    return true;
}

/**
 * Refert changes, do not save model: does nothing because in-memory model
 */
void ACC_BankImportCheckDialog::fileRevert() {
    // nothing
}

/**
 * Button open file clicked
 */
void ACC_BankImportCheckDialog::on_pbOpenFile_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mIsImportInProgress = true;
    mBankAccount = "";

    // Open file
    RB_String fileName;
    DB_ActionFileOpen a;
    a.getOpenFileName(fileName);

    if (fileName.isEmpty()) {
        leFileName->clear();
        QApplication::restoreOverrideCursor();
        mIsImportInProgress = false;
        return;
    }

    leFileName->setText(fileName);

    // Clear imported statement list if already and delete objects
    mBankImportList->erase();

    // Execute parsing
    ACC_BankParser* parser = new ACC_BankParser(fileName, mBankImportList);
    bool success = parser->exec();
    RB_String msg;
    parser->getMessage(msg);
    delete parser;

    if (!success) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr(msg.toStdString().c_str()));
        QApplication::restoreOverrideCursor();
        return;
    }

    // sort on date because some CSV files do not use ascending order
    mBankImportList->sort("transentrytimestamp", RB2::SortOrderAscending, RB2::MemberChar20);

    if (success) {
        fillTable(mBankImportList);
        QApplication::restoreOverrideCursor();
    } else {
        QApplication::restoreOverrideCursor();
        tw->clear();
        tw->setRowCount(0);
        tw->setColumnCount(0);
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Parse error, check settings."));
    }

    mIsImportInProgress = false;

    if (success && tw->rowCount() > 0) {
        pbNext->setEnabled(true);
    } else {
        pbNext->setEnabled(false);
    }
}

/**
 * Button select item account clicked
 */
void ACC_BankImportCheckDialog::on_ileItemAccount_clicked() {
    if (!tvItem->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectChartMaster);


    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();

        if (obj) {
            if (isDebtorCreditorAccount(obj)) {
                return; // do nothing
            }

            QModelIndex index = mItemModel->getProxyIndex();
            QModelIndex idx = mItemModel->index(
                        index.row(), mItemModel->fieldIndex("chartmaster_idx"));
            RB_String acctId = obj->getId();
            mItemModel->setData(idx, acctId
                            + obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString());

            int row = tvDocument->currentIndex().row();
            QModelIndex custSupIdIdx;
            if (mTransType == ACC2::TransDebtor) {
                custSupIdIdx = mDocModel->index(row, mDocModel->fieldIndex("debtor_idx"));
            } else if (mTransType == ACC2::TransCreditor) {
                custSupIdIdx = mDocModel->index(row, mDocModel->fieldIndex("creditor_idx"));
            }
            RB_String csId = mDocModel->data(custSupIdIdx, RB2::RoleOrigData).toString();

            ACC_SqlCommonFunctions oper;

            if (mTransType == ACC2::TransDebtor && !csId.isEmpty()) {
                oper.update("ACC_Customer", "lastusedacct_id", acctId, csId);
            } else if (mTransType == ACC2::TransCreditor && !csId.isEmpty()) {
                oper.update("ACC_Supplier", "lastusedacct_id", acctId, csId);
            }

            if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
                idx = mItemModel->index(row, mItemModel->fieldIndex("taxhighamt"));
                mItemModel->setData(idx, 0.0);
                idx = mItemModel->index(row, mItemModel->fieldIndex("taxlowamt"));
                mItemModel->setData(idx, 0.0);
                idx = mItemModel->index(row, mItemModel->fieldIndex("taxotheramt"));
                mItemModel->setData(idx, 0.0);
            }
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No account selected.\n"
                                                   "Data is unchanged."));
    }

    dlg->deleteLater();
//    slotDataChanged();
}

/**
 * Button allocation clicked, to allocate invoice with payments or journal entry
 */
void ACC_BankImportCheckDialog::on_ileAllocation_clicked() {
    if (!tvItem->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    // Open dialog to select transaction document customer (debtor) or supplier (creditor)
    ACC_SelectAllocnDialog* dlg = new ACC_SelectAllocnDialog(this);
    dlg->init();

    if (leItemAmount->text().toDouble() >= 0.0) {
        dlg->setFilterType(ACC2::AllocnCustomer);
    } else {
        dlg->setFilterType(ACC2::AllocnSupplier);
    }

    dlg->init();

    if (dlg->exec() != QDialog::Accepted) {
        dlg->deleteLater();
        return;
    }

    // Handle allocation between current item model and (alloc.) document model
    RB_ObjectBase* obj = dlg->getCurrentObject();
    if (!obj) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected,\n"
                                                   "allocation unchanged."));
        dlg->deleteLater();
        return;
    }
    RB_String docToId = obj->getId();
    RB_String docToDspl = obj->getValue("transno").toString(); //display value
    int transType = obj->getValue("doctype").toInt();

    // Reset to the receivable or payable GL account
    if (transType == ACC2::TransDebtor || transType == ACC2::TransSalesOrder) {
        mItemModel->setCurrentValue("chartmaster_idx",
                                    ACC_QACHARTMASTER->getAccRecId()
                                    + ACC_QACHARTMASTER->getAccRecName(),
                                    Qt::EditRole);
    } else if (transType == ACC2::TransCreditor || transType == ACC2::TransPurchOrder) {
        mItemModel->setCurrentValue("chartmaster_idx",
                                    ACC_QACHARTMASTER->getAccPayId()
                                    + ACC_QACHARTMASTER->getAccPayName(),
                                    Qt::EditRole);
    } else {
        RB_DEBUG->error("ACC_GlTransactionWidget::on_ileAllocation_clicked() "
                        "Document type ERROR");
        return;
    }

    mHandleAllocn.addAllocn(mItemModel, docToId, docToDspl,
                            mDocModel->getCurrentId());

    dlg->deleteLater();
//    slotDataChanged();
}

/**
 * Button clear/remove allocation clicked
 */
void ACC_BankImportCheckDialog::on_ileAllocation_clear() {
    if (!tvItem->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item is selected.\n"
                                                   "Allocation is unchanged."));
        return;
    }

    mHandleAllocn.delItemAllocn(mItemModel, false);
    mItemModel->setCurrentValue("chartmaster_idx", "0", Qt::EditRole);
    //    slotDataChanged();
}

/**
 * Button Help clicked
 */
void ACC_BankImportCheckDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button Previous clicked
 */
void ACC_BankImportCheckDialog::on_pbPrevious_clicked() {
    tvItem->setModel(NULL);
    tvDocument->setModel(NULL);
    delete mItemModel;
    mItemModel = NULL;
    delete mDocModel;
    mDocModel = NULL;

    pbPrevious->setEnabled(false);
    pbNext->setEnabled(true); // still valid data
    pbImport->setEnabled(false);
    stackedWidget->setCurrentIndex(0);
}

/**
 * Button Next clicked, (re-)initializes stack page 2
 */
void ACC_BankImportCheckDialog::on_pbNext_clicked() {
    // mBankGlAccId mBankGlAccName set during fillTable()

    if (mBankGlAccId.size() < 38) {
        ACC_DIALOGFACTORY->requestWarningDialog(
                    tr("Could not find the bank account for\n"
                       "the imported data. (Is the bank account\n"
                       "perhaps part of a different project?)"));
        return;
    }

    // Did the user update the statement numbers
    if (!mIsNumbersUpdated) {
        int res = ACC_DIALOGFACTORY->requestYesNoDialog(
                    tr("Statement Numbers Update"),
                    tr("Are the statement numbers up-to-date?\n"
                       "Yes continue, No abort."));
        if (res == QMessageBox::No) {
            return;
        }
    }

    ACC_SqlCommonFunctions f;

    // Numbers should start with last used number plus 1
    // Next number in sequence should be equal or one higher
    // The dates should be increasing in time
    bool isOk = true;

    int seqNoRunner = mLastTransNo;
    int currentSeqNo = 0;
    QDate lastTransDate = f.lastBankTransactionDate(mBankGlAccId); // account number with bank
    QDate dateRunner = lastTransDate;
    QDate currentDate = QDate();
    QDate fiscalYearEnd;
    int rowCount = tw->rowCount();

    for (int row = 0; row < rowCount && isOk; ++row) {
        QTableWidgetItem* item = tw->item(row, STATNUMBER - RB2::HIDDENCOLUMNS );
        currentSeqNo = item->data(Qt::DisplayRole).toInt();
        item = tw->item(row, TRANSENTRYTIMESTAMP - RB2::HIDDENCOLUMNS);
        currentDate = item->data(Qt::DisplayRole).toDate();


        if (row == 0) {
            // sequence number check
            if (seqNoRunner + 1 == currentSeqNo) {
                seqNoRunner = currentSeqNo;
            } else {
                isOk = false;
                continue;
            }

            // date check
            if (lastTransDate < currentDate) {
                dateRunner = currentDate;
                fiscalYearEnd = ACC_QACHARTMASTER->getFiscalYearEnd(currentDate);
            } else {
                isOk = false;
                continue;
            }
        } else {
            if (seqNoRunner == currentSeqNo || seqNoRunner + 1 == currentSeqNo) {
                seqNoRunner = currentSeqNo;
            } else {
                // not correct following number
                isOk = false;
                continue;
            }

            if ((dateRunner == currentDate || dateRunner < currentDate)
                    && currentDate <= fiscalYearEnd) {
                dateRunner = currentDate;
            } else {
                isOk = false;
                continue;
            }
        }
    }

    if (!isOk) {
        ACC_DIALOGFACTORY->requestWarningDialog(
                    tr("Could not import the bank statements, because:\n"
                       "- the initial bank statement is missing or\n"
                       "- the bank statement numbers are not in sequence or\n"
                       "- the numbers do not start with last statement number + 1 or\n"
                       "- the dates are already existing or beyond in the accounting or\n"
                       "- the dates are of two fiscal years or\n"
                       "- the dates are not chronological."));
        return;
    }

    mLastImportedNo = seqNoRunner;

    // Start preparation for check
    QApplication::setOverrideCursor(Qt::WaitCursor);

    bool success = prepareImportList(lastTransDate);

    if (!success) {
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestInformationDialog(tr("Nothing to import"));
        return;
    }

    // Create the bank statements (ACC_TransDocList), details and the GL transactions
    if (!prepareTransDoc()) {
        QApplication::restoreOverrideCursor();
        return; // error or user canceled
    }

    QApplication::restoreOverrideCursor();

    // Stack page 2 initializing
    mInitInProgress = true; // HACK: to prevent maybeSave() in endless loop
    RB_ObjectContainer* transDocList = mTransDocListRoot->getContainer("ACC_TransDocList");

    // Get bank GL account ID
    RB_ObjectIterator* docIter = transDocList->createIterator();
    docIter->first();
    RB_ObjectBase* transDoc = docIter->currentObject();
    mBankGlAccId = transDoc->getValue("parent").toString();
    delete docIter;
    docIter = NULL;

    // Set correct ACC_ChartMaster text, do not use model, store id
    RB_ObjectBase* bankObj = ACC_QACHARTMASTER->getAcctObj(mBankGlAccId);

    if (bankObj) {
        mBankGlAccName = bankObj->getValue("accountname").toString();
        cbLedgerAccount->addItem(mBankGlAccName);
        cbLedgerAccount->setEnabled(false);
        pbImport->setEnabled(true);
    } else {
        mBankGlAccName = tr("Unknown bank account");
        cbLedgerAccount->addItem(mBankGlAccName);
        cbLedgerAccount->setEnabled(false);
        // disable pushbutton 'Import'
        pbImport->setEnabled(false);
    }

    // To make sure when saving the data
    mTransDocListRoot->setId(mBankGlAccId);

    //
    // 1. Set model
    //
    mDocModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelImportTransDoc);
    mDocModel->setRoot(mTransDocListRoot);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mDocMapper = mDocModel->getMapper();
    mDocMapper->addMapping(deDate, mDocModel->fieldIndex("transdate"));
    mDocMapper->addMapping(leDocumentNo, mDocModel->fieldIndex("transno"));
    QIntValidator* intVal = new QIntValidator(this);
    intVal->setBottom(0);
    leDocumentNo->setValidator(intVal);
    mDocMapper->addMapping(leDescription, mDocModel->fieldIndex("description"));
    mDocMapper->addMapping(leTotalRec, mDocModel->fieldIndex("totalamountrec"));
    leTotalRec->setValidator(new QDoubleValidator(-9999999.99, 9999999.99, 2, this));
    mDocMapper->addMapping(leTotalPay, mDocModel->fieldIndex("totalamountpay"));
    leTotalPay->setValidator(new QDoubleValidator(-9999999.99, 9999999.99, 2, this));

    deDate->setDisplayFormat("yyyy-MM-dd");

    setFormatTableView(tvDocument, mDocModel);

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotTabChanged(int)));

    // Initialize the mDocModel, children models
    if (cbLedgerAccount->count() > 0) {
        setWindowModified(false);
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(
                tr("ERROR:\nthe chart of accounts is invalid,\nrefer to the manual."));
    }

    // Set basic validators for numeric lineEdits
    leItemAmount->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));

    int count = mDocModel->columnCount(QModelIndex());
    for (int i = 0; i < count; ++i) {
       if (i != mDocModel->fieldIndex("transdate")
               && i != mDocModel->fieldIndex("transno")
               && i != mDocModel->fieldIndex("description")
               && i != mDocModel->fieldIndex("totalamountrec")
               && i != mDocModel->fieldIndex("totalamountpay")) {
           tvDocument->hideColumn(i);
       } else {
           tvDocument->showColumn(i);

           if (tvDocument->columnWidth(i) < 5) {
               tvDocument->setColumnWidth(i, 100);
           }
       }
    }

    // Bank transaction item model
    mItemModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelImportBankTrans);
    mItemMapper = mItemModel->getMapper();
    mItemMapper->addMapping(leItemDescription, mItemModel->fieldIndex("description"));
    mItemMapper->addMapping(ileItemAccount,
                            mItemModel->fieldIndex("chartmaster_idx"));
    mItemMapper->addMapping(ileAllocation,
                            mItemModel->fieldIndex("transallocn_idx"));
    mItemMapper->addMapping(leItemAmount, mItemModel->fieldIndex("amount"));

    // Different from ACC_GlTransWidget::setItemModel()
    setFormatTableView(tvItem, mItemModel);

    for (int i = 0; i < mItemModel->columnCount(QModelIndex()); ++i) {
        if (i != mItemModel->fieldIndex("description")
            && i != mItemModel->fieldIndex("chartmaster_idx")
            && i != mItemModel->fieldIndex("amount")) {
            tvItem->hideColumn(i);
        } else {
            tvItem->showColumn(i);

            if (tvItem->columnWidth(i) < 5) {
                tvItem->setColumnWidth(i, 100);
            }
        }
    }

    // tvDocument->sortByColumn(-1); better (?): or tvDocument->undoFilterSort()
    tvDocument->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    mInitInProgress = false;

    pbPrevious->setEnabled(true);
    pbNext->setEnabled(false);
    // pbImport->setEnabled(true); already done above
    stackedWidget->setCurrentIndex(1);
}

/**
 * Button OK clicked
 */
void ACC_BankImportCheckDialog::on_pbImport_clicked() {
    fileSave(true);
    ACC_DIALOGFACTORY->emitBankStatementsImported();
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_BankImportCheckDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Is the user trying to book directly against the debtor or creditor
 * account, because this is not allowed
 * @param actObj account object (ACC_ChartMaster)
 * @return true if account object is of debtor or creditor GL account
 */
bool ACC_BankImportCheckDialog::isDebtorCreditorAccount(RB_ObjectBase* actObj) {
    if (!actObj) return false;

    int actControl = actObj->getValue("accountcontrol").toInt();

    if (actControl == (int) ACC2::ControlDebtor
            || actControl == (int) ACC2::ControlCreditor) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Cannot book against debtor \n"
                                                   "or creditor account directly."));
        return true;
    }

    return false;
}

/**
 * HACK
 */
bool ACC_BankImportCheckDialog::ensureTransDocExisted(RB_ObjectContainer *transDocList) {
    if (!transDocList) {
        RB_DEBUG->error("ACC_BankImportCheckDialog::ensureTransDocExisted() ERROR");
    }
    bool allExisted = true;
    ACC_SqlCommonFunctions f;
    RB_String strId = "";
    RB_String strResult = "";
    RB_ObjectIterator* iter = transDocList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* transDoc = iter->currentObject();
        strId = transDoc->getId();
        strResult = f.selectFromWhereId("name", "acc_transdoc", strId).toString();

        if (strResult.isEmpty()) {
            transDoc->delFlag(RB2::FlagFromDatabase);
            transDoc->dbUpdate(ACC_MODELFACTORY->getDatabase(), RB2::ResolveNone);
            allExisted = false;
        }
    }

    delete iter;
    return allExisted;
}

/**
 * Slot update statement number for the next rows, if not a higher integer
 */
void ACC_BankImportCheckDialog::slotUpdateStatementNumbers(QTableWidgetItem *currentItem) {
    if (mIsImportInProgress) {
        return;
    }

    bool success = false;
    int currentNumber = currentItem->data(Qt::DisplayRole).toInt(&success);

    if (!success) {
        return;
    }

    QTableWidgetItem* twItem;
    int number;
    int col = currentItem->column();

    for (int row = currentItem->row() + 1; row < tw->rowCount(); ++row) {
        twItem = tw->item(row, col);
        number = twItem->data(Qt::DisplayRole).toInt(&success);

        if (!success) { // text
            twItem->setData(Qt::DisplayRole, currentNumber);
        } else if (number < currentNumber) { // current is lower
            twItem->setData(Qt::DisplayRole, currentNumber);
        }
    }

    mIsNumbersUpdated = true;

}

/**
 * Slot tab changed
 * @param tabNo new selected tab number
 */
void ACC_BankImportCheckDialog::slotTabChanged(int tabNo) {
    if (tabNo == 1) {
        // update the table widget twGlTransactions
        updateGlTransWidget();
    }
}

/**
 * Fill table with parse bank statement data
 * @param importBankList list with data objects from bank statements
 */
void ACC_BankImportCheckDialog::fillTable(RB_ObjectBase* importBankList) {
    tw->clear();
    tw->setEditTriggers(QAbstractItemView::EditKeyPressed |
                        QAbstractItemView::DoubleClicked);
    tw->setSelectionMode(QAbstractItemView::ContiguousSelection);

    bool isHeaderSet = false;
    int row = 0;
    ACC_SqlCommonFunctions f;

    RB_ObjectContainer* list = dynamic_cast<RB_ObjectContainer*>(importBankList);
    RB_ObjectIterator* iter = list->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (!isHeaderSet) {
            // set bank account id and name
            mBankAccount = obj->getValue("statactnumber").toString();
            ACC_QACHARTMASTER->getBankGlAcctIdName(mBankGlAccId, mBankGlAccName, mBankAccount);

            // check whether imported data have valid statement numbers
            mBankStatSeqNo = obj->getValue("statnumber").toInt();
            if (mBankStatSeqNo <= 0) {
                mLastTransNo = f.getLastTransNo(ACC2::TransBankCash, mBankGlAccId);
                mBankStatSeqNo = mLastTransNo + 1;
            } else {
                mBankStatSeqNo = 0; // back to zero
            }

            // set table dimensions and header
            mColCount = obj->countMember();
            tw->setColumnCount(mColCount - RB2::HIDDENCOLUMNS);
            tw->setRowCount(list->countObject());
            setTableHeader(obj);
            isHeaderSet = true;
        }

        setTableRow(row, obj);
        ++row;
     }

    delete iter;
}

/**
 * Set table header
 * @param obj data object
 */
void ACC_BankImportCheckDialog::setTableHeader(RB_ObjectBase *obj) {
    tw->verticalHeader()->setDefaultSectionSize(20); // row height

    QTableWidgetItem* headerItem;

    for (int col = RB2::HIDDENCOLUMNS; col < mColCount; ++col) {
        RB_ObjectMember* mem = obj->getMember(col);
        headerItem = new QTableWidgetItem(mem->getName());
        tw->setHorizontalHeaderItem(col - RB2::HIDDENCOLUMNS, headerItem);
    }

    tw->hideColumn(IMPORTMESSAGE - RB2::HIDDENCOLUMNS); // importmessage
    tw->hideColumn(STATBANKNAME - RB2::HIDDENCOLUMNS); // statbankname
    tw->hideColumn(STATSTARTAMOUNT - RB2::HIDDENCOLUMNS); // statstartamount
    tw->hideColumn(STATENDAMOUNT - RB2::HIDDENCOLUMNS); // statendamount
    tw->hideColumn(STATTIMESTAMP - RB2::HIDDENCOLUMNS); // stattimestamp
    tw->hideColumn(TRANSVALUETIMESTAMP - RB2::HIDDENCOLUMNS); // transvaluetimestamp
}

/**
 * Set table row data. stattimestamp, statnumber are editable
 * @param row row number
 * @param obj data object
 */
void ACC_BankImportCheckDialog::setTableRow(int row, RB_ObjectBase* obj) {
    QTableWidgetItem* twItem = NULL;

    for (int col = RB2::HIDDENCOLUMNS; col < mColCount; ++col) {
        if (col == STATNUMBER && mBankStatSeqNo > 0) {
            // apparently the imported statement sequence number was not numeric
            twItem = new QTableWidgetItem(RB_String::number(mBankStatSeqNo));
        } else {
            twItem = new QTableWidgetItem(obj->getValue(col).toString());
        }

        if (col != TRANSENTRYTIMESTAMP && col != STATNUMBER) { // stattimestamp, statnumber editable
            twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
        } else {
            twItem->setBackgroundColor(mColorLightYellow);

            if (col == TRANSENTRYTIMESTAMP) { // set date delegate
                twItem->setData(RB2::RoleEditor, RB2::EditorDate);
            } else if (col == STATNUMBER) { // set integer (spinbox) delegate
                twItem->setData(RB2::RoleEditor, RB2::EditorInteger);
            }
        }

        tw->setItem(row, col - RB2::HIDDENCOLUMNS, twItem);

        if (col == STATACTNUMBER && mBankAccount.isEmpty()) { // statactnumber = bank account
            mBankAccount = obj->getValue(col).toString();
        }
    }
}

/**
 * Prepare import list for posting. Transactions that are before the
 * last date in the database are removed and the statement numbers are
 * checked for ascended sorted order.
 * @param lastTransDate last bank transaction date in the database
 */
bool ACC_BankImportCheckDialog::prepareImportList(const QDate& lastTransDate) {
    mBankImportList->erase(); // delete objects and clear container

    // transentrytimestamp is later than the transvaluetimestamp
    int rowCount = tw->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* twItem = tw->item(row, TRANSENTRYTIMESTAMP - RB2::HIDDENCOLUMNS);

        if (twItem && (twItem->data(Qt::DisplayRole).toDate() > lastTransDate)) {
            ACC_BankImport* obj = new ACC_BankImport("", mBankImportList, "ACC_BankImport");

            obj->setValue("importmessage", tw->item(row, IMPORTMESSAGE - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("statbankname", tw->item(row, STATBANKNAME - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("statactnumber", tw->item(row, STATACTNUMBER - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("statstartamount", tw->item(row, STATSTARTAMOUNT - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole)); //double
            obj->setValue("statendamount", tw->item(row, STATENDAMOUNT - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole)); // double
            obj->setValue("stattimestamp", tw->item(row, STATTIMESTAMP - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("statnumber", tw->item(row, STATNUMBER - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transaccount", tw->item(row, TRANSACCOUNT - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transactnumber", tw->item(row, TRANSACTNUMBER - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transamount", tw->item(row, TRANSAMOUNT - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole)); // double
            obj->setValue("transdebitcredit", tw->item(row, TRANSDEBITCREDIT - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transdescription", tw->item(row, TRANSDESCRIPTION - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transseconddescription", tw->item(row, TRANSSECONDDESCRIPTION - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transentrytimestamp", tw->item(row, TRANSENTRYTIMESTAMP - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transvaluetimestamp", tw->item(row, TRANSVALUETIMESTAMP - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));
            obj->setValue("transcode", tw->item(row, TRANSCODE - RB2::HIDDENCOLUMNS)->data(Qt::DisplayRole));

            mBankImportList->addObject(obj);
        }
    }

    if (mBankImportList->countObject() <= 0) {
        return false;
    }

    return true;
}

/**
 * Prepare bank transaction documents
 * @returns true on success
 */
bool ACC_BankImportCheckDialog::prepareTransDoc() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_ObjectContainer* transDocList = mTransDocListRoot->getContainer("ACC_TransDocList");

    // Reset bank transaction documents
    if (transDocList) {
        // only erases children from containers (lists)
        mTransDocListRoot->eraseChildren();
    } else {
        transDocList = new RB_ObjectContainer("", mTransDocListRoot, "ACC_TransDocList",
                                              ACC_OBJECTFACTORY);
        mTransDocListRoot->addObject(transDocList);
    }

    // Set transaction document data and relevant taxes of this order
    ACC_BankTransDoc oper;
    if (!oper.execute(transDocList, mBankImportList)) {
        QApplication::restoreOverrideCursor();
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Could not process the bank statements. \n"
                                                   "Check the bank import data file for errors."));
        return false;
    }

    QApplication::restoreOverrideCursor();
    return true;
}

/**
 * Update GL transaction widget
 */
void ACC_BankImportCheckDialog::updateGlTransWidget() {
    double totalPositiveAmount = 0.0;
    double totalNegativeAmount = 0.0;
    createNewGlTrans(); // selected document only = true

    // Clear table widget
    twGlTransactions->clear();
    QTableWidgetItem* item;

    // date, description, account, debet, credit
    int colCount = 6;
    twGlTransactions->setColumnCount(colCount);
    int rowCount = mGlTransList->countObject(); // depending the number of GL transactions
    twGlTransactions->setRowCount(rowCount + 1); // + 1 for totals
    twGlTransactions->verticalHeader()->setDefaultSectionSize(20); // row height
    twGlTransactions->horizontalHeader()->setDefaultSectionSize(80);
    twGlTransactions->setColumnWidth(1, 250);
    twGlTransactions->setColumnWidth(2, 180);
    twGlTransactions->setColumnWidth(3, 180);

    twGlTransactions->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Date")));
    twGlTransactions->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Description")));
    twGlTransactions->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Account")));
    twGlTransactions->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Cost Center")));
    twGlTransactions->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("Debit")));
    twGlTransactions->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Credit")));

    int row = 0;
    RB_ObjectIterator* iter = mGlTransList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        item = new QTableWidgetItem(obj->getValue("transdate").toString());
        twGlTransactions->setItem(row, 0, item);
        item = new QTableWidgetItem(obj->getValue("description").toString());
        twGlTransactions->setItem(row, 1, item);
        item = new QTableWidgetItem(obj->getDValue("chartmaster_idx").toString()); // display value
        twGlTransactions->setItem(row, 2, item);
        item = new QTableWidgetItem(obj->getDValue("costcenter_idx").toString()); // display value
        twGlTransactions->setItem(row, 3, item);

        double amount = obj->getValue("amount").toDouble();

        if (amount >= 0) {
            // Debit
            item = new QTableWidgetItem(RB_String::number(amount, 'f', 2));
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            twGlTransactions->setItem(row, 4, item);
            totalPositiveAmount += amount;
        } else {
            // Credit
            item = new QTableWidgetItem(RB_String::number(-amount, 'f', 2));
            item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            twGlTransactions->setItem(row, 5, item);
            totalNegativeAmount += amount;
        }
        ++row;
    }
    delete iter;

    // Set totals
    item = new QTableWidgetItem(tr("TOTALS"));
    twGlTransactions->setItem(row, 1, item);
    item = new QTableWidgetItem(RB_String::number(totalPositiveAmount, 'f', 2));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    twGlTransactions->setItem(row, 4, item);
    item = new QTableWidgetItem(RB_String::number(-totalNegativeAmount, 'f', 2));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    twGlTransactions->setItem(row, 5, item);
}

/**
 * Create new GL transactions
 * @param clearList true if transaction list should be cleared
 */
void ACC_BankImportCheckDialog::createNewGlTrans(bool isSelectedDocOnly) {
    if (!mDocModel || !mItemModel) {
        RB_DEBUG->error("ACC_BankImportCheckDialog::createNewGlTrans() model ERROR");
        return;
    }

    double totalAmount = 0.0;
    double totalDebit = 0.0;
    double totalCredit = 0.0; // negative amounts
    RB_ObjectBase* gltrans = NULL;

    if (mGlTransList->countMember() > 0) mGlTransList->erase();

    mRootId = ACC_MODELFACTORY->getRootId();

    RB_ObjectContainer* transDocList = mTransDocListRoot->getContainer("ACC_TransDocList");
    RB_ObjectIterator* docIter = transDocList->createIterator();

    for (docIter->first(); !docIter->isDone(); docIter->next()) {
        RB_ObjectBase* transDoc = docIter->currentObject();

        if (!isSelectedDocOnly || (isSelectedDocOnly && mDocModel->getCurrentId() == transDoc->getId())) {
            RB_ObjectContainer* itemList = transDoc->getContainer("ACC_BankTransList");
            RB_ObjectIterator* itemIter = itemList->createIterator();

            mType = transDoc->getValue("doctype").toInt();
            mTypeNo = transDoc->getValue("transno").toInt();
            mDocId = transDoc->getId();
            RB_String docDescr = transDoc->getValue("description").toString();
            RB_String accountId = "";
            double amount = 0.0;

            for (itemIter->first(); !itemIter->isDone(); itemIter->next()) {
                RB_ObjectBase* item = itemIter->currentObject();

                // add transaction per item
                gltrans = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(), mGlTransList, "", true);

                // Note: set date based on bank imported transaction date,
                // this is the transaction document date in the manual
                // bank entries and the debtor/creditor/memorial entries
                mDate = item->getValue("transdate").toString();
                QDateTime dateTime = item->getValue("transdate").toDateTime();
                mPeriod = dateTime.date().year() * 100 + dateTime.date().month();

                // set parent id since no parent object of mGlTransList
                gltrans->setValue("parent", mRootId);
                gltrans->setValue("transdate", mDate);
                gltrans->setValue("type", mType);
                gltrans->setValue("transdocno", mTypeNo);
                gltrans->setValue("periodno", mPeriod);
                gltrans->setValue("transdoc_id", mDocId);

                gltrans->setValue("description", item->getValue("description").toString());
                gltrans->setDValue("chartmaster_idx", item->getDValue("chartmaster_idx").toString()); // display role value
                accountId = item->getValue("chartmaster_idx").toString();
                gltrans->setValue("chartmaster_idx", accountId);  // original data role value, ID

                // no cost center
                gltrans->setValue("costcenter_idx", "0");

                // accountcontrol
                RB_ObjectBase* aObj = ACC_QACHARTMASTER->getAcctObj(accountId);
                if (aObj) {
                    gltrans->setValue("accountcontrol", aObj->getValue("accountcontrol"));
                } else {
                    gltrans->setValue("accountcontrol", 0);
                }

                amount = item->getValue("amount").toDouble();
                gltrans->setValue("amount", -amount); // credit
                totalAmount += amount;

                // set allocation id and reference for bank, memorandum transaction and totals,
                gltrans->setValue("transallocn_idx", item->getValue("transallocn_idx").toString());
                gltrans->setDValue("transallocn_idx", item->getDValue("transallocn_idx").toString());

                // totals for bank, debit/credit is separate for bank only
                if (amount >= 0.0) {
                    totalDebit += amount;
                } else {
                    totalCredit += amount; // negative amounts
                }
            }

            // add corresponding transaction for bank/cash TODO: combine debit credit
            createNewHelper(docDescr, mBankGlAccName,
                            mBankGlAccId, totalDebit);
            createNewHelper(docDescr, mBankGlAccName,
                            mBankGlAccId, totalCredit);
            totalDebit = 0.0;
            totalCredit = 0.0;
        }
    }
}

void ACC_BankImportCheckDialog::createNewHelper(const RB_String& descr,
        const RB_String& acctName, const RB_String& acctId, double amount) {
    if (amount == 0.0) return; // no gltrans required

    RB_ObjectBase* gltrans
        = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(), mGlTransList, "", true);
    gltrans->setValue("parent", mRootId);
    gltrans->setValue("transdate", mDate);
    gltrans->setValue("type", mType);
    gltrans->setValue("transdocno", mTypeNo);
    gltrans->setValue("periodno", mPeriod);
    gltrans->setValue("transdoc_id", mDocId);
    gltrans->setValue("description", descr);
    gltrans->setValue("chartmaster_idx", acctId);  // original data role value
    gltrans->setDValue("chartmaster_idx", acctName); // display role value
    // accountcontrol
    RB_ObjectBase* aObj = ACC_QACHARTMASTER->getAcctObj(acctId);
    if (aObj) {
        gltrans->setValue("accountcontrol", aObj->getValue("accountcontrol"));
    } else {
        gltrans->setValue("accountcontrol", 0);
    }
    gltrans->setValue("amount", amount); // debit
}

/**
 * @returns true if bank documents contain valid transactions
 */
bool ACC_BankImportCheckDialog::isValidTransDoc() {
    RB_String message = "";
    double totalPositive = 0.0; // debit
    double totalNegative = 0.0; // credit also positive
//    QModelIndex idx;
    RB_String acct = "";

    double amount = 0.0;

    RB_ObjectContainer* transDocList = mTransDocListRoot->getContainer("ACC_TransDocList");
    RB_ObjectIterator* docIter = transDocList->createIterator();

    for (docIter->first(); !docIter->isDone(); docIter->next()) {
        RB_ObjectBase* transDoc = docIter->currentObject();
        RB_ObjectContainer* itemList = transDoc->getContainer("ACC_BankTransList");
        RB_ObjectIterator* itemIter = itemList->createIterator();

        for (itemIter->first(); !itemIter->isDone(); itemIter->next()) {
            RB_ObjectBase* item = itemIter->currentObject();

            // check valid account, TODO: check also for relevancy of account
            message += !item->getValue("chartmaster_idx").isValid() ? tr("- invalid item account\n") : "" ;

            amount = item->getValue("amount").toDouble();

            if (amount >= 0) {
                totalPositive += amount;
            } else {
                totalNegative += -amount;
            }

            acct = item->getValue("chartmaster_idx").toString(); // id only

            // Can only book against Amount Payable (Crediteuren)
            // or Amount Receivable (Debiteuren) if allocation exists
            RB_String allocId = item->getValue("transallocn_idx").toString(); // id only

            if (((acct == ACC_QACHARTMASTER->getAccPayId()
                        || acct == ACC_QACHARTMASTER->getAccRecId())
                    && (allocId.isEmpty() || allocId == "" || allocId == "0"))
                    || acct == ACC_QACHARTMASTER->getAccMemoId()) {
                message += tr("- Cannot book against Amount Receivable or Payable\n"
                              "   if no allocation exists or Memorandum Account");
            } else if (acct == "" || acct == "0") {
                message += tr("- Item GL account missing");
            }
        }

        // Check amounts
        if (std::fabs(totalPositive
                      - transDoc->getValue("totalamountrec").toDouble()) >= 0.005) {
            message += tr("- Debit- or receivable amounts do not match\n");
        }

        if (std::fabs(totalNegative
                      - transDoc->getValue("totalamountpay").toDouble()) >= 0.005) {
            // totalNegative is a positive number. Creditor is changed to negative at
            // generating gltrans
            message += tr("- Credit- or payable amounts do not match\n");
        }

        totalPositive = 0.0; // debit
        totalNegative = 0.0; // credit also positive
        acct = "";
        amount = 0.0;
    }

    if (!message.isEmpty()) {
        mValidationMessage = tr("Transaction is invalid, check:\n") + message;
        DB_DIALOGFACTORY->commandMessage(mValidationMessage);
    } else {
        mValidationMessage = "";
        return true;
    }

    return false;
}

/**
 * Update (complete) transaction fields such as transaction date before saving
 */
void ACC_BankImportCheckDialog::updateBeforeSave() {
    int transNo = 0;
    QDateTime transDate = QDateTime();
    RB_String transDocId = "";
    int transType = (int)ACC2::TransBankCash;

    RB_ObjectContainer* transDocList = mTransDocListRoot->getContainer("ACC_TransDocList");
    RB_ObjectIterator* docIter = transDocList->createIterator();

    for (docIter->first(); !docIter->isDone(); docIter->next()) {
        RB_ObjectBase* transDoc = docIter->currentObject();
        RB_ObjectContainer* itemList = transDoc->getContainer("ACC_BankTransList");
        RB_ObjectIterator* itemIter = itemList->createIterator();

        transNo = transDoc->getValue("transno").toInt();
        transDate = transDoc->getValue("transdate").toDateTime();
        transDocId = transDoc->getValue("id").toString();

        for (itemIter->first(); !itemIter->isDone(); itemIter->next()) {
            RB_ObjectBase* item = itemIter->currentObject();

            item->setValue("type", transType);
            item->setValue("transno", transNo);

            // Set transaction date only if not imported from bank
            if (item->getValue("transdate").toString() < "1Z") {
                item->setValue("transdate", transDate);
            }

            item->setValue("transdoc_id", transDocId);
        }
    }
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_BankImportCheckDialog::getHelpSubject() const {
    return objectName();
}

/**
 * Notify user of unsaved data and ask whether data should be saved.
 * Overrides the standard maybeSave() because of mSaveInProgress requirement
 */
bool ACC_BankImportCheckDialog::maybeSave() {
    if (isWindowModified() && !mSaveInProgress) {
        int ret = DB_DIALOGFACTORY->requestYesNoCancelDialog(
                tr("Data changed ..."),
                tr("The data has been modified.\n"
                   "Do you want to keep your changes?"));
        if (ret == QMessageBox::Yes) {
            mSaveInProgress = true;

            if (!fileSave(true)) { // if saved message by action
                DB_DIALOGFACTORY->statusBarMessage(tr("Data not saved"), 2000);
            } else {
                DB_DIALOGFACTORY->statusBarMessage(tr("Data saved"), 2000);
                // in fileSave() is setWindowModified(false);
            }

            mSaveInProgress = false;
        } else if (ret == QMessageBox::No) {
            DB_DIALOGFACTORY->statusBarMessage(tr("Data not saved"), 2000);
            fileRevert(); // will setWindowModified(false)
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }

    return true;
}

/**
 * Change event, for example translation
 */
void ACC_BankImportCheckDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

