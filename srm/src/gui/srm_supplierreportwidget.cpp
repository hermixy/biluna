/*****************************************************************
 * $Id: srm_supplierreportwidget.cpp 2132 2014-03-29 09:59:36Z rutger $
 * Created: Jun 4, 2012 9:14:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_supplierreportwidget.h"

#include <QPrintDialog>
#include <QPrinter>
#include "db_actionfilesaveas.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"


/**
 * Constructor
 */
SRM_SupplierReportWidget::SRM_SupplierReportWidget(QWidget *parent)
                        : SRM_ReportWidget(parent) {
    setupUi(this);

    mTeReport = teReport;
}

/**
 * Destructor
 */
SRM_SupplierReportWidget::~SRM_SupplierReportWidget() {
    // nothing
}

/**
 * File Save, can be called by closeWidget()
 */
bool SRM_SupplierReportWidget::fileSave(bool /*withSelect*/) {
    if (mFullFileName.isEmpty()) {
        return fileSaveAs();
    }
    return saveFile(mFullFileName);
}

/**
 * File Save As, the DB_ActionFileSaveAs calls saveFile directly
 */
bool SRM_SupplierReportWidget::fileSaveAs() {
    DB_ActionFileSaveAs a;
    a.setWidget(this);
    a.trigger();
    return true;
}

/**
 * Slot File export PDF.
 * @param printer
 */
void SRM_SupplierReportWidget::filePdf(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print.
 * @param printer
 */
void SRM_SupplierReportWidget::filePrint(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Slot File Print Preview.
 * @param printer
 */
void SRM_SupplierReportWidget::filePrintPreview(QPrinter* printer) {
    teReport->print(printer);
}

/**
 * Edit Cut.
 */
void SRM_SupplierReportWidget::editCut() {
    teReport->cut();
}

/**
 * Edit Copy.
 */
void SRM_SupplierReportWidget::editCopy() {
    teReport->copy();
}

/**
 * Edit Paste.
 */
void SRM_SupplierReportWidget::editPaste() {
    teReport->paste();
}

/**
 * Edit Undo.
 */
void SRM_SupplierReportWidget::editUndo() {
    teReport->undo();
}

/**
 * Edit Redo.
 */
void SRM_SupplierReportWidget::editRedo() {
    teReport->redo();
}

/**
 * Edit Select All.
 */
void SRM_SupplierReportWidget::editSelectAll() {
    teReport->selectAll();
}

/**
 * Insert image from file
 * @param fn file path and name
 */
void SRM_SupplierReportWidget::editInsertImage(const QString& fn) {
    teReport->insertImage(fn);
}

/**
 * @brief SRM_SupplierReportWidget::editZoomIn
 */
void SRM_SupplierReportWidget::editZoomIn(double /*factor*/) {
   teReport->fontZoomIn(1);
}

/**
 * @brief SRM_SupplierReportWidget::editZoomOut
 */
void SRM_SupplierReportWidget::editZoomOut(double /*factor*/) {
    teReport->fontZoomOut(1);
}

/**
 * Save file
 * @param fn filename
 * @return true if success
 */
bool SRM_SupplierReportWidget::saveFile(const RB_String &fn) {
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
 * Initialize widget
 */
void SRM_SupplierReportWidget::init() {
    SRM_ReportWidget::init();

    setComboBoxes();
    cbFromCustSupp->setCurrentIndex(0);
    int count = cbToCustSupp->count();
    cbToCustSupp->setCurrentIndex(count - 1);
    cbFilterByAccount->setChecked(true);
}

/**
 * Initialize and create trial balance
 */
void SRM_SupplierReportWidget::on_pbRefresh_clicked() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Supplier List
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

    setReportTitle(tr("Supplier List"));
    setCompanyName(SRM_MODELFACTORY->getRoot()->getValue("code").toString());
    RB_String limit = tr("Supplier: ") + cbFromCustSupp->currentText();
    limit += " - " + cbToCustSupp->currentText();
    setReportLimit(limit);

    RB_String fromCustSupp = cbFromCustSupp->currentText();
    RB_String toCustSupp = cbToCustSupp->currentText();

    QSqlQuery q(SRM_MODELFACTORY->getDatabase());
    RB_String str = "SELECT suppname, address1, address2, address3, "
            "address4, address5, address6 FROM acc_supplier where suppname >= '";
    str += fromCustSupp;
    str += "' AND suppname <= '";
    str += toCustSupp;
    if (cbFilterByAccount->isChecked()) {
        str += "' AND srm_parent = '";
        str += SRM_MODELFACTORY->getRootId();
    }
    str += "' ORDER BY suppname ASC;";

    if (!q.exec(str)) {
        // error
        RB_DEBUG->error("SRM_SupplierReportWidget::on_pbRefresh_clicked() "
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
RB_String SRM_SupplierReportWidget::getHelpSubject() const {
    return objectName();
}

/**
 * Set comboboxes with alphabeth.
 */
void SRM_SupplierReportWidget::setComboBoxes() {
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
void SRM_SupplierReportWidget::changeEvent(QEvent *e) {
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}



