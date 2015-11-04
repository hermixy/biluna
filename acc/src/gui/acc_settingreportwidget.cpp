/*****************************************************************
 * $Id: acc_settingreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Dec 8, 2010 12:29:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_settingreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc.h"
#include "acc_createinvoiceaged.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "db_modelfactory.h"
#include "rb_sqlrelation.h"


/**
 * Constructor
 */
ACC_SettingReportWidget::ACC_SettingReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);

    mColCount = 0;
    mReportTitle = tr("None");
    mModel = NULL;
    mChildModel = NULL;
}

/**
 * Destructor
 */
ACC_SettingReportWidget::~ACC_SettingReportWidget() {
    delete mChildModel;
    mChildModel = NULL;
    delete mModel;
    mModel = NULL;
}

/**
 * Initialize
 */
void ACC_SettingReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    setComboBox();

    mHtml = "<p>";
    mHtml += tr("Select setting above, and click refresh button ...");
    mHtml += "</p>";

    te->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_SettingReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_SettingReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_SettingReportWidget::filePdf(QPrinter* printer) {
    te->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_SettingReportWidget::filePrint(QPrinter* printer) {
    te->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_SettingReportWidget::filePrintPreview(QPrinter* printer) {
    te->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_SettingReportWidget::editCut() {
    te->cut();
}

/**
 * Edit Copy.
 */
void ACC_SettingReportWidget::editCopy() {
    te->copy();
}

/**
 * Edit Paste.
 */
void ACC_SettingReportWidget::editPaste() {
    te->paste();
}

/**
 * Edit Undo.
 */
void ACC_SettingReportWidget::editUndo() {
    te->undo();
}

/**
 * Edit Redo.
 */
void ACC_SettingReportWidget::editRedo() {
    te->redo();
}

/**
 * Edit Select All.
 */
void ACC_SettingReportWidget::editSelectAll() {
    te->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_SettingReportWidget::editInsertImage(const QString& fn) {
    te->insertImage(fn);
}

/**
 * @brief ACC_SettingReportWidget::editZoomIn
 */
void ACC_SettingReportWidget::editZoomIn(double /*factor*/) {
   te->fontZoomIn(1);
}

/**
 * @brief ACC_SettingReportWidget::editZoomOut
 */
void ACC_SettingReportWidget::editZoomOut(double /*factor*/) {
    te->fontZoomOut(1);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool ACC_SettingReportWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ACC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        ACC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << te->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    ACC_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    emit modificationChanged(false);
    return true;
}


/**
 * Initialize and create trial balance
 */
void ACC_SettingReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (mChildModel) {
        delete mChildModel;
        mChildModel = NULL;
    }

    if (mModel) {
        delete mModel;
        mModel = NULL;
    }

    mReportTitle = cbSetting->currentText();
    int reportIdx = cbSetting->currentIndex();
    mStartColumn = RB2::HIDDENCOLUMNS;

    switch (reportIdx) {
    case 0: { // None
        mModel = NULL;
        break;
    }
    case 1: { // Units of Measurement, shared=false
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelUnitOfMeasure, false);
        break;
    }
    case 2: { // Currency
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCurrency, false);
        break;
    }
    case 3: { // Account Control
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAccountControl, false);
        break;
    }
    case 4: { // Account Groups
        break;
    }
    case 5: { // Chart of Accounts
        break;
    }
    case 6: { // Tax Categories
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxCategory, false);
        break;
    }
    case 7: { // Tax Provinces
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxProvince, false);
        break;
    }
    case 8: { // Tax Authorities/Rates
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxAuthority, false);
        mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxAuthRate, false);
        mChildModel->setRelation(mChildModel->fieldIndex("dispatchtaxprovince_id"),
                                 RB_SqlRelation("ACC_TaxProvince", "id", "taxprovincename"));
        mChildModel->setRelation(mChildModel->fieldIndex("taxcat_id"),
                                 RB_SqlRelation("ACC_TaxCategory", "id", "taxcatname"));
        break;
    }
    case 9: { // Tax Group Taxes
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxGroup, false);
        mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelTaxGroupAuth, false);
        mChildModel->setRelation(mChildModel->fieldIndex("taxauth_id"),
                                 RB_SqlRelation("ACC_TaxAuthority", "id", "description"));
        break;
    }
    case 10: { // Stock/Office Location
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelLocation, false);
        // relation taxprovince_id
        mModel->setRelation(mModel->fieldIndex("taxprovince_id"),
                            RB_SqlRelation("ACC_TaxProvince", "id", "taxprovincename"));
        break;
    }
    case 11: { // Shipper
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelShipper, false);
        break;
    }
    case 12: { // Factor Companies
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelFactorCompany, false);
        break;
    }
    case 13: { // Sales Types/Price Lists
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesType, false);
        break;
    }
    case 14: { // Sales/Purchase Area
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelArea, false);
        break;
    }
    case 15: { // Sales GL Postings
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesGlPosting, false);
        // relations
        mModel->setRelation(mModel->fieldIndex("taxauth_id"),
                            RB_SqlRelation("ACC_TaxAuthority", "id", "description"));
        mModel->setRelation(mModel->fieldIndex("area_id"),
                            RB_SqlRelation("ACC_Area", "id", "areacode"));
        mModel->setRelation(mModel->fieldIndex("salestype_id"),
                            RB_SqlRelation("ACC_SalesType", "id", "typeabbrev"));

        break;
    }
    case 16: { // COGS GL Postings
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCogsGlPosting, false);
        // relations
        mModel->setRelation(mModel->fieldIndex("area_id"),
                            RB_SqlRelation("ACC_Area", "id", "areacode"));
        break;
    }
    case 17: { // Sales Tax Postings
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesTaxPosting, false);
        // relations
        mModel->setRelation(mModel->fieldIndex("taxauth_id"),
                            RB_SqlRelation("ACC_TaxAuthority", "id", "description"));
        mModel->setRelation(mModel->fieldIndex("taxcat_id"),
                            RB_SqlRelation("ACC_TaxCategory", "id", "taxcatname"));
        break;
    }
    case 18: { // Price
        break;
    }
    case 19: { // Payment Terms
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelPaymentTerm, false);
        break;
    }
    case 20: { // Credit Status
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelHoldReason, false);
        break;
    }
    case 21: { // Debtor Type
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelDebtorType, false);
        break;
    }
    case 22: { // Sales People
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSalesMan, false);
        break;
    }
    case 23: { // Asset Types
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelAssetType, false);
        break;
    }
    case 24: { // System Account Control
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSystemAccountControl, false);
        break;
    }
    case 25: { // Cost Center Groups
        break;
    }
    case 26: { // Cost Centers
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCostCenter, false);
        break;
    }
    case 27: { // Customer Cost Center Part
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer, false);
        mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustCostPart, false);
        break;
    }
    case 28: { // Supplier Cost Center Part
        mModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplier, false);
        mChildModel = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelSupplCostPart, false);
        break;
    }
    default :
        mHtml = tr("Setting model is empty");
        break;
    }

    if (reportIdx == 4 || reportIdx == 5 || reportIdx == 18 || reportIdx == 25) {
        // in memory tree models and mHtml will be created
        mModel = NULL;
        createSql(reportIdx);
    } else if (mModel && reportIdx != 15 && reportIdx != 16
            && reportIdx != 17 && reportIdx != 24 && reportIdx != 26
            && reportIdx != 27 && reportIdx != 28) {
        mModel->setRoot(DB_MODELFACTORY->getRootId());
        mModel->select();
        mColCount = mModel->columnCount();
        createTable();
    } else if (mModel && (reportIdx == 27 || reportIdx == 28)) {
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();
        mColCount = RB2::HIDDENCOLUMNS + 2;
        createTable();
    } else if (mModel && reportIdx != 24) {
        mModel->setRoot(ACC_MODELFACTORY->getRootId());
        mModel->select();
        mColCount = mModel->columnCount();
        createTable();
    } else if (mModel && reportIdx == 24) {
        mColCount = mModel->columnCount();
        createTable();
    }

    te->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Create setting table
 */
