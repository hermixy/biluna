/*****************************************************************
 * $Id: acc_gltransactionwidget.cpp 2217 2015-02-13 18:18:14Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_gltransactionwidget.h"

#include <cmath>
#include <QStringListModel>
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_orderpreviewdialog.h"
#include "acc_postgltransaction.h"
#include "acc_purchaseorderhtml.h"
#include "acc_qachartmaster.h"
#include "acc_salesinvoicehtml.h"
#include "acc_selectallocndialog.h"
#include "acc_selectcustbranchdialog.h"
#include "acc_sqlcommonfunctions.h"
#include "db_dialogfactory.h"
#include "rb_datawidgetmapper.h"
#include "rb_mdiwindow.h"
#include "rb_membereditdialog.h"
#include "rb_settings.h"
#include "rb_sqlrelationaldelegate.h"

// #include "rb_signalspydialog.h"

/**
 * Constructor
 */
ACC_GlTransactionWidget::ACC_GlTransactionWidget(QWidget *parent)
        : RB_Widget(parent) {

    setupUi(this);

    mChartMasterModel = NULL;
    mTransDocModel = NULL;
    mTransDocMapper = NULL;
    mItemTransModel = NULL;
    mItemTransMapper = NULL;

    mGlTransList = new RB_ObjectContainer("", NULL, "ACC_GlTransList", ACC_OBJECTFACTORY);

    mInitInProgress = false;
    mSaveInProgress = false;
    mAddDeleteInProgress = false;
    mValidationMessage = "";

    mIsValidateAmounts = false;
    mIsItemChangedPassed = false;
}

/**
 * Destructor
 */
ACC_GlTransactionWidget::~ACC_GlTransactionWidget() {
    // store selected gl number from cbLedgerAccount
    RB_SETTINGS->beginGroup(objectName());
    RB_SETTINGS->setValue("glno", cbLedgerAccount->currentIndex());
    RB_SETTINGS->endGroup();

    // non-shared models are always deleted by the widgets,
    // can be MDI window or dialog, but also a dockWindow with table or tree
    //    delete mAllocModel;

    delete mChartMasterModel; // shared because of parent of document model
    delete mTransDocModel;  // OK if not used anywhere else
    delete mItemTransModel; // OK if not used anywhere else

    delete mGlTransList;
    RB_DEBUG->print("ACC_GlTransactionWidget::~ACC_GlTransactionWidget() OK");
}

/**
 * Initialize widget and models
 */
void ACC_GlTransactionWidget::init() {
    mInitInProgress = true; // HACK: to prevent maybeSave() in endless loop
    //
    // Set ledger account model and selection combobox
    //
    mChartMasterModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelChartMaster);
    mChartMasterModel->setRoot(ACC_MODELFACTORY->getRootId());
    RB_String filter = mChartMasterModel->getSourceFilter();
    filter += " AND `acc_chartmaster`.`accountcontrol`>=1000";
    mChartMasterModel->setSourceFilter(filter);
    cbLedgerAccount->setModel(mChartMasterModel);
    cbLedgerAccount->setModelColumn(mChartMasterModel->fieldIndex("accountname"));
    mChartMasterModel->select();
    // instead of:
//    int colName = mChartMasterModel->fieldIndex("accountname");
//    int colControl = mChartMasterModel->fieldIndex("accountcontrol");
//    QString name;
//    double val;
//    QModelIndex idx;

//    for (int i = 0; i < mChartMasterModel->rowCount(); ++i) {
//        idx = mChartMasterModel->index(i, colName);
//        name = mChartMasterModel->data(idx).toString();
//        idx = mChartMasterModel->index(i, colControl);
//        val = mChartMasterModel->data(idx).toDouble();
//        cbLedgerAccount->addItem(name, val);
//    }

    connect(cbLedgerAccount, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotLedgerAccountChanged(int)));
    connect(ACC_DIALOGFACTORY, SIGNAL(bankStatementsImported()),
            this, SLOT(slotRefreshBankModel()));

    //
    // 1. Set model
    //
    mTransDocModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTransDoc);

    //
    // 2. Set relations and mapper for line edits etc.
    //
    mTransDocMapper = mTransDocModel->getMapper();
    mTransDocMapper->addMapping(deDate, mTransDocModel->fieldIndex("transdate"));
    mTransDocMapper->addMapping(leDocumentNo, mTransDocModel->fieldIndex("transno"));
    QIntValidator* intVal = new QIntValidator(this);
    intVal->setBottom(0);
    leDocumentNo->setValidator(intVal);
    mTransDocMapper->addMapping(ileCustomer, mTransDocModel->fieldIndex("debtor_idx"));
    mTransDocMapper->addMapping(ileSupplier, mTransDocModel->fieldIndex("creditor_idx"));
    mTransDocMapper->addMapping(leReferenceNo, mTransDocModel->fieldIndex("refno"));
    mTransDocMapper->addMapping(leDescription, mTransDocModel->fieldIndex("description"));
    mTransDocMapper->addMapping(leTotalRec, mTransDocModel->fieldIndex("totalamountrec"));
    leTotalRec->setValidator(new QDoubleValidator(-9999999.99, 9999999.99, 2, this));
    mTransDocMapper->addMapping(leTotalPay, mTransDocModel->fieldIndex("totalamountpay"));
    leTotalPay->setValidator(new QDoubleValidator(-9999999.99, 9999999.99, 2, this));
    mTransDocMapper->addMapping(leTotalTax, mTransDocModel->fieldIndex("totaltax"));
    leTotalTax->setValidator(new QDoubleValidator(-9999999.99, 9999999.99, 2, this));
    mTransDocMapper->addMapping(deDueDate, mTransDocModel->fieldIndex("duedate"));

    deDate->setDisplayFormat("yyyy-MM-dd");
    deDueDate->setDisplayFormat("yyyy-MM-dd");

    setFormatTableView(tvDocument, mTransDocModel);

    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(slotTabChanged(int)));
    connect(mTransDocModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));

    // Initialize the mTransDocModel, children models
    if (cbLedgerAccount->count() > 0) {
        setWindowModified(false);
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(
                tr("ERROR:\nthe chart of accounts is invalid,\nrefer to the manual."));
        ACC_DIALOGFACTORY->commandMessage("ACC_ChartMaster ERROR:\n"
                                          " filter = " + filter);
    }

    // Set basic validators for numeric lineEdits
    leHighTaxAmt->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));
    leLowTaxAmt->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));
    leOtherTaxAmt->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));
    leItemAmount->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));

    // First read settings
    readSettings();

    // then set to last selected general ledger in cbLedgerAccount
    RB_SETTINGS->beginGroup(objectName());
    int glno = RB_SETTINGS->value("glno", 0).toInt();
    RB_SETTINGS->endGroup();
    cbLedgerAccount->setCurrentIndex(glno); // sets tableView

    // tvDocument->sortByColumn(-1); better (?): or tvDocument->undoFilterSort()
    tvDocument->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    mInitInProgress = false;

    // test only
    // RB_SignalSpyDialog* ssd = new RB_SignalSpyDialog(this, mTransDocModel);
    // ssd->show();
}

/**
 * Slot File export PDF.
 * @param printer
 */
//void ACC_GlTransactionWidget::filePdf(QPrinter* printer) {
//    QPainter p(printer);
//    QPixmap pm = QPixmap::grabWidget(this);
//    p.drawPixmap(0, 0, pm);
//}

/**
 * Slot File Print.
 * @param printer
 */
//void ACC_GlTransactionWidget::filePrint(QPrinter* printer) {
//    QPainter p(printer);
//    QPixmap pm = QPixmap::grabWidget(this);
//    p.drawPixmap(0, 0, pm);
//}

/**
 * Slot File Print Preview.
 * @param printer
 */
//void ACC_GlTransactionWidget::filePrintPreview(QPrinter* printer) {
//    QPainter p(printer);
//    QPixmap pm = QPixmap::grabWidget(this);
//    p.drawPixmap(0, 0, pm);
//}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave(). Always does a select() for now.
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_GlTransactionWidget::fileSave(bool /*withSelect*/) {
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

    // save and post GL transactions, database transaction/commit
    bool success = savePostGlTrans();
    // save allocation, related transdoc and itemtrans, database transaction/commit
    success = success ? mHandleAllocn.submitAllAndSelect() : false;
    // save visible models
    success = success ? mItemTransModel->submitAllAndSelect() : false ;

    if ((mTransType == ACC2::TransCreditor || mTransType == ACC2::TransDebtor)
                && mTransDocModel->getCurrentValue("refno").toString().isEmpty()) {
        // make sure reference number is set
        RB_String refNo = mTransDocModel->getCurrentValue("transno").toString();
        mTransDocModel->setCurrentValue("refno", refNo, Qt::EditRole);
    }

    success = success ? mTransDocModel->submitAllAndSelect() : false;

    if (success) {
        setWindowModified(false);
    } else {
        RB_DEBUG->error("ACC_GlTransactionWidget::fileSave() transaction 2 ERROR");
        RB_DEBUG->error(ACC_MODELFACTORY->getDatabase().lastError().text());
        fileRevert();
    }

    mIsValidateAmounts = false;
    mIsItemChangedPassed = false;
    mSaveInProgress = false;
    enableWidgets(true);

    leBalance->clear();

    QApplication::restoreOverrideCursor();

    // TODO: remove, debug and temporary only:
    ACC_SqlCommonFunctions f;
    if (f.isGlTransOrphanExisting() || f.isAllocNotConsistent()) {
        ACC_DIALOGFACTORY->requestWarningDialog("Orphaned ACC_GlTrans rows\n"
                                                "or inconsistent allocations.\n"
                                                "Remember what you did\n"
                                                "and inform the development team.");
    }

    return true;
}

/**
 * Save and post GL transactions to database, GL Summary and Cost Summary
 */
bool ACC_GlTransactionWidget::savePostGlTrans() {
    // Clear container and create existing transactions
    createExistingGlTrans();

    // Set existing to be deleted
    RB_ObjectIterator* iter = mGlTransList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* glTrans = iter->currentObject();
        glTrans->setFlag(RB2::FlagIsDeleted);
    }
    delete iter;

    bool success = processGlTrans();

    // Clear container and create new GL transations
    createNewGlTrans();

    success = success ? processGlTrans() : false;
    return success;
}

/**
 * Process the save and post GL transactions to database, GL Summary
 * and Cost Summary. Done in a database transaction/commit
 */
bool ACC_GlTransactionWidget::processGlTrans() {
    // Start transaction
    ACC_MODELFACTORY->getDatabase().transaction();

    bool success = true;
    ACC_PostGlTransaction oper;

    RB_ObjectIterator* iter = mGlTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* glTrans = iter->currentObject();
        success = oper.execute(glTrans);

        if (!success) {
            // Abort
            ACC_MODELFACTORY->getDatabase().rollback();
            RB_DEBUG->error("ACC_GlTransactionWidget::processGlTrans() post glTrans ERROR");
            return false;
        }
    }

    delete iter;

    // Insert glTrans items
    success = success ? mGlTransList->dbUpdateList(ACC_MODELFACTORY->getDatabase(),
                                                 RB2::ResolveOne) : false;
    // Commit
    ACC_MODELFACTORY->getDatabase().commit();

    if (!success) {
        RB_DEBUG->error("ACC_GlTransactionWidget::processTrans() update mGlTransList ERROR");
    }

    return success;
}

/**
 * Reverse all changes
 */
void ACC_GlTransactionWidget::fileRevert() {
    mDeleteDocIdList.clear();
    mHandleAllocn.revertAll();
    mItemTransModel->revertAll();
    mTransDocModel->revertAll();

    mIsItemChangedPassed = false;
    setWindowModified(false);

    leBalance->clear();
}

/**
 * Button Check Balance clicked, this will calculate
 * the balance of the bank account selected upto
 * and including the transaction document selected
 * TODO: move SQL statement to ACC_SqlCommonFunctions
 */
