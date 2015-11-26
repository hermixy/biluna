/*****************************************************************
 * $Id: acc_orderreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Feb 22, 2012 17:00:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_orderreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc_createorderreport.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_OrderReportWidget::ACC_OrderReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);

    mOrderNumber = "";
    mOrderTotal = 0.0;
    mOrderGrandTotal = 0.0;
    mInvoicedTotal = 0.0;
    mInvoicedGrandTotal = 0.0;

    mIsPurchaseOrder = false;
}

/**
 * Destructor
 */
ACC_OrderReportWidget::~ACC_OrderReportWidget() {
    // nothing
}

RB_String ACC_OrderReportWidget::getName() const {
    if (mIsPurchaseOrder) {
        return tr("Purchase Order Report");
    }
    return tr("Sales Order Report");
}

/**
 * Initialize and create trial balance
 */
void ACC_OrderReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    QDateTime dt;
    int year = dt.currentDateTime().date().year();
    sbFromYear->setValue(year);

    cbFromMonth->addItem(tr("January"), "-01");
    cbFromMonth->addItem(tr("February"), "-02");
    cbFromMonth->addItem(tr("March"), "-03");
    cbFromMonth->addItem(tr("April"), "-04");
    cbFromMonth->addItem(tr("May"), "-05");
    cbFromMonth->addItem(tr("June"), "-06");
    cbFromMonth->addItem(tr("July"), "-07");
    cbFromMonth->addItem(tr("August"), "-08");
    cbFromMonth->addItem(tr("September"), "-09");
    cbFromMonth->addItem(tr("October"), "-10");
    cbFromMonth->addItem(tr("November"), "-11");
    cbFromMonth->addItem(tr("December"), "-12");
    cbFromMonth->setCurrentIndex(0);

    cbToMonth->addItem(tr("January"), "-01");
    cbToMonth->addItem(tr("February"), "-02");
    cbToMonth->addItem(tr("March"), "-03");
    cbToMonth->addItem(tr("April"), "-04");
    cbToMonth->addItem(tr("May"), "-05");
    cbToMonth->addItem(tr("June"), "-06");
    cbToMonth->addItem(tr("July"), "-07");
    cbToMonth->addItem(tr("August"), "-08");
    cbToMonth->addItem(tr("September"), "-09");
    cbToMonth->addItem(tr("October"), "-10");
    cbToMonth->addItem(tr("November"), "-11");
    cbToMonth->addItem(tr("December"), "-12");
    cbToMonth->setCurrentIndex(11);

    mHtml = "<p>";
    mHtml += tr("Select from and to period above and click refresh button ...");
    mHtml += "</p>";

    teOrder->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_OrderReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_OrderReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_OrderReportWidget::filePdf(QPrinter* printer) {
    teOrder->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_OrderReportWidget::filePrint(QPrinter* printer) {
    teOrder->print(printer);

    // Hack:
//    QWebFrame* wf = wvOrder->page()->mainFrame();
//    QTextDocument doc;
//    doc.setHtml(wf->toHtml());
//    doc.print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_OrderReportWidget::filePrintPreview(QPrinter* printer) {
    teOrder->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_OrderReportWidget::editCut() {
    teOrder->cut();
}

/**
 * Edit Copy.
 */
void ACC_OrderReportWidget::editCopy() {
    teOrder->copy();
}

/**
 * Edit Paste.
 */
void ACC_OrderReportWidget::editPaste() {
    teOrder->paste();
}

/**
 * Edit Undo.
 */
void ACC_OrderReportWidget::editUndo() {
    teOrder->undo();
}

/**
 * Edit Redo.
 */
void ACC_OrderReportWidget::editRedo() {
    teOrder->redo();
}

/**
 * Edit Select All
 */
void ACC_OrderReportWidget::editSelectAll() {
    teOrder->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_OrderReportWidget::editInsertImage(const QString& fn) {
    teOrder->insertImage(fn);
}

/**
 * @brief ACC_OrderReportWidget::editZoomIn
 */
void ACC_OrderReportWidget::editZoomIn(double /*factor*/) {
   teOrder->fontZoomIn(1);
}

/**
 * @brief ACC_OrderReportWidget::editZoomOut
 */
void ACC_OrderReportWidget::editZoomOut(double /*factor*/) {
    teOrder->fontZoomOut(1);
}

/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_OrderReportWidget::saveFile(const QString &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ACC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        ACC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teOrder->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    ACC_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Initialize and create trial balance
 */
void ACC_OrderReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    ACC_CreateOrderReport* oper = new ACC_CreateOrderReport();
    RB_ObjectContainer* orderList = new RB_ObjectContainer("", NULL, "ACC_OrderList");

    mFromPrd = RB_String::number(sbFromYear->value())
            + cbFromMonth->itemData(cbFromMonth->currentIndex(), Qt::UserRole).toString();
    if (cbFromMonth->currentIndex() <= cbToMonth->currentIndex()) {
        // same year
        mToPrd = RB_String::number(sbFromYear->value())
                + cbToMonth->itemData(cbToMonth->currentIndex(), Qt::UserRole).toString();
    } else {
        // next year
        mToPrd = RB_String::number(sbFromYear->value() + 1)
                + cbToMonth->itemData(cbToMonth->currentIndex(), Qt::UserRole).toString();
    }

    oper->setPeriod(mFromPrd, mToPrd);
    if (mIsPurchaseOrder) {
        oper->setPurchaseOrder();
    }
    if (!chbOnlyOpenOrders->isChecked()) {
        oper->setShowAllOrders();
    }
    oper->execute(orderList);
    createTable(orderList);

    delete orderList;
    delete oper;

    teOrder->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Create trial balance table in webview, including header and footer
 */
void ACC_OrderReportWidget::createTable(RB_ObjectContainer* orderList) {
    /*
    orderno
    deliverto
    custsuppref
    orddate
    deliverydate
    stk_idx
    stkdescription
    stkuom
    unitprice
    discountpercent
    quantity
    qtyinvoiced
    qtydispatched
    itemdue
    */

    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("company").toString();
    QDateTime dt = QDateTime::currentDateTime();
    mOrderNumber = "";
    mOrderTotal = 0.0;
    mOrderGrandTotal = 0.0;
    mInvoicedTotal = 0.0;
    mInvoicedGrandTotal = 0.0;

    RB_String title;
    if (!mIsPurchaseOrder) {
        title = tr("Sales Order Report");
    } else {
        title = tr("Purchase Order Report");
    }

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=9 align='center' style='font-size:11pt;'>" + title + " - " + coyName + "</th></tr>"
    "<tr><td colspan=9 align='center'>" + tr("Period: ")
            + mFromPrd + " - " + mToPrd + "</td></tr>"
    "<tr><td colspan=9 align='center'>" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=9 style='background-color:" + mTextColor.name() + ";'>" + teOrder->getOnePixelTransparentImage() + "</td></tr>"
    "<tr><th colspan=9 align='left'>" + tr("Order no.") + " - " + tr("Name") + " - "
            + tr("Reference") + " - " + tr("Order Date") + " - " + tr("Disp.Date") + "</th></tr>"
    "<tr><td>Item</td><td>Description</td>"
    "<td align='center'>" + tr("Unit") + "</td><td align='center'>" + tr("Unit Price") + "</td>"
    "<td align='center'>" + tr("Qty Order") + "</td><td align='center'>" + tr("Qty Inv.") + "</td>"
    "<td align='center'>" + tr("Qty Disp.") + "</td><td align='center'>" + tr("Item Due") + "</td>"
    "<td align='center'>" + tr("Difference") + "</td></tr>"
    "<tr><td colspan=9 style='background-color:" + mTextColor.name() + ";'>" + teOrder->getOnePixelTransparentImage() + "</td></tr>"
    "</thead>"
    "<tbody>";

    RB_ObjectIterator* iter = orderList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (mOrderNumber != obj->getValue("orderno").toString()) {
            if(!mOrderNumber.isEmpty()) {
                addFooterRow();
            }

            // add subtotal to grandtotal
            mOrderGrandTotal += mOrderTotal;
            mInvoicedGrandTotal += mInvoicedTotal;

            // set current order number
            mOrderNumber = obj->getValue("orderno").toString();

            // reset subtotal
            mOrderTotal = 0.0;
            mInvoicedTotal = 0.0;

            addHeaderRow(obj);
        }

        addDataRow(obj);

        mOrderTotal += obj->getValue("unitprice").toDouble()
                * (100.0 - obj->getValue("discountpercent").toDouble()) / 100.0
                * obj->getValue("quantity").toDouble();
        mInvoicedTotal += obj->getValue("unitprice").toDouble()
                * (100.0 - obj->getValue("discountpercent").toDouble()) / 100.0
                * obj->getValue("qtyinvoiced").toDouble();
    }
    delete iter;

    // Last time add subtotal to grandtotal
    mOrderGrandTotal += mOrderTotal;
    mInvoicedGrandTotal += mInvoicedTotal;

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
void ACC_OrderReportWidget::addHeaderRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td colspan=9><strong>";

    mHtml += mOrderNumber + " - "; // same as obj->getValue("orderno").toString();
    mHtml += obj->getValue("delivertofrom").toString() + " - ";
    mHtml += obj->getValue("custsuppref").toString() + " - ";
    mHtml += obj->getValue("orddate").toDate().toString("yyyy-MM-dd") + " - ";
    mHtml += obj->getValue("deliverydate").toDate().toString("yyyy-MM-dd");

    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_OrderReportWidget::addDataRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td>";
    mHtml += obj->getDValue("stk_idx").toString();
    mHtml += "</td><td>";
    RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";

    mHtml += obj->getValue("stkdescription").toString()
            + "</td><td align='center'>";
    mHtml += obj->getValue("stkuom").toString() + betweenCellsWhite;

    double amt = obj->getValue("unitprice").toDouble()
            * (100.0 - obj->getValue("discountpercent").toDouble()) / 100.0;
    mHtml += RB_String::number(amt,'f', 2) + betweenCellsGray;

    amt = obj->getValue("quantity").toDouble();
    mHtml += RB_String::number(amt,'f', 2) + betweenCellsGray;

    amt = obj->getValue("qtyinvoiced").toDouble();
    mHtml += RB_String::number(amt,'f', 2) + betweenCellsWhite;

    amt = obj->getValue("qtydispatched").toDouble();
    mHtml += RB_String::number(amt,'f', 2) + betweenCellsWhite;

    mHtml += obj->getValue("itemdue").toDate().toString("yyyy-MM-dd");
    mHtml += betweenCellsWhite;
    mHtml += " &nbsp;";
    mHtml += "</td></tr>";
}

/**
 * Add an account group footer row
 */
void ACC_OrderReportWidget::addFooterRow() {
    mHtml += "<tr><td colspan=2>" + teOrder->getOnePixelTransparentImage() + "</td>"
            "<td colspan=7 style='background-color:" + mTextColor.name() + ";'>" + teOrder->getOnePixelTransparentImage() + "</td></tr>";
    mHtml += "<tr><td colspan=2 align='right'><i>";

    RB_String betweenCellsGray = "</i></td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";"
                                 "border-top: solid 1px " + mTextColor.name() + ";'><i>";
    RB_String betweenCellsWhite = "</i></td><td align='right' style='border-top: solid 1px " + mTextColor.name() + ";'><i>";

    mHtml += mOrderNumber + " " + tr("Totals") + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsGray;
    mHtml += RB_String::number(mOrderTotal,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mInvoicedTotal,'f', 2) + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsWhite;
    mHtml += RB_String::number(mOrderTotal - mInvoicedTotal,'f', 2);

    mHtml += "</i></td></tr>";
}

/**
 * Add footer with totals
 */
void ACC_OrderReportWidget::addFooter() {
    mHtml += "<tfoot style='font-size:8pt;'>";
    mHtml += "<tr><td colspan=9 style='background-color:" + mTextColor.name() + ";'>"
            + teOrder->getOnePixelTransparentImage() + "</td></tr>";
    mHtml += "<tr><td colspan=2>";

    RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";

    mHtml += tr("GRAND TOTALS") + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsGray;
    mHtml += RB_String::number(mOrderGrandTotal,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mInvoicedGrandTotal,'f', 2) + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsWhite;
    mHtml += "&nbsp;" + betweenCellsWhite;
    mHtml += RB_String::number(mOrderGrandTotal - mInvoicedGrandTotal,'f', 2);
    mHtml += "</td></tr></tfoot>";
}

void ACC_OrderReportWidget::setPurchaseOrder() {
    mIsPurchaseOrder = true;
}

/**
 * Change event such as language change
 */
void ACC_OrderReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}