void ACC_SettingReportWidget::createTable() {
    if (cbSetting->currentIndex() == 0 || !mModel) {
        mHtml = tr("No data. Please select a setting first.");
        return;
    }

    createHeader();

    for (int col = mStartColumn; col < mColCount; ++col) {
        mHtml += "<th>" + mModel->headerData(col, Qt::Horizontal).toString() + "</th>";
    }

    mHtml += "</tr><tr><td colspan=" + RB_String::number(mColCount - mStartColumn)
            + " style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage()
            + "</td></tr></thead><tbody style='font-size:8pt;'>";

    int rowCount =  mModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        addDataRow(row);

        if (mChildModel) {
            // Because not shared models
            QModelIndex idx = mModel->index(row, 0, QModelIndex());
            RB_String id = mModel->data(idx, RB2::RoleOrigData).toString();
            mChildModel->setRoot(id); // is parent field
            mChildModel->select();
            // or use:
            // mModel row is not selected, therefore not:
            // mChildModel->slotParentCurrentRowChanged(idx, QModelIndex());

            createChildTable();
        }
    }

    mHtml += "</tbody>";
    mHtml += "</table>";
    mHtml += "<p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Create HTML header
 */
void ACC_SettingReportWidget::createHeader() {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("coyname").toString();
    QDateTime dt = QDateTime::currentDateTime();

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=" + RB_String::number(mColCount - mStartColumn)
            + " align='center' style='font-size:11pt;'>" + mReportTitle + " - " + coyName + "</th></tr>"
    "<tr><td colspan=" + RB_String::number(mColCount - mStartColumn)
            + " align='center'>" + tr("Created: ")
            + dt.toString(Qt::ISODate) + "</td></tr><tr><td colspan=" + RB_String::number(mColCount - mStartColumn)
            + " style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage() + "</td></tr><tr>";
}