void ACC_GlTransactionWidget::on_pbCheckBalance_clicked() {
    // select sum(`amount`) as `balance` from `acc_gltrans`
    // where `transdate`>='2011-01-01T00:00:00'
    // and `transdate`<'2011-07-12T00:00:00'
    // and SUBSTR(`chartmaster_idx`, 1, 38)='{00000000-0000-0000-0000-000000ID1100}';
    // NOTE: SUBSTR(`chartmaster_idx`, 0, 38) is the same as above

    int row = cbLedgerAccount->currentIndex();
    QModelIndex idx = mChartMasterModel->index(row, 0);
    RB_String id = mChartMasterModel->data(idx, Qt::EditRole).toString();

    QDate dt = mTransDocModel->getCurrentValue("transdate", RB2::RoleOrigData).toDate();
    RB_String startDate = RB_String::number(dt.year());
    RB_String endDate = dt.toString(Qt::ISODate);

    ACC_SqlCommonFunctions util;
    RB_String qStr = "SELECT SUM(`amount`) as `balance` from `acc_gltrans` "
            "WHERE SUBSTR(`transdate`,1,10)>='" + startDate + "' "
            "AND SUBSTR(`transdate`,1,10)<='" + endDate + "' "
            "AND ";
    util.substrIdxId("chartmaster_idx", qStr);
    qStr += "='" + id + "'; ";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_GlTransactionWidget::on_pbCheckBalance_clicked() "
                        + query.lastError().text() + " ERROR");
        return;
    }

    if (query.first()) {
        double amount = query.record().value(0).toDouble();
        RB_String str = RB_String::number(amount, 'f', 2);
        leBalance->setText(str);
    }
}

/**
 * Button add clicked
 */
void ACC_GlTransactionWidget::on_pbAddDoc_clicked() {
    if (!mTransDocModel) return;
    mAddDeleteInProgress = true;
    mTransDocModel->undoFilterSort();

    // always insert at the start
    int row = 0; // mTransDocModel->rowCount();
    mTransDocModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QDateTime today = QDateTime::currentDateTime();
    QModelIndex index = mTransDocModel->index(row, mTransDocModel->fieldIndex("transdate"));
    // mTransDocModel->setData(index, today.date().toString("yyyy-MM-dd"), Qt::EditRole);
    mTransDocModel->setData(index, today, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("creditor_idx"));
    mTransDocModel->setData(index, "0", Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("debtor_idx"));
    mTransDocModel->setData(index, "0", Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("doctype"));
    mTransDocModel->setData(index, mTransType, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("docname"));
    mTransDocModel->setData(index, cbLedgerAccount->currentText(), Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("totalamountrec"));
    mTransDocModel->setData(index, 0, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("totalamountpay"));
    mTransDocModel->setData(index, 0, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("totaltax"));
    mTransDocModel->setData(index, 0, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("duedate"));
    // mTransDocModel->setData(index, today.date().toString("yyyy-MM-dd"), Qt::EditRole);
    mTransDocModel->setData(index, today, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("rate"));
    mTransDocModel->setData(index, 1.0, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("alloc"));
    mTransDocModel->setData(index, 0, Qt::EditRole);
    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("settled"));
    mTransDocModel->setData(index, 0, Qt::EditRole);
    // end NOTE

    // Get next document number
    int ledgerRow = cbLedgerAccount->currentIndex();
    QModelIndex idx = mChartMasterModel->index(ledgerRow, mChartMasterModel->fieldIndex("id"));
    RB_String transTypeId = idx.data().toString();
    idx = mChartMasterModel->index(ledgerRow,  mChartMasterModel->fieldIndex("accountname"));
    RB_String transTypeName = idx.data().toString();
    ACC_SqlCommonFunctions oper;
    int number = 0;

    if (mTransType == ACC2::TransDebtor) {
        number = oper.getNextTransNo((int)mTransType, "none", "Debtor");
    } else if (mTransType == ACC2::TransCreditor) {
        number = oper.getNextTransNo((int)mTransType, "none", "Creditor");
    } else if (mTransType == ACC2::TransMemo) {
        number = oper.getNextTransNo((int)mTransType, "none", "Memo");
    } else { // ACC2::TransBankCash
        number = oper.getNextTransNo((int)mTransType, transTypeId, transTypeName);
    }

    index = mTransDocModel->index(row, mTransDocModel->fieldIndex("transno"));
    mTransDocModel->setData(index, number, Qt::EditRole);

    tvDocument->setCurrentIndex(mTransDocModel->index(row, 3, QModelIndex()));
    tvDocument->scrollTo(tvDocument->currentIndex());
    deDate->setFocus();
    mAddDeleteInProgress = false;
}

/**
 * Button delete clicked
 */
void ACC_GlTransactionWidget::on_pbDeleteDoc_clicked() {
    if (!mTransDocModel || !mItemTransModel) return;
    QModelIndex index = tvDocument->currentIndex();
    if (!index.isValid()) return;

    mAddDeleteInProgress = true;
    int ret = QMessageBox::No;

    if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo) {
        ret = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("Delete document ..."),
                tr("This action cannot be undone.\n"
                   "Do you want to delete this document?"));
    } else if (mTransType == ACC2::TransCreditor || mTransType == ACC2::TransDebtor) {
        // Allocated amount
        double allocAmt = mTransDocModel->getCurrentValue("alloc").toDouble();

        if (allocAmt < -0.005 || allocAmt > 0.005) {
            ret = ACC_DIALOGFACTORY->requestYesNoDialog(
                        tr("Delete document ..."),
                        tr("This action cannot be undone.\n"
                           "Do you want to delete this document\n"
                           "and the related allocation?"));
        } else {
            ret = DB_DIALOGFACTORY->requestYesNoDialog(
                    tr("Delete document ..."),
                    tr("This action cannot be undone.\n"
                       "Do you want to delete this document?"));
        }
    }

    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        return;
    }

    // reverse, delete and remove allocation
    if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo) {
        mHandleAllocn.delItemListAllocn(mItemTransModel);
    } else if (mTransType == ACC2::TransCreditor || mTransType == ACC2::TransDebtor) {
        RB_String docId = mTransDocModel->getCurrentId();
        mHandleAllocn.delDocAllocn(docId, true);
    } else {
        return;
    }

    // remove gltrans (which reverses chartdetails), code is partly from fileSave()
    ACC_MODELFACTORY->getDatabase().transaction();
    createExistingGlTrans();

    // Unpost existing transactions
    ACC_PostGlTransaction oper;

    RB_ObjectIterator* iter = mGlTransList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        // set display value for account
        RB_ObjectBase* obj = iter->currentObject();
        obj->setFlag(RB2::FlagIsDeleted);
        if (!oper.execute(obj)) {
            RB_DEBUG->error("ACC_GlTransactionWidget::fileSave() unposting ERROR");
        }
    }
    delete iter;

    // Delete GL transactions, if not journal item model
    RB_String transDocId = mTransDocModel->getCurrentId();
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String sqlStr = "DELETE FROM acc_gltrans WHERE transdoc_id='" + transDocId + "';";

    if (!query.exec(sqlStr)) {
        RB_DEBUG->error("ACC_GlTransactionWidget::fileSave() delete gltrans ERROR");
    }

    ACC_MODELFACTORY->getDatabase().commit();

    // Remove item and document model rows
    //
    int noRows = mItemTransModel->rowCount(); // not direct childs of documents
    for (int i = 0; i < noRows; ++i) {
        mItemTransModel->removeRows(0, 1);
    }
    mTransDocModel->removeRows(index.row(), 1, index.parent());


    // Commit changes
    //
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mSaveInProgress = true;
    ACC_MODELFACTORY->getDatabase().transaction();

    bool success = true;
    success = success ? mItemTransModel->submitAll() : false ; // TODO: also in in RB_MmProxy::slotParentCurrentRowChanged()
    success = success ? mTransDocModel->submitAllAndSelect() : false;
    success = success ? mHandleAllocn.submitAllAndSelect() : false;

    if (success && ACC_MODELFACTORY->getDatabase().commit()) {
        setWindowModified(false);
    } else {
        RB_DEBUG->error("ACC_GlTransactionWidget::pbDeleteDoc() transaction ERROR");
    }

    mIsValidateAmounts = false;
    mSaveInProgress = false;
    enableWidgets(true);
    QApplication::restoreOverrideCursor();

    mAddDeleteInProgress = false;
    DB_DIALOGFACTORY->statusBarMessage(tr("Document deleted"), 2000);
}

/**
 * Button select customer clicked
 * TODO: set due date base on customer payment terms, also when invoice date is changed
 */
void ACC_GlTransactionWidget::on_ileCustomer_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCustBranch);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvDocument->currentIndex().row();
            QModelIndex index = mTransDocModel->index(row, mTransDocModel->fieldIndex("debtor_idx"));
            mTransDocModel->setData(index, obj->getId()
                               + obj->getValue("mname").toString(), Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No branch selected.\n"
                                                   "Customer branch is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button select supplier clicked
 * TODO: set due date base on customer payment terms, also when invoice date is changed
 */
void ACC_GlTransactionWidget::on_ileSupplier_clicked() {
    if (!tvDocument->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No document selected.\n"
                                                   "Please select a document first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectSupplier);

    if (dlg->exec() == QDialog::Accepted) {
        RB_ObjectBase* obj = dlg->getCurrentObject();
        if (obj) {
            int row = tvDocument->currentIndex().row();
            QModelIndex index = mTransDocModel->index(row, mTransDocModel->fieldIndex("creditor_idx"));
            mTransDocModel->setData(index, obj->getId()
                               + obj->getValue("suppname").toString(), Qt::EditRole);
        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No supplier selected.\n"
                                                   "Supplier is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Button reprint invoice clicked. Active when (debtor)
 * transaction document is generated from a sales order.
 */
void ACC_GlTransactionWidget::on_pbReprintInvoice_clicked() {
    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Save your data first."));
        return;
    }

    if (!tvDocument->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No transaction document selected."));
        return;
    }

    RB_String orderId = mTransDocModel->getCurrentValue("order_id", RB2::RoleOrigData).toString();

    if (orderId.isEmpty() || orderId == "0") {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Transaction document is\n"
                                                   "not based on a sales order."));
        return;
    }

    RB_String transDocId = mTransDocModel->getCurrentValue("id", RB2::RoleOrigData).toString();
    RB_ObjectContainer* debtorTransList = new RB_ObjectContainer(RB_Uuid::createUuid().toString(), NULL,
                                                                 "ACC_DebtorTransList", ACC_OBJECTFACTORY);
    debtorTransList->setValue("parent", transDocId);
    debtorTransList->dbReadList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveOne, "transdoc_id");

    RB_ObjectBase* transDoc = ACC_OBJECTFACTORY->newSingleObject("ACC_TransDoc");
    transDoc->setId(transDocId);
    transDoc->dbRead(ACC_MODELFACTORY->getDatabase());

    // Show invoice dialog, all from ACC_SalesOrderWidget
    // ACC_ReportPreviewDialog* dlg = new ACC_ReportPreviewDialog(this); QWebView
    ACC_OrderPreviewDialog* dlg = new ACC_OrderPreviewDialog(this);
    dlg->setWindowTitle("ACC - Sales Invoice");

    ACC_SalesInvoiceHtml oper;
    oper.execute(transDoc, debtorTransList);

    dlg->setCurrentFileName(tr("so_invoice") + "_"
                            + transDoc->getValue("transno").toString());
    dlg->setHtml(oper.getHtml());
    dlg->exec();
    dlg->deleteLater();
}

/**
 * Button reprint purchase order clicked. Active when (creditor)
 * transaction document is generated from a purchase order.
 */
void ACC_GlTransactionWidget::on_pbReprintPurchaseOrder_clicked() {
    if (isWindowModified()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Save your data first."));
        return;
    }

    if (!tvDocument->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No transaction document selected."));
        return;
    }

    RB_String orderId = mTransDocModel->getCurrentValue("order_id", RB2::RoleOrigData).toString();

    if (orderId.isEmpty() || orderId == "0") {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Transaction document is\n"
                                                   "not based on a purchase order."));
        return;
    }

    RB_String transDocId = mTransDocModel->getCurrentValue("id", RB2::RoleOrigData).toString();
    RB_ObjectContainer* creditorTransList = new RB_ObjectContainer(RB_Uuid::createUuid().toString(), NULL,
                                                                 "ACC_CreditorTransList", ACC_OBJECTFACTORY);
    creditorTransList->setValue("parent", transDocId);
    creditorTransList->dbReadList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveOne, "transdoc_id");

    RB_ObjectBase* transDoc = ACC_OBJECTFACTORY->newSingleObject("ACC_TransDoc");
    transDoc->setId(transDocId);
    transDoc->dbRead(ACC_MODELFACTORY->getDatabase());

    // Show invoice dialog, all from ACC_SalesOrderWidget
    // ACC_ReportPreviewDialog* dlg = new ACC_ReportPreviewDialog(this); QWebView
    ACC_OrderPreviewDialog* dlg = new ACC_OrderPreviewDialog(this);
    dlg->setWindowTitle("ACC - Purchase Order");

    ACC_PurchaseOrderHtml oper;
    oper.setIsCheckPrint(false);
    oper.execute(transDoc, creditorTransList);

    dlg->setHtml(oper.getHtml());
    dlg->setCurrentFileName(tr("po_") + oper.getOrderNumber());
    dlg->exec();
    dlg->deleteLater();
}

/**
 * Button add clicked
 */
