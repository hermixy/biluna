/*****************************************************************
 * $Id: acc_bankmappingdialog.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Jul 01, 2013 10:04:25 AM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_bankmappingdialog.h"

#include <cmath>
#include <QPrintDialog>
#include <QPrinter>
#include <QStringListModel>
#include <QTableWidgetItem>
#include "acc_bankimport.h"
#include "acc_banktransdoc.h"
#include "acc_csvbankparser.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_postgltransaction.h"
#include "acc_qachartmaster.h"
// #include "acc_reportpreviewtedialog.h"
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
ACC_BankMappingDialog::ACC_BankMappingDialog(QWidget *parent)
        : RB_Dialog(parent) {

    setupUi(this);

    mModel = NULL;
    mMapper = NULL;
    mBankImportList = NULL;

    mColorLightYellow.setNamedColor("lightyellow"); // rgb(255,255,224)
    mColCount = 0;
}

/**
 * Destructor
 */
ACC_BankMappingDialog::~ACC_BankMappingDialog() {
    delete mBankImportList;
    delete mMapper;
    delete mModel;
    RB_DEBUG->print("ACC_BankMappingDialog::~ACC_BankMappingDialog() OK");
}

/**
 * Initialize widget and models
 * @param statList bank statement list
 */
void ACC_BankMappingDialog::init() {
    setWindowTitle(tr("Bank CSV Mapping[*]"));

    mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelBankMapping);
    mModel->setRoot(ACC_MODELFACTORY->getRootId());

    mMapper = mModel->getMapper();
    mMapper->addMapping(leBankAccount, mModel->fieldIndex("bankaccount"));
    mMapper->addMapping(leDateFormat, mModel->fieldIndex("dateformat"));
    mMapper->addMapping(leCreditName, mModel->fieldIndex("creditname"));

    RB_StringList items;
    items << tr("Comma [,]") << tr("Semi-Colon [;]") << tr("Tab");
    cbColumnSeparator->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbColumnSeparator, mModel->fieldIndex("separator_id"),
                        "currentIndex");
    items.clear();
    items << tr("No") << tr("Yes");
    cbFirstRowIsTitle->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbFirstRowIsTitle, mModel->fieldIndex("firstrowtitle_id"),
                        "currentIndex");
    cbStripQuotes->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbStripQuotes, mModel->fieldIndex("stripquotes_id"),
                        "currentIndex");
    items.clear();
    items << tr("Point [.]") << tr("Comma [,]");
    cbDecimalSeparator->setModel(new QStringListModel(items, this));
    mMapper->addMapping(cbDecimalSeparator, mModel->fieldIndex("decimal_id"),
                        "currentIndex");
    mMapper->addMapping(sbBankAccount, mModel->fieldIndex("colnobankaccount"));
    mMapper->addMapping(sbTransactionDate, mModel->fieldIndex("colnotransactiondate"));
    mMapper->addMapping(sbCounterBankAccount, mModel->fieldIndex("colnocounterbankaccount"));
    mMapper->addMapping(sbBankAccountHolderName, mModel->fieldIndex("colnocounteraccountholder"));
    mMapper->addMapping(sbDebitCredit, mModel->fieldIndex("colnodebitcredit"));
    mMapper->addMapping(sbAmount, mModel->fieldIndex("colnoamount"));
    mMapper->addMapping(sbDescription, mModel->fieldIndex("colnodescription"));
    mMapper->addMapping(sbSecondDescription, mModel->fieldIndex("colnoseconddescription"));
    mMapper->addMapping(sbTransactionCode, mModel->fieldIndex("colnotransactioncode"));
    mMapper->addMapping(sbStatementNumber, mModel->fieldIndex("colnostatnumber"));
    mModel->select();

    setFormatTableView(tvMapping, mModel);
    readSettings();

    // Hidden columns only for tableviews
    int colCount = mModel->columnCount();
    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS || i > RB2::HIDDENCOLUMNS + 2) {
            tvMapping->hideColumn(i);
        } else {
            tvMapping->showColumn(i);

            if (tvMapping->columnWidth(i) < 5) {
                tvMapping->setColumnWidth(i, 100);
            }
        }
    }

    if (!mBankImportList) {
        mBankImportList = new RB_ObjectContainer("", NULL, "ACC_BankImportList");
    }

}

/**
 * Save data, can be called directly or indirectly via closeEvent()
 * and maybeSave().
 * @param withSelect true if select() is to be done after submitAll()
 */
