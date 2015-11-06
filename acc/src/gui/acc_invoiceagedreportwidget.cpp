/*****************************************************************
 * $Id: acc_invoiceagedreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Dec 8, 2010 12:29:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_invoiceagedreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc.h"
#include "acc_createinvoiceaged.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_InvoiceAgedReportWidget::ACC_InvoiceAgedReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);

    mCustSuppName = "";

    mSumAmount = 0.0;
    mSumAlloc = 0.0;
    mSumDue = 0.0;
    mSumOverDue1 = 0.0;
    mSumOverDue2 = 0.0;

    mTotalAmount = 0.0;
    mTotalAlloc = 0.0;
    mTotalDue = 0.0;
    mTotalOverDue1 = 0.0;
    mTotalOverDue2 = 0.0;

    mOverDueDays1 = 30; // TODO: from general settings
    mOverDueDays2 = 60; // TODO: from general settings

    mReportTitle = "";
}

/**
 * Destructor
 */
ACC_InvoiceAgedReportWidget::~ACC_InvoiceAgedReportWidget() {
    // nothing
}

/**
 * @returns widget name based on document setting Debtor or Creditor
 */
RB_String ACC_InvoiceAgedReportWidget::getName() const {

    if (mDocType == ACC2::TransCreditor) {
        return "ACC Purchase Invoice Aged Report";
    } else if (mDocType == ACC2::TransDebtor) {
        return "ACC Sales Invoices Aged Report";
    }

    return "Invoice Aged Report";
}

/**
 * Initialize
 */