void ACC_GlTransactionWidget::on_pbAddItem_clicked() {
    if (!mItemTransModel || !mTransDocModel->getProxyIndex().isValid()) return;

    mAddDeleteInProgress = true;
    mItemTransModel->undoFilterSort();

    // always insert at the end
    int row = mItemTransModel->rowCount();
    mItemTransModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    if (mTransType == ACC2::TransDebtor) {

        QModelIndex idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("transdoc_id"));
        RB_String docId = mTransDocModel->getCurrentId();
        mItemTransModel->setData(idx, docId, Qt::EditRole);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxauth_id"), QModelIndex());
        mItemTransModel->setData(idx, "0", Qt::EditRole); // TODO: via company

        // Tax
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxhighchartmaster_idx"));
        mItemTransModel->setData(idx, ACC_QACHARTMASTER->getAccSalesTaxHighId()
                            + ACC_QACHARTMASTER->getAccSalesTaxHighName());
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxlowchartmaster_idx"));
        mItemTransModel->setData(idx, ACC_QACHARTMASTER->getAccSalesTaxLowId()
                            + ACC_QACHARTMASTER->getAccSalesTaxLowName());
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxotherchartmaster_idx"));
        mItemTransModel->setData(idx, ACC_QACHARTMASTER->getAccSalesTaxOtherId()
                            + ACC_QACHARTMASTER->getAccSalesTaxOtherName());
        sbHighTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxHighPercentage());
        sbLowTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxLowPercentage());
        sbOtherTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxOtherPercentage());

        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("hold"), QModelIndex());
        mItemTransModel->setData(idx, 0, Qt::EditRole);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("costcenter_idx"), QModelIndex());
        mItemTransModel->setData(idx, "0", Qt::EditRole);

        // use last used customer GL account
        int docRow = tvDocument->currentIndex().row();
        QModelIndex custSupIdIdx = mTransDocModel->index(docRow, mTransDocModel->fieldIndex("debtor_idx"));
        RB_String csId = mTransDocModel->data(custSupIdIdx, RB2::RoleOrigData).toString();
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("chartmaster_idx"));
        mItemTransModel->setData(idx, "0", Qt::EditRole);

        // Actual value if exists
        if (!csId.isEmpty()) {
            ACC_SqlCommonFunctions oper;
            RB_String acctId = oper.selectFromWhereId("lastusedacct_id",
                                                       "ACC_Customer", csId).toString();
            if (!acctId.isEmpty() && acctId != "0") {
                RB_ObjectBase* obj = ACC_QACHARTMASTER->getAcctObj(acctId);

                if (obj) {
                    acctId += obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString();
                    mItemTransModel->setData(idx, acctId, Qt::EditRole);
                }
            }

            // Cost center
            RB_String ccId = oper.selectFromWhereId("lastusedcostcenter_id",
                                                    "ACC_Customer", csId).toString();

            if (!ccId.isEmpty() && ccId != "0") {
                RB_String ccCode = oper.selectFromWhereId("centercode",
                                                          "ACC_CostCenter", ccId).toString();
                RB_String ccName = oper.selectFromWhereId("centername",
                                                          "ACC_CostCenter", ccId).toString();
                idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("costcenter_idx"));
                mItemTransModel->setData(idx, ccId + ccCode + " - " + ccName, Qt::EditRole);
            }
        }

    } else if (mTransType == ACC2::TransCreditor) {

        QModelIndex idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("transdoc_id"));
        RB_String docId = mTransDocModel->getCurrentId();
        mItemTransModel->setData(idx, docId, Qt::EditRole);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxauth_id"), QModelIndex());
        mItemTransModel->setData(idx, "0", Qt::EditRole); // TODO: implement

        // Tax
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxhighchartmaster_idx"));
        mItemTransModel->setData(idx, ACC_QACHARTMASTER->getAccPurchaseTaxHighId()
                            + ACC_QACHARTMASTER->getAccPurchaseTaxHighName());
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxlowchartmaster_idx"));
        mItemTransModel->setData(idx, ACC_QACHARTMASTER->getAccPurchaseTaxLowId()
                            + ACC_QACHARTMASTER->getAccPurchaseTaxLowName());
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxotherchartmaster_idx"));
        mItemTransModel->setData(idx, ACC_QACHARTMASTER->getAccPurchaseTaxOtherId()
                            + ACC_QACHARTMASTER->getAccPurchaseTaxOtherName());
        sbHighTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxHighPercentage());
        sbLowTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxLowPercentage());
        sbOtherTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxOtherPercentage());

        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("hold"), QModelIndex());
        mItemTransModel->setData(idx, 0, Qt::EditRole);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("costcenter_idx"), QModelIndex());
        mItemTransModel->setData(idx, "0", Qt::EditRole);

        // use last used customer GL account
        int docRow = tvDocument->currentIndex().row();
        QModelIndex custSupIdIdx = mTransDocModel->index(docRow, mTransDocModel->fieldIndex("creditor_idx"));
        RB_String csId = mTransDocModel->data(custSupIdIdx, RB2::RoleOrigData).toString();
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("chartmaster_idx"));
        mItemTransModel->setData(idx, "0", Qt::EditRole);

        // Actual value if exists
        if (!csId.isEmpty()) {
            ACC_SqlCommonFunctions oper;
            RB_String acctId = oper.selectFromWhereId("lastusedacct_id",
                                                       "ACC_Supplier", csId).toString();
            if (!acctId.isEmpty() && acctId != "0") {
                RB_ObjectBase* obj = ACC_QACHARTMASTER->getAcctObj(acctId);
                if (obj) {
                    acctId += obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString();
                    mItemTransModel->setData(idx, acctId, Qt::EditRole);
                }
            }

            // Cost center
            RB_String ccId = oper.selectFromWhereId("lastusedcostcenter_id",
                                                    "ACC_Supplier", csId).toString();

            if (!ccId.isEmpty() && ccId != "0") {
                RB_String ccCode = oper.selectFromWhereId("centercode",
                                                          "ACC_CostCenter", ccId).toString();
                RB_String ccName = oper.selectFromWhereId("centername",
                                                          "ACC_CostCenter", ccId).toString();
                idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("costcenter_idx"));
                mItemTransModel->setData(idx, ccId + ccCode + " - " + ccName, Qt::EditRole);
            }
        }

    } else if (mTransType == ACC2::TransBankCash) {

        QModelIndex idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("transdoc_id"));
        RB_String id = mTransDocModel->getCurrentId();
        mItemTransModel->setData(idx, id, Qt::EditRole);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("type"), QModelIndex());
        mItemTransModel->setData(idx, (int)ACC2::TransBankCash, Qt::EditRole); // bank/cash document
        // set defaults
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("transdate"), QModelIndex());
        RB_Variant dateTime = mTransDocModel->getCurrentValue("transdate");
        mItemTransModel->setData(idx, dateTime, Qt::EditRole);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("currency_id"), QModelIndex());
        mItemTransModel->setData(idx, "EUR", Qt::EditRole);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("chartmaster_idx"), QModelIndex());
        mItemTransModel->setData(idx, "0", Qt::EditRole);

    } else if (mTransType == ACC2::TransMemo) {

        QModelIndex idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("transdoc_id"));
        RB_String docId = mTransDocModel->getCurrentId();
        mItemTransModel->setData(idx, docId, Qt::EditRole);
        // set defaults
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("chartmaster_idx"), QModelIndex());
        mItemTransModel->setData(idx, "0", Qt::EditRole);

        // set difference in total amount value
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("amount"));
        mItemTransModel->setData(idx, 0.0, Qt::EditRole); // make sure current is zero
        int rowCount = mItemTransModel->rowCount();

        if (rowCount > 1) {
            double total = 0.0;

            for (int i = 0; i < rowCount; ++i) {
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("amount"));
                total += idx.data().toDouble();
            }

            idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("amount"));
            mItemTransModel->setData(idx, -total, Qt::EditRole);
        }

    }
    // end NOTE

    tvItem->setCurrentIndex(mItemTransModel->index(row, mItemTransModel->fieldIndex("description")));
    // copy description from document
    RB_String descr = mTransDocModel->getCurrentValue("description").toString();
    mItemTransModel->setCurrentValue("description", descr, Qt::EditRole);

    tvItem->scrollTo(tvItem->currentIndex());
    leItemDescription->setFocus();
    leItemDescription->selectAll();

    mAddDeleteInProgress = false;
}

/**
 * Button delete clicked
 * Note: although the deletion is save immediately, the transaction
 * as a whole still needs to be saved which requires a balanced transaction.
 * The cleaning up is done during fileSave()
 */
void ACC_GlTransactionWidget::on_pbDeleteItem_clicked() {
    int ret = QMessageBox::No;
    mAddDeleteInProgress = true;

    if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo) {
        ret = DB_DIALOGFACTORY->requestYesNoDialog(
                tr("Delete item ..."),
                tr("This action cannot be undone.\n"
                   "Do you want to delete this item?"));
    } else if (mTransType == ACC2::TransCreditor || mTransType == ACC2::TransDebtor) {
        // Allocated amount
        double allocAmt = mTransDocModel->getCurrentValue("alloc").toDouble();

        if (allocAmt < -0.005 || allocAmt > 0.005) {
            ret = ACC_DIALOGFACTORY->requestYesNoDialog(
                        tr("Delete item ..."),
                        tr("This action cannot be undone.\n"
                           "Do you want to delete this item\n"
                           "and the related allocation?"));
        } else {
            ret = DB_DIALOGFACTORY->requestYesNoDialog(
                    tr("Delete item ..."),
                    tr("This action cannot be undone.\n"
                       "Do you want to delete this item?"));
        }
    }

    if (ret == QMessageBox::No) {
        DB_DIALOGFACTORY->statusBarMessage(tr("Deletion canceled"), 2000);
        mAddDeleteInProgress = false;
        return;
    }

    if (!mItemTransModel || !mTransDocModel->getProxyIndex().isValid()) {
        mAddDeleteInProgress = false;
        return;
    }

    QModelIndex index = tvItem->currentIndex();
    if (!index.isValid()) {
        mAddDeleteInProgress = false;
        return;
    }

    // reverse, remove allocation in case of bank/cash or journal item
    mIsValidateAmounts = true; // prevent single shot for validation to early

    if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo){
        // delete allocation and document settled
        mHandleAllocn.delItemAllocn(mItemTransModel, true);
    } else if (mTransType == ACC2::TransCreditor || mTransType == ACC2::TransDebtor) {
        // not here, only required when amount changes
//        RB_String docId = mTransDocModel->getCurrentId();
//        mHandleAllocn.delDocAllocn(docId, true);
//        mTransDocModel->setCurrentValue("alloc", 0.0, Qt::EditRole);
//        mTransDocModel->setCurrentValue("settled", 0, Qt::EditRole);
    }

    // remove gltrans (which reverses chartdetails/trialbalance), done at fileSave()
    mItemTransModel->removeRows(index.row(), 1, QModelIndex());
//    mItemTransModel->submitAllAndSelect(); might save a non valid transaction
//    mHandleAllocn.submitAllAndSelect();

    mIsValidateAmounts = false;
    setWindowModified(true);

    bool success = isValidTransDoc();
    enableWidgets(success);

    mAddDeleteInProgress = false;
}

/**
 * Button save clicked
 */
void ACC_GlTransactionWidget::on_pbSave_clicked() {
    fileSave(true);
}

/**
 * Button undo clicked, removed because gave errors in acc_gltrans and acc_glsum
 */
void ACC_GlTransactionWidget::on_pbRevert_clicked() {
    int ret = ACC_DIALOGFACTORY->requestYesNoDialog(
                tr("Revert changes ..."),
                tr("Do you want to undo all your changes?"));

    if (ret == QMessageBox::No) {
        ACC_DIALOGFACTORY->statusBarMessage(tr("Revert changes canceled"), 2000);
        return;
    }

    fileRevert();
    tvDocument->setCurrentIndex(QModelIndex());
    enableWidgets(true);
    mIsValidateAmounts = false;
}

/**
 * Find (next) in selected table column
 */
void ACC_GlTransactionWidget::on_pbFind_clicked() {
    RB_Widget::findInTable(leFind->text(), tvDocument);
}

/**
 * Pushbutton total receivable clicked, calculates
 * and sets total receivable amount
 */