/**
 * Create setting table with SQL in case createTable() had no model
 * @param reportIdx report index
 */
void ACC_SettingReportWidget::createSql(int reportIdx) {
    if (cbSetting->currentIndex() == 0) {
        mHtml = tr("No data. Please select a setting first.");
        return;
    }

    mColCount = 0;
    mStartColumn = 0;

    switch (reportIdx) {
    case 4 :
        createAccountGroupTable();
        break;
    case 5 :
        createChartMasterTable();
        break;
    case 18 :
        createPriceTable();
        break;
    case 25 :
        createCostCenterGroupTable();
        break;
    default :
        break;

    }

    mHtml += "</tbody>";
    mHtml += "</table>";
    mHtml += "</body></html>";
}

/**
 * Create header columns based on query
 * @param q SQL query
 */
void ACC_SettingReportWidget::createSqlHeaderColumns(const QSqlRecord& rec) {
    for (int col = mStartColumn; col < mColCount; ++col) {
        mHtml += "<th>" + rec.fieldName(col) + "</th>";
    }

    mHtml += "</tr><tr><td colspan=" + RB_String::number(mColCount - mStartColumn)
            + " style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage()
            + "</td></tr></thead><tbody style='font-size:8pt;'>";

//    mHtml += "</tr></thead>"
//    "<tbody style='font-size:8pt;'>";
}

/**
 * Create child table if exists
 */
void ACC_SettingReportWidget::createChildTable() {
    if (!mChildModel) {
        return;
    }

    int childColCount = mChildModel->columnCount();
    int childRowCount = mChildModel->rowCount();

    if (childColCount < 1 || childRowCount < 1) {
        return;
    }

    mHtml += "<tr><td colspan=" + RB_String::number(mColCount - mStartColumn) + ">";

    // create table header

    mHtml += "<table cellpadding=3 rules=groups frame=box width='100%' style='font-size:11pt; background-color: " + mAlternateBaseColor.name() + ";'>"
            "<thead><tr>";
    for (int col = mStartColumn; col < childColCount; ++col) {
        mHtml += "<td>" + mChildModel->headerData(col, Qt::Horizontal).toString() + "</td>";
    }

    mHtml += "</tr></thead>"
            "<tbody style='font-size:8pt;'>";

    // add rows
    for (int row = 0; row < childRowCount; ++row) {
        mHtml += "<tr>";
        for (int col = mStartColumn; col < childColCount; ++col) {
            mHtml += "<td>" + mChildModel->data(mChildModel->index(row, col)).toString() + "</td>";
        }
        mHtml += "</tr>";
    }

    mHtml += "</tbody>";
    mHtml += "</table>";
    mHtml += "</td></tr>";
}

/**
 * Create Account Group table
 */
