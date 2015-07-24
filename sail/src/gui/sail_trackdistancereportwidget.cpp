/*****************************************************************
 * $Id: sail_trackdistancereportwidget.cpp 2200 2014-12-15 10:44:04Z rutger $
 * Created: Aug 11, 2012 8:58:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_trackdistancereportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QWebFrame>
#include "sail_dialogfactory.h"
#include "sail_modelfactory.h"
#include "sail_trackoperation.h"
#include "db_actionfilesaveas.h"

/**
 * Constructor
 */
SAIL_TrackDistanceReportWidget::SAIL_TrackDistanceReportWidget(QWidget *parent)
                        : RB_Widget(parent) {
    setupUi(this);
}

/**
 * Destructor
 */
SAIL_TrackDistanceReportWidget::~SAIL_TrackDistanceReportWidget() {
    // nothing
}

/**
 * @returns widget name based on document setting Debtor or Creditor
 */
RB_String SAIL_TrackDistanceReportWidget::getName() const {
    return "SAIL Track Distance Report";
}

/**
 * Initialize
 */
void SAIL_TrackDistanceReportWidget::init() {
    mHtml = "<p>";
    mHtml += tr("Set wind direction above, and click refresh button ...");
    mHtml += "</p>";

    wvReport->setHtml(mHtml);
    readSettings();
}

/**
 * File Save, can be called by closeWidget()
 */
bool SAIL_TrackDistanceReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool SAIL_TrackDistanceReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void SAIL_TrackDistanceReportWidget::filePdf(QPrinter* printer) {
    wvReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void SAIL_TrackDistanceReportWidget::filePrint(QPrinter* printer) {
    wvReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void SAIL_TrackDistanceReportWidget::filePrintPreview(QPrinter* printer) {
    wvReport->print(printer);
}

/**
 * Edit Cut.
 */
void SAIL_TrackDistanceReportWidget::editCut() {
    wvReport->pageAction(QWebPage::Cut)->trigger();
}

/**
 * Edit Copy.
 */
void SAIL_TrackDistanceReportWidget::editCopy() {
    wvReport->pageAction(QWebPage::Copy)->trigger();
}

/**
 * Edit Paste.
 */
void SAIL_TrackDistanceReportWidget::editPaste() {
    wvReport->triggerPageAction(QWebPage::Paste);
}

/**
 * Edit Undo.
 */
void SAIL_TrackDistanceReportWidget::editUndo() {
    wvReport->triggerPageAction(QWebPage::Undo);
}

/**
 * Edit Redo.
 */
void SAIL_TrackDistanceReportWidget::editRedo() {
    wvReport->triggerPageAction(QWebPage::Redo);
}

/**
 * Edit Select All.
 */
void SAIL_TrackDistanceReportWidget::editSelectAll() {
    wvReport->triggerPageAction(QWebPage::SelectAll);
}

/**
 * Save report in HTML to file
 * @param fn fileName
 * @return true on success
 */
bool SAIL_TrackDistanceReportWidget::saveFile(const QString &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        SAIL_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        SAIL_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << wvReport->page()->mainFrame()->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    SAIL_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    return true;
}

/**
 * Initialize and create trial balance
 */
void SAIL_TrackDistanceReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    RB_ObjectContainer* trackList = new RB_ObjectContainer("", NULL, "SAIL_TrackDistanceList");
    /* Wind direction
    double windDirection = leWindFrom->text().toDouble();
    double windFromDirection = 0.0;

    if (0 <= windDirection && windDirection <= 180.0) {
        windFromDirection = windDirection + 180.0;
    } else if (180.0 < windDirection && windDirection <= 360.0) {
        windFromDirection = windDirection - 180.0;
    } else {
        mHtml = "<p>" + tr("Wind direction out of range") + "</p>";
        wvReport->setHtml(mHtml);
        QApplication::restoreOverrideCursor();
        return;
    }
    */

    // Wind from
    double windFromDirection = leWindFrom->text().toDouble();
    if (windFromDirection < 0 || 360.0 < windFromDirection) {
        mHtml = "<p>" + tr("Wind direction out of range") + "</p>";
        wvReport->setHtml(mHtml);
        QApplication::restoreOverrideCursor();
        return;
    }

    SAIL_TrackOperation oper;
    oper.setWindFromDirection(windFromDirection);
    oper.execute(trackList);
    createTable(trackList);

    delete trackList;

    /* test only
    double lat1 = 35; // 52.7666667; // radians EL-A IJsselmeer
    double lon1 = 45; // 5.4863833;
    double lat2 = 35; // 52.7066167; // radians EZ-C IJsselmeer
    double lon2 = 135; // 5.4326167;

    double bearing  = mCalc.bearingStart(lat1, lon1, lat2, lon2);
    mHtml = "<p>Bearing start = " + RB_String::number(bearing) + "</p>";
    bearing  = mCalc.bearingEnd(lat1, lon1, lat2, lon2);
    mHtml += "<p>Bearing end = " + RB_String::number(bearing) + "</p>";
    */

    wvReport->setHtml(mHtml);
    QApplication::restoreOverrideCursor();
}

/**
 * Create overdue report table in webview, including header and footer
 */
void SAIL_TrackDistanceReportWidget::createTable(RB_ObjectContainer* trackList) {
    RB_ObjectBase* root = SAIL_MODELFACTORY->getRoot();
    RB_String reportName = root->getValue("code").toString();

    QDateTime dt = QDateTime::currentDateTime();

    mHtml = "<table cellpadding=3 rules=groups frame=box width='100%' style='font-size:11pt;'>"
    "<thead>"
    "<tr><th colspan=10 align='center'>SAIL Track Distance Report - " + reportName + "</th></tr>"
    "<tr><td colspan=10 align='center' style='font-size:8pt;'>" + tr("Wind from [degree]: ")
            + leWindFrom->text() + "</td></tr>"
    "<tr><td colspan=10 align='center' style='font-size:8pt;'>" + tr("Created: ")
            + dt.toString(Qt::ISODate) + "</td></tr>"
    "<tr><th>" + tr("Track") + "</th><th>" + tr("From") + "</th>"
    "<th>" + tr("To") + "</th><th>" + tr("Weight") + "</th>"
    "<th>" + tr("Distance") + "</th><th>" + tr("Speed") + "</th>"
    "<th>" + tr("Efficiency") + "</th><th>" + tr("Bearing Start") + "</th>"
    "<th>" + tr("Bearing End") + "</th><th>" + tr("Comment") + "</th></tr>"
    "</thead>"
    "<tbody style='font-size:8pt;'>";


    RB_ObjectIterator* iter = trackList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        addDataRow(obj);

    }
    delete iter;



    /*
    RB_MmProxy* model = SAIL_MODELFACTORY->getModel(SAIL_ModelFactory::ModelTrack, false);
    model->setRoot(SAIL_MODELFACTORY->getRootId());
    model->select();

    RB_String str;
    double value;
    int rowCount = model->rowCount();

    for (int i = 0; i < rowCount; ++i) {
        mHtml += "<tr>";
        mHtml += "<td>" + model->data(
                    model->index(i, model->fieldIndex("mname"))).toString() + "</td>";
        mHtml += "<td>" + model->data(
                    model->index(i, model->fieldIndex("from_idx"))).toString() + "</td>";
        mHtml += "<td>" + model->data(
                    model->index(i, model->fieldIndex("to_idx"))).toString() + "</td>";
        mHtml += "<td colspan=\"6\">" + model->data(
                    model->index(i, model->fieldIndex("weightfactor"))).toString() + "</td>";
        mHtml += "<td>" + model->data(
                    model->index(i, model->fieldIndex("comment"))).toString() + "</td>";
        mHtml += "</tr>";
    }

    delete model;
    */

    mHtml += "</tbody>";
    mHtml += "</table>";
    mHtml += "</body></html>";
}