void ACC_GlTransactionWidget::on_pbTotalReceivable_clicked() {
    double totalRec = 0.0;
    double amount = 0.0;
    QModelIndex idx;
    int itemRowCount = mItemTransModel->rowCount();
    RB_String acct = ""; // chartmaster_idx

    // debtor transactions, positive bank/cash journal transactions
    for (int i = 0; i < itemRowCount; ++i) {
        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("amount"));
        amount = mItemTransModel->data(idx).toDouble();

        if (mTransType == ACC2::TransDebtor) {
            totalRec += amount;
        } else if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo) {
            totalRec += amount >=0 ? amount : 0.0; // bank/cash and journal can be negative
        }

        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("chartmaster_idx"));
        acct = mItemTransModel->data(idx, RB2::RoleOrigData).toString(); // RB2::RoleOrigData gets ID

        if (mTransType == ACC2::TransDebtor && acct != ACC_QACHARTMASTER->getAccSalesRevId()
                    && acct != ACC_QACHARTMASTER->getAccPurchRevId()) {
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxhighamt"));
            totalRec += mItemTransModel->data(idx).toDouble();
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxlowamt"));
            totalRec += mItemTransModel->data(idx).toDouble();
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxotheramt"));
            totalRec += mItemTransModel->data(idx).toDouble();
        }
    }

    // set total transaction
    int row = tvDocument->currentIndex().row();
    idx = mTransDocModel->index(row, mTransDocModel->fieldIndex("totalamountrec"));
    mTransDocModel->setData(idx, totalRec);

//    slotAmountDocChanged();
//    slotDataIsChanged();
}

/**
 * Pushbutton total payable clicked, calculates
 * and sets total payable amount
 */
void ACC_GlTransactionWidget::on_pbTotalPayable_clicked() {
    double totalPay = 0.0;
    double amount = 0.0;
    QModelIndex idx;
    int itemRowCount = mItemTransModel->rowCount();
    RB_String acct = ""; // chartmaster_idx

    // creditor transactions, negative bank/cash journal transactions
    for (int i = 0; i < itemRowCount; ++i) {
        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("amount"));
        amount = mItemTransModel->data(idx).toDouble();

        if (mTransType == ACC2::TransCreditor) {
            totalPay += amount;
        } else if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo) {
            totalPay += amount < 0 ? -amount : 0.0; // total payable is a positive number
        }
        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("chartmaster_idx"));
        acct = mItemTransModel->data(idx, RB2::RoleOrigData).toString();

        if (mTransType == ACC2::TransCreditor && acct != ACC_QACHARTMASTER->getAccSalesRevId()
                        && acct != ACC_QACHARTMASTER->getAccPurchRevId()) {
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxhighamt"));
            totalPay += mItemTransModel->data(idx).toDouble();
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxlowamt"));
            totalPay += mItemTransModel->data(idx).toDouble();
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxotheramt"));
            totalPay += mItemTransModel->data(idx).toDouble();
        }
    }

    // set total transaction
    int row = tvDocument->currentIndex().row();
    idx = mTransDocModel->index(row, mTransDocModel->fieldIndex("totalamountpay"));
    mTransDocModel->setData(idx, totalPay);
}

/**
 * Calculate and set total amount of tax for this document base on the
 * items in the document
 */
void ACC_GlTransactionWidget::on_pbTotalTax_clicked() {
    if (mTransType != ACC2::TransDebtor && mTransType != ACC2::TransCreditor) {
        return; // overdone because button is hidden
    }

    double totalTax = 0.0;
    QModelIndex idx;
    int itemRowCount = mItemTransModel->rowCount();
    RB_String acct = ""; // chartmaster_idx

    for (int i = 0; i < itemRowCount; ++i) {
        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("chartmaster_idx"));
        acct = mItemTransModel->data(idx, RB2::RoleOrigData).toString();

        if (acct != ACC_QACHARTMASTER->getAccSalesRevId()
                    && acct != ACC_QACHARTMASTER->getAccPurchRevId()) {
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxhighamt"));
            totalTax += mItemTransModel->data(idx).toDouble();
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxlowamt"));
            totalTax += mItemTransModel->data(idx).toDouble();
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxotheramt"));
            totalTax += mItemTransModel->data(idx).toDouble();
        }
    }

    // set total tax
    int row = tvDocument->currentIndex().row();
    idx = mTransDocModel->index(row, mTransDocModel->fieldIndex("totaltax"));
    mTransDocModel->setData(idx, totalTax);
}

/**
 * @brief ACC_GlTransactionWidget::on_pbEditInvoiceItemDetail_clicked
 */
void ACC_GlTransactionWidget::on_pbEditInvoiceItemDetail_clicked() {
    if (!tvItem->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_ObjectBase* obj = mItemTransModel->getCurrentObject();
    QStringList strList;

    if (mTransType == ACC2::TransDebtor) {
        strList << "narrative" << "unitprice" << "qtydispatched" << "stkuom"
                << "discountpercent";
    } else if (mTransType == ACC2::TransCreditor) {
        strList << "narrative" << "unitprice" << "qtyreceived" << "stkuom"
                << "discountpercent";
    }

    RB_MemberEditDialog* dlg = new RB_MemberEditDialog(this);
    dlg->showObjectMembers(obj, strList);
    dlg->exec();

    mItemTransModel->updateCurrentObject(obj);
    dlg->deleteLater();
    enableWidgets(isValidTransDoc());
}

/**
 * Button select item account clicked
 */
void ACC_GlTransactionWidget::on_ileItemAccount_clicked() {
    if (!tvItem->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectChartMaster);

    if (dlg->exec() != QDialog::Accepted) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No account selected.\n"
                                                   "Data is unchanged."));
        dlg->deleteLater();
        return;
    }

    RB_ObjectBase* obj = dlg->getCurrentObject();

    if (!obj) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No account selected.\n"
                                                   "Data is unchanged."));
        dlg->deleteLater();
        return;
    }

    if ((mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor)
            && isDebtorCreditorAccount(obj)) {
        return; // not allowed, do nothing
    }

    // delete allocation if any
    on_ileAllocation_clear();

    QModelIndex index = mItemTransModel->getProxyIndex();
    QModelIndex idx = mItemTransModel->index(
                index.row(), mItemTransModel->fieldIndex("chartmaster_idx"));
    RB_String acctId = obj->getId();
    mItemTransModel->setData(idx, acctId
                    + obj->getValue("accountcode").toString() + " - "
                    + obj->getValue("accountname").toString());

    int row = tvDocument->currentIndex().row();
    QModelIndex custSupIdIdx;
    if (mTransType == ACC2::TransDebtor) {
        custSupIdIdx = mTransDocModel->index(row, mTransDocModel->fieldIndex("debtor_idx"));
    } else if (mTransType == ACC2::TransCreditor) {
        custSupIdIdx = mTransDocModel->index(row, mTransDocModel->fieldIndex("creditor_idx"));
    }
    RB_String csId = mTransDocModel->data(custSupIdIdx, RB2::RoleOrigData).toString();

    ACC_SqlCommonFunctions oper;

    if (mTransType == ACC2::TransDebtor && !csId.isEmpty()) {
        oper.update("ACC_Customer", "lastusedacct_id", acctId, csId);
    } else if (mTransType == ACC2::TransCreditor && !csId.isEmpty()) {
        oper.update("ACC_Supplier", "lastusedacct_id", acctId, csId);
    }

    if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxhighamt"));
        mItemTransModel->setData(idx, 0.0);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxlowamt"));
        mItemTransModel->setData(idx, 0.0);
        idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxotheramt"));
        mItemTransModel->setData(idx, 0.0);
    }

    dlg->deleteLater();
}

/**
 * Button clear/remove allocation clicked
 */
void ACC_GlTransactionWidget::on_ileItemAccount_clear() {
    on_ileAllocation_clear();
}

/**
 * Button allocation clicked, to allocate invoice with payments or journal entry
 */
void ACC_GlTransactionWidget::on_ileAllocation_clicked() {
    if (!tvItem->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    // Open dialog to select transaction document customer (debtor) or supplier (creditor)
    ACC_SelectAllocnDialog* dlg = new ACC_SelectAllocnDialog(this);
    dlg->init();
    dlg->updateAllocns(mHandleAllocn.getTransDocList());

    if (leItemAmount->text().toDouble() >= 0.0) {
        dlg->setFilterType(ACC2::AllocnCustomer);
    } else {
        dlg->setFilterType(ACC2::AllocnSupplier);
    }


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
    mHandleAllocn.addAllocn(mItemTransModel, docToId, docToDspl,
                            mTransDocModel->getCurrentId());

    // Set customer or supplier bank account if still empty
    // TODO: the ACC does not handle multiple bank accounts
    RB_String fieldName = "bankaccountnumber";
    RB_String bankAccount =
            mItemTransModel->getCurrentValue(fieldName).toString();

    if (mTransType == ACC2::TransBankCash && !bankAccount.isEmpty()) {
        RB_String tableName = "acc_customer";
        RB_String custSuppId = obj->getIdValue("debtor_idx").toString();

        if (custSuppId.isEmpty()) {
            tableName = "acc_supplier";
            custSuppId = obj->getIdValue("creditor_idx").toString();
        }

        if (!custSuppId.isEmpty()) {
            custSuppId = custSuppId.remove(38, custSuppId.length());

            // check if a bank account number already exists
            ACC_SqlCommonFunctions f;
            RB_String existingBankAccount =
                    f.selectFromWhereId(fieldName,
                                        tableName, custSuppId).toString();
            if (existingBankAccount.isEmpty()) {
                // set bank account number based on information from the payment
                f.update(tableName, fieldName, bankAccount, custSuppId);
            } else if (existingBankAccount != bankAccount) {
                int result = ACC_DIALOGFACTORY->requestYesNoDialog(
                            tr("Bank Account"),
                            existingBankAccount + tr(" is the existing acount")
                            + ",\n" + bankAccount
                            + tr(" is the current account.") + ".\n"
                            + tr("Replace the existing account?"));
                if (result == QDialog::Accepted) {
                    f.update(tableName, fieldName, bankAccount, custSuppId);
                }
            }
        }
    }

    dlg->deleteLater();
}

/**
 * Button clear/remove allocation clicked
 */
void ACC_GlTransactionWidget::on_ileAllocation_clear() {
    if (!tvItem->currentIndex().isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No item is selected.\n"
                                                   "Allocation is unchanged."));
        return;
    }

    mHandleAllocn.delItemAllocn(mItemTransModel, false);
    mItemTransModel->setCurrentValue("chartmaster_idx", "0", Qt::EditRole);
}

/**
 * Button cost center select clicked
 */
void ACC_GlTransactionWidget::on_ileCostCenter_clicked() {
    QModelIndex index = tvItem->currentIndex();

    if (!index.isValid()) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No transaction item selected.\n"
                                                   "Please select an item first."));
        return;
    }

    RB_Dialog* dlg = ACC_DIALOGFACTORY->getDialog(ACC_DialogFactory::DialogSelectCostCenter);

    if (dlg->exec() != QDialog::Accepted) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center selected.\n"
                                                   "Cost center is unchanged."));
        dlg->deleteLater();
        return;
    }

    RB_ObjectBase* obj = dlg->getCurrentObject();

    if (!obj) {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No cost center selected.\n"
                                                   "Cost center is unchanged."));
        dlg->deleteLater();
        return;
    }

    index = mItemTransModel->index(index.row(),
                              mItemTransModel->fieldIndex("costcenter_idx"));

    RB_String ccId = obj->getId();
    mItemTransModel->setData(index, ccId
                    + obj->getValue("centercode").toString() + " - "
                    + obj->getValue("centername").toString(), Qt::EditRole);

    int row = tvDocument->currentIndex().row();
    QModelIndex custSupIdIdx;
    if (mTransType == ACC2::TransDebtor) {
        custSupIdIdx = mTransDocModel->index(row, mTransDocModel->fieldIndex("debtor_idx"));
    } else if (mTransType == ACC2::TransCreditor) {
        custSupIdIdx = mTransDocModel->index(row, mTransDocModel->fieldIndex("creditor_idx"));
    }
    RB_String csId = mTransDocModel->data(custSupIdIdx, RB2::RoleOrigData).toString();
    ACC_SqlCommonFunctions oper;

    if (mTransType == ACC2::TransDebtor && !csId.isEmpty()) {
        oper.update("ACC_Customer", "lastusedcostcenter_id", ccId, csId);
    } else if (mTransType == ACC2::TransCreditor && !csId.isEmpty()) {
        oper.update("ACC_Supplier", "lastusedcostcenter_id", ccId, csId);
    }

    dlg->deleteLater();
}

void ACC_GlTransactionWidget::on_pbHighTaxAmount_clicked() {
    if (mTransType != ACC2::TransDebtor && mTransType != ACC2::TransCreditor) {
        return; // overdone because button is hidden
    }

    int row = tvItem->currentIndex().row();
    QModelIndex idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("amount"));
    double tax = mItemTransModel->data(idx).toDouble() * sbHighTaxPerc->value() / 100;
    tax = ACC2::roundMoney(tax);
    idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxhighamt"));
    mItemTransModel->setData(idx, tax);
}