void ACC_SettingReportWidget::createAccountGroupTable() {
    RB_String tableName = "acc_accountgroup";
    RB_String rootId = DB_MODELFACTORY->getRootId();

    RB_String qStr =
            "SELECT t4.* FROM " + tableName.toLower() + " as t4 "
                 "LEFT JOIN " + tableName.toLower() + " as t3 on t4.parent=t3.id "
                 "LEFT JOIN " + tableName.toLower() + " as t2 on t3.parent=t2.id "
                 "LEFT JOIN " + tableName.toLower() + " as t1 on t2.parent=t1.id "
             "WHERE t1.parent='" + rootId + "' "
                 "OR t2.parent='" + rootId + "' "
                 "OR t3.parent='" + rootId + "' "
                 "OR t4.parent='" + rootId + "' ORDER BY sequenceintb ASC";
    QSqlQuery q(ACC_MODELFACTORY->getDatabase());
    q.setForwardOnly(true);

    if (!q.exec(qStr)) {
        RB_DEBUG->error("ACC_SettingReportWidget::createAccountGroupTable() ERROR");
        RB_DEBUG->print("  " + q.lastError().text());
    }

    mColCount = q.record().count();
    mStartColumn = RB2::HIDDENCOLUMNS;

    createHeader();
    createSqlHeaderColumns(q.record());

    while (q.next()) {
        addSqlDataRow(q.record());
    }
}

/**
 * Create chartmaster table
 */
void ACC_SettingReportWidget::createChartMasterTable() {
    RB_String rootId = ACC_MODELFACTORY->getRootId();

    RB_String qStr =
            "SELECT acc_chartmaster.accountcode, acc_chartmaster.accountname, "
                "acc_accountgroup.groupname, "
                "acc_accountgroup.pandl, "
                "acc_accountcontrol.description "
            "FROM acc_chartmaster "
                "LEFT JOIN acc_accountgroup ON acc_chartmaster.accountgroup_id=acc_accountgroup.id "
                "LEFT JOIN acc_accountcontrol ON acc_chartmaster.accountcontrol=acc_accountcontrol.controlnumber "
             "WHERE acc_chartmaster.parent='" + rootId + "' "
             "ORDER BY accountcode ASC";

    QSqlQuery q(ACC_MODELFACTORY->getDatabase());
    q.setForwardOnly(true);

    if (!q.exec(qStr)) {
        RB_DEBUG->error("ACC_SettingReportWidget::createChartMasterTable() ERROR");
        RB_DEBUG->print("  " + q.lastError().text());
    }

    mColCount = q.record().count();
    mStartColumn = 0;

    createHeader();
    createSqlHeaderColumns(q.record());

    while (q.next()) {
        addSqlDataRow(q.record());
    }
}

/**
 * Create price table
 */
void ACC_SettingReportWidget::createPriceTable() {
    RB_String rootId = ACC_MODELFACTORY->getRootId();

    RB_String qStr =
            "SELECT acc_stockcategory.categorycode, acc_stockmaster.stockcode, "
                "acc_stockmaster.description, acc_salestype.typeabbrev, "
                "acc_currency.currcode, acc_price.price "
            "FROM acc_stockcategory "
            "INNER JOIN acc_stockmaster ON acc_stockcategory.id=acc_stockmaster.parent "
            "INNER JOIN acc_price ON acc_stockmaster.id=acc_price.parent "
            "INNER JOIN acc_salestype ON acc_price.salestype_id=acc_salestype.id "
            "INNER JOIN acc_currency ON acc_currency.id=acc_price.currency_id "
            "WHERE acc_stockcategory.parent='" + rootId + "' "
            "ORDER BY acc_stockcategory.categorycode, acc_stockmaster.stockcode ASC";

    QSqlQuery q(ACC_MODELFACTORY->getDatabase());
    q.setForwardOnly(true);

    if (!q.exec(qStr)) {
        RB_DEBUG->error("ACC_SettingReportWidget::createPriceTable() ERROR");
        RB_DEBUG->print("  " + q.lastError().text());
    }

    mColCount = q.record().count();
    mStartColumn = 0;

    createHeader();
    createSqlHeaderColumns(q.record());

    while (q.next()) {
        addSqlDataRow(q.record());
    }
}

/**
 * Create cost center group table()
 */
