/*****************************************************************
 * $Id: crm_customerreportwidget.cpp 2132 2014-03-29 09:59:36Z rutger $
 * Created: Jun 4, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_customerreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
CRM_CustomerReportWidget::CRM_CustomerReportWidget(QWidget *parent)
                        : CRM_ReportWidget(parent) {
    setupUi(this);

    mTeReport = teReport;
}

/**
 * Destructor
 */
CRM_CustomerReportWidget::~CRM_CustomerReportWidget() {
    // nothing
}

/**
 * Initialize widget
 */
void CRM_CustomerReportWidget::init() {
    CRM_ReportWidget::init();

    setComboBoxes();
    cbFromCustSupp->setCurrentIndex(0);
    int count = cbToCustSupp->count();
    cbToCustSupp->setCurrentIndex(count - 1);
    cbFilterByAccount->setChecked(true);
}

/**
 * File Save, can be called by closeWidget()
 */
bool CRM_CustomerReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool CRM_CustomerReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void CRM_CustomerReportWidget::filePdf(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void CRM_CustomerReportWidget::filePrint(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void CRM_CustomerReportWidget::filePrintPreview(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Edit Cut.
 */
void CRM_CustomerReportWidget::editCut() {
    teReport->cut();
}

/**
 * Edit Copy.
 */
void CRM_CustomerReportWidget::editCopy() {
    teReport->copy();
}

/**
 * Edit Paste.
 */
void CRM_CustomerReportWidget::editPaste() {
    teReport->paste();
}

/**
 * Edit Undo.
 */
void CRM_CustomerReportWidget::editUndo() {
    teReport->undo();
}

/**
 * Edit Redo.
 */
void CRM_CustomerReportWidget::editRedo() {
    teReport->redo();
}

/**
 * Edit Select All.
 */
void CRM_CustomerReportWidget::editSelectAll() {
    teReport->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void CRM_CustomerReportWidget::editInsertImage(const QString& fn) {
    teReport->insertImage(fn);
}

/**
 * @brief CRM_CustomerReportWidget::editZoomIn
 */
void CRM_CustomerReportWidget::editZoomIn(double /*factor*/) {
   teReport->fontZoomIn(1);
}

/**
 * @brief CRM_CustomerReportWidget::editZoomOut
 */
void CRM_CustomerReportWidget::editZoomOut(double /*factor*/) {
    teReport->fontZoomOut(1);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool CRM_CustomerReportWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        CRM_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        CRM_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teReport->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    CRM_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    emit modificationChanged(false);
    return true;
}

/**
 * Initialize and create trial balance
 */
void CRM_CustomerReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // test  only
//    setColumnWidth(50.0); // column 1
//    setColumnWidth(20.0); // column 2
//    setColumnWidth(15.0); // column 3
//    setColumnWidth(15.0); // column 4
//    setReportTitle("Customer List");
//    setCompanyName("Red-Bag");
//    setReportLimit("A-ZZZZ");
//    RB_ObjectContainer* dataList = new RB_ObjectContainer();
//    for (int i = 0; i < 1000; ++i) {
//        RB_ObjectAtomic* obj = new RB_ObjectAtomic("", dataList);
//        dataList->addObject(obj);
//        for (int j = 0; j < 6; ++j) {
//            obj->addMember(obj, "Column " + RB_String::number(j), "nounit", j + i, RB2::MemberInteger);
//        }
//    }
//    RB_String html;
//    if (dataList) {
//        createReport(html, dataList);
//    } else {
//        html = "No data";
//    }
//    mTeReport->setHtml(html);
//    delete dataList;
//    return;


    // Customer List
    clearColumnWidths();
    setColumnWidth(20);
    setColumnWidth(20);
    setColumnWidth(16);
    setColumnWidth(14);
    setColumnWidth(14);
    setColumnWidth(8);
    setColumnWidth(8);
    setAlternatingRow(true);
    // setFirstDataFullRow(true);

    setReportTitle(tr("Customer List"));
    setCompanyName(CRM_MODELFACTORY->getRoot()->getValue("code").toString());
    RB_String limit = tr("Customer: ") + cbFromCustSupp->currentText();
    limit += " - " + cbToCustSupp->currentText();
    setReportLimit(limit);

    RB_String fromCustSupp = cbFromCustSupp->currentText();
    RB_String toCustSupp = cbToCustSupp->currentText();

    QSqlQuery q(CRM_MODELFACTORY->getDatabase());
    RB_String str = "SELECT mname, brpostaddress1, brpostaddress2, brpostaddress3, "
            "brpostaddress4, brpostaddress5, brpostaddress6, brstreetaddress1, "
            "brstreetaddress2, brstreetaddress3, brstreetaddress4, brstreetaddress5, "
            "brstreetaddress6 FROM acc_customer where mname >= '";
    str += fromCustSupp;
    str += "' AND mname <= '";
    str += toCustSupp;
    if (cbFilterByAccount->isChecked()) {
        str += "' AND crm_parent = '";
        str += CRM_MODELFACTORY->getRootId();
    }
    str += "' ORDER BY mname ASC;";

    if (!q.exec(str)) {
        // error
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_CustomerReportWidget::on_pbRefresh_clicked() %s ERROR",
                        q.lastError().text().toStdString().c_str());
        return;
    }

    RB_String html;
    createReport(html, q);
    mTeReport->setHtml(html);

    QApplication::restoreOverrideCursor();
}

/**
 * @returns the help subject, in this case the widget objectname
 */
RB_String CRM_CustomerReportWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set comboboxes with alphabeth.
 */
void CRM_CustomerReportWidget::setComboBoxes() {
    RB_StringList strL;
    strL << "0" << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I"
            << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R"
            << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Zzzz";
    cbFromCustSupp->addItems(strL);
    cbToCustSupp->addItems(strL);
}

/**
 * Change event such as language change
 */
void CRM_CustomerReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}