bool ACC_BankMappingDialog::fileSave(bool withSelect) {
    // QApplication::setOverrideCursor(Qt::WaitCursor);
    beforeFileSave();
    bool success = false;

    if (withSelect) {
        success = mModel->submitAllAndSelect();

    } else {
        success = mModel->submitAll();
    }
    // QApplication::restoreOverrideCursor();

    if (!success) {
        return false;
    }

    setWindowModified(false);
    return true;
}

/**
 * Reverse all changes
 */
void ACC_BankMappingDialog::fileRevert() {
    mModel->revert();
    setWindowModified(false);
}


/**
 * Push button add clicked()
 */
void ACC_BankMappingDialog::on_pbAdd_clicked() {
    if (!mModel) return;
    mModel->undoFilterSort();

    if (isWindowModified()) {
        fileSave(true);
    }

    // always insert at the beginning
    int row = 0; // mModel->rowCount();
    mModel->insertRows(row, 1, QModelIndex());

    // NOTE: do not forget to set the default column values, specially for the
    //       relational table otherwise new row will not show!
    QModelIndex index = mModel->index(row, mModel->fieldIndex("bankaccount"));
    mModel->setData(index, "<NEW>", Qt::EditRole);
    index = mModel->index(row, mModel->fieldIndex("dateformat"));
    mModel->setData(index, "yyyy-MM-dd", Qt::EditRole);
    // end NOTE

    tvMapping->setCurrentIndex(mModel->index(row, RB2::HIDDENCOLUMNS, QModelIndex()));
    tvMapping->scrollTo(tvMapping->currentIndex());
    leBankAccount->setFocus();
    leBankAccount->selectAll();
}

/**
 * Push button delete clicked)(
 */
void ACC_BankMappingDialog::on_pbDelete_clicked() {
    if (!mModel) return;

    int res = DB_DIALOGFACTORY->requestYesNoDialog(tr("Delete item"),
                                          tr("Deletion cannot be undone.\n"
                                             "Do you want to delete the item?"));
    if (res != QMessageBox::Yes) return;

    QModelIndex index = tvMapping->currentIndex();
    if (index.isValid()) {
        mModel->removeRows(index.row(), 1, QModelIndex());
        fileSave(true);
    }
}

/**
 * Button open file clicked
 */