void ACC_SettingReportWidget::createCostCenterGroupTable() {
    RB_String tableName = "acc_costgroup";
    RB_String rootId = DB_MODELFACTORY->getRootId();

    RB_String qStr =
            "SELECT t4.* FROM " + tableName.toLower() + " as t4 "
                 "LEFT JOIN " + tableName.toLower() + " as t3 on t4.parent=t3.id "
                 "LEFT JOIN " + tableName.toLower() + " as t2 on t3.parent=t2.id "
                 "LEFT JOIN " + tableName.toLower() + " as t1 on t2.parent=t1.id "
             "WHERE t1.parent='" + rootId + "' "
                 "OR t2.parent='" + rootId + "' "
                 "OR t3.parent='" + rootId + "' "
                 "OR t4.parent='" + rootId + "' ORDER BY sequenceno ASC";
    QSqlQuery q(ACC_MODELFACTORY->getDatabase());
    q.setForwardOnly(true);

    if (!q.exec(qStr)) {
        RB_DEBUG->error("ACC_SettingReportWidget::createCostCenterGroupTable() ERROR");
        RB_DEBUG->print("  " + q.lastError().text());
    }

    mColCount = q.record().count();
    mStartColumn = RB2::HIDDENCOLUMNS;

    createHeader();
    createSqlHeaderColumns(q.record());

    while (q.next()) {
        addSqlDataRow(q.record());
    }
}

/**
 * Add an account group header row
 */
void ACC_SettingReportWidget::addHeaderRow() {
    mHtml += "<tr><td colspan=10><strong>";
//    mHtml += mCustSuppName;
    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_SettingReportWidget::addDataRow(int row) {
    mHtml += "<tr>";

    for (int col = mStartColumn; col < mColCount; ++col) {
        mHtml += "<td>" + mModel->data(mModel->index(row, col)).toString() + "</td>";
    }

    mHtml += "</tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_SettingReportWidget::addSqlDataRow(const QSqlRecord& rec) {
    mHtml += "<tr>";

    for (int col = mStartColumn; col < mColCount; ++col) {
        mHtml += "<td>" + rec.value(col).toString() + "</td>";
    }

    mHtml += "</tr>";
}

/**
 * Add an account group footer row
 */
void ACC_SettingReportWidget::addFooterRow() {
    RB_String betweenCellsRight = "</i></td><td align='right' style='border-top: solid 1px " + mTextColor.name() + ";'><i>";

//    mHtml += "<tr><td colspan=5 align='right'><i>";
//    mHtml += mCustSuppName + " " + tr("Totals") + betweenCellsRight;
//    mHtml += RB_String::number(mSumAmount,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumAlloc,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumDue,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumOverDue1,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumOverDue2,'f', 2);
    mHtml += "</i></td></tr>"; // <tr><td colspan=10>&nbsp;</td></tr> empty line
}

/**
 * Add footer with totals
 */
void ACC_SettingReportWidget::addFooter() {
    RB_String betweenCellsWhite = "</td><td align='right'>";

    mHtml += "<tfoot style='font-size:8pt;'><tr><td colspan=5 align='right'>";
    mHtml += tr("Grand Totals");
    mHtml += "</td><td align='right'>";
//    mHtml += RB_String::number(mTotalAmount,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalAlloc,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalDue,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalOverDue1,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalOverDue2,'f', 2);
    mHtml += "</td></tr></tfoot>";
}

void ACC_SettingReportWidget::setComboBox() {
    RB_StringList strL;
    strL << tr("None") << tr("Units of Measurement") << tr("Currency")
            << tr("Account Control") << tr("Account Groups")
            << tr("Chart of Accounts") << tr("Tax Categories")
            << tr("Tax Provinces") << tr("Tax Authorities/Rates")
            << tr("Tax Group Taxes") << tr("Stock/Office Location")
            << tr("Shipper") << tr("Factor Companies")
            << tr("Sales Types/Price Lists") << tr("Sales/Purchase Area")
            << tr("Sales GL Postings") << tr("COGS GL Postings")
            << tr("Sales Tax Postings") << tr("Price")
            << tr("Payment Terms") << tr("Credit Status")
            << tr("Debtor Type") << tr("Sales People")
            << tr("Asset Types") << tr("System Account Control")
            << tr("Cost Center Groups") << tr("Cost Centers")
            << tr("Customer Cost Center Part") << tr("Supplier Cost Center Part");
    cbSetting->addItems(strL);
}

/**
 * Change event such as language change
 */
void ACC_SettingReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
