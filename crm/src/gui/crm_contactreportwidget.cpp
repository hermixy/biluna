/*****************************************************************
 * $Id: crm_contactreportwidget.cpp 2132 2014-03-29 09:59:36Z rutger $
 * Created: Jun 7, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_contactreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
CRM_ContactReportWidget::CRM_ContactReportWidget(QWidget *parent)
                        : CRM_ReportWidget(parent) {
    setupUi(this);

    mTeReport = teReport;
}

/**
 * Destructor
 */
CRM_ContactReportWidget::~CRM_ContactReportWidget() {
    // nothing
}

/**
 * Initialize widget
 */
void CRM_ContactReportWidget::init() {
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
bool CRM_ContactReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool CRM_ContactReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void CRM_ContactReportWidget::filePdf(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void CRM_ContactReportWidget::filePrint(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void CRM_ContactReportWidget::filePrintPreview(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Edit Cut.
 */
void CRM_ContactReportWidget::editCut() {
    teReport->cut();
}

/**
 * Edit Copy.
 */
void CRM_ContactReportWidget::editCopy() {
    teReport->copy();
}

/**
 * Edit Paste.
 */
void CRM_ContactReportWidget::editPaste() {
    teReport->paste();
}

/**
 * Edit Undo.
 */
void CRM_ContactReportWidget::editUndo() {
    teReport->undo();
}

/**
 * Edit Redo.
 */
void CRM_ContactReportWidget::editRedo() {
    teReport->redo();
}

/**
 * Edit Select All.
 */
void CRM_ContactReportWidget::editSelectAll() {
    teReport->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void CRM_ContactReportWidget::editInsertImage(const QString& fn) {
    teReport->insertImage(fn);
}

/**
 * @brief CRM_ContactReportWidget::editZoomIn
 */
void CRM_ContactReportWidget::editZoomIn(double /*factor*/) {
   teReport->fontZoomIn(1);
}

/**
 * @brief CRM_ContactReportWidget::editZoomOut
 */
void CRM_ContactReportWidget::editZoomOut(double /*factor*/) {
    teReport->fontZoomOut(1);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool CRM_ContactReportWidget::saveFile(const RB_String &fn) {
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
void CRM_ContactReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Contact List
    clearColumnWidths();
    setColumnWidth(8); // firstname
    setColumnWidth(12); // lastname
    setColumnWidth(8); // phonework
    setColumnWidth(18); // email1
    setColumnWidth(12); // primaryaddress1
    setColumnWidth(12); // primaryaddress2
    setColumnWidth(10); // primaryaddress3
    setColumnWidth(8); // primaryaddress4
    setColumnWidth(8); // primaryaddress5
    setColumnWidth(4); // primaryaddress6
    setAlternatingRow(true);
    // setFirstDataFullRow(true);

    setReportTitle(tr("Contact List"));
    setCompanyName(CRM_MODELFACTORY->getRoot()->getValue("code").toString());
    RB_String limit = tr("Contact: ") + cbFrom->currentText();
    limit += " - " + cbTo->currentText();
    setReportLimit(limit);

    RB_String fromContact = cbFrom->currentText();
    RB_String toContact = cbTo->currentText();

    QSqlQuery q(CRM_MODELFACTORY->getDatabase());
    RB_String str = "SELECT acc_contact.firstname, acc_contact.lastname, "
            "acc_contact.phonework, acc_contact.email1, "
            "acc_contact.primaryaddress1 as address1, acc_contact.primaryaddress2 as address2, "
            "acc_contact.primaryaddress3 as address3, acc_contact.primaryaddress4 as address4, "
            "acc_contact.primaryaddress5 as address5, acc_contact.primaryaddress6 as address6 "
            "FROM acc_contact ";
    if (cbFilterByAccount->isChecked()) {
        str += "INNER JOIN acc_customer ON acc_customer.id=acc_contact.parent ";
    }
    str += "WHERE lastname >= '";
    str += fromContact;
    str += "' AND lastname <= '";
    str += toContact;
    if (cbFilterByAccount->isChecked()) {
        str += "' AND acc_customer.crm_parent = '";
        str += CRM_MODELFACTORY->getRootId();
    }
    str += "' ORDER BY lastname ASC;";

    if (!q.exec(str)) {
        // error
        RB_DEBUG->error("CRM_ContactReportWidget::on_pbRefresh_clicked() "
                        + q.lastError().text() + " ERROR");
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
RB_String CRM_ContactReportWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set comboboxes with alphabeth.
 */
void CRM_ContactReportWidget::setComboBoxes() {
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
void CRM_ContactReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}