/**
 * Add an account group header row
 */
void SAIL_TrackDistanceReportWidget::addHeaderRow() {
//    mHtml += "<tr><td colspan=10><strong>";
//    mHtml += mCustSuppName;
//    mHtml += "</strong></td></tr>";
}

/**
 * Add a data row with amounts
 */
void SAIL_TrackDistanceReportWidget::addDataRow(RB_ObjectBase* obj) {
    RB_String betweenCellsLeft = "</td><td>";
    RB_String betweenCellsRight = "</td><td align='right'>";
    mHtml += "<tr><td>";
    mHtml += obj->getValue("mname").toString();
    mHtml += betweenCellsLeft + obj->getValue("from_name").toString();
    mHtml += betweenCellsLeft + obj->getValue("to_name").toString();
    mHtml += betweenCellsRight + RB_String::number(obj->getValue("weightfactor").toDouble(),'f', 2);
    mHtml += betweenCellsRight + RB_String::number(obj->getValue("distance").toDouble(),'f', 2);
    mHtml += betweenCellsRight + RB_String::number(obj->getValue("speed").toDouble(),'f', 2);
    mHtml += betweenCellsRight + RB_String::number(obj->getValue("efficiency").toDouble(),'f', 2);
    mHtml += betweenCellsRight + RB_String::number(obj->getValue("bearingstart").toDouble(),'f', 2);
    mHtml += betweenCellsRight + RB_String::number(obj->getValue("bearingend").toDouble(),'f', 2);
    mHtml += betweenCellsLeft + obj->getValue("comment").toString();
    mHtml += "</td></tr>";
}

/**
 * Add an account group footer row
 */
void SAIL_TrackDistanceReportWidget::addFooterRow() {
//    RB_String betweenCellsRight = "</i></td><td align='right' style='border-top: solid 1px " + mTextColor.name() + ";'><i>";

//    mHtml += "<tr><td colspan=5 align='right'><i>";
//    mHtml += mCustSuppName + " " + tr("Totals") + betweenCellsRight;
//    mHtml += RB_String::number(mSumAmount,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumAlloc,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumDue,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumOverDue1,'f', 2) + betweenCellsRight;
//    mHtml += RB_String::number(mSumOverDue2,'f', 2);
//    mHtml += "</i></td></tr>"; // <tr><td colspan=10>&nbsp;</td></tr> empty line
}

/**
 * Add footer with totals
 */
void SAIL_TrackDistanceReportWidget::addFooter() {
//    RB_String betweenCellsWhite = "</td><td align='right'>";

//    mHtml += "<tfoot style='font-size:8pt;'><tr><td colspan=5 align='right'>";
//    mHtml += tr("Grand Totals");
//    mHtml += "</td><td align='right'>";
//    mHtml += RB_String::number(mTotalAmount,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalAlloc,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalDue,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalOverDue1,'f', 2) + betweenCellsWhite;
//    mHtml += RB_String::number(mTotalOverDue2,'f', 2);
//    mHtml += "</td></tr></tfoot>";
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String SAIL_TrackDistanceReportWidget::getHelpSubject() const {
    return objectName();
}

bool SAIL_TrackDistanceReportWidget::closeWidget() {
    return RB_Widget::closeWidget();
//    if (maybeSave()) {
//        return true;
//    }
//    return false;
}

/**
 * Change event such as language change
 */
void SAIL_TrackDistanceReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