void ACC_GlTransactionWidget::on_pbLowTaxAmount_clicked() {
    if (mTransType != ACC2::TransDebtor && mTransType != ACC2::TransCreditor) {
        return; // overdone because button is hidden
    }

    int row = tvItem->currentIndex().row();
    QModelIndex idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("amount"));
    double tax = mItemTransModel->data(idx).toDouble() * sbLowTaxPerc->value() / 100;
    tax = ACC2::roundMoney(tax);
    idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxlowamt"));
    mItemTransModel->setData(idx, tax);
}

void ACC_GlTransactionWidget::on_pbOtherTaxAmount_clicked() {
    if (mTransType != ACC2::TransDebtor && mTransType != ACC2::TransCreditor) {
        return; // overdone because button is hidden
    }

    int row = tvItem->currentIndex().row();
    QModelIndex idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("amount"));
    double tax = mItemTransModel->data(idx).toDouble() * sbOtherTaxPerc->value() / 100;
    tax = ACC2::roundMoney(tax);
    idx = mItemTransModel->index(row, mItemTransModel->fieldIndex("taxotheramt"));
    mItemTransModel->setData(idx, tax);
}

void ACC_GlTransactionWidget::on_ileHighTaxAccount_clicked() {
    taxAccountClicked("taxhighchartmaster_idx");
}

void ACC_GlTransactionWidget::on_ileLowTaxAccount_clicked() {
    taxAccountClicked("taxlowchartmaster_idx");
}

void ACC_GlTransactionWidget::on_ileOtherTaxAccount_clicked() {
    taxAccountClicked("taxotherchartmaster_idx");
}

/**
 * Helper function for the three tax account buttons,
 * all select an account that is stored only in different fields
 * @param taxAccountFieldName tax account field name
 */
void ACC_GlTransactionWidget::taxAccountClicked(const QString& taxAccountFieldName) {
    if (!mItemTransModel || !mItemTransModel->getProxyIndex().isValid()) {
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

            QModelIndex index = mItemTransModel->getProxyIndex();
            QModelIndex idx = mItemTransModel->index(
                        index.row(), mItemTransModel->fieldIndex(taxAccountFieldName));
            mItemTransModel->setData(idx, obj->getId()
                            + obj->getValue("accountcode").toString() + " - "
                            + obj->getValue("accountname").toString());

            // TODO: test, short would be:
            // mItemTransModel->setCurrentValue(taxAccountFieldName, obj->getId()
            //            + obj->getValue("accountcode").toString() + " - "
            //            + obj->getValue("accountname").toString());

        }
    } else {
        ACC_DIALOGFACTORY->requestWarningDialog(tr("No account selected.\n"
                                                   "Data is unchanged."));
    }

    dlg->deleteLater();
}

/**
 * Is the user trying to book directly against the debtor or creditor
 * account, because this is not allowed when working with sales or
 * purchase invoices.
 * @param actObj account object (ACC_ChartMaster)
 * @return true if account object is of debtor or creditor GL account
 */
bool ACC_GlTransactionWidget::isDebtorCreditorAccount(RB_ObjectBase* actObj) {
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
 * Slot selected Ledger Account changed
 * @param number index of combobox or row in model
 */
void ACC_GlTransactionWidget::slotLedgerAccountChanged(int number) {
    if (!mInitInProgress) {
        if (!maybeSave()) return;
    }

    int col = cbLedgerAccount->modelColumn();
    QModelIndex idx = mChartMasterModel->index(number, mChartMasterModel->fieldIndex("accountcontrol"));
    int accountType = mChartMasterModel->data(idx, Qt::EditRole).toInt();

    if (accountType == ACC2::ControlDebtor) {
        mTransType = ACC2::TransDebtor;
    } else if (accountType == ACC2::ControlCreditor) {
        mTransType = ACC2::TransCreditor;
    } else if (ACC2::ControlCash <= accountType && accountType < ACC2::ControlMemoBook) {
        mTransType = ACC2::TransBankCash;
    } else /* if (accountType == ACC2::ControlMemoBook) */ {
        mTransType = ACC2::TransMemo; // Memoriaal
    }

    idx = mChartMasterModel->index(number, col, QModelIndex());
    mTransDocModel->slotParentCurrentRowChanged(idx, QModelIndex());
    setDocWidgets(mTransType);
    setItemModel(mTransType);

    // Set the Account Bank/Cash ID and name for later use.
    // These values are frequently used when the user edits the allocation of
    // sales- and purchase invoices to bank/cash transactions
    if (mTransType == ACC2::TransBankCash) {
        idx = mChartMasterModel->index(number, mChartMasterModel->fieldIndex("id"));
        ACC_QACHARTMASTER->setAccBankId(mChartMasterModel->data(idx, Qt::EditRole).toString());
        RB_String str;
        idx = mChartMasterModel->index(number, mChartMasterModel->fieldIndex("accountcode"));
        str = mChartMasterModel->data(idx, Qt::EditRole).toString() + " - ";
        idx = mChartMasterModel->index(number, mChartMasterModel->fieldIndex("accountname"));
        str += mChartMasterModel->data(idx, Qt::EditRole).toString();
        ACC_QACHARTMASTER->setAccBankName(str);
    }
}

/**
 * Slot tab changed
 * @param tabNo new selected tab number
 */
void ACC_GlTransactionWidget::slotTabChanged(int tabNo) {
    if (tabNo == 1) {
        // update the table widget twGlTransactions
        updateGlTransWidget();
    }
}

/**
 * Slot handling check of data. As a result enables or disables
 * the widgets if transaction is not valid. Used for:
 * \li all models - set window modified
 * \li doc model (bankcash or memo) transdate - update transaction dates, is valid transdoc
 * \li item model (all) chartmaster_idx - is valid transdoc
 * \li doc model (debtor or creditor) totalamountrec, -pay - handle allocation, is valid transdoc
 * \li item model (bankcash or memo) amount - handle allocation, is valid transdoc
 * \li doc model totaltax, item model charmastertax taxes - is valid transdoc
 * Not for deletion of item or document.
 * @param current current index in model
 */
void ACC_GlTransactionWidget::slotDataIsChanged(const QModelIndex& current,
                                              const QModelIndex& previous) {
    if (!mTransDocModel || !mItemTransModel) {
        return;
    }

    RB_Widget::slotDataIsChanged(current, previous);

    if (mAddDeleteInProgress) {
        return;
    }

    // Bank or memorial document date changed
    if ((mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo)
            && current.model() == mTransDocModel
            && current.column() == mTransDocModel->fieldIndex("transdate")) {
        mHandleAllocn.updateTransDate(mItemTransModel, current.data().toDateTime());
        bool success = isValidTransDoc();
        enableWidgets(success);
    }

    // Item GL chartmaster account changed
    if (current.model() == mItemTransModel && current.column() == mItemTransModel->fieldIndex("chartmaster_idx")) {
        // allocation already handled by button actions
        bool success = isValidTransDoc();
        enableWidgets(success);
    }

    // Amounts changed
    if ((current.model() == mTransDocModel && current.column() == mTransDocModel->fieldIndex("totalamountrec"))
            || (current.model() == mTransDocModel && current.column() == mTransDocModel->fieldIndex("totalamountpay"))) {
        if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
            double prevAllocAmt = mTransDocModel->getCurrentValue("alloc", Qt::EditRole).toDouble();

            if (std::fabs(prevAllocAmt) > 0.005) {
                // Show message of allocation deletion and bank/memo transaction numbers
                ACC_DIALOGFACTORY->requestInformationDialog(tr("The related allocation(s)\n"
                                                               "will be removed."));
                mHandleAllocn.delDocAllocn(mTransDocModel->getCurrentId(), false);
                mTransDocModel->setCurrentValue("alloc", 0.0, Qt::EditRole);
                mTransDocModel->setCurrentValue("settled", 0, Qt::EditRole);
            }
        }

        bool success = isValidTransDoc();
        enableWidgets(success);
    }

    if (current.model() == mItemTransModel && current.column() == mItemTransModel->fieldIndex("amount")) {
        if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo) {
            // Check if there is an allocation, already in ACC_HandleAllocn
//            RB_String allocId = mItemTransModel->getCurrentValue("transallocn_idx",
//                                                            RB2::RoleOrigData).toString();
//            if (allocId.size() >= 38) {
                // Amount from bank/cash (ACC_BankTrans) or from journal (ACC_GlTrans)
                double amt = current.data().toDouble(); // leItemAmount->text().toDouble();

                // a bank/cash amount change to a larger amount
                // is reversed to maximum allocatable by below operation
                mHandleAllocn.updateItemAllocnAmt(mItemTransModel, amt);
//            }
        }

        bool success = isValidTransDoc();
        enableWidgets(success);
    }

    if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
        if ((current.model() == mTransDocModel && current.column() == mTransDocModel->fieldIndex("totaltax"))
                || (current.model() == mItemTransModel && (current.column() == mItemTransModel->fieldIndex("taxhighamt")
                                                      || current.column() == mItemTransModel->fieldIndex("taxlowamt")
                                                      || current.column() == mItemTransModel->fieldIndex("taxotheramt")
                                                      || current.column() == mItemTransModel->fieldIndex("taxhighchartmaster_idx")
                                                      || current.column() == mItemTransModel->fieldIndex("taxlowchartmaster_idx")
                                                      || current.column() == mItemTransModel->fieldIndex("taxotherchartmaster_idx")))) {
            bool success = isValidTransDoc();
            enableWidgets(success);
        }
    }
}

void ACC_GlTransactionWidget::slotValidateAmounts() {
    mIsValidateAmounts = false;
}

/**
 * Slot called when bank statement have been imported, will
 * refresh the document model in case Bank/Cash is selected.
 * Does nothing if data is modified/window is dirty.
 * @brief ACC_GlTransactionWidget::slotBankStatementsImported
 */
void ACC_GlTransactionWidget::slotRefreshBankModel() {
    if (mTransType != ACC2::TransBankCash) {
        return;
    } else if (isWindowModified()) {
        RB_DEBUG->error("ACC_GlTransactionWidget::slotBankStatementsImported()");
        return;
    }

    fileSave(true);
}

/**
 * Set document widgets based on transaction type
 * @param type transaction type
 */
void ACC_GlTransactionWidget::setDocWidgets(ACC2::TransType type) {
    int count = mTransDocModel->columnCount(QModelIndex());
    for (int i = 0; i < count; ++i) {
       if (i != mTransDocModel->fieldIndex("transdate")
               && i != mTransDocModel->fieldIndex("transno")
               && i != mTransDocModel->fieldIndex("description")
               && i != mTransDocModel->fieldIndex("creditor_idx")
               && i != mTransDocModel->fieldIndex("debtor_idx")
               && i != mTransDocModel->fieldIndex("totalamountrec")
               && i != mTransDocModel->fieldIndex("totalamountpay")) {
           tvDocument->hideColumn(i);
       } else {
           tvDocument->showColumn(i);

           if (tvDocument->columnWidth(i) < 5) {
               tvDocument->setColumnWidth(i, 100);
           }
       }
    }

    switch (type) {
    case ACC2::TransDebtor : {
        lblBalance->hide();
        leBalance->hide();
        pbCheckBalance->hide();

        tvDocument->hideColumn(mTransDocModel->fieldIndex("creditor_idx"));
        tvDocument->showColumn(mTransDocModel->fieldIndex("debtor_idx"));
        tvDocument->showColumn(mTransDocModel->fieldIndex("totalamountrec"));
        tvDocument->hideColumn(mTransDocModel->fieldIndex("totalamountpay"));

        lblCustomer->setText(tr("Customer"));
        lblCustomer->show();
        ileCustomer->show();
        ileSupplier->hide();
        pbReprintInvoice->show();
        pbReprintPurchaseOrder->hide();

        lblReferenceNo->show();
        leReferenceNo->show();

        lblTotalRec->show();
        leTotalRec->show();
        lblTotalPay->hide();
        leTotalPay->hide();

        lblTotalTax->show();
        leTotalTax->show();
        pbTotalTax->show();

        lblDueDate->show();
        deDueDate->show();

        break;
    }
    case ACC2::TransCreditor : {
        lblBalance->hide();
        leBalance->hide();
        pbCheckBalance->hide();

        tvDocument->showColumn(mTransDocModel->fieldIndex("creditor_idx"));
        tvDocument->hideColumn(mTransDocModel->fieldIndex("debtor_idx"));
        tvDocument->hideColumn(mTransDocModel->fieldIndex("totalamountrec"));
        tvDocument->showColumn(mTransDocModel->fieldIndex("totalamountpay"));

        lblCustomer->setText(tr("Supplier"));
        lblCustomer->show();
        ileCustomer->hide();
        ileSupplier->show();
        pbReprintInvoice->hide();
        pbReprintPurchaseOrder->show();

        lblReferenceNo->show();
        leReferenceNo->show();

        lblTotalRec->hide();
        leTotalRec->hide();
        lblTotalPay->show();
        leTotalPay->show();

        lblTotalTax->show();
        leTotalTax->show();
        pbTotalTax->show();

        lblDueDate->show();
        deDueDate->show();

        break;
    }
    case ACC2::TransBankCash : {
        lblBalance->show();
        leBalance->show();
        pbCheckBalance->show();

        tvDocument->hideColumn(mTransDocModel->fieldIndex("creditor_idx"));
        tvDocument->hideColumn(mTransDocModel->fieldIndex("debtor_idx"));
        tvDocument->showColumn(mTransDocModel->fieldIndex("totalamountrec"));
        tvDocument->showColumn(mTransDocModel->fieldIndex("totalamountpay"));

        lblCustomer->hide();
        ileCustomer->hide();
        ileSupplier->hide();
        pbReprintInvoice->hide();
        pbReprintPurchaseOrder->hide();

        lblReferenceNo->hide();
        leReferenceNo->hide();

        lblTotalRec->show();
        leTotalRec->show();
        lblTotalPay->show();
        leTotalPay->show();

        lblTotalTax->hide();
        leTotalTax->hide();
        pbTotalTax->hide();

        lblDueDate->hide();
        deDueDate->hide();

        break;
    }
    case ACC2::TransMemo : {// Memorandum
        lblBalance->hide();
        leBalance->hide();
        pbCheckBalance->hide();

        tvDocument->hideColumn(mTransDocModel->fieldIndex("creditor_idx"));
        tvDocument->hideColumn(mTransDocModel->fieldIndex("debtor_idx"));
        tvDocument->hideColumn(mTransDocModel->fieldIndex("totalamountrec"));
        tvDocument->hideColumn(mTransDocModel->fieldIndex("totalamountpay"));

        lblCustomer->hide();
        ileCustomer->hide();
        ileSupplier->hide();
        pbReprintInvoice->hide();
        pbReprintPurchaseOrder->hide();

        lblReferenceNo->hide();
        leReferenceNo->hide();

        lblTotalRec->hide();
        leTotalRec->hide();
        lblTotalPay->hide();
        leTotalPay->hide();

        lblTotalTax->hide();
        leTotalTax->hide();
        pbTotalTax->hide();

        lblDueDate->hide();
        deDueDate->hide();

        break;
    }
    default :
        break;
    }
}

