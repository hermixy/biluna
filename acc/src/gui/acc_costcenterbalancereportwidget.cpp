/*****************************************************************
 * $Id: acc_costcenterbalancereportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Sept 29, 2012 10:18:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costcenterbalancereportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc_createcostcenterbalancereport.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"

/**
 * Constructor
 */
ACC_CostCenterBalanceReportWidget::ACC_CostCenterBalanceReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);
}

/**
 * Destructor
 */
ACC_CostCenterBalanceReportWidget::~ACC_CostCenterBalanceReportWidget() {
    // nothing
}

/**
 * Initialize and create trial balance
 */
void ACC_CostCenterBalanceReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    QDateTime dt;
    int year = dt.currentDateTime().date().year();
    sbFromYear->setValue(year);

    cbFromMonth->addItem(tr("January"), 1);
    cbFromMonth->addItem(tr("February"), 2);
    cbFromMonth->addItem(tr("March"), 3);
    cbFromMonth->addItem(tr("April"), 4);
    cbFromMonth->addItem(tr("May"), 5);
    cbFromMonth->addItem(tr("June"), 6);
    cbFromMonth->addItem(tr("July"), 7);
    cbFromMonth->addItem(tr("August"), 8);
    cbFromMonth->addItem(tr("September"), 9);
    cbFromMonth->addItem(tr("October"), 10);
    cbFromMonth->addItem(tr("November"), 11);
    cbFromMonth->addItem(tr("December"), 12);
    cbFromMonth->setCurrentIndex(0);

    cbToMonth->addItem(tr("January"), 1);
    cbToMonth->addItem(tr("February"), 2);
    cbToMonth->addItem(tr("March"), 3);
    cbToMonth->addItem(tr("April"), 4);
    cbToMonth->addItem(tr("May"), 5);
    cbToMonth->addItem(tr("June"), 6);
    cbToMonth->addItem(tr("July"), 7);
    cbToMonth->addItem(tr("August"), 8);
    cbToMonth->addItem(tr("September"), 9);
    cbToMonth->addItem(tr("October"), 10);
    cbToMonth->addItem(tr("November"), 11);
    cbToMonth->addItem(tr("December"), 12);
    cbToMonth->setCurrentIndex(11);

    mHtml = "<p>";
    mHtml += tr("Select from and to period above and click refresh button ...");
    mHtml += "</p>";

    teCostCenter->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_CostCenterBalanceReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_CostCenterBalanceReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_CostCenterBalanceReportWidget::filePdf(QPrinter* printer) {
    teCostCenter->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_CostCenterBalanceReportWidget::filePrint(QPrinter* printer) {
    teCostCenter->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_CostCenterBalanceReportWidget::filePrintPreview(QPrinter* printer) {
    teCostCenter->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_CostCenterBalanceReportWidget::editCut() {
    teCostCenter->cut();
}

/**
 * Edit Copy.
 */
void ACC_CostCenterBalanceReportWidget::editCopy() {
    teCostCenter->copy();
}

/**
 * Edit Paste.
 */
void ACC_CostCenterBalanceReportWidget::editPaste() {
    teCostCenter->paste();
}

/**
 * Edit Undo.
 */
void ACC_CostCenterBalanceReportWidget::editUndo() {
    teCostCenter->undo();
}

/**
 * Edit Redo.
 */
void ACC_CostCenterBalanceReportWidget::editRedo() {
    teCostCenter->redo();
}

/**
 * Select All.
 */
void ACC_CostCenterBalanceReportWidget::editSelectAll() {
    teCostCenter->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_CostCenterBalanceReportWidget::editInsertImage(const QString& fn) {
    teCostCenter->insertImage(fn);
}

/**
 * @brief ACC_CostCenterBalanceReportWidget::editZoomIn
 */
void ACC_CostCenterBalanceReportWidget::editZoomIn(double /*factor*/) {
   teCostCenter->fontZoomIn(1);
}

/**
 * @brief ACC_CostCenterBalanceReportWidget::editZoomOut
 */
void ACC_CostCenterBalanceReportWidget::editZoomOut(double /*factor*/) {
    teCostCenter->fontZoomOut(1);
}

/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_CostCenterBalanceReportWidget::saveFile(const QString &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ACC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        ACC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teCostCenter->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    ACC_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Initialize and create trial balance
 */
void ACC_CostCenterBalanceReportWidget::on_pbRefresh_clicked() {
    RB_ObjectContainer* centerBalanceList = new RB_ObjectContainer("", NULL, "ACC_CostCenterBalanceList");

    ACC_CreateCostCenterBalanceReport* oper = new ACC_CreateCostCenterBalanceReport();
    mFromPrd = sbFromYear->value() * 100
               + cbFromMonth->itemData(cbFromMonth->currentIndex(), Qt::UserRole).toInt();
    if (cbFromMonth->currentIndex() <= cbToMonth->currentIndex()) {
        // same year
        mToPrd = sbFromYear->value() * 100
                 + cbToMonth->itemData(cbToMonth->currentIndex(), Qt::UserRole).toInt();
    } else {
        // next year
        mToPrd = (sbFromYear->value() + 1) * 100
                 + cbToMonth->itemData(cbToMonth->currentIndex(), Qt::UserRole).toInt();
    }

    oper->setPeriod(mFromPrd, mToPrd);
    oper->execute(centerBalanceList);
    createTable(centerBalanceList);

    delete oper;
    delete centerBalanceList;
    teCostCenter->setHtml(mHtml);
}

/**
 * Create detail table
 * The data objects will have:
 * 0 suppname_mname (ACC_Supplier/ACC_Customer)
 * 1 sequenceno (ACC_CostGroup)
 * 2 groupname (ACC_CostGroup)
 * 3 costprofit (ACC_CostGroup)
 * 4 centercode (ACC_CostCenter)
 * 5 centername (ACC_CostCenter)
 * 6 budget (ACC_CostBudget)
 * 7 forecast (ACC_CostBudget)
 * 8 actual (ACC_CostSum)
 * 9 weight (ACC_SupplCostPart/ACC_CustCostPart)
 * 10 type_id (ACC_SupplCostPart/ACC_CustCostPart) related to budget,
 * forecast or actual value
 * 11 startdate (ACC_SupplCostPart/ACC_CustCostPart)
 * 12 enddate (ACC_SupplCostPart/ACC_CustCostPart)
 * @param centerBalanceList list of data objects
 */
void ACC_CostCenterBalanceReportWidget::createTable(RB_ObjectContainer* centerBalanceList) {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("company").toString();
    mGroupBudget1 = 0.0;
    mGroupForecast1 = 0.0;
    mGroupActual1 = 0.0;
    mTotalBudget1 = 0.0;
    mTotalForecast1 = 0.0;
    mTotalActual1 = 0.0;
    mGroupName = "";

    QDateTime dt = QDateTime::currentDateTime();

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=11 align='center' style=\"font-size:11pt;\">" + tr("Cost Center Balance Details") + " - " + coyName + "</th></tr>"
    "<tr><td colspan=11 align='center'>" + tr("Period: ")
            + RB_String::number(mFromPrd) + " - " + RB_String::number(mToPrd) + "</td></tr>"
    "<tr><td colspan=11 align='center'>" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
            "<tr><td colspan=11 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>"
    "<tr><th>" + tr("Name") + "</th><th>" + tr("Seq.No.") + "</th>"
            "<th>" + tr("Group Name") + "</th><th>" + tr("Cost/Profit") + "</th>"
            "<th>" + tr("Center Code") + "</th><th>" + tr("Center Name") + "</th>"
            "<th>" + tr("Budget") + "</th><th>" + tr("Forecast") + "</th>"
            "<th>" + tr("Actual") + "</th><th>" + tr("Start") + "</th>"
            "<th>" + tr("End") + "</th></tr>"
            "<tr><td colspan=11 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>"
    "</thead>"
    "<tbody>";

    bool isProfitCenterSet = false;
    bool isCostCenterSet = false;

    RB_ObjectIterator* iter = centerBalanceList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (!isProfitCenterSet && obj->getValue("costprofit").toInt() == 1) {
            mHtml += "<tr><td colspan=3>&nbsp;</td><td colspan=8><strong><u>" + tr("Profit Centers") + "</u></strong></td></tr>";
            isProfitCenterSet = true;
        }
        if (!isCostCenterSet && obj->getValue("costprofit").toInt() == 0) {
            mHtml += "<tr><td colspan=3>&nbsp;</td><td colspan=8><strong><u>" + tr("Cost Centers") + "</u></strong></td></tr>";
            isCostCenterSet = true;
        }

        // add data row
        mHtml += "<tr><td>" + obj->getValue("suppname_mname").toString();
        mHtml += "</td><td>" + obj->getValue("sequenceno").toString();
        mHtml += "</td><td>" + obj->getValue("groupname").toString();
        mHtml += "</td><td>" + obj->getValue("costprofit").toString();
        mHtml += "</td><td>" + obj->getValue("centercode").toString();
        mHtml += "</td><td>" + obj->getValue("centername").toString();
        mHtml += "</td><td align='right'>" + obj->getValue("budget").toString();
        mHtml += "</td><td align='right'>" + obj->getValue("forecast").toString();
        mHtml += "</td><td align='right'>" + obj->getValue("actual").toString();
        mHtml += "</td><td align='right'>" + obj->getValue("startdate").toDate().toString(Qt::ISODate);
        mHtml += "</td><td align='right'>" + obj->getValue("enddate").toDate().toString(Qt::ISODate);
        mHtml += "</td></tr>";

    }
    delete iter;

    mHtml += "</tbody>";
    mHtml += "</table>";
    mHtml += "<p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Add an account group header row
 */
void ACC_CostCenterBalanceReportWidget::addHeaderRow() {
    mHtml += "<tr><td colspan=5><strong>";
    mHtml += mGroupName;
    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_CostCenterBalanceReportWidget::addDataRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td>";

    mHtml += obj->getValue("centercode").toString();
    mHtml += "</td><td>";
    RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";
    mHtml += obj->getValue("centername").toString();

    setHtmlTableCell(betweenCellsWhite, obj->getValue("budget").toDouble());
    setHtmlTableCell(betweenCellsWhite, obj->getValue("forecast").toDouble());
    setHtmlTableCell(betweenCellsWhite, obj->getValue("actual").toDouble());

    mHtml += "</td></tr>";
}

/**
 * Set HTML table cell tag and innertext (amount) for data row
 * @param cellTag cell tag such as </td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>
 * @param amt amount
 */
void ACC_CostCenterBalanceReportWidget::setHtmlTableCell(const RB_String& cellTag, double amt) {
    if (amt != 0.0) {
        mHtml += cellTag + RB_String::number(amt,'f', 2);
    } else {
        mHtml += cellTag + "&nbsp;";
    }
}

/**
 * Add an account group footer row
 */
void ACC_CostCenterBalanceReportWidget::addFooterRow() {
    mHtml += "<tr><td colspan=2 align='right'><i>";

//    RB_String betweenCellsGray = "</i></td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";"
//                                 "border-top: solid 1px " + mTextColor.name() + ";'><i>";
    RB_String betweenCellsWhite = "</i></td><td align='right' style='border-top: solid 1px " + mTextColor.name() + ";'><i>";

    mHtml += mGroupName + " " + tr("Totals") + betweenCellsWhite;
    mHtml += RB_String::number(mGroupBudget1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupForecast1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupActual1,'f', 2); // + betweenCellsWhite;

    mHtml += "</i></td></tr>";
}

/**
 * Add footer with totals
 */
void ACC_CostCenterBalanceReportWidget::addFooter() {
    mHtml += "<tfoot style='font-size:8pt;'><tr><td colspan=2>";
    mHtml += "&nbsp;</td><td align='right' style='background-color: "
            + mBaseColor.name() + ";'>";
    RB_String betweenCellsGray = "</td><td align='right' style='background-color: "
            + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";
    mHtml += RB_String::number(mTotalBudget1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalForecast1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalActual1,'f', 2);

    mHtml += "</td></tr></tfoot>";
}

/**
 * Change event such as language change
 */
void ACC_CostCenterBalanceReportWidget::changeEvent(QEvent *e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
