/*****************************************************************
 * $Id: acc_salestaxreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 10, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salestaxreportwidget.h"

#include <cmath>
#include <QPrintDialog>
#include <QPrinter>
#include <QWebFrame>
#include "acc_calcsalestax.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_qachartmaster.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_SalesTaxReportWidget::ACC_SalesTaxReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);

    mSumObj = NULL;
    mSumAmount = 0.0;
    mSumTaxAmount = 0.0;
    mSumTaxTotal = 0.0;
    mSumTaxRev = 0.0;
}

/**
 * Destructor
 */
ACC_SalesTaxReportWidget::~ACC_SalesTaxReportWidget() {
    delete mSumObj;
}

/**
 * Initialize and prepare journal(=document) transactions
 */
void ACC_SalesTaxReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    mSumObj = ACC_OBJECTFACTORY->newSingleObject("ACC_GlTrans"); // refer ACC_CalcSalesTax
    mSumObj->addMember("accountcontrol", "-", 0, RB2::MemberInteger);
    mSumObj->addMember("taxamount", "-", 0, RB2::MemberInteger);
    mSumObj->setValue("amount", 0.0);
    mSumObj->setValue("taxamount", 0.0);

    QDateTime dt;
    int year = dt.currentDateTime().date().year();
    QDate dateStart(year, 1, 1);
    dtFrom->setDate(dateStart);
    QDate dateEnd(year, 12, 31);
    dtTo->setDate(dateEnd);

    mHtml = "<p>";
    mHtml += tr("Select from/to period above, and click refresh button ...");
    mHtml += "</p>";

    teTax->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_SalesTaxReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_SalesTaxReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * File export PDF.
 * @param printer
 */
void ACC_SalesTaxReportWidget::filePdf(QPrinter* printer) {
    teTax->print(printer);
}

/**
 * File Print.
 * @param printer
 */
void ACC_SalesTaxReportWidget::filePrint(QPrinter* printer) {
    teTax->print(printer);
}

/**
 * File Print Preview.
 * @param printer
 */