/**
 * Set document item model and widgets based on transaction type
 * @param type transaction type
 */
void ACC_GlTransactionWidget::setItemModel(ACC2::TransType type) {
    if (!mInitInProgress) {
        disconnect(mTransDocModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                   mItemTransModel, SLOT(slotParentCurrentRowChanged(QModelIndex,QModelIndex)));
        disconnect(tvItem->selectionModel(),
                   SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
                   mItemTransModel, SLOT(slotChangeCurrentRow(const QModelIndex&, const QModelIndex&)));
        disconnect(mItemTransModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
                   this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
    }

    if (mItemTransModel) {
        // HACK:
        tvItem->setModel(NULL);
        delete mItemTransModel;
        mItemTransModel = NULL;
        mItemTransMapper = NULL; // deleted with model
    }

    switch (type) {
    case ACC2::TransBankCash : {
        mItemTransModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelBankTrans);
        mItemTransMapper = mItemTransModel->getMapper();
        mItemTransMapper->addMapping(leItemDescription, mItemTransModel->fieldIndex("description"));
        mItemTransMapper->addMapping(ileItemAccount,
                                mItemTransModel->fieldIndex("chartmaster_idx"));
        mItemTransMapper->addMapping(ileAllocation,
                                mItemTransModel->fieldIndex("transallocn_idx"));
        mItemTransMapper->addMapping(leItemAmount, mItemTransModel->fieldIndex("amount"));
        // Bank can be negative or positive value (debit or credit)
//        leItemAmount->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));
        mItemTransMapper->addMapping(leItemBankAccount, mItemTransModel->fieldIndex("bankaccountnumber"));
        mItemTransMapper->addMapping(deItemDate, mItemTransModel->fieldIndex("transdate"));

        pbTotalPayable->show();
        pbTotalReceivable->show();
        pbEditInvoiceItemDetail->hide();
        lblAllocation->show();
        ileAllocation->show();
        lblDebitCredit->show();
        lblIleCostCenter->hide();
        ileCostCenter->hide();
        lblItemDate->show();
        deItemDate->show();
        lblItemBankAccount->show();
        leItemBankAccount->show();
        gbItemTax->hide();
        break;
    }
    case ACC2::TransCreditor : {
        mItemTransModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCreditorTrans);
        mItemTransMapper = mItemTransModel->getMapper();
        mItemTransMapper->addMapping(leItemDescription, mItemTransModel->fieldIndex("description"));
        mItemTransMapper->addMapping(ileItemAccount,
                                mItemTransModel->fieldIndex("chartmaster_idx"));
        mItemTransMapper->addMapping(leItemAmount, mItemTransModel->fieldIndex("amount"));
//        leItemAmount->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));
        mItemTransMapper->addMapping(ileCostCenter,
                                mItemTransModel->fieldIndex("costcenter_idx"));
        mItemTransMapper->addMapping(ileHighTaxAccount,
                                mItemTransModel->fieldIndex("taxhighchartmaster_idx"));
        mItemTransMapper->addMapping(ileLowTaxAccount,
                                mItemTransModel->fieldIndex("taxlowchartmaster_idx"));
        mItemTransMapper->addMapping(ileOtherTaxAccount,
                                mItemTransModel->fieldIndex("taxotherchartmaster_idx"));
        mItemTransMapper->addMapping(leHighTaxAmt, mItemTransModel->fieldIndex("taxhighamt"));
//        leHighTaxAmt->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));
        mItemTransMapper->addMapping(leLowTaxAmt, mItemTransModel->fieldIndex("taxlowamt"));
//        leLowTaxAmt->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));
        mItemTransMapper->addMapping(leOtherTaxAmt, mItemTransModel->fieldIndex("taxotheramt"));
//        leOtherTaxAmt->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));

        pbTotalPayable->show();
        pbTotalReceivable->hide();
        pbEditInvoiceItemDetail->show();
        lblAllocation->hide();
        ileAllocation->hide();
        lblDebitCredit->hide();
        lblIleCostCenter->show();
        ileCostCenter->show();
        lblItemDate->hide();
        deItemDate->hide();
        lblItemBankAccount->hide();
        leItemBankAccount->hide();
        gbItemTax->show();
        break;
    }
    case ACC2::TransDebtor : {
        mItemTransModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelDebtorTrans);
        mItemTransMapper = mItemTransModel->getMapper();
        mItemTransMapper->addMapping(leItemDescription, mItemTransModel->fieldIndex("description"));
        mItemTransMapper->addMapping(ileItemAccount,
                                mItemTransModel->fieldIndex("chartmaster_idx"));
        mItemTransMapper->addMapping(leItemAmount, mItemTransModel->fieldIndex("amount"));
//        leItemAmount->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));
        mItemTransMapper->addMapping(ileCostCenter,
                                mItemTransModel->fieldIndex("costcenter_idx"));
        mItemTransMapper->addMapping(ileHighTaxAccount,
                                mItemTransModel->fieldIndex("taxhighchartmaster_idx"));
        mItemTransMapper->addMapping(ileLowTaxAccount,
                                mItemTransModel->fieldIndex("taxlowchartmaster_idx"));
        mItemTransMapper->addMapping(ileOtherTaxAccount,
                                mItemTransModel->fieldIndex("taxotherchartmaster_idx"));
        mItemTransMapper->addMapping(leHighTaxAmt, mItemTransModel->fieldIndex("taxhighamt"));
//        leHighTaxAmt->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));
        mItemTransMapper->addMapping(leLowTaxAmt, mItemTransModel->fieldIndex("taxlowamt"));
//        leLowTaxAmt->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));
        mItemTransMapper->addMapping(leOtherTaxAmt, mItemTransModel->fieldIndex("taxotheramt"));
//        leOtherTaxAmt->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));

        pbTotalPayable->hide();
        pbTotalReceivable->show();
        pbEditInvoiceItemDetail->show();
        lblAllocation->hide();
        ileAllocation->hide();
        lblDebitCredit->hide();
        lblIleCostCenter->show();
        ileCostCenter->show();
        lblItemDate->hide();
        deItemDate->hide();
        lblItemBankAccount->hide();
        leItemBankAccount->hide();
        gbItemTax->show();
        break;
    }
    case ACC2::TransMemo : {
        mItemTransModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelMemoTrans);
        mItemTransMapper = mItemTransModel->getMapper();
        mItemTransMapper->addMapping(leItemDescription, mItemTransModel->fieldIndex("description"));
        mItemTransMapper->addMapping(ileItemAccount,
                                mItemTransModel->fieldIndex("chartmaster_idx"));
        mItemTransMapper->addMapping(ileAllocation,
                                mItemTransModel->fieldIndex("transallocn_idx"));
        mItemTransMapper->addMapping(leItemAmount, mItemTransModel->fieldIndex("amount"));
//        leItemAmount->setValidator(new QDoubleValidator(-999999.99, 999999.99, 2, this));

        pbTotalPayable->hide();
        pbTotalReceivable->hide();
        pbEditInvoiceItemDetail->hide();
        lblAllocation->show();
        ileAllocation->show();
        lblDebitCredit->show();
        lblIleCostCenter->hide();
        ileCostCenter->hide();
        lblItemDate->hide();
        deItemDate->hide();
        lblItemBankAccount->hide();
        leItemBankAccount->hide();
        gbItemTax->hide();
        break;
    }
    default :
        return;
    }

    // Further custom set model
    mItemTransModel->setParentManager(mTransDocModel);
    mItemTransModel->setRoot(ACC_MODELFACTORY->getRootId());
    mItemTransModel->setSecondParent("transdoc_id");
    mItemTransModel->setRoot("NONE"); // to deselect all previous and do a select(), do not use ""

    // attach model to main view
    if (tvItem->selectionModel()) {
        delete tvItem->selectionModel();
    }
    QItemSelectionModel* selModel = new QItemSelectionModel(mItemTransModel);
    tvItem->setModel(mItemTransModel);
    tvItem->setSelectionModel(selModel);
    mItemTransModel->setSelectionModel(selModel);
    tvItem->setAlternatingRowColors(true);
    tvItem->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tvItem->setSortingEnabled(true);
    mItemTransModel->undoFilterSort();
    tvItem->verticalHeader()->setDefaultSectionSize(20); // row height
    if (tvItem->itemDelegate()) {
        delete tvItem->itemDelegate();
    }
    tvItem->setItemDelegate(new RB_SqlRelationalDelegate(mItemTransModel, this));

    switch (type) {
    case ACC2::TransBankCash : {
        for (int i = 0; i < mItemTransModel->columnCount(QModelIndex()); ++i) {
            if (i != mItemTransModel->fieldIndex("description")
                    && i != mItemTransModel->fieldIndex("chartmaster_idx")
                    && i != mItemTransModel->fieldIndex("amount")
                    && i != mItemTransModel->fieldIndex("bankaccountnumber")
                    && i != mItemTransModel->fieldIndex("transdate")) {
                tvItem->hideColumn(i);
            } else {
                tvItem->showColumn(i);
                int width = tvItem->width();

                // TODO: improve and save setting per type
                // 3 columns @ 100 with 50 for left header and slider
                if (width > 350) {
                    int extraWidth = width - 350;

                    if (i == mItemTransModel->fieldIndex("description")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.75);
                    } else if (i == mItemTransModel->fieldIndex("chartmaster_idx")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.25);
                    }
                }

                if (tvItem->columnWidth(i) < 5) {
                    tvItem->setColumnWidth(i, 100);
                }
            }
        }
        break;
    }
    case ACC2::TransCreditor : {
        for (int i = 0; i < mItemTransModel->columnCount(QModelIndex()); ++i) {
            if (i != mItemTransModel->fieldIndex("description")
                && i != mItemTransModel->fieldIndex("chartmaster_idx")
                && i != mItemTransModel->fieldIndex("amount")
                && i != mItemTransModel->fieldIndex("taxhighamt")
                && i != mItemTransModel->fieldIndex("taxlowamt")
                && i != mItemTransModel->fieldIndex("taxotheramt")
                && i != mItemTransModel->fieldIndex("stk_idx")) {
                tvItem->hideColumn(i);
            } else {
                tvItem->showColumn(i);
                int width = tvItem->width();

                // 7 columns @ 100 with 50 for left header and slider
                if (width > 750) {
                    int extraWidth = width - 750;

                    if (i == mItemTransModel->fieldIndex("description")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.75);
                    } else if (i == mItemTransModel->fieldIndex("chartmaster_idx")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.25);
                    }
                }

                if (tvItem->columnWidth(i) < 5) {
                    tvItem->setColumnWidth(i, 100);
                }
            }
        }
        break;
    }
    case  ACC2::TransDebtor : {
        for (int i = 0; i < mItemTransModel->columnCount(QModelIndex()); ++i) {
            if (i != mItemTransModel->fieldIndex("description")
                && i != mItemTransModel->fieldIndex("chartmaster_idx")
                && i != mItemTransModel->fieldIndex("amount")
                && i != mItemTransModel->fieldIndex("taxhighamt")
                && i != mItemTransModel->fieldIndex("taxlowamt")
                && i != mItemTransModel->fieldIndex("taxotheramt")
                && i != mItemTransModel->fieldIndex("stk_idx")) {
                tvItem->hideColumn(i);
            } else {
                tvItem->showColumn(i);
                int width = tvItem->width();

                // 7 columns @ 100 with 50 for left header and slider
                if (width > 750) {
                    int extraWidth = width - 750;

                    if (i == mItemTransModel->fieldIndex("description")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.75);
                    } else if (i == mItemTransModel->fieldIndex("chartmaster_idx")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.25);
                    }
                }

                if (tvItem->columnWidth(i) < 5) {
                    tvItem->setColumnWidth(i, 100);
                }
            }
        }
        break;
    }
    case ACC2::TransMemo : {
        for (int i = 0; i < mItemTransModel->columnCount(QModelIndex()); ++i) {
            if (i != mItemTransModel->fieldIndex("description")
                && i != mItemTransModel->fieldIndex("chartmaster_idx")
                && i != mItemTransModel->fieldIndex("amount")) {
                tvItem->hideColumn(i);
            } else {
                tvItem->showColumn(i);
                int width = tvItem->width();

                // 3 columns @ 100 with 50 for left header and slider
                if (width > 350) {
                    int extraWidth = width - 350;

                    if (i == mItemTransModel->fieldIndex("description")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.75);
                    } else if (i == mItemTransModel->fieldIndex("chartmaster_idx")) {
                        tvItem->setColumnWidth(i, 100 + extraWidth * 0.25);
                    }
                }

                if (tvItem->columnWidth(i) < 5) {
                    tvItem->setColumnWidth(i, 100);
                }
            }
        }
        break;
    }
    default :
        break;
    }

    // Tax
    if (type == ACC2::TransCreditor || type == ACC2::TransDebtor) {
        sbHighTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxHighPercentage());
        sbLowTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxLowPercentage());
        sbOtherTaxPerc->setValue(ACC_QACHARTMASTER->getAccSalesTaxOtherPercentage());
    }

    // Change selection of item model
    connect(mTransDocModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            mItemTransModel, SLOT(slotParentCurrentRowChanged(QModelIndex,QModelIndex)));
    // Update mapper fields
    connect(tvItem->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            mItemTransModel, SLOT(slotChangeCurrentRow(const QModelIndex&, const QModelIndex&)));
    // Set window modified and validate the transaction document
    connect(mItemTransModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDataIsChanged(QModelIndex,QModelIndex)));
}

