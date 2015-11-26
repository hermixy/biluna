/*****************************************************************
 * $Id: acc_trialbalancewidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Oct 13, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_trialbalancewidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc_creategltrialbalance.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"
#include "rb_textedit.h"


/**
 * Constructor
 */
ACC_TrialBalanceWidget::ACC_TrialBalanceWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);
}

/**
 * Destructor
 */
ACC_TrialBalanceWidget::~ACC_TrialBalanceWidget() {
    // nothing
}

/**
 * Initialize and create trial balance
 */
void ACC_TrialBalanceWidget::init() {
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

    teTrialBalance->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_TrialBalanceWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_TrialBalanceWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_TrialBalanceWidget::filePdf(QPrinter* printer) {
    teTrialBalance->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_TrialBalanceWidget::filePrint(QPrinter* printer) {
    teTrialBalance->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_TrialBalanceWidget::filePrintPreview(QPrinter* printer) {
    teTrialBalance->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_TrialBalanceWidget::editCut() {
    teTrialBalance->cut();
}

/**
 * Edit Copy.
 */
void ACC_TrialBalanceWidget::editCopy() {
    teTrialBalance->copy();
}

/**
 * Edit Paste.
 */
void ACC_TrialBalanceWidget::editPaste() {
    teTrialBalance->paste();
}

/**
 * Edit Undo.
 */
void ACC_TrialBalanceWidget::editUndo() {
    teTrialBalance->undo();
}

/**
 * Edit Redo.
 */
void ACC_TrialBalanceWidget::editRedo() {
    teTrialBalance->redo();
}

/**
 * Edit Select All
 */
void ACC_TrialBalanceWidget::editSelectAll() {
    teTrialBalance->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_TrialBalanceWidget::editInsertImage(const QString& fn) {
    teTrialBalance->insertImage(fn);
}

/**
 * @brief ACC_TrialBalanceWidget::editZoomIn
 */
void ACC_TrialBalanceWidget::editZoomIn(double /*factor*/) {
   teTrialBalance->fontZoomIn(1);
}

/**
 * @brief ACC_TrialBalanceWidget::editZoomOut
 */
void ACC_TrialBalanceWidget::editZoomOut(double /*factor*/) {
    teTrialBalance->fontZoomOut(1);
}

/**
 * Save trial balance in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_TrialBalanceWidget::saveFile(const QString &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ACC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        ACC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teTrialBalance->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    ACC_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Initialize and create trial balance
 */
void ACC_TrialBalanceWidget::on_pbRefresh_clicked() {
    ACC_CreateGlTrialBalance* oper = new ACC_CreateGlTrialBalance();
    RB_ObjectContainer* trialBalList = new RB_ObjectContainer("", NULL, "ACC_TrialBalanceList");

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
    oper->execute(trialBalList);
    createTable(trialBalList);

    delete trialBalList;
    delete oper;

    teTrialBalance->setHtml(mHtml);
}

/**
 * Create trial balance table in webview, including header and footer
 */
void ACC_TrialBalanceWidget::createTable(RB_ObjectContainer* trialBalanceList) {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("company").toString();
    mGroupSumDebit = 0.0;
    mGroupSumCredit = 0.0;
    mGroupTrialDebit = 0.0;
    mGroupTrialCredit = 0.0;
    mGroupPandlDebit = 0.0;
    mGroupPandlCredit = 0.0;
    mGroupBalDebit = 0.0;
    mGroupBalCredit = 0.0;
    mTotalSumDebit = 0.0;
    mTotalSumCredit = 0.0;
    mTotalTrialDebit = 0.0;
    mTotalTrialCredit = 0.0;
    mTotalPandlDebit = 0.0;
    mTotalPandlCredit = 0.0;
    mTotalBalDebit = 0.0;
    mTotalBalCredit = 0.0;
    mGroupName = "";
    QDateTime dt = QDateTime::currentDateTime();

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=10 align='center' style='font-size:11pt;'>" + tr("Trial Balance") + " - " + coyName + "</th></tr>"
    "<tr><td colspan=10 align='center'>" + tr("Period: ")
            + RB_String::number(mFromPrd) + " - " + RB_String::number(mToPrd) + "</td></tr>"
    "<tr><td colspan=10 align='center'>" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=10 style='background-color:" + mTextColor.name() + ";'>" + teTrialBalance->getOnePixelTransparentImage() + "</td></tr>"
    "<tr><th>" + tr("Code") + "</th><th>" + tr("Account Name") + "</th>"
    "<th colspan=2>" + tr("Journal") + "</th>"
    "<th colspan=2>" + tr("Trial Balance") + "</th>"
    "<th colspan=2>" + tr("Profit/Loss") + "</th>"
    "<th colspan=2>" + tr("Balance") + "</th></tr>"
    "<tr><td>&nbsp;</td><td>&nbsp;</td>"
    "<td align='center'>" + tr("Debit") + "</td><td align='center'>" + tr("Credit") + "</td>"
    "<td align='center'>" + tr("Debit") + "</td><td align='center'>" + tr("Credit") + "</td>"
    "<td align='center'>" + tr("Debit") + "</td><td align='center'>" + tr("Credit") + "</td>"
    "<td align='center'>" + tr("Debit") + "</td><td align='center'>" + tr("Credit") + "</td></tr>"
    "<tr><td colspan=10 style='background-color:" + mTextColor.name() + ";'>" + teTrialBalance->getOnePixelTransparentImage() + "</td></tr>"
    "</thead>"
    "<tbody>";

    RB_ObjectIterator* iter = trialBalanceList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (mGroupName != obj->getValue("groupname").toString()) {
            if(!mGroupName.isEmpty()) {
                addFooterRow();
            }

            mTotalSumDebit += mGroupSumDebit;
            mTotalSumCredit += mGroupSumCredit;
            mTotalTrialDebit += mGroupTrialDebit;
            mTotalTrialCredit += mGroupTrialCredit;
            mTotalPandlDebit += mGroupPandlDebit;
            mTotalPandlCredit += mGroupPandlCredit;
            mTotalBalDebit += mGroupBalDebit;
            mTotalBalCredit += mGroupBalCredit;

            mGroupName = obj->getValue("groupname").toString();

            mGroupSumDebit = 0.0;
            mGroupSumCredit = 0.0;
            mGroupTrialDebit = 0.0;
            mGroupTrialCredit = 0.0;
            mGroupPandlDebit = 0.0;
            mGroupPandlCredit = 0.0;
            mGroupBalDebit = 0.0;
            mGroupBalCredit = 0.0;

            addHeaderRow();
        }
        addDataRow(obj);

        mGroupSumDebit += obj->getValue("sumdebit").toDouble();
        mGroupSumCredit += obj->getValue("sumcredit").toDouble();
        mGroupTrialDebit += obj->getValue("trialdebit").toDouble();
        mGroupTrialCredit += obj->getValue("trialcredit").toDouble();
        mGroupPandlDebit += obj->getValue("pandldebit").toDouble();
        mGroupPandlCredit += obj->getValue("pandlcredit").toDouble();
        mGroupBalDebit += obj->getValue("baldebit").toDouble();
        mGroupBalCredit += obj->getValue("balcredit").toDouble();

    }
    delete iter;

    addFooterRow();

    mTotalSumDebit += mGroupSumDebit;
    mTotalSumCredit += mGroupSumCredit;
    mTotalTrialDebit += mGroupTrialDebit;
    mTotalTrialCredit += mGroupTrialCredit;
    mTotalPandlDebit += mGroupPandlDebit;
    mTotalPandlCredit += mGroupPandlCredit;
    mTotalBalDebit += mGroupBalDebit;
    mTotalBalCredit += mGroupBalCredit;

    addResultRow();

    mHtml += "</tbody>";

    addFooter();

    mHtml += "</table>";
    mHtml += "<p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Add an account group header row
 */
void ACC_TrialBalanceWidget::addHeaderRow() {
    mHtml += "<tr><td colspan=10><strong>";
    mHtml += mGroupName;
    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_TrialBalanceWidget::addDataRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td>";

    mHtml += obj->getValue("accountcode").toString();
    mHtml += "</td><td>";
    RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";
    mHtml += obj->getValue("accountname").toString() + betweenCellsGray;

    double amt = obj->getValue("sumdebit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsGray;
    } else {
        mHtml += "&nbsp;" + betweenCellsGray;
    }
    amt = obj->getValue("sumcredit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsWhite;
    } else {
        mHtml += "&nbsp;" + betweenCellsWhite;
    }

    amt = obj->getValue("trialdebit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsWhite;
    } else {
        mHtml += "&nbsp;" + betweenCellsWhite;
    }
    amt = obj->getValue("trialcredit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsGray;
    } else {
        mHtml += "&nbsp;" + betweenCellsGray;
    }

    amt = obj->getValue("pandldebit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsGray;
    } else {
        mHtml += "&nbsp;" + betweenCellsGray;
    }
    amt = obj->getValue("pandlcredit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsWhite;
    } else {
        mHtml += "&nbsp;" + betweenCellsWhite;
    }

    amt = obj->getValue("baldebit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsWhite;
    } else {
        mHtml += "&nbsp;" + betweenCellsWhite;
    }
    amt = obj->getValue("balcredit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2);
    } else {
        mHtml += "&nbsp;";
    }

    mHtml += "</td></tr>";
}

/**
 * Add an account group footer row
 */
void ACC_TrialBalanceWidget::addFooterRow() {
    mHtml += "<tr><td colspan=2>" + teTrialBalance->getOnePixelTransparentImage() + "</td>";
    mHtml += "<td colspan=8 style='background-color:" + mTextColor.name() + ";'>" + teTrialBalance->getOnePixelTransparentImage() + "</td></tr>";
    mHtml += "<tr><td colspan=2 align='right'><i>";

    RB_String betweenCellsGray = "</i></td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";"
                                 "border-top: solid 1px " + mTextColor.name() + ";'><i>";
    RB_String betweenCellsWhite = "</i></td><td align='right' style='border-top: solid 1px " + mTextColor.name() + ";'><i>";

    mHtml += mGroupName + " " + tr("Totals") + betweenCellsGray;
    mHtml += RB_String::number(mGroupSumDebit,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mGroupSumCredit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupTrialDebit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupTrialCredit,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mGroupPandlDebit,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mGroupPandlCredit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupBalDebit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mGroupBalCredit,'f', 2);

    mHtml += "</i></td></tr>";
}

/**
 * Add the result (profit/loss) row
 */
void ACC_TrialBalanceWidget::addResultRow() {
    mHtml += "<tr><td colspan=2><i>";

    RB_String betweenCells = "</i></td><td align='right'><i>";

    mHtml += tr("Result") + betweenCells;
    mHtml += "&nbsp;" + betweenCells;
    mHtml += "&nbsp;" + betweenCells;
    mHtml += "&nbsp;" + betweenCells;
    mHtml += "&nbsp;" + betweenCells;
    if (mTotalPandlDebit >= mTotalPandlCredit) { // profit
        mHtml += "&nbsp;" + betweenCells;
        mHtml += RB_String::number(mTotalPandlDebit - mTotalPandlCredit,'f', 2) + betweenCells;
        // now set equal for bottom line
        mTotalPandlCredit = mTotalPandlDebit;
    } else {
        mHtml += RB_String::number(-mTotalPandlDebit + mTotalPandlCredit,'f', 2) + betweenCells;
        mHtml += "&nbsp;" + betweenCells;
        mTotalPandlDebit = mTotalPandlCredit;
    }
    if (mTotalBalDebit <= mTotalBalCredit) { // increase equity
        mHtml += RB_String::number(mTotalBalCredit - mTotalBalDebit,'f', 2) + betweenCells;
        mHtml += "&nbsp;";
        mTotalBalCredit = mTotalBalDebit;
    } else {
        mHtml += "&nbsp;" + betweenCells;
        mHtml += RB_String::number(-mTotalBalCredit + mTotalBalDebit,'f', 2);
        mTotalBalCredit = mTotalBalDebit;
    }


    mHtml += "</i></td></tr>";
}

/**
 * Add footer with totals
 */
void ACC_TrialBalanceWidget::addFooter() {
    mHtml += "<tfoot style='font-size:8pt;'>";
    mHtml += "<tr><td colspan=10 style='background-color:" + mTextColor.name() + ";'>" + teTrialBalance->getOnePixelTransparentImage() + "</td></tr>";
    mHtml += "<tr><td colspan=2><i>" + tr("Grand Totals");
    mHtml += "</i></td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    RB_String betweenCellsWhite = "</td><td align='right'>";
    mHtml += RB_String::number(mTotalSumDebit,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mTotalSumCredit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalTrialDebit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalTrialCredit,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mTotalPandlDebit,'f', 2) + betweenCellsGray;
    mHtml += RB_String::number(mTotalPandlCredit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalBalDebit,'f', 2) + betweenCellsWhite;
    mHtml += RB_String::number(mTotalBalCredit,'f', 2);
    mHtml += "</td></tr></tfoot>";
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String ACC_TrialBalanceWidget::getHelpSubject() const {
    return objectName();
}

bool ACC_TrialBalanceWidget::closeWidget() {
    return RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return true;
//    }
//    return false;
}

/**
 * Change event such as language change
 */
void ACC_TrialBalanceWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
