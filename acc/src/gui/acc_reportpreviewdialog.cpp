/*****************************************************************
 * $Id: acc_reportpreviewdialog.cpp 2168 2014-09-25 13:34:45Z rutger $
 * Created: Mar 20, 2010 12:14:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_reportpreviewdialog.h"

#include <QPrintDialog>
#include <QWebFrame>
#include "acc_dialogfactory.h"
#include "db_actionfilepdf.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_ReportPreviewDialog::ACC_ReportPreviewDialog(QWidget *parent) :
                            RB_Dialog(parent) {

    setupUi(this);

    setWindowIcon(QIcon(":/images/biluna16.png"));
    setWindowTitle("ACC Report Preview");
}

/**
 * Destructor
 */
ACC_ReportPreviewDialog::~ACC_ReportPreviewDialog() {
    // nothing
}

/**
 * Initialize the widget
 */
void ACC_ReportPreviewDialog::init() {
    readSettings();
}

/**
 * Set html
 * @param content html content
 */
void ACC_ReportPreviewDialog::setHtml(const RB_String& content) {
    webView->setHtml(content);
}

/**
 * Button print clicked
 */
void ACC_ReportPreviewDialog::on_pbPrint_clicked() {
#ifndef QT_NO_PRINTER
    QPrinter pr;
    pr.setPageMargins(17, 14, 15, 15, QPrinter::Millimeter);

    QPrintDialog* dlg = new QPrintDialog(&pr, this);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() != QDialog::Accepted) return;
    webView->print(&pr);
#endif
 #endif
}

/**
 * Button save PDF clicked
 */
void ACC_ReportPreviewDialog::on_pbSavePdf_clicked() {
#ifndef QT_NO_PRINTER
    RB_String fn;
    RB_String filters = "PDF files (*.pdf);;All files (*.*)";
    DB_ActionFilePdf a;
    a.getSaveFileName(fn, filters);

    if (fn.isEmpty()) return;

    QPrinter pr;
    pr.setPageSize(QPrinter::A4);
    pr.setPageMargins(17 , 14, 15, 10, QPrinter::Millimeter);
//    pr.setPageMargins(0 , 0, 0, 0, QPrinter::Millimeter);
    pr.setOutputFormat(QPrinter::PdfFormat);
    pr.setOutputFileName(fn);
    webView->print(&pr);
 #endif
}

/**
 * Button save as XHTML clicked
 */
void ACC_ReportPreviewDialog::on_pbSaveHtml_clicked() {
    RB_String fn =  "acc_report.html";
    RB_String filters = "HTML files (*.html);;All files (*.*)";
    DB_ActionFileSaveAs a;
    a.getSaveFileName(fn, filters);

    QFile file(fn);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        ACC_DIALOGFACTORY->requestWarningDialog(
            tr("Cannot write file %1:\n%2.").arg(fn).arg(file.errorString()));
        ACC_DIALOGFACTORY->statusBarMessage(tr("File save ERROR"), 2000);
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << webView->page()->mainFrame()->toHtml();
    QApplication::restoreOverrideCursor();
}


void ACC_ReportPreviewDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
