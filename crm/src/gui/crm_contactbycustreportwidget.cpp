/*****************************************************************
 * $Id: crm_contactbycustreportwidget.cpp 2132 2014-03-29 09:59:36Z rutger $
 * Created: Jun 8, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_contactbycustreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
CRM_ContactByCustReportWidget::CRM_ContactByCustReportWidget(QWidget *parent)
                        : CRM_ReportWidget(parent) {
    setupUi(this);

    mTeReport = teReport;
}

/**
 * Destructor
 */
CRM_ContactByCustReportWidget::~CRM_ContactByCustReportWidget() {
    // nothing
}

/**
 * Initialize widget
 */
void CRM_ContactByCustReportWidget::init() {
    CRM_ReportWidget::init();

    setComboBoxes();
    cbFrom->setCurrentIndex(0);
    int count = cbTo->count();
    cbTo->setCurrentIndex(count - 1);
    cbFilterByAccount->setChecked(true);
}

/**
 * File Save, can be called by closeWidget()
 */
bool CRM_ContactByCustReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool CRM_ContactByCustReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void CRM_ContactByCustReportWidget::filePdf(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void CRM_ContactByCustReportWidget::filePrint(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void CRM_ContactByCustReportWidget::filePrintPreview(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Edit Cut.
 */
void CRM_ContactByCustReportWidget::editCut() {
    teReport->cut();
}

/**
 * Edit Copy.
 */
void CRM_ContactByCustReportWidget::editCopy() {
    teReport->copy();
}

/**
 * Edit Paste.
 */
void CRM_ContactByCustReportWidget::editPaste() {
    teReport->paste();
}

/**
 * Edit Undo.
 */
void CRM_ContactByCustReportWidget::editUndo() {
    teReport->undo();
}

/**
 * Edit Redo.
 */
void CRM_ContactByCustReportWidget::editRedo() {
    teReport->redo();
}

/**
 * Edit Select All.
 */
void CRM_ContactByCustReportWidget::editSelectAll() {
    teReport->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void CRM_ContactByCustReportWidget::editInsertImage(const QString& fn) {
    teReport->insertImage(fn);
}

/**
 * @brief CRM_ContactByCustReportWidget::editZoomIn
 */
void CRM_ContactByCustReportWidget::editZoomIn(double /*factor*/) {
   teReport->fontZoomIn(1);
}

/**
 * @brief CRM_ContactByCustReportWidget::editZoomOut
 */
void CRM_ContactByCustReportWidget::editZoomOut(double /*factor*/) {
    teReport->fontZoomOut(1);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool CRM_ContactByCustReportWidget::saveFile(const RB_String &fn) {
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
void CRM_ContactByCustReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Contact List
    clearColumnWidths();
    setColumnWidth(20); // customer
    setColumnWidth(10); // customerno
    setColumnWidth(15); // firstname
    setColumnWidth(20); // lastname
    setColumnWidth(15); // phonework
    setColumnWidth(20); // email1
    setAlternatingRow(true);
    // setFirstDataFullRow(true);

    setReportTitle(tr("Contact by Customer List"));
    setCompanyName(CRM_MODELFACTORY->getRoot()->getValue("code").toString());
    RB_String limit = tr("Customer: ") + cbFrom->currentText();
    limit += " - " + cbTo->currentText();
    setReportLimit(limit);

    RB_String fromContact = cbFrom->currentText();
    RB_String toContact = cbTo->currentText();

    QSqlQuery q(CRM_MODELFACTORY->getDatabase());
    RB_String str = "SELECT mname as customer, customerno, firstname, lastname, phonework, email1 "
            "FROM acc_contact INNER JOIN acc_customer ON acc_customer.id=acc_contact.parent "
            "WHERE customerno >= '";
    str += fromContact;
    str += "' AND customerno <= '";
    str += toContact;
    if (cbFilterByAccount->isChecked()) {
        str += "' AND acc_customer.crm_parent = '";
        str += CRM_MODELFACTORY->getRootId();
    }
    str += "' ORDER BY customerno, lastname ASC;";

    if (!q.exec(str)) {
        // error
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_ContactByCustReportWidget::on_pbRefresh_clicked() %s ERROR",
                        q.lastError().text().toStdString().c_str());
        QApplication::restoreOverrideCursor();
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
RB_String CRM_ContactByCustReportWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set comboboxes with alphabeth.
 */
void CRM_ContactByCustReportWidget::setComboBoxes() {
    RB_StringList strL;
    strL << "0" << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I"
            << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R"
            << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Zzzz";
    cbFrom->addItems(strL);
    cbTo->addItems(strL);
}

/**
 * Change event such as language change
 */
void CRM_ContactByCustReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}



