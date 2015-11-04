/*****************************************************************
 * $Id: acc_costcenterreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jul 9, 2012 6:48:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_costcenterreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc_createcostcenter3report.h"
#include "acc_createcostcenterreport.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"

/**
 * Constructor
 */
ACC_CostCenterReportWidget::ACC_CostCenterReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);
}

/**
 * Destructor
 */
ACC_CostCenterReportWidget::~ACC_CostCenterReportWidget() {
    // nothing
}

/**
 * Initialize and create trial balance
 */
void ACC_CostCenterReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    cbPeriodSelection->addItem(tr("Period from:"));
    cbPeriodSelection->addItem(tr("3 months:"));
    cbPeriodSelection->addItem(tr("3 years:"));
    cbPeriodSelection->setCurrentIndex(0);

    connect(cbPeriodSelection, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotPeriodSelectionChanged(int)));

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
bool ACC_CostCenterReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_CostCenterReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_CostCenterReportWidget::filePdf(QPrinter* printer) {
    teCostCenter->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_CostCenterReportWidget::filePrint(QPrinter* printer) {
    teCostCenter->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_CostCenterReportWidget::filePrintPreview(QPrinter* printer) {
    teCostCenter->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_CostCenterReportWidget::editCut() {
    teCostCenter->cut();
}

/**
 * Edit Copy.
 */
void ACC_CostCenterReportWidget::editCopy() {
    teCostCenter->copy();
}

/**
 * Edit Paste.
 */
void ACC_CostCenterReportWidget::editPaste() {
    teCostCenter->paste();
}

/**
 * Edit Undo.
 */
void ACC_CostCenterReportWidget::editUndo() {
    teCostCenter->undo();
}

/**
 * Edit Redo.
 */
void ACC_CostCenterReportWidget::editRedo() {
    teCostCenter->redo();
}

/**
 * Select All.
 */
void ACC_CostCenterReportWidget::editSelectAll() {
    teCostCenter->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_CostCenterReportWidget::editInsertImage(const QString& fn) {
    teCostCenter->insertImage(fn);
}

/**
 * @brief ACC_CostCenterReportWidget::editZoomIn
 */
void ACC_CostCenterReportWidget::editZoomIn(double /*factor*/) {
   teCostCenter->fontZoomIn(1);
}

/**
 * @brief ACC_CostCenterReportWidget::editZoomOut
 */
void ACC_CostCenterReportWidget::editZoomOut(double /*factor*/) {
    teCostCenter->fontZoomOut(1);
}

/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_CostCenterReportWidget::saveFile(const QString &fn) {
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
void ACC_CostCenterReportWidget::on_pbRefresh_clicked() {
    RB_ObjectContainer* costCenterSumList = new RB_ObjectContainer("", NULL, "ACC_CostCenterSummaryList");

    if (cbPeriodSelection->currentIndex() == 0) {
        ACC_CreateCostCenterReport* oper = new ACC_CreateCostCenterReport();
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
        oper->execute(costCenterSumList);
        createTable(costCenterSumList);

        delete oper;
    } else {
        ACC_CreateCostCenter3Report* oper = new ACC_CreateCostCenter3Report();

        if (cbPeriodSelection->currentIndex() == 1) {
            // middle month
            mToPrd = sbFromYear->value() * 100
                    + cbFromMonth->itemData(cbFromMonth->currentIndex(),
                                            Qt::UserRole).toInt();
            if (cbFromMonth->itemData(cbFromMonth->currentIndex(),
                                      Qt::UserRole).toInt() > 1) {
                mFromPrd = mToPrd - 1;
            } else {
                // 201201 -> 201112
                mFromPrd = mToPrd - 89;
            }

            if (cbFromMonth->itemData(cbFromMonth->currentIndex(),
                                      Qt::UserRole).toInt() < 12) {
                mPrd3 = mToPrd + 1;
            } else {
                // 201112 -> 201201
                mPrd3 = mToPrd + 89;
            }

        } else {
            // middle year
            mToPrd = sbFromYear->value() * 100 + 1;
            mFromPrd = mToPrd - 100;
            mPrd3 = mToPrd + 100;
        }

        oper->setPeriod(mFromPrd, mToPrd, mPrd3);
        oper->execute(costCenterSumList);
        createTable(costCenterSumList);

        delete oper;
    }


    delete costCenterSumList;
    teCostCenter->setHtml(mHtml);
}

/**
 * Slot period selection changed
 * @param index index of period selected
 */
void ACC_CostCenterReportWidget::slotPeriodSelectionChanged(int index) {
    switch (index) {
    case 0:
        cbFromMonth->show();
        lblTo->show();
        cbToMonth->show();
        break;
    case 1:
        cbFromMonth->show();
        lblTo->hide();
        cbToMonth->hide();
        break;
    case 2:
        cbFromMonth->hide();
        lblTo->hide();
        cbToMonth->hide();
        break;
    default:
        cbFromMonth->show();
        lblTo->show();
        cbToMonth->show();
        break;
    }
}

/**
 * Create trial balance table in webview, including header and footer
 */
void ACC_CostCenterReportWidget::createTable(RB_ObjectContainer* trialBalanceList) {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("coyname").toString();
    mGroupBudget1 = 0.0;
    mGroupForecast1 = 0.0;
    mGroupActual1 = 0.0;
    mGroupBudget2 = 0.0;
    mGroupForecast2 = 0.0;
    mGroupActual2 = 0.0;
    mGroupBudget3 = 0.0;
    mGroupForecast3 = 0.0;
    mGroupActual3 = 0.0;
    mTotalBudget1 = 0.0;
    mTotalForecast1 = 0.0;
    mTotalActual1 = 0.0;
    mTotalBudget2 = 0.0;
    mTotalForecast2 = 0.0;
    mTotalActual2 = 0.0;
    mTotalBudget3 = 0.0;
    mTotalForecast3 = 0.0;
    mTotalActual3 = 0.0;
    mGroupName = "";

    QDateTime dt = QDateTime::currentDateTime();

    if (cbPeriodSelection->currentIndex() == 0) {
        mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
        "<thead>"
        "<tr><th colspan=5 align='center' style='font-size:11pt;'>" + tr("Cost Center Summary") + " - " + coyName + "</th></tr>"
        "<tr><td colspan=5 align='center'>" + tr("Period: ")
                + RB_String::number(mFromPrd) + " - " + RB_String::number(mToPrd) + "</td></tr>"
        "<tr><td colspan=5 align='center'>" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
        "<tr><td colspan=5 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>"
        "<tr><th>" + tr("Code") + "</th><th>" + tr("Center Name") + "</th>"
                "<th colspan=3>" + tr("Period") + "</th></tr>"
        "<tr><td>&nbsp;</td><td>&nbsp;</td>"
        "<td align='center'>" + tr("Budget") + "</td><td align='center'>" + tr("Forecast")
                + "</td><td align='center'>" + tr("Actual") + "</td></tr>"
        "<tr><td colspan=5 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>"
        "</thead>"
        "<tbody style='font-size:8pt;'>";
    } else {
        mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
        "<thead>"
        "<tr><th colspan=11 align='center'>" + tr("Cost Center Summary") + " - " + coyName + "</th></tr>"
        "<tr><td colspan=11 align='center' style='font-size:8pt;'>" + tr("Period: ")
                + RB_String::number(mFromPrd) + " - " + RB_String::number(mToPrd) + " - " + RB_String::number(mPrd3) + "</td></tr>"
        "<tr><td colspan=11 align='center' style='font-size:8pt;'>" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
        "<tr><td colspan=11 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>"
        "<tr><th>" + tr("Code") + "</th><th>" + tr("Center Name") + "</th>"
                "<th colspan=3>" + tr("Period 1") + "</th><th colspan=3>" + tr("Period 2")
                    + "</th><th colspan=3>" + tr("Period 3") + "</th></tr>"
        "<tr><td>&nbsp;</td><td>&nbsp;</td>"
                "<td align='center'>" + tr("Budget") + "</td><td align='center'>" + tr("Forecast")
                        + "</td><td align='center'>" + tr("Actual") + "</td>"
                "<td align='center'>" + tr("Budget") + "</td><td align='center'>" + tr("Forecast")
                        + "</td><td align='center'>" + tr("Actual") + "</td>"
                "<td align='center'>" + tr("Budget") + "</td><td align='center'>" + tr("Forecast")
                        + "</td><td align='center'>" + tr("Actual") + "</td></tr>"
        "<tr><td colspan=11 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>"
        "</thead>"
        "<tbody style='font-size:8pt;'>";
    }

    RB_ObjectIterator* iter = trialBalanceList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (mGroupName != obj->getValue("groupname").toString()) {
            if(!mGroupName.isEmpty()) {
                addFooterRow();
            }

            mTotalBudget1 += mGroupBudget1;
            mTotalForecast1 += mGroupForecast1;
            mTotalActual1 += mGroupActual1;

            if (cbPeriodSelection->currentIndex() != 0) {
                mTotalBudget2 += mGroupBudget2;
                mTotalForecast2 += mGroupForecast2;
                mTotalActual2 += mGroupActual2;
                mTotalBudget3 += mGroupBudget3;
                mTotalForecast3 += mGroupForecast3;
                mTotalActual3 += mGroupActual3;
            }

            mGroupName = obj->getValue("groupname").toString();

            mGroupBudget1 = 0.0;
            mGroupForecast1 = 0.0;
            mGroupActual1 = 0.0;

            if (cbPeriodSelection->currentIndex() != 0) {
                mGroupBudget2 = 0.0;
                mGroupForecast2 = 0.0;
                mGroupActual2 = 0.0;
                mGroupBudget3 = 0.0;
                mGroupForecast3 = 0.0;
                mGroupActual3 = 0.0;
            }

            addHeaderRow();
        }
        addDataRow(obj);

        mGroupBudget1 += obj->getValue("budget").toDouble();
        mGroupForecast1 += obj->getValue("forecast").toDouble();
        mGroupActual1 += obj->getValue("actual").toDouble();

        if (cbPeriodSelection->currentIndex() != 0) {
            mGroupBudget2 += obj->getValue("budget2").toDouble();
            mGroupForecast2 += obj->getValue("forecast2").toDouble();
            mGroupActual2 += obj->getValue("actual2").toDouble();
            mGroupBudget3 += obj->getValue("budget3").toDouble();
            mGroupForecast3 += obj->getValue("forecast3").toDouble();
            mGroupActual3 += obj->getValue("actual3").toDouble();
        }
    }
    delete iter;

    addFooterRow();

    mTotalBudget1 += mGroupBudget1;
    mTotalForecast1 += mGroupForecast1;
    mTotalActual1 += mGroupActual1;

    if (cbPeriodSelection->currentIndex() != 0) {
        mTotalBudget2 += mGroupBudget2;
        mTotalForecast2 += mGroupForecast2;
        mTotalActual2 += mGroupActual2;
        mTotalBudget3 += mGroupBudget3;
        mTotalForecast3 += mGroupForecast3;
        mTotalActual3 += mGroupActual3;
    }

    mHtml += "</tbody>";

    addFooter();

    mHtml += "</table>";
    mHtml += "<p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Add an account group header row
 */
void ACC_CostCenterReportWidget::addHeaderRow() {
    if (cbPeriodSelection->currentIndex() == 0) {
        mHtml += "<tr><td colspan=5><strong>";
    } else {
        mHtml += "<tr><td colspan=11><strong>";
    }
    mHtml += mGroupName;
    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_CostCenterReportWidget::addDataRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td>";

    mHtml += obj->getValue("centercode").toString();
    mHtml += "</td><td>";
    RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";
    mHtml += obj->getValue("centername").toString();

    setHtmlTableCell(betweenCellsWhite, obj->getValue("budget").toDouble());
    setHtmlTableCell(betweenCellsWhite, obj->getValue("forecast").toDouble());
    setHtmlTableCell(betweenCellsWhite, obj->getValue("actual").toDouble());

    if (cbPeriodSelection->currentIndex() != 0) {
        setHtmlTableCell(betweenCellsGray, obj->getValue("budget2").toDouble());
        setHtmlTableCell(betweenCellsGray, obj->getValue("forecast2").toDouble());
        setHtmlTableCell(betweenCellsGray, obj->getValue("actual2").toDouble());

        setHtmlTableCell(betweenCellsWhite, obj->getValue("budget3").toDouble());
        setHtmlTableCell(betweenCellsWhite, obj->getValue("forecast3").toDouble());
        setHtmlTableCell(betweenCellsWhite, obj->getValue("actual3").toDouble());
    }

    mHtml += "</td></tr>";
}

/**
 * Set HTML table cell tag and innertext (amount) for data row
 * @param cellTag cell tag such as </td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>
 * @param amt amount
 */
void ACC_CostCenterReportWidget::setHtmlTableCell(const RB_String& cellTag, double amt) {
    if (amt != 0.0) {
        mHtml += cellTag + RB_String::number(amt,'f', 2);
    } else {
        mHtml += cellTag + "&nbsp;";
    }
}

/**
 * Add an account group footer row
 */
void ACC_CostCenterReportWidget::addFooterRow() {
    if (cbPeriodSelection->currentIndex() == 0) {
        mHtml += "<tr><td colspan=2>" + teCostCenter->getOnePixelTransparentImage()
                + "</td><td colspan=3 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>";
    } else {
        mHtml += "<tr><td colspan=2>" + teCostCenter->getOnePixelTransparentImage()
                + "</td><td colspan=9 style='background-color:" + mTextColor.name() + ";'>" + teCostCenter->getOnePixelTransparentImage() + "</td></tr>";
    }
    mHtml += "<tr><td colspan=2 align='right'><i>";

    RB_String betweenCellsGray = "</i></td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";"
                                 "border-top: solid 1px " + mTextColor.name() + ";'><i>";
    RB_String betweenCellsWhite = "</i></td><td align='right' style='border-top: solid 1px " + mTextColor.name() + ";'><i>";

    mHtml += mGroupName + " " + tr("Totals") + betweenCellsWhite;
    mHtml += RB_String::number(mGroupBudget1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupForecast1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupActual1,'f', 2); // + betweenCellsWhite;

    if (cbPeriodSelection->currentIndex() != 0) {
        mHtml += betweenCellsGray + RB_String::number(mGroupBudget2,'f', 2);
        mHtml += betweenCellsGray + RB_String::number(mGroupForecast2,'f', 2);
        mHtml += betweenCellsGray + RB_String::number(mGroupActual2,'f', 2);
        mHtml += betweenCellsWhite + RB_String::number(mGroupBudget3,'f', 2);
        mHtml += betweenCellsWhite + RB_String::number(mGroupForecast3,'f', 2);
        mHtml += betweenCellsWhite + RB_String::number(mGroupActual3,'f', 2);
    }

    mHtml += "</i></td></tr>";
}

/**
 * Add footer with totals
 */
void ACC_CostCenterReportWidget::addFooter() {
    mHtml += "<tfoot style='font-size:8pt;'><tr><td colspan=2>";

    mHtml += "&nbsp;</td><td align='right' style='background-color: " + mBaseColor.name() + ";'>";
    RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";
    mHtml += RB_String::number(mTotalBudget1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalForecast1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalActual1,'f', 2);

    if (cbPeriodSelection->currentIndex() != 0) {
        mHtml += betweenCellsGray + RB_String::number(mTotalBudget2,'f', 2);
        mHtml += betweenCellsGray + RB_String::number(mTotalForecast2,'f', 2);
        mHtml += betweenCellsGray + RB_String::number(mTotalActual2,'f', 2);
        mHtml += betweenCellsWhite + RB_String::number(mTotalBudget3,'f', 2);
        mHtml += betweenCellsWhite + RB_String::number(mTotalForecast3,'f', 2);
        mHtml += betweenCellsWhite + RB_String::number(mTotalActual3,'f', 2);
    }

    mHtml += "</td></tr></tfoot>";
}

/**
 * Change event such as language change
 */
void ACC_CostCenterReportWidget::changeEvent(QEvent *e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