void ACC_BankMappingDialog::on_pbOpenFile_clicked() {
    if (!tvMapping->currentIndex().isValid()) {
        DB_DIALOGFACTORY->requestWarningDialog(tr("No mapping selected"));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Open file
    RB_String fileName;
    DB_ActionFileOpen a;
    a.getOpenFileName(fileName);

    if (fileName.isEmpty()) {
        leFileName->clear();
        QApplication::restoreOverrideCursor();
        return;
    }

    leFileName->setText(fileName);

    // Clear imported statement list if already and delete objects
    mBankImportList->erase();

    // Execute parsing
    ACC_CsvBankParser* parser = new ACC_CsvBankParser(fileName, mBankImportList);
    setParserSettings(parser);
    bool success = parser->exec();
    delete parser;

    if (success) {
        fillTable(mBankImportList);
    } else {
        tw->clear();
        tw->setRowCount(0);
        tw->setColumnCount(0);
        ACC_DIALOGFACTORY->requestWarningDialog(tr("Parse error, check settings."));
    }

    QApplication::restoreOverrideCursor();
}

/**
 * Button Help clicked
 */
void ACC_BankMappingDialog::on_pbHelp_clicked() {
    RB_MainWindow* mw = ACC_DIALOGFACTORY->getMainWindow();
    mw->slotHelpSubject(this->objectName());
}

/**
 * Button OK clicked
 */
void ACC_BankMappingDialog::on_pbOk_clicked() {
    fileSave(true);
    accept();
}

/**
 * Button Cancel clicked
 */
void ACC_BankMappingDialog::on_pbCancel_clicked() {
    reject();
}

/**
 * Set parser settings
 * @param parser
 */
void ACC_BankMappingDialog::setParserSettings(ACC_CsvBankParser* parser) {
    if (!parser || !tvMapping->currentIndex().isValid()) {
        return;
    }

    parser->setBankAccount(leBankAccount->text());
    parser->setDateFormat(leDateFormat->text());
    parser->setCreditIdentifier(leCreditName->text()); // Optional/required

    int idx = cbColumnSeparator->currentIndex();
    RB_String sep = "";

    if (idx == 0) {
        sep = ",";
    } else if (idx == 1) {
        sep = ";";
    } else if (idx == 2) {
        sep = "\t";
    }

    parser->setColumnSeparator(sep);
    bool isFirstRowTitle = cbFirstRowIsTitle->currentIndex() == 1 ? true : false;
    parser->setFirstLineIsTitleRow(isFirstRowTitle);
    bool isStripQuotes = cbStripQuotes->currentIndex() == 1 ? true : false;
    parser->setRemoveQuotes(isStripQuotes);

    idx = cbDecimalSeparator->currentIndex();
    sep = "";

    if (idx == 0) {
        sep = ".";
    } else if (idx == 1) {
        sep = ",";
    }

    parser->setDecimalSeparator(sep);
    parser->addColumnFieldMap(sbBankAccount->value()-1, "statactnumber");
    // parser->addColumnFieldMap(sbTransactionDate->value()-1, "stattimestamp"); // not used
    parser->addColumnFieldMap(sbTransactionDate->value()-1, "transentrytimestamp");
    parser->addColumnFieldMap(sbBankAccountHolderName->value()-1, "transaccount");
    parser->addColumnFieldMap(sbCounterBankAccount->value()-1, "transactnumber");
    parser->addColumnFieldMap(sbDebitCredit->value()-1, "transdebitcredit"); // Optional/required
    parser->addColumnFieldMap(sbAmount->value()-1, "transamount");
    parser->addColumnFieldMap(sbDescription->value()-1, "transdescription");
    parser->addColumnFieldMap(sbSecondDescription->value()-1, "transseconddescription"); // Optional
    parser->addColumnFieldMap(sbTransactionCode->value()-1, "transcode"); // Optional
    parser->addColumnFieldMap(sbStatementNumber->value()-1, "statnumber"); // Optional

}

/**
 * Fill table with parse bank statement data
 * @param importBankList list with data objects from bank statements
 */
void ACC_BankMappingDialog::fillTable(RB_ObjectBase* importBankList) {
    tw->clear();
    tw->setRowCount(0);
    tw->setColumnCount(0);
    tw->setEditTriggers(QAbstractItemView::EditKeyPressed |
                        QAbstractItemView::DoubleClicked);
    tw->setSelectionMode(QAbstractItemView::ContiguousSelection);

    bool isHeaderSet = false;
    int row = 0;
//    ACC_SqlCommonFunctions f;

    RB_ObjectContainer* list = dynamic_cast<RB_ObjectContainer*>(importBankList);
    RB_ObjectIterator* iter = list->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (!isHeaderSet) {
            // set bank account id and name
//            mBankAccount = obj->getValue("statactnumber").toString();
//            ACC_QACHARTMASTER->getBankGlAcctIdName(mBankGlAccId, mBankGlAccName, mBankAccount);

            // check whether imported data have valid statement numbers
//            mBankStatSeqNo = obj->getValue("statnumber").toInt();
//            if (mBankStatSeqNo <= 0) {
//                mLastTransNo = f.getLastTransNo(ACC2::TransBankCash, mBankGlAccId);
//                mBankStatSeqNo = mLastTransNo + 1;
//            } else {
//                mBankStatSeqNo = 0; // back to zero
//            }

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
void ACC_BankMappingDialog::setTableHeader(RB_ObjectBase *obj) {
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
 * Set table row data. transentrytimestamp, statnumber are editable
 * @param row row number
 * @param obj data object
 */
void ACC_BankMappingDialog::setTableRow(int row, RB_ObjectBase* obj) {
    QTableWidgetItem* twItem = NULL;

    for (int col = RB2::HIDDENCOLUMNS; col < mColCount; ++col) {
        if (col == STATNUMBER /*&& mBankStatSeqNo > 0*/) {
            // apparently the imported statement sequence number was not numeric
            twItem = new QTableWidgetItem(tr("(Not checked)") /*RB_String::number(mBankStatSeqNo)*/);
        } else {
            twItem = new QTableWidgetItem(obj->getValue(col).toString());
        }

        if (col != TRANSENTRYTIMESTAMP && col != STATNUMBER) { // transentrytimestamp, statnumber editable
            twItem->setFlags(twItem->flags() & ~Qt::ItemIsEnabled);
        } else {
            twItem->setBackgroundColor(mColorLightYellow);

            if (col == STATTIMESTAMP) { // set date delegate
                twItem->setData(RB2::RoleEditor, RB2::EditorDate);
            } else if (col == STATNUMBER) { // set integer (spinbox) delegate
                twItem->setData(RB2::RoleEditor, RB2::EditorInteger);
            }
        }

        tw->setItem(row, col - RB2::HIDDENCOLUMNS, twItem);

//        if (col == STATACTNUMBER && mBankAccount.isEmpty()) { // statactnumber = bank account
//            mBankAccount = obj->getValue(col).toString();
//        }
    }
}

/**
 * Change event, for example translation
 */
void ACC_BankMappingDialog::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}


