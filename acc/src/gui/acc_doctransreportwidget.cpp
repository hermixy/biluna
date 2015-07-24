/*****************************************************************
 * $Id: acc_doctransreportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 10, 2010 9:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_doctransreportwidget.h"

#include <cmath>
#include <QPrintDialog>
#include <QPrinter>
#include <QWebFrame>
#include "acc_createdoctrans.h"
#include "acc_dialogfactory.h"
#include "acc_modelfactory.h"
#include "db_actionfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_DocTransReportWidget::ACC_DocTransReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);
//    readSettings(); not yet in RB_MdiWindow
    mDocId = "";
    mDocInfo = "";
}

/**
 * Destructor
 */
ACC_DocTransReportWidget::~ACC_DocTransReportWidget() {
    // nothing
}

/**
 * Initialize and prepare document transactions
 */
void ACC_DocTransReportWidget::init() {
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

    // Create the journal (same as some controlled GL accounts) map
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String sqlStr = "SELECT id, accountcode, accountname "
                       "FROM acc_chartmaster "
                       "WHERE acc_chartmaster.parent='"
                        + ACC_MODELFACTORY->getRootId() + "';";
    query.exec(sqlStr);

    while (query.next()) {
        mChartMasterMap[query.record().value(0).toString()]
                = query.record().value(1).toString() + " "
                  + query.record().value(2).toString();
    }

    mHtml = "<p>";
    mHtml += tr("Select from/to period above, and click refresh button ...");
    mHtml += "</p>";

    te->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool ACC_DocTransReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool ACC_DocTransReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void ACC_DocTransReportWidget::filePdf(QPrinter* printer) {
    te->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void ACC_DocTransReportWidget::filePrint(QPrinter* printer) {
    te->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void ACC_DocTransReportWidget::filePrintPreview(QPrinter* printer) {
    te->print(printer);
}

/**
 * Edit Cut.
 */
void ACC_DocTransReportWidget::editCut() {
    te->cut();
}

/**
 * Edit Copy.
 */
void ACC_DocTransReportWidget::editCopy() {
    te->copy();
}

/**
 * Edit Paste.
 */
void ACC_DocTransReportWidget::editPaste() {
    te->paste();
}

/**
 * Edit Undo.
 */
void ACC_DocTransReportWidget::editUndo() {
    te->undo();
}

/**
 * Edit Redo.
 */
void ACC_DocTransReportWidget::editRedo() {
    te->redo();
}

/**
 * Edit Select All
 */
void ACC_DocTransReportWidget::editSelectAll() {
    te->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void ACC_DocTransReportWidget::editInsertImage(const QString& fn) {
    te->insertImage(fn);
}

/**
 * @brief ACC_DocTransReportWidget::editZoomIn
 */
void ACC_DocTransReportWidget::editZoomIn(double /*factor*/) {
   te->fontZoomIn(1);
}

/**
 * @brief ACC_DocTransReportWidget::editZoomOut
 */
void ACC_DocTransReportWidget::editZoomOut(double /*factor*/) {
    te->fontZoomOut(1);
}

/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool ACC_DocTransReportWidget::saveFile(const QString &fn) {
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
 * Initialize and create document transaction report
 */
void ACC_DocTransReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ACC_CreateDocTrans* oper = new ACC_CreateDocTrans();
    RB_ObjectContainer* docTransactionList = new RB_ObjectContainer("", NULL, "ACC_GlTransactionList");
    mStartDate = dtFrom->date();
    mEndDate = dtTo->date();
    oper->setPeriod(mStartDate, mEndDate);
    oper->execute(docTransactionList);
    createTable(docTransactionList);

    delete docTransactionList;
    delete oper;

    te->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Create document transaction table in webview, including header and footer
 */
void ACC_DocTransReportWidget::createTable(RB_ObjectContainer* docTransactionList) {
    RB_String coyName = ACC_MODELFACTORY->getRoot()->getValue("coyname").toString();
    mSumDebit = 0.0;
    mSumCredit = 0.0;
    mDocId = "";
    QDateTime dt = QDateTime::currentDateTime();

    mHtml = "<table cellpadding=0 cellspacing=2 width='100%' style=\"font-size:8pt;font-family:'Times New Roman', Times, serif;\">"
    "<thead>"
    "<tr><th colspan=5 align='center' style='font-size:11pt;'>" + tr("Document Transaction Report") + " - " + coyName + "</th></tr>"
    "<tr><td colspan=5 align='center'>" + tr("Period: ")
            + mStartDate.toString(Qt::ISODate) + " - " + mEndDate.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=5 align='center' style='font-size:8pt;'>" + tr("Created: ") + dt.toString(Qt::ISODate) + "</td></tr>"
    "<tr><td colspan=5 style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage() + "</td></tr>"
    "<tr><th>" + tr("Document/Account") + "</th><th>" + tr("Description") + "</th>"
    "<th>" + tr("Reference Nr.") + "</th><th>" + tr("Debit") + "</th>"
    "<th>" + tr("Credit") + "</th></tr>"
    "<tr><td colspan=5 style='background-color:" + mTextColor.name() + ";'>" + te->getOnePixelTransparentImage() + "</td></tr>"
    "</thead>"
    "<tbody>";
    RB_ObjectIterator* iter = docTransactionList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        if (mDocId != obj->getValue("transdocid").toString()) {
            if (std::fabs(mSumDebit - mSumCredit) >= 0.01) {
                addErrorRow();
            }
            mDocId = obj->getValue("transdocid").toString();
            mDocInfo = obj->getValue("transdate").toDate().toString(Qt::ISODate)
                       + " " + obj->getValue("transdocname").toString()
                       + " " + tr("Doc.No.") + " " + obj->getValue("transno").toString();
            mSumDebit = 0.0;
            mSumCredit = 0.0;

            addHeaderRow();
        }
        addDataRow(obj);

        mSumDebit += obj->getValue("debit").toDouble();
        mSumCredit += obj->getValue("credit").toDouble();
    }
    delete iter;

//    addFooterRow();
    mHtml += "</tbody></table>";
    mHtml += "<p align=\"center\" style=\"font-size:8pt;"
            "font-family:'Times New Roman', Times, serif;\">--- END OF REPORT ---</p>";
}

/**
 * Add an account group header row
 */
void ACC_DocTransReportWidget::addHeaderRow() {
    mHtml += "<tr><td colspan=5><strong>";
    mHtml += mDocInfo;
    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void ACC_DocTransReportWidget::addDataRow(RB_ObjectBase* obj) {
    mHtml += "<tr><td>";

    std::map<RB_String, RB_String>::iterator iter;
    iter = mChartMasterMap.find(obj->getValue("chartmasterid").toString());
    if (iter != mChartMasterMap.end()) {
        mHtml += iter->second;
    } else {
        mHtml += obj->getValue("chartmasterid").toString();
    }

    mHtml += "</td><td>";
    RB_String betweenCellsLeft = "</td><td>";
    RB_String betweenCellsRight = "</td><td align='right'>";
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
void ACC_DocTransReportWidget::addErrorRow() {
    mHtml += "<tr><td colspan=3 align='right'><i>";

    RB_String betweenCellsRight = "</i></td><td align='right' style='border-top: solid 1px " + mTextColor.name() + ";'><i>";

    mHtml += /* mDocInfo + " " + */ tr("Document Total ERROR") + betweenCellsRight;
    mHtml += RB_String::number(mSumDebit,'f', 2) + betweenCellsRight;
    mHtml += RB_String::number(mSumCredit,'f', 2);

    mHtml += "</i></td></tr>"; // <tr><td colspan=7>&nbsp;</td></tr> empty line
}

/**
 * Change event such as language change
 */
void ACC_DocTransReportWidget::changeEvent(QEvent *e) {
    RB_Widget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
