/*****************************************************************
 * $Id: acc_gltransreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 9, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_gltransreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "acc_creategltransactions.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_GlTransReportWidget::ACC_GlTransReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);

    mAccountCode = "";
    mAccountName = "";
}

/**
 * Destructor
 */
ACC_GlTransReportWidget::~ACC_GlTransReportWidget() {
    // nothing
}

/**
 * Initialize and create GL transaction widget
 */
void ACC_GlTransReportWidget::init() {
    DB_ACTIONFACTORY->enableEditMenu(getWidgetType());
    // DB_ACTIONFACTORY->enableFormatMenu(getWidgetType());

    QDateTime dt;
    int year = dt.currentDateTime().date().year();
    int startMonth = dt.currentDateTime().date().month();
    int endMonth = startMonth;
    int day = dt.currentDateTime().date().day();
    if (startMonth > 1) {
        --startMonth; // one full month before
    }
    QDate dateStart(year, startMonth, 1);
    dtFrom->setDate(dateStart);
    QDate dateEnd(year, endMonth, day);
    dtTo->setDate(dateEnd);

    // Set the available account codes
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String sqlStr = "SELECT accountcode FROM acc_chartmaster "
                        "WHERE acc_chartmaster.parent='"
                        + ACC_MODELFACTORY->getRootId() + "' "
                        "ORDER BY acc_chartmaster.accountcode;";

    query.exec(sqlStr);

    while (query.next()) {
        cbFromAccount->addItem(query.record().value(0).toString());
        cbToAccount->addItem(query.record().value(0).toString());
    }

    cbFromAccount->setCurrentIndex(0);
    int count = cbToAccount->count();
    cbToAccount->setCurrentIndex(count - 1);

    // Create the journal (same as some controlled GL accounts) map
    sqlStr = "SELECT id, accountname FROM acc_chartmaster "
                        "WHERE acc_chartmaster.parent='"
                        + ACC_MODELFACTORY->getRootId() + "' "
                        "AND acc_chartmaster.accountcontrol>0;";
    query.exec(sqlStr);

    while (query.next()) {
        mChartMasterMap[query.record().value(0).toString()]
                = query.record().value(1).toString();
    }

    mHtml = "<p>";
    mHtml += tr("Select from/to period and account above, and click refresh button ...");
    mHtml += "</p>";

    teGlTrans->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_GlTransReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_GlTransReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_GlTransReportWidget::filePdf(QPrinter* printer) {
    teGlTrans->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_GlTransReportWidget::filePrint(QPrinter* printer) {
    teGlTrans->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_GlTransReportWidget::filePrintPreview(QPrinter* printer) {
    teGlTrans->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_GlTransReportWidget::editCut() {
    teGlTrans->cut();
}

/**
 * Edit Copy.
 */
void ACC_GlTransReportWidget::editCopy() {
    teGlTrans->copy();
}

/**
 * Edit Paste.
 */
void ACC_GlTransReportWidget::editPaste() {
    teGlTrans->paste();
}

/**
 * Edit Undo.
 */
void ACC_GlTransReportWidget::editUndo() {
    teGlTrans->undo();
}

/**
 * Edit Redo.
 */
void ACC_GlTransReportWidget::editRedo() {
    teGlTrans->redo();
}

/**
 * Edit Select All
 */
void ACC_GlTransReportWidget::editSelectAll() {
    teGlTrans->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_GlTransReportWidget::editInsertImage(const QString& fn) {
    teGlTrans->insertImage(fn);
}

/**
 * @brief ACC_GlTransReportWidget::editZoomIn
 */
void ACC_GlTransReportWidget::editZoomIn(double /*factor*/) {
   teGlTrans->fontZoomIn(1);
}

/**
 * @brief ACC_GlTransReportWidget::editZoomOut
 */
void ACC_GlTransReportWidget::editZoomOut(double /*factor*/) {
    teGlTrans->fontZoomOut(1);
}


/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_GlTransReportWidget::saveFile(const QString &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ACC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        ACC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teGlTrans->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    ACC_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Initialize and create trial balance
 */
void ACC_GlTransReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ACC_CreateGlTransactions* oper = new ACC_CreateGlTransactions();
    RB_ObjectContainer* glTransactionList = new RB_ObjectContainer("", NULL, "ACC_GlTransactionList");

    oper->setAccountRange(cbFromAccount->currentText(),
                          cbToAccount->currentText());
    mStartDate = dtFrom->date();
    mEndDate = dtTo->date();
    oper->setPeriod(mStartDate, mEndDate);
    oper->execute(glTransactionList);
    createTable(glTransactionList);

    delete glTransactionList;
    delete oper;

    teGlTrans->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Create trial balance table in webview, including header and footer
 */
void ACC_GlTransReportWidget::createTable(RB_ObjectContainer* glTransactionList) {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("coyname").toString();
    mSumDebit = 0.0;
    mSumCredit = 0.0;
    mAccountCode = "";
    QDateTime dt = QDateTime::currentDateTime();

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=7 align='center' style='font-size:11pt;'>" + tr("GL Transactions Report") + " - " + coyName + "</th></tr>"
    "<tr><td colspan=7 align='center'>" + tr("Period: ")
            + mStartDate.toString(Qt::ISODate) + " - " + mEndDate.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=7 align='center'>" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=7 style='background-color:" + mTextColor.name() + ";'>" + teGlTrans->getOnePixelTransparentImage() + "</td></tr>"
    "<tr><th width=\"15%\">" + tr("Account/Journal") + "</th><th width=\"8%\">" + tr("Date") + "</th>"
    "<th width=\"8%\">" + tr("Doc. Nr.") + "</th><th>" + tr("Description") + "</th>"
    "<th width=\"10%\">" + tr("Reference Nr.") + "</th><th width=\"8%\">" + tr("Debit") + "</th>"
    "<th width=\"8%\">" + tr("Credit") + "</th></tr>"
    "<tr><td colspan=7 style='background-color:" + mTextColor.name() + ";'>" + teGlTrans->getOnePixelTransparentImage() + "</td></tr>"
    "</thead>"
    "<tbody>";
    RB_ObjectIterator* iter = glTransactionList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (mAccountCode != obj->getValue("accountcode").toString()) {
            if(!mAccountCode.isEmpty()) { // not the first time
                addFooterRow();
            }

            mAccountCode = obj->getValue("accountcode").toString();
            mAccountName = obj->getValue("accountname").toString();
            mSumDebit = 0.0;
            mSumCredit = 0.0;

            addHeaderRow();
        }
        addDataRow(obj);

        mSumDebit += obj->getValue("debit").toDouble();
        mSumCredit += obj->getValue("credit").toDouble();
    }
    delete iter;

    addFooterRow();
    mHtml += "</tbody></table><p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Add an account group header row
 */
void ACC_GlTransReportWidget::addHeaderRow() {
    mHtml += "<tr><td colspan=7><strong>";
    mHtml += mAccountCode + " " + mAccountName;
    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_GlTransReportWidget::addDataRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td>";

    std::map<RB_String, RB_String>::iterator iter;
    iter = mChartMasterMap.find(obj->getValue("docparent").toString());
    if (iter != mChartMasterMap.end()) {
        mHtml += iter->second;
    } else {
        mHtml += obj->getValue("docparent").toString();
    }

    mHtml += "</td><td>";
    // RB_String betweenCellsGray = "</td><td align='right' style='background-color: " + mAlternateBaseColor.name() + ";'>";
    // RB_String betweenCellsWhite = "</td><td align='right'>";
    RB_String betweenCellsLeft = "</td><td>";
    RB_String betweenCellsRight = "</td><td align='right'>";
    mHtml += obj->getValue("transdate").toDate().toString(Qt::ISODate) + betweenCellsLeft;
    mHtml += obj->getValue("transdocno").toString() + betweenCellsLeft;
    mHtml += obj->getValue("description").toString() + betweenCellsLeft;
    mHtml += obj->getValue("refno").toString() + betweenCellsRight;
    double amt = obj->getValue("debit").toDouble();
    if (amt != 0.0) {
        mHtml += RB_String::number(amt,'f', 2) + betweenCellsRight;
    } else {
        mHtml += "&nbsp;" + betweenCellsRight;
    }
    amt = obj->getValue("credit").toDouble();
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
void ACC_GlTransReportWidget::addFooterRow() {
    mHtml += "<tr><td colspan=5>" + teGlTrans->getOnePixelTransparentImage() + "</td>"
             "<td colspan=2 style='background-color:" + mTextColor.name() + ";'>" + teGlTrans->getOnePixelTransparentImage() + "</td></tr>";
    mHtml += "<tr><td colspan=5 align='right'><i>";

    RB_String betweenCellsRight = "</i></td><td align='right'><i>";

    mHtml += mAccountCode + " " + mAccountName + " " + tr("Totals") + betweenCellsRight;
    mHtml += RB_String::number(mSumDebit,'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(mSumCredit,'f', 2);

    mHtml += "</i></td></tr>";
}

/**
 * Change event such as language change
 */
void ACC_GlTransReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