void ACC_InvoiceAgedReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    if (mDocType == ACC2::TransCreditor) {
        // Standard is: Customer - From
        lblCustomerFrom->setText(tr("Supplier - From"));
    }

    setComboBoxes();

    cbFromCustSupp->setCurrentIndex(0);
    int count = cbToCustSupp->count();
    cbToCustSupp->setCurrentIndex(count - 1);

    mHtml = "<p>";
    mHtml += tr("Select from/to name above, and click refresh button ...");
    mHtml += "</p>";

    te->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_InvoiceAgedReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_InvoiceAgedReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_InvoiceAgedReportWidget::filePdf(QPrinter* printer) {
    te->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_InvoiceAgedReportWidget::filePrint(QPrinter* printer) {
    te->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_InvoiceAgedReportWidget::filePrintPreview(QPrinter* printer) {
    te->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_InvoiceAgedReportWidget::editCut() {
    te->cut();
}

/**
 * Edit Copy.
 */
void ACC_InvoiceAgedReportWidget::editCopy() {
    te->copy();
}

/**
 * Edit Paste.
 */
void ACC_InvoiceAgedReportWidget::editPaste() {
    te->paste();
}

/**
 * Edit Undo.
 */
void ACC_InvoiceAgedReportWidget::editUndo() {
    te->undo();
}

/**
 * Edit Redo.
 */
void ACC_InvoiceAgedReportWidget::editRedo() {
    te->redo();
}

/**
 * Edit Select All.
 */
void ACC_InvoiceAgedReportWidget::editSelectAll() {
    te->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_InvoiceAgedReportWidget::editInsertImage(const QString& fn) {
    te->insertImage(fn);
}

/**
 * @brief ACC_InvoiceAgedReportWidget::editZoomIn
 */
void ACC_InvoiceAgedReportWidget::editZoomIn(double /*factor*/) {
   te->fontZoomIn(1);
}

/**
 * @brief ACC_InvoiceAgedReportWidget::editZoomOut
 */
void ACC_InvoiceAgedReportWidget::editZoomOut(double /*factor*/) {
    te->fontZoomOut(1);
}

/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_InvoiceAgedReportWidget::saveFile(const QString &fn) {
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
    return true;
}

/**
 * Initialize and create trial balance
 */
void ACC_InvoiceAgedReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (mDocType == ACC2::TransCreditor) {
        mReportTitle = tr("Purchase Invoices Overdue Report");
    } else if (mDocType == ACC2::TransDebtor) {
        mReportTitle = tr("Sales Invoices Overdue Report");
    }

    ACC_CreateInvoiceAged* oper = new ACC_CreateInvoiceAged();
    RB_ObjectContainer* invoiceList = new RB_ObjectContainer("", NULL, "ACC_InvoiceList");

    oper->setDocType(mDocType);
    oper->setCustSuppRange(cbFromCustSupp->currentText(),
                           cbToCustSupp->currentText());
    oper->setOverDue(mOverDueDays1, mOverDueDays2);
    oper->execute(invoiceList);
    createTable(invoiceList);

    delete invoiceList;
    delete oper;

    te->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Create overdue report table in webview, including header and footer
 */
void ACC_InvoiceAgedReportWidget::createTable(RB_ObjectContainer* invoiceList) {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("coyname").toString();
    mCustSuppName = "";

    mSumAmount = 0.0;
    mSumAlloc = 0.0;
    mSumDue = 0.0;
    mSumOverDue1 = 0.0;
    mSumOverDue2 = 0.0;

    mTotalAmount = 0.0;
    mTotalAlloc = 0.0;
    mTotalDue = 0.0;
    mTotalOverDue1 = 0.0;
    mTotalOverDue2 = 0.0;

    QDateTime dt = QDateTime::currentDateTime();

/*    mHtml = "<!DOCTYPE HTML><html><head>"
    "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>"
    "<title>Biluna</title><style type='text/css'>"
    " @page { table { page-break-inside:auto }"
    " tr { page-break-inside:avoid; page-break-after:auto }"
    " thead { display:table-header-group }"
    " tfoot { display:table-footer-group } }"
    "</style></head><body>" does not work in QWebEngineView (webkit)*/

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=10 align='center' style='font-size:11pt;'>" + mReportTitle + " - " + coyName + "</th></tr>"
    "<tr><td colspan=10 align='center'>" + tr("Created: ")
            + dt.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=10 style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage() + "</td></tr>"
    "<tr><th>" + tr("Doc. Nr.") + "</th><th>" + tr("Description") + "</th>"
    "<th>" + tr("Date") + "</th><th>" + tr("Due Date") + "</th>"
    "<th>" + tr("Currency") + "</th><th>" + tr("Total") + "</th>"
    "<th>" + tr("Cleared") + "</th><th>" + tr("Due") + "</th>"
    "<th>&gt; " + RB_String::number(mOverDueDays1) + " days</th>"
    "<th>&gt; " + RB_String::number(mOverDueDays2) + " days</th></tr>"
    "<tr><td colspan=10 style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage() + "</td></tr>"
    "</thead>"
    "<tbody>";
    RB_ObjectIterator* iter = invoiceList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (mCustSuppName != obj->getValue("suppname").toString()) {
            if(!mCustSuppName.isEmpty()) { // not the first time
                addFooterRow();
            }

            mCustSuppName = obj->getValue("suppname").toString();
            mSumAmount = 0.0;
            mSumAlloc = 0.0;
            mSumDue = 0.0;
            mSumOverDue1 = 0.0;
            mSumOverDue2 = 0.0;

            addHeaderRow();
        }
        addDataRow(obj);

        mSumAmount += obj->getValue("amount").toDouble();
        mSumAlloc += obj->getValue("alloc").toDouble();
        mSumDue += obj->getValue("due").toDouble();
        mSumOverDue1 += obj->getValue("overdue1").toDouble();
        mSumOverDue2 += obj->getValue("overdue2").toDouble();
        mTotalAmount += obj->getValue("amount").toDouble();
        mTotalAlloc += obj->getValue("alloc").toDouble();
        mTotalDue += obj->getValue("due").toDouble();
        mTotalOverDue1 += obj->getValue("overdue1").toDouble();
        mTotalOverDue2 += obj->getValue("overdue2").toDouble();
    }
    delete iter;

    addFooterRow();
    mHtml += "</tbody>";
    addFooter();
    mHtml += "</table>";
    mHtml += "<p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Add an account group header row
 */
void ACC_InvoiceAgedReportWidget::addHeaderRow() {
    mHtml += "<tr><td colspan=10><strong>";
    mHtml += mCustSuppName;
    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_InvoiceAgedReportWidget::addDataRow(RB_ObjectBase* obj) {
    /*
    0 suppname
    1 transno
    2 description
    3 transdate
    4 duedate
    5 currabrev
    6 totalamountpay
    7 alloc
    8 due
    9 overdue1
    10 overdue2
    */

    // RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    // RB_String betweenCellsWhite = "</td><td align='right'>";
    RB_String betweenCellsLeft = "</td><td>";
    RB_String betweenCellsRight = "</td><td align='right'>";
    mHtml += "<tr><td>";
    mHtml += obj->getValue("transno").toString() + betweenCellsLeft;
    mHtml += obj->getValue("description").toString() + betweenCellsLeft;
    mHtml += obj->getValue("transdate").toDate().toString(Qt::ISODate) + betweenCellsLeft;
    mHtml += obj->getValue("duedate").toDate().toString(Qt::ISODate) + betweenCellsRight;
    mHtml += obj->getValue("currency_id").toString() + betweenCellsRight;
    mHtml += RB_String::number(obj->getValue("amount").toDouble(),'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(obj->getValue("alloc").toDouble(),'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(obj->getValue("due").toDouble(),'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(obj->getValue("overdue1").toDouble(),'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(obj->getValue("overdue2").toDouble(),'f', 2);
    mHtml += "</td></tr>";
}

/**
 * Add an account group footer row
 */
void ACC_InvoiceAgedReportWidget::addFooterRow() {
    RB_String betweenCellsRight = "</i></td><td align='right'><i>";

    mHtml += "<tr><td colspan=5 " + te->getOnePixelTransparentImage() + "</td>"
            "<td colspan=5 style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage() + "</td></tr>";
    mHtml += "<tr><td colspan=5 align='right'><i>";
    mHtml += mCustSuppName + " " + tr("Totals") + betweenCellsRight;
    mHtml += RB_String::number(mSumAmount,'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(mSumAlloc,'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(mSumDue,'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(mSumOverDue1,'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(mSumOverDue2,'f', 2);
    mHtml += "</i></td></tr>"; // <tr><td colspan=10>&nbsp;</td></tr> empty line
}

void ACC_InvoiceAgedReportWidget::setComboBoxes() {
    RB_StringList strL;
    strL << "0" << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I"
            << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R"
            << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Zzzz";
    cbFromCustSupp->addItems(strL);
    cbToCustSupp->addItems(strL);
}

/**
 * Add footer with totals
 */
void ACC_InvoiceAgedReportWidget::addFooter() {
    RB_String betweenCellsWhite = "</td><td align='right'>";

    mHtml += "<tfoot style='font-size:8pt;'>";
    mHtml += "<tr><td colspan=10 style='background-color:" + mTextColor.name() + ";'>"
            + te->getOnePixelTransparentImage() + "</td></tr>";
    mHtml += "<tr><td colspan=5 align='right'>";
    mHtml += tr("Grand Totals");
    mHtml += "</td><td align='right'>";
    mHtml += RB_String::number(mTotalAmount,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalAlloc,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalDue,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalOverDue1,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalOverDue2,'f', 2);
    mHtml += "</td></tr></tfoot>";
}

/**
 * Change event such as language change
 */
void ACC_InvoiceAgedReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