/**
 * Enable/disable widgets while editing the document. The widgets
 * cbLedgerAccount, pbAddDoc and tvDocument are affected
 * @param enable is true to enable the widgets
 */
void ACC_GlTransactionWidget::enableWidgets(bool enable) {
    if (isWindowModified()) {
        cbLedgerAccount->setEnabled(false);
        pbAddDoc->setEnabled(false);
        pbDeleteDoc->setEnabled(false);
        pbFind->setEnabled(false);
        tvDocument->setEnabled(false);
    } else {
        cbLedgerAccount->setEnabled(true);
        pbAddDoc->setEnabled(true);
        pbDeleteDoc->setEnabled(true);
        pbFind->setEnabled(true);
        tvDocument->setEnabled(true);
    }

    pbSave->setEnabled(enable);
}

/**
 * Update GL transaction widget
 */
void ACC_GlTransactionWidget::updateGlTransWidget() {
    double totalPositiveAmount = 0.0;
    double totalNegativeAmount = 0.0;
    updateGlTransactions();

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
    twGlTransactions->setColumnWidth(1, 200);
    twGlTransactions->setColumnWidth(2, 180);

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
 * Update GL transactions
 */
void ACC_GlTransactionWidget::updateGlTransactions() {
    if (isWindowModified()) {
        // new if window is modified
        createNewGlTrans();
    } else {
        // use existing if window is not modified
        createExistingGlTrans();
    }
}

/**
 * Create new GL transactions in case user has edited the transactions
 * TODO in case amount is zero and only tax exists do not create glTrans
 * of zero for the amount but only for the tax, this is the case with
 * salesorder and complex taxation
 * @param clearList true if transaction list should be cleared
 */
void ACC_GlTransactionWidget::createNewGlTrans() {
    double totalAmount = 0.0;
    double taxRev = 0.0;
    double totalDebit = 0.0;
    double totalCredit = 0.0; // negative amounts
    RB_String taxAcctId = "";
    RB_String taxAcctName = "";
    RB_ObjectBase* gltrans = NULL;
    QModelIndex idxItem = tvItem->currentIndex();
    QModelIndex idx;

    if (mGlTransList->countMember() > 0) mGlTransList->erase();

    mRootId = ACC_MODELFACTORY->getRootId();
    mType = mTransDocModel->getCurrentValue("doctype").toInt();
    mTypeNo = mTransDocModel->getCurrentValue("transno").toInt();
    mDate = mTransDocModel->getCurrentValue("transdate", Qt::DisplayRole).toString();
    QDateTime dateTime = mTransDocModel->getCurrentValue("transdate", Qt::DisplayRole).toDateTime();
    mPeriod = dateTime.date().year() * 100 + dateTime.date().month();
    mDocId = mTransDocModel->getCurrentId();
    RB_String docDescr = mTransDocModel->getCurrentValue("description", Qt::DisplayRole).toString();
    RB_String itemDescr = "";
    RB_String accountId = "";
    double amount = 0.0;
    int itemRowCount = mItemTransModel->rowCount();

    for (int i = 0; i < itemRowCount; ++i) {
        // add transaction per item
        gltrans = ACC_OBJECTFACTORY->newObject(RB_Uuid::createUuid().toString(), mGlTransList, "", true);

        // set parent id since no parent object of mGlTransList
        gltrans->setValue("parent", mRootId);
        // Note: in imported bank date is set by the imported transaction items
        // not based on the transaction document date
        gltrans->setValue("transdate", mDate);
        gltrans->setValue("type", mType);
        gltrans->setValue("transdocno", mTypeNo);
        gltrans->setValue("periodno", mPeriod);
        gltrans->setValue("transdoc_id", mDocId);

        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("description"));
        itemDescr = idx.isValid() ? mItemTransModel->data(idx).toString() : "ERROR";
        gltrans->setValue("description", itemDescr);
        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("chartmaster_idx"));
        RB_String accountName = mItemTransModel->data(idx).toString();
        accountId = mItemTransModel->data(idx, RB2::RoleOrigData).toString();
        gltrans->setValue("chartmaster_idx", accountId + accountName);

        if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("costcenter_idx"));
            RB_String costCenterName = mItemTransModel->data(idx).toString();
            RB_String costCenterId = mItemTransModel->data(idx, RB2::RoleOrigData).toString();
            gltrans->setValue("costcenter_idx", costCenterId + costCenterName);
        }

        // accountcontrol
        RB_ObjectBase* aObj = ACC_QACHARTMASTER->getAcctObj(accountId);
        if (aObj) {
            gltrans->setValue("accountcontrol", aObj->getValue("accountcontrol"));
        } else {
            gltrans->setValue("accountcontrol", 0);
        }

        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("amount"));
        amount = mItemTransModel->data(idx).toDouble();

        if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransBankCash
                || mTransType == ACC2::TransMemo) {
            gltrans->setValue("amount", -amount); // credit
        } else {
            gltrans->setValue("amount", amount); // debit
        }

        totalAmount += amount;

        // set allocation id and reference for bank, memorandum transaction and totals,
        if (mTransType == ACC2::TransBankCash || mTransType == ACC2::TransMemo) {
            RB_String str;
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transallocn_idx"));
            str += mItemTransModel->data(idx, Qt::EditRole).toString();
            gltrans->setValue("transallocn_idx", str);

            // totals for bank, debit/credit is separate for bank only
            if (amount >= 0.0) {
                totalDebit += amount;
            } else {
                totalCredit += amount; // negative amounts
            }
        }

        bool isTax = ACC_QACHARTMASTER->isTaxableAccount(accountId);
        RB_ObjectBase* chartObj = ACC_QACHARTMASTER->getAcctObj(accountId);
        int itemAcctControl = 0;

        if (chartObj) {
            itemAcctControl = chartObj->getValue("accountcontrol").toInt();
        }

        // Sales taxes for regular debtor and creditor
        if ((mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor)
                                            && isTax) {
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxhighamt"));
            amount = mItemTransModel->data(idx).toDouble();

            if (std::fabs(amount) > 0.005) {
                // not necessary:  && accountId != mSalesRevId && accountId != mPurchRevId) {
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxhighchartmaster_idx"));
                taxAcctId = mItemTransModel->data(idx, RB2::RoleOrigData).toString();
                taxAcctName = mItemTransModel->data(idx, Qt::DisplayRole).toString();

                if (mTransType == ACC2::TransDebtor) {
                    createNewHelper(itemDescr, taxAcctName, taxAcctId, -amount); // credit
                } else {
                    createNewHelper(itemDescr, taxAcctName, taxAcctId, amount); // debit
                }

                totalAmount += amount;
            }

            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxlowamt"));
            amount = mItemTransModel->data(idx).toDouble();

            if (std::fabs(amount) > 0.005) {
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxlowchartmaster_idx"));
                taxAcctId = mItemTransModel->data(idx, RB2::RoleOrigData).toString();
                taxAcctName = mItemTransModel->data(idx, Qt::DisplayRole).toString();

                if (mTransType == ACC2::TransDebtor) {
                    createNewHelper(itemDescr, taxAcctName, taxAcctId, -amount); // credit
                } else {
                    createNewHelper(itemDescr, taxAcctName, taxAcctId, amount); // debit
                }

                totalAmount += amount;
            }

            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxotheramt"));
            amount = mItemTransModel->data(idx).toDouble();

            if (std::fabs(amount) > 0.005) {
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxotherchartmaster_idx"));
                taxAcctId = mItemTransModel->data(idx, RB2::RoleOrigData).toString();
                taxAcctName = mItemTransModel->data(idx, Qt::DisplayRole).toString();

                if (mTransType == ACC2::TransDebtor) {
                    createNewHelper(itemDescr, taxAcctName, taxAcctId, -amount); // credit
                } else {
                    createNewHelper(itemDescr, taxAcctName, taxAcctId, amount); // debit
                }

                totalAmount += amount;
            }
        } // end if regular debtor or creditor

        // Add transactions for reversed tax per item, based on taxotheramt
        // if function 14 Sales reversed tax
        // or function 34 Purchase reversed tax
        // tax reversed
        if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
            if (itemAcctControl == ACC2::ControlSalesRevTax /*14*/
                    || itemAcctControl == ACC2::ControlPurchaseRevTax /*34*/) {
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxotheramt"));
                taxRev = mItemTransModel->data(idx).toDouble();
            }

            if (itemAcctControl == ACC2::ControlSalesRevTax && std::fabs(taxRev) > 0.005) {
                // function 25 and 24
                createNewHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxSalesPayRevName(),
                                ACC_QACHARTMASTER->getAccTaxSalesPayRevId(), -taxRev); // credit
                createNewHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxSalesRecRevName(),
                                ACC_QACHARTMASTER->getAccTaxSalesRecRevId(), taxRev); // debit
            } else if (itemAcctControl == ACC2::ControlPurchaseRevTax && std::fabs(taxRev) > 0.005) {
                // function 45 and 44
                createNewHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxPurchPayRevName(),
                                ACC_QACHARTMASTER->getAccTaxPurchPayRevId(), -taxRev); // credit
                createNewHelper(itemDescr, ACC_QACHARTMASTER->getAccTaxPurchRecRevName(),
                                ACC_QACHARTMASTER->getAccTaxPurchRecRevId(), taxRev); // debit
            }
        }

        taxRev = 0.0;
    } // for (item)

    // add corresponding transaction for bank/cash, debitor and creditor accountId
    if (mTransType == ACC2::TransBankCash) {
        createNewHelper(docDescr, ACC_QACHARTMASTER->getAccBankName(),
                        ACC_QACHARTMASTER->getAccBankId(), totalDebit); // debit separate TODO: combine debit credit
        createNewHelper(docDescr, ACC_QACHARTMASTER->getAccBankName(),
                        ACC_QACHARTMASTER->getAccBankId(), totalCredit); // credit separate TODO: combine debit credit
    } else if (mTransType == ACC2::TransDebtor) {
        createNewHelper(docDescr, ACC_QACHARTMASTER->getAccRecName(),
                        ACC_QACHARTMASTER->getAccRecId(), totalAmount); // debit
    } else if (mTransType == ACC2::TransCreditor) {
        createNewHelper(docDescr, ACC_QACHARTMASTER->getAccPayName(),
                        ACC_QACHARTMASTER->getAccPayId(), -totalAmount); // credit
    } else if (mTransType == ACC2::TransMemo) {
        createNewHelper(docDescr, ACC_QACHARTMASTER->getAccMemoName(),
                        ACC_QACHARTMASTER->getAccMemoId(), totalDebit); // debit
        createNewHelper(docDescr, ACC_QACHARTMASTER->getAccMemoName(),
                        ACC_QACHARTMASTER->getAccMemoId(), totalCredit); // credit
    }

    tvItem->setCurrentIndex(idxItem);
}

