/*****************************************************************
 * $Id: srm_contactbysuppreportwidget.cpp 2132 2014-03-29 09:59:36Z rutger $
 * Created: Jun 8, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_contactbysuppreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "db_actionfilesaveas.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"


/**
 * Constructor
 */
SRM_ContactBySuppReportWidget::SRM_ContactBySuppReportWidget(QWidget *parent)
                        : SRM_ReportWidget(parent) {
    setupUi(this);

    mTeReport = teReport;
}

/**
 * Destructor
 */
SRM_ContactBySuppReportWidget::~SRM_ContactBySuppReportWidget() {
    // nothing
}

/**
 * Initialize widget
 */
void SRM_ContactBySuppReportWidget::init() {
    SRM_ReportWidget::init();

    setComboBoxes();
    cbFrom->setCurrentIndex(0);
    int count = cbTo->count();
    cbTo->setCurrentIndex(count - 1);
    cbFilterByAccount->setChecked(true);
}

/**
 * File Save, can be called by closeWidget()
 */
bool SRM_ContactBySuppReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool SRM_ContactBySuppReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void SRM_ContactBySuppReportWidget::filePdf(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void SRM_ContactBySuppReportWidget::filePrint(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void SRM_ContactBySuppReportWidget::filePrintPreview(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Edit Cut.
 */
void SRM_ContactBySuppReportWidget::editCut() {
    teReport->cut();
}

/**
 * Edit Copy.
 */
void SRM_ContactBySuppReportWidget::editCopy() {
    teReport->copy();
}

/**
 * Edit Paste.
 */
void SRM_ContactBySuppReportWidget::editPaste() {
    teReport->paste();
}

/**
 * Edit Undo.
 */
void SRM_ContactBySuppReportWidget::editUndo() {
    teReport->undo();
}

/**
 * Edit Redo.
 */
void SRM_ContactBySuppReportWidget::editRedo() {
    teReport->redo();
}

/**
 * Edit Select All.
 */
void SRM_ContactBySuppReportWidget::editSelectAll() {
    teReport->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void SRM_ContactBySuppReportWidget::editInsertImage(const QString& fn) {
    teReport->insertImage(fn);
}

/**
 * @brief SRM_ContactBySuppReportWidget::editZoomIn
 */
void SRM_ContactBySuppReportWidget::editZoomIn(double /*factor*/) {
   teReport->fontZoomIn(1);
}

/**
 * @brief SRM_ContactBySuppReportWidget::editZoomOut
 */
void SRM_ContactBySuppReportWidget::editZoomOut(double /*factor*/) {
    teReport->fontZoomOut(1);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool SRM_ContactBySuppReportWidget::saveFile(const RB_String &fn) {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        SRM_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        SRM_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << teReport->toHtml();
    QApplication::restoreOverrideCursor();

    setCurrentFileName(fn);
    SRM_DIALOGFACTORY->statusBarMessage(tr("File saved"), 2000);
    emit modificationChanged(false);
    return true;
}

/**
 * Initialize and create trial balance
 */
void SRM_ContactBySuppReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Contact List
    clearColumnWidths();
    setColumnWidth(20); // supplier
    setColumnWidth(10); // suppliercode
    setColumnWidth(15); // firstname
    setColumnWidth(20); // lastname
    setColumnWidth(15); // phonework
    setColumnWidth(20); // email1
    setAlternatingRow(true);
    // setFirstDataFullRow(true);

    setReportTitle(tr("Contact by Supplier List"));
    setCompanyName(SRM_MODELFACTORY->getRoot()->getValue("code").toString());
    RB_String limit = tr("Supplier: ") + cbFrom->currentText();
    limit += " - " + cbTo->currentText();
    setReportLimit(limit);

    RB_String fromContact = cbFrom->currentText();
    RB_String toContact = cbTo->currentText();

    QSqlQuery q(SRM_MODELFACTORY->getDatabase());
    RB_String str = "SELECT suppname as supplier, suppliercode as code, firstname, lastname, phonework, email1 "
            "FROM acc_contact INNER JOIN acc_supplier ON acc_supplier.id=acc_contact.parent "
            "WHERE suppliercode >= '";
    str += fromContact;
    str += "' AND suppliercode <= '";
    str += toContact;
    if (cbFilterByAccount->isChecked()) {
        str += "' AND acc_supplier.srm_parent = '";
        str += SRM_MODELFACTORY->getRootId();
    }
    str += "' ORDER BY suppliercode, lastname ASC;";

    if (!q.exec(str)) {
        // error
        RB_DEBUG->error("SRM_ContactByCustReportWidget::on_pbRefresh_clicked() "
                        + q.lastError().text() + " ERROR");
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
RB_String SRM_ContactBySuppReportWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set comboboxes with alphabeth.
 */
void SRM_ContactBySuppReportWidget::setComboBoxes() {
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
void SRM_ContactBySuppReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}