void ACC_SalesTaxReportWidget::filePrintPreview(QPrinter* printer) {
    teTax->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_SalesTaxReportWidget::editCut() {
    teTax->cut();
}

/**
 * Edit Copy.
 */
void ACC_SalesTaxReportWidget::editCopy() {
    teTax->copy();
}

/**
 * Edit Paste.
 */
void ACC_SalesTaxReportWidget::editPaste() {
    teTax->paste();
}

/**
 * Edit Undo.
 */
void ACC_SalesTaxReportWidget::editUndo() {
    teTax->undo();
}

/**
 * Edit Redo.
 */
void ACC_SalesTaxReportWidget::editRedo() {
    teTax->redo();
}

/**
 * Select All.
 */
void ACC_SalesTaxReportWidget::editSelectAll() {
    teTax->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_SalesTaxReportWidget::editInsertImage(const QString& fn) {
    teTax->insertImage(fn);
}

/**
 * @brief ACC_SalesTaxReportWidget::editZoomIn
 */
void ACC_SalesTaxReportWidget::editZoomIn(double /*factor*/) {
   teTax->fontZoomIn(1);
}

/**
 * @brief ACC_SalesTaxReportWidget::editZoomOut
 */
void ACC_SalesTaxReportWidget::editZoomOut(double /*factor*/) {
    teTax->fontZoomOut(1);
}

/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_SalesTaxReportWidget::saveFile(const QString &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ACC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        ACC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teTax->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    ACC_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Initialize and create trial balance
 */
void ACC_SalesTaxReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    mSumAmount = 0.0;
    mSumTaxAmount = 0.0;
    mSumTaxTotal = 0.0;
    mSumTaxRev = 0.0;

    ACC_CalcSalesTax* oper = new ACC_CalcSalesTax();
    RB_ObjectContainer* taxList = new RB_ObjectContainer("", NULL, "ACC_GlTransList");

    bool sumOnly = ckbDetails->isChecked() ? false : true;
    oper->setPeriod(dtFrom->dateTime(), dtTo->dateTime(), sumOnly);
    oper->execute(taxList);
    createTable(taxList);

    delete taxList;
    delete oper;

    teTax->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Create journal transaction table in webview, including header and footer
 */
void ACC_SalesTaxReportWidget::createTable(RB_ObjectContainer* taxList) {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("coyname").toString();
    QDateTime dt = QDateTime::currentDateTime();

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=5 align='center' style='font-size:11pt;'>" + tr("Sales Tax Report") + " - " + coyName + "</th></tr>"
    "<tr><td colspan=5 align='center'>Period: "
    + dtFrom->date().toString(Qt::ISODate) + " / " + dtTo->date().toString(Qt::ISODate) + "<br />"
    + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=5 style='background-color:" + mTextColor.name() + ";'>" + teTax->getOnePixelTransparentImage() + "</td></tr>"
    "<tr><th>" + tr("Item") + "</th><th>" + tr("Description") + "</th>"
    "<th>" + tr("Function") + "</th><th>" + tr("Amount excl. Tax") + "</th>"
    "<th>" + tr("Tax Amount") + "</th></tr>"
    "<tr><td colspan=5 style='background-color:" + mTextColor.name() + ";'>" + teTax->getOnePixelTransparentImage() + "</td></tr>"
    "</thead>"
    "<tbody>";
    RB_ObjectIterator* iter = taxList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (!ckbDetails->isChecked()) {
            addSumRow(obj);
        } else {
            addDataRow(obj);
        }
    }
    delete iter;

    mHtml += "</tbody></table>";
    mHtml += "<p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Add a data row with amounts
 */
void ACC_SalesTaxReportWidget::addDataRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td>";

    int ac = obj->getValue("accountcontrol").toInt();
    RB_String itemNr = "";
    RB_String subject = "";
    dutchSalesTaxItem(ac, itemNr, subject);

    mHtml += itemNr + "</td><td>";
    RB_String betweenCellsLeft = "</td><td>";
    RB_String betweenCellsRight = "</td><td align='right'>";

    if (!ckbDetails->isChecked()) {
        mHtml += subject + betweenCellsLeft;
    } else {
        mHtml += obj->getValue("description").toString() + betweenCellsLeft;
    }

    mHtml += RB_String::number(ac) + betweenCellsRight;

    double amount = 0.0;
    double taxAmount = 0.0;

    if (!ckbDetails->isChecked()) {
        amount = obj->getValue("amount").toDouble();
        taxAmount = obj->getValue("taxamount").toDouble();
    } else if (ac == ACC2::ControlSalesHighTax
               || ac == ACC2::ControlSalesLowTax
               || ac == ACC2::ControlSalesOtherTax
               || ac == ACC2::ControlPrivateUseTax
               || ac == ACC2::ControlSalesNoTax
               || ac == ACC2::ControlSalesOutEu
               || ac == ACC2::ControlPurchaseRevTax
               || ac == ACC2::ControlSalesInEu
               || ac == ACC2::ControlServiceInEu
               || ac == ACC2::ControlInstallInEu
               || ac == ACC2::ControlPurchInEuLow
               || ac == ACC2::ControlPurchInEuHigh
               || ac == ACC2::ControlPurchInEuOther
               || ac == ACC2::ControlPurchOutEuLow
               || ac == ACC2::ControlPurchOutEuHigh
               || ac == ACC2::ControlPurchOutEuOther) {
        amount = obj->getValue("amount").toDouble();
        taxAmount = 0.0;
    } else {
        amount = 0.0;
        taxAmount = obj->getValue("amount").toDouble();
    }

    if (amount == 0.0) {
        mHtml += "&nbsp;" + betweenCellsRight;
    } else {
        mHtml += RB_String::number(-amount,'f', 2) + betweenCellsRight;
    }

    if (taxAmount == 0.0) {
        mHtml += "&nbsp;";
    } else {
        mHtml += RB_String::number(-taxAmount,'f', 2);
    }

    mHtml += "</td></tr>";
}

/**
 * Add a data row with amounts. This can be handled with the
 * switch and case commands because the originating query
 * is using the same sequence for the accountcontrol
 */
void ACC_SalesTaxReportWidget::addSumRow(RB_ObjectBase* obj) {
    int ac = obj->getValue("accountcontrol").toInt();
    double taxRate = 0.0;
    double amount = 0.0;

    // not all are necessary for example 9, 30, 32, 31 and 33
    switch (ac) {
    case ACC2::ControlTaxPayHigh : // 22
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlTaxPayGlobHigh : // 27
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlSalesHighTax : // 12
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlSalesGlobHigh : // 92
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlPurchGlobHigh : // 95
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 1a
        //
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxHighPercentage() / 100;
        if (std::fabs(mSumAmount * taxRate - mSumTaxAmount) < 0.5) {
            // normal
            mSumObj->setValue("amount", mSumAmount);
            mSumObj->setValue("accountcontrol", ac);
            mSumObj->setValue("taxamount", mSumTaxAmount);
            addDataRow(mSumObj);
        } else if (std::fabs(mSumAmount) >= 0.5) {
            // calculated
            mSumObj->setValue("amount", mSumAmount);
            mSumObj->setValue("accountcontrol", ac);
            mSumObj->setValue("taxamount", mSumTaxAmount);
            addDataRow(mSumObj);
            addErrorRow(tr("Warning: the booked tax is inconsistent "
                           " or related account is not used. Calculated: %1")
                        .arg(-mSumAmount * taxRate, 0, 'f', 2));
        } else {
            // function 12, 92, 95 have not been used
            mSumObj->setValue("amount", 0.0);
            mSumObj->setValue("accountcontrol", ac);
            mSumObj->setValue("taxamount", 0.0); // calculated
            addDataRow(mSumObj);
            if (std::fabs(mSumTaxAmount) > 0.005) {
                addErrorRow(tr("Warning: booked tax is not used, "
                               "because the corresponding amount is zero."));
            }
        }
        mSumTaxTotal += mSumTaxAmount;
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlTaxPayLow : // 21
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlTaxPayGlobLow : // 26
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlSalesLowTax : // 11
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlSalesGlobLow : // 91
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlPurchGlobLow : // 94
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 1b
        //
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxLowPercentage() / 100;
        if (std::fabs(mSumAmount * taxRate - mSumTaxAmount) < 0.5) {
            // normal
            mSumObj->setValue("amount", mSumAmount);
            mSumObj->setValue("accountcontrol", ac);
            mSumObj->setValue("taxamount", mSumTaxAmount);
            addDataRow(mSumObj);
        } else if (std::fabs(mSumAmount) >= 0.5) {
            // calculated
            mSumObj->setValue("amount", mSumAmount);
            mSumObj->setValue("accountcontrol", ac);
            mSumObj->setValue("taxamount", mSumTaxAmount);
            addDataRow(mSumObj);
            addErrorRow(tr("Warning: the booked tax is inconsistent "
                           " or related account is not used. Calculated: %1")
                        .arg(-mSumAmount * taxRate, 0, 'f', 2));
        } else {
            // function 11, 91, 94 have not been used
            mSumObj->setValue("amount", 0.0);
            mSumObj->setValue("accountcontrol", ac);
            mSumObj->setValue("taxamount", 0.0);
            addDataRow(mSumObj);
            if (std::fabs(mSumTaxAmount) > 0.005) {
                addErrorRow(tr("Warning: booked tax is not used, "
                               "because the corresponding amount is zero."));
            }
        }
        mSumTaxTotal += mSumTaxAmount;
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlTaxPayOther : // 23
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlTaxPayGlobOther : // 28
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlSalesOtherTax : // 13
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlSalesGlobOther : // 93
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlPurchGlobOther : // 96
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 1c
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumTaxTotal += mSumTaxAmount;
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlTaxPrivateUse : // 29
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlPrivateUseTax : // 19
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 1d
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumTaxTotal += mSumTaxAmount;
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlSalesNoTax : // 10
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlSalesRevTax : // 14
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 1e
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlTaxPurchPayRev : // 44
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlPurchaseRevTax : // 34
        mHtml += "<tr><td colspan=5>&nbsp;</td></tr>";
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 2a
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumTaxRev = mSumTaxAmount;
        mSumTaxTotal += mSumTaxAmount;
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlSalesOutEu : // 81
        mHtml += "<tr><td colspan=5>&nbsp;</td></tr>";
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 3a
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlSalesInEu : // 80
        mSumAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlServiceInEu : // 88
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 3b
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlInstallInEu : // 84
        mSumAmount += obj->getValue("amount").toDouble();

        // Handle 3c
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlTaxPurchOutEu : // 76
        // normally not used because no tax should be booked, except 'reversed' tax
        amount = obj->getValue("amount").toDouble();
        mSumTaxAmount -= amount;
        break;
    case ACC2::ControlPurchOutEuLow : // 71
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxLowPercentage() / 100;
        amount = obj->getValue("amount").toDouble();
//        mSumTaxAmount -= amount * taxRate; Not automatically calculated only from control 76
        mSumAmount -= amount;
        break;
    case ACC2::ControlPurchOutEuHigh : // 72
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxHighPercentage() / 100;
        amount = obj->getValue("amount").toDouble();
//        mSumTaxAmount -= amount * taxRate; Not automatically calculated only from control 76
        mSumAmount -= amount;
        break;
    case ACC2::ControlPurchOutEuOther : // 73
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxOtherPercentage() / 100;
        amount = obj->getValue("amount").toDouble();
//        mSumTaxAmount -= amount * taxRate; Not automatically calculated only from control 76
        mSumAmount -= amount;
        mHtml += "<tr><td colspan=5>&nbsp;</td></tr>";

        // Handle 4a
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumTaxTotal += mSumTaxAmount;
        mSumTaxRev += mSumTaxAmount;
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlTaxPurchInEu : // 66
        // normally not used because no tax should be booked, except 'reversed' tax
        amount = obj->getValue("amount").toDouble();
        mSumTaxAmount -= amount;
        break;
    case ACC2::ControlPurchInEuLow : // 61
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxLowPercentage() / 100;
        amount = obj->getValue("amount").toDouble();
//        mSumTaxAmount -= amount * taxRate; Not automatically calculated only from control 66
        mSumAmount -= amount;
        break;
    case ACC2::ControlPurchInEuHigh : // 62
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxHighPercentage() / 100;
        amount = obj->getValue("amount").toDouble();
//        mSumTaxAmount -= amount * taxRate; Not automatically calculated only from control 66
        mSumAmount -= amount;
        break;
    case ACC2::ControlPurchInEuOther : // 63
        taxRate = ACC_QACHARTMASTER->getAccSalesTaxOtherPercentage() / 100;
        amount = obj->getValue("amount").toDouble();
//        mSumTaxAmount -= amount * taxRate; Not automatically calculated only from control 66
        mSumAmount -= amount;

        // Handle 4b
        //
        mSumObj->setValue("amount", mSumAmount);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount);
        addDataRow(mSumObj);
        mSumTaxTotal += mSumTaxAmount;
        mSumTaxRev += mSumTaxAmount;
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        mHtml += "<tr><td colspan=4>"
                + teTax->getOnePixelTransparentImage() + "</td>"
                "<td style='background-color:" + mTextColor.name() + ";'>"
                + teTax->getOnePixelTransparentImage() + "</td></tr>";
        mHtml += "<tr><td colspan=5>&nbsp;</td></tr>";

        // Handle 5a
        //
        mSumObj->setValue("amount", 0.0);
        mSumObj->setValue("accountcontrol", (int)ACC2::ControlUserDefined);
        mSumObj->setValue("taxamount", mSumTaxTotal);
        addDataRow(mSumObj);
        mSumAmount = 0.0;
        mSumTaxAmount = 0.0;
        break;
    case ACC2::ControlTaxRecLow : // 41
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlTaxRecHigh : // 42
        mSumTaxAmount += obj->getValue("amount").toDouble();
        break;
    case ACC2::ControlTaxRecOther : //43
        mSumTaxAmount += obj->getValue("amount").toDouble();

        // Handle 5b
        //
        mSumObj->setValue("amount", 0.0);
        mSumObj->setValue("accountcontrol", ac);
        mSumObj->setValue("taxamount", mSumTaxAmount - mSumTaxRev);
        addDataRow(mSumObj);
        mSumAmount = 0.0;
        mHtml += "<tr><td colspan=4>"
                + teTax->getOnePixelTransparentImage() + "</td>"
                "<td style='background-color:" + mTextColor.name() + ";'>"
                + teTax->getOnePixelTransparentImage() + "</td></tr>";
        mHtml += "<tr><td colspan=5>&nbsp;</td></tr>";

        // Handle 5c
        //
        mSumObj->setValue("amount", 0.0);
        mSumObj->setValue("accountcontrol", (int)ACC2::ControlUserDefined + 1);
        mSumObj->setValue("taxamount", mSumTaxTotal + mSumTaxAmount - mSumTaxRev);
        addDataRow(mSumObj);

        // Handle 5d
        //
        mSumObj->setValue("amount", 0.0);
        mSumObj->setValue("accountcontrol", (int)ACC2::ControlUserDefined + 2);
        mSumObj->setValue("taxamount", 0.0);
        addDataRow(mSumObj);

        // Handle 5e
        //
        mSumObj->setValue("amount", 0.0);
        mSumObj->setValue("accountcontrol", (int)ACC2::ControlUserDefined + 3);
        mSumObj->setValue("taxamount", 0.0);
        addDataRow(mSumObj);

        // Handle 5f
        //
        mSumObj->setValue("amount", 0.0);
        mSumObj->setValue("accountcontrol", (int)ACC2::ControlUserDefined + 4);
        mSumObj->setValue("taxamount", 0.0);
        addDataRow(mSumObj);
        mHtml += "<tr><td colspan=4>"
                + teTax->getOnePixelTransparentImage() + "</td>"
                "<td style='background-color:" + mTextColor.name() + ";'>"
                + teTax->getOnePixelTransparentImage() + "</td></tr>";
        mHtml += "<tr><td colspan=5>&nbsp;</td></tr>";

        // Handle 5g
        //
        mSumObj->setValue("amount", 0.0);
        mSumObj->setValue("accountcontrol", (int)ACC2::ControlUserDefined + 5);
        mSumObj->setValue("taxamount", mSumTaxTotal + mSumTaxAmount - mSumTaxRev);
        addDataRow(mSumObj);
        break;
    default :
        break;
    }

}

/**
 * Show error message in case amount * taxRate is unequal to tax
 * @param errorMessage error message to be shown
 */
void ACC_SalesTaxReportWidget::addErrorRow(const RB_String& errorMessage) {
    mHtml += "<tr><td>&nbsp;</td>"
             "<td colspan=4><strong>" + errorMessage + "</strong></td></tr>";
}

/**
 * Set dutch sales tax number and subject
 * @param ac account control number (tax category)
 */
void ACC_SalesTaxReportWidget::dutchSalesTaxItem(int ac, RB_String& itemNr,
                                                 RB_String& subject) {
    switch (ac) {
    case ACC2::ControlTaxPayHigh :
    case ACC2::ControlTaxPayGlobHigh :
    case ACC2::ControlSalesHighTax :
    case ACC2::ControlSalesGlobHigh :
    case ACC2::ControlPurchGlobHigh :
        itemNr = "1a";
        subject = "Leveringen/diensten belast met ";
        subject += RB_String::number(ACC_QACHARTMASTER->getAccSalesTaxHighPercentage());
        subject += "%";
        break;
    case ACC2::ControlTaxPayLow :
    case ACC2::ControlTaxPayGlobLow :
    case ACC2::ControlSalesLowTax :
    case ACC2::ControlSalesGlobLow :
    case ACC2::ControlPurchGlobLow :
        itemNr = "1b";
        subject = "Leveringen/diensten belast met ";
        subject += RB_String::number(ACC_QACHARTMASTER->getAccSalesTaxLowPercentage());
        subject += "%";
        break;
    case ACC2::ControlTaxPayOther :
    case ACC2::ControlTaxPayGlobOther :
    case ACC2::ControlSalesOtherTax :
    case ACC2::ControlSalesGlobOther :
    case ACC2::ControlPurchGlobOther :
        itemNr = "1c";
        subject = "Leveringen/diensten belast met overige tarieven behalve 0%";
        break;
    case ACC2::ControlTaxPrivateUse :
    case ACC2::ControlPrivateUseTax :
        itemNr = "1d";
        subject = "Prive gebruik";
        break;
    case ACC2::ControlSalesNoTax :
    case ACC2::ControlSalesRevTax :
        itemNr = "1e";
        subject = "Leveringen/diensten belast met 0% of niet bij u belast";
        break;
    case ACC2::ControlTaxPurchRecRev :
    case ACC2::ControlPurchaseRevTax :
        itemNr ="2a";
        subject = "Leveringen/diensten waarbij de heffing van de omzetbelasting "
                  "naar u is verlegd";
        break;
    case ACC2::ControlSalesOutEu :
        itemNr = "3a";
        subject = "Leveringen naar landen buiten de EU (uitvoer)";
        break;
    case ACC2::ControlSalesInEu :
    case ACC2::ControlServiceInEu :
        itemNr = "3b";
        subject = "Leveringen naar/diensten in landen binnen de EU";
        break;
    case ACC2::ControlInstallInEu :
        itemNr = "3c";
        subject = "Installatie/afstands verkopen binnen de EU";
        break;
    case ACC2::ControlTaxPurchOutEu :
    case ACC2::ControlPurchOutEuLow :
    case ACC2::ControlPurchOutEuHigh :
    case ACC2::ControlPurchOutEuOther :
        itemNr = "4a";
        subject = "Leveringen/diensten uit landen buiten de EU";
        break;
    case ACC2::ControlTaxPurchInEu :
    case ACC2::ControlPurchInEuLow :
    case ACC2::ControlPurchInEuHigh :
    case ACC2::ControlPurchInEuOther :
        itemNr = "4b";
        subject = "Leveringen/diensten uit landen binnen de EU";
        break;
    case ACC2::ControlUserDefined :
        itemNr = "5a";
        subject = "Verschuldigde omzetbelasting (rubrieken 1a t/m 4b)";
        break;
    case ACC2::ControlTaxRecLow :
    case ACC2::ControlTaxRecHigh :
    case ACC2::ControlTaxRecOther :
        itemNr = "5b";
        subject = "Voorbelasting";
        break;
    case ACC2::ControlUserDefined + 1 :
        itemNr = "5c";
        subject = "Subtotaal (rubriek 5a min 5b)";
        break;
    case ACC2::ControlUserDefined + 2 :
        itemNr = "5d";
        subject = "Vermindering volgens kleineondernemersregeling";
        break;
    case ACC2::ControlUserDefined + 3 :
        itemNr = "5e";
        subject = "Schatting vorige aangifte(n)";
        break;
    case ACC2::ControlUserDefined + 4 :
        itemNr = "5f";
        subject = "Schatting deze aangifte";
        break;
    case ACC2::ControlUserDefined + 5 :
        itemNr = "5g";
        subject = "Totaal (negatief is terug te vragen)";
        break;
    default :
        itemNr = "&nbsp;";
        break;
    }

}

/**
 * Change event such as language change
 */
void ACC_SalesTaxReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