void ACC_GlTransactionWidget::createNewHelper(const RB_String& descr,
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
    gltrans->setValue("chartmaster_idx", acctId + acctName);
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
 * (Re)Create the existing transactions
 */
void ACC_GlTransactionWidget::createExistingGlTrans() {
    if (mGlTransList->countMember() > 0) mGlTransList->erase();
    QModelIndex idxDoc = tvDocument->currentIndex();
    RB_String docId = mTransDocModel->data(mTransDocModel->index(idxDoc.row(), 0)).toString();
    mGlTransList->setValue("parent", docId); // NOTE: actual parent is ACC_Project
    mGlTransList->dbReadList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveAll, "transdoc_id");
}

/**
 * @returns true if document contains valid transactions
 */
bool ACC_GlTransactionWidget::isValidTransDoc() {
    RB_String message = "";
    double totalPositive = 0.0; // debit
    double totalNegative = 0.0; // credit also positive
    double totalTax = 0.0;
    QModelIndex idx;
    RB_String acct = "";

    double amount;
    int itemRowCount = mItemTransModel->rowCount();

    for (int i = 0; i < itemRowCount; ++i) {
        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("chartmaster_idx"));
        acct = mItemTransModel->data(idx, RB2::RoleOrigData).toString();

        // check valid account, TODO: check also for relevancy of account
        RB_String chartmasterIdx = mItemTransModel->data(idx, RB2::RoleOrigData).toString();
        message += chartmasterIdx.size() < 38
                ? tr("- invalid item account\n") : "" ;

        idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("amount"));
        amount = mItemTransModel->data(idx).toDouble();

        if (amount >= 0) {
            if (mTransType != ACC2::TransCreditor) {
                totalPositive += amount;
            } else {
                totalNegative += amount;
            }
        } else if (mTransType == ACC2::TransDebtor) {
            // negative in case of credit sales invoice
            totalPositive += amount;
        } else if (mTransType == ACC2::TransCreditor) {
            // negative in case of credit purchase invoice
            totalNegative += amount;
        } else {
            totalNegative += -amount;
        }

        if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
            if (acct != ACC_QACHARTMASTER->getAccSalesRevId()
                        && acct != ACC_QACHARTMASTER->getAccPurchRevId()) {
                // include tax only if not tax is reversed
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxhighamt"));
                amount = mItemTransModel->data(idx).toDouble();
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxlowamt"));
                amount += mItemTransModel->data(idx).toDouble();
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxotheramt"));
                amount += mItemTransModel->data(idx).toDouble();

                if (!ACC_QACHARTMASTER->isTaxableAccount(acct) && amount > 0.0) {
                    message += tr("- Tax should be zero in case of a no tax account\n");
                }

                if (mTransType == ACC2::TransDebtor) {
                    totalPositive += amount;
                } else {
                    totalNegative += amount;
                }

                totalTax += amount;
            } else {
                // high and low tax should be zero if tax is reversed
                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxhighamt"));
                amount = mItemTransModel->data(idx).toDouble();
                if (amount != 0.0) {
                    message += tr("- High tax should be zero in case of tax reversed\n");
                }

                idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxlowamt"));
                amount += mItemTransModel->data(idx).toDouble();
                if (amount != 0.0) {
                    message += tr("- Low tax should be zero in case of tax reversed\n");
                }
            }

            if (mTransType == ACC2::TransDebtor || mTransType == ACC2::TransCreditor) {
                // Cannot book against Amount Payable or Amount Receivable
                if (acct == ACC_QACHARTMASTER->getAccPayId()
                            || acct == ACC_QACHARTMASTER->getAccRecId()) {
                    message += tr("- Cannot book against Amount Receivable or Payable\n");
                }
            }
        } else if (mTransType == ACC2::TransBankCash) {
            // Can only book against Amount Payable (Crediteuren)
            // or Amount Receivable (Debiteuren) if allocation exists
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transallocn_idx"));
            RB_String allocId = mItemTransModel->data(idx, RB2::RoleOrigData).toString();

            if (((acct == ACC_QACHARTMASTER->getAccPayId()
                        || acct == ACC_QACHARTMASTER->getAccRecId())
                    && (allocId.isEmpty() || allocId == "" || allocId == "0"))
                    || acct == ACC_QACHARTMASTER->getAccMemoId()) {
                message += tr("- Cannot book against Amount Receivable or Payable\n"
                              "   if no allocation exists or Memorandum Account");
            } else if (acct == "" || acct == "0") {
                message += tr("- Item GL account missing");
            }
        } else if (mTransType == ACC2::TransMemo) {
            if (acct == "" || acct == "0") {
                message += tr("- Item GL account missing");
            }
        }
    }

    // Check amounts
    if (mTransType == ACC2::TransMemo) {
        if (std::fabs(totalPositive - totalNegative) >= 0.005) {
            message += tr("- Debit and Credit amounts are not equal\n");
        }
    } else {
        if (std::fabs(totalPositive - leTotalRec->text().toDouble()) >= 0.005
                && !leTotalRec->isHidden()) {
            message += tr("- Debit- or receivable amounts do not match\n");
        }

        if (std::fabs(totalNegative - leTotalPay->text().toDouble()) >= 0.005
                && !leTotalPay->isHidden()) {
            // totalNegative is a positive number. Creditor is changed to negative at
            // generating gltrans
            message += tr("- Credit- or payable amounts do not match\n");
        }
        if ((std::fabs(totalTax - leTotalTax->text().toDouble()) >= 0.005)
                && !leTotalTax->isHidden()) {
            // Both for debtor and creditor positive number,
            // creditor is changed to negative at generating gltrans
            message += tr("- Tax amounts do not match\n");
        }
    }

    // TODO: Check also mGlTransList before sending to database?
    // TODO: Check that in case of Journal entry the allocated document has equal amount
    //  refer to on_pbAlloc_clicked()
    //    RB_ObjectBase* trans = NULL;

    if (!message.isEmpty()) {
        mValidationMessage = tr("Transaction is invalid, check:\n") + message;
        DB_DIALOGFACTORY->commandMessage(mValidationMessage);
    } else {
        mValidationMessage = tr("Transaction is valid.\n");
        DB_DIALOGFACTORY->commandMessage(mValidationMessage);
        return true;
    }

    return false;
}

/**
 * Update (complete) transaction fields such as transaction date before saving
 */
void ACC_GlTransactionWidget::updateBeforeSave() {

    int typeNo = mTransDocModel->getCurrentValue("transno").toInt();
    QDateTime docDate = mTransDocModel->getCurrentValue("transdate").toDateTime();
    QString transDocDate = docDate.toString(Qt::ISODate);
    RB_String transDocId = mTransDocModel->getCurrentValue("id").toString();
    QModelIndex idx;

    switch (mTransType) {
    case ACC2::TransBankCash : {
        int transType = (int)ACC2::TransBankCash;
        int itemRowCount = mItemTransModel->rowCount();

        for (int i = 0; i < itemRowCount; ++i) {
            // update itemModel, ACC_BankTrans
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("type"));
            mItemTransModel->setData(idx, transType, Qt::EditRole);
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transno"));
            mItemTransModel->setData(idx, typeNo, Qt::EditRole);
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transdate"));
            QString itemDate = mItemTransModel->data(idx).toString();

            if (itemDate > transDocDate) {
                // apparently bank input by hand and document date updated
                // only if date has been set to a previous date
                mItemTransModel->setData(idx, transDocDate, Qt::EditRole);
            }

            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transdoc_id"));
            mItemTransModel->setData(idx, transDocId, Qt::EditRole);
        }

        mHandleAllocn.updateTransDate(mItemTransModel, docDate);
        break;
    }
    case ACC2::TransCreditor : {
        int itemRowCount = mItemTransModel->rowCount();

        for (int i = 0; i < itemRowCount; ++i) {
            // update itemModel, ACC_CreditorTrans
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transdoc_id"));
            mItemTransModel->setData(idx, transDocId, Qt::EditRole);

            // Get tax authority id
            QString provId = ACC_MODELFACTORY->getRoot()->getValue("taxprovince_id").toString();
            QString whereStr = "dispatchtaxprovince_id='" + provId + "'";
            QSqlQuery query(ACC_MODELFACTORY->getDatabase());

            ACC_SqlCommonFunctions oper;
            oper.selectAllFromWhere(query, "ACC_TaxAuthRate", whereStr);
            int fieldNo = query.record().indexOf("parent"); // = tax authority id
            RB_String taxAuthId = "0";
            if (query.first()) {
                taxAuthId = query.value(fieldNo).toString();
            }

            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxauth_id"));
            mItemTransModel->setData(idx, taxAuthId, Qt::EditRole);
        }

        break;
    }
    case ACC2::TransDebtor : {
        int itemRowCount = mItemTransModel->rowCount();

        for (int i = 0; i < itemRowCount; ++i) {
            // update itemModel, ACC_DebtorTrans
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transdoc_id"));
            mItemTransModel->setData(idx, transDocId, Qt::EditRole);

            // Get tax authority id
            QString provId = ACC_MODELFACTORY->getRoot()->getValue("taxprovince_id").toString();
            QString whereStr = "dispatchtaxprovince_id='" + provId + "'";
            QSqlQuery query(ACC_MODELFACTORY->getDatabase());

            ACC_SqlCommonFunctions oper;
            oper.selectAllFromWhere(query, "ACC_TaxAuthRate", whereStr);
            int fieldNo = query.record().indexOf("parent"); // = tax authority id
            RB_String taxAuthId = "0";
            if (query.first()) {
                taxAuthId = query.value(fieldNo).toString();
            }

            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("taxauth_id"));
            mItemTransModel->setData(idx, taxAuthId, Qt::EditRole);
        }

        break;
    }
    case ACC2::TransMemo : {
        int itemRowCount = mItemTransModel->rowCount();

        for (int i = 0; i < itemRowCount; ++i) {
            // update itemModel, ACC_MemoTrans
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transdoc_id"));
            mItemTransModel->setData(idx, transDocId, Qt::EditRole);
            idx = mItemTransModel->index(i, mItemTransModel->fieldIndex("transno"));
            mItemTransModel->setData(idx, typeNo, Qt::EditRole);
        }

        mHandleAllocn.updateTransDate(mItemTransModel, docDate);
        break;
    }
    default :
        break;
    }

    // update mGlTransList done at fileSave()
}

/**
 * @returns the help subject, in this case the widget objectname
 */
//RB_String ACC_GlTransactionWidget::getHelpSubject() const {
//    return objectName();
//}

//bool ACC_GlTransactionWidget::closeWidget() {
//    RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return true;
//    }
//    return false;
//}

/**
 * Notify user of unsaved data and ask whether data should be saved.
 * Overrides the standard maybeSave() because of mSaveInProgress requirement
 */
bool ACC_GlTransactionWidget::maybeSave() {
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
 * Mouse press event, to catch click in case of disabled widgets.
 * Some widgets are disabled in case data has been edited and does not validate.
 */
void ACC_GlTransactionWidget::mousePressEvent(QMouseEvent* e) {
    QWidget* wdgt = childAt(QPoint(e->x(), e->y()));
    if (wdgt == cbLedgerAccount || wdgt == pbAddDoc || wdgt == pbSave
        || wdgt == pbFind || wdgt == tvDocument) {

        DB_DIALOGFACTORY->requestWarningDialog(
                mValidationMessage +
                tr("update or cancel transaction."));
    } else {
        RB_Widget::mousePressEvent(e);
    }
}

/**
 * Change event, for example translation
 */
void ACC_GlTransactionWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
