/*****************************************************************
 * $Id: acc_reportpreviewtedialog.cpp 2167 2014-09-25 13:34:35Z rutger $
 * Created: Jul 14, 2011 12:14:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_reportpreviewtedialog.h"

#include <QPrintDialog>
#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_actionfilepdf.h"
#include "db_actionfileprint.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_ReportPreviewTeDialog::ACC_ReportPreviewTeDialog(QWidget *parent) :
                            RB_Dialog(parent) {

    setupUi(this);

    setWindowIcon(QIcon(":/images/biluna16.png"));
    setWindowTitle("ACC Report Preview");

    mTdReportHeader = NULL;
    mTdReportFooter = NULL;
    mTdContent = NULL;
    mHeaderHeight = 0;
    mFooterHeight = 0;

}

/**
 * Destructor
 */
ACC_ReportPreviewTeDialog::~ACC_ReportPreviewTeDialog() {
    // nothing
}

QString ACC_ReportPreviewTeDialog::getSaveAsFileName() {
    return mFullFileName;
}

/**
 * Initialize the widget
 */
void ACC_ReportPreviewTeDialog::init() {
    readSettings();
}

/**
 * File export PDF.
 */
void ACC_ReportPreviewTeDialog::filePdf(QPrinter* printer) {
    printContent(printer);
}

/**
 * File Print.
 */
void ACC_ReportPreviewTeDialog::filePrint(QPrinter* printer) {
    printContent(printer);
}


/**
 * Set html to show with this window,
 * usually ReportHeader+Content+ReportFooter.
 * The PageHeader and PageFooter are only used
 * for prining more than one page.
 * @param content html content
 */
void ACC_ReportPreviewTeDialog::setHtml(const RB_String& content) {
    mHtml = content;
    textEdit->setHtml(content);
}

/**
 * @brief ACC_ReportPreviewTeDialog::setPrinterProperties
 * @param pr
 */
void ACC_ReportPreviewTeDialog::setPrinterProperties(QPrinter* pr) {
    pr->setPageSize(QPrinter::A4);
    pr->setPageMargins(17, 10, 15, 15, QPrinter::Millimeter);
}

/**
 * Button print clicked
 */
void ACC_ReportPreviewTeDialog::on_pbPrint_clicked() {
//#ifndef QT_NO_PRINTER
//    QPrinter pr;
//    pr.setPageSize(QPrinter::A4);
//    pr.setPageMargins(17, 10, 15, 15, QPrinter::Millimeter);

//    QPrintDialog* dlg = new QPrintDialog(&pr, this);
//    dlg->setWindowTitle(tr("Print Document"));
//    if (dlg->exec() != QDialog::Accepted) return;

//    printContent(pr);
// #endif

    DB_ActionFilePrint a;
    a.printDialog(this);
}

/**
 * Button save PDF clicked
 */
void ACC_ReportPreviewTeDialog::on_pbSavePdf_clicked() {
//#ifndef QT_NO_PRINTER
//    RB_String fn = mFileName + ".pdf";
//    RB_String filters = "PDF files (*.pdf);;All files (*.*)";
//    DB_ActionFilePdf a;
//    a.getSaveFileName(fn, filters);

//    if (fn.isEmpty()) return;

//    QPrinter pr;
//    pr.setPageSize(QPrinter::A4);
//    pr.setPageMargins(17, 10, 15, 15, QPrinter::Millimeter);
//    // TODO: page setting depending culture for example in US based on letter
//    // pr.setFullPage(true);
//    // pr.paperSize();
//    pr.setOutputFormat(QPrinter::PdfFormat);
//    pr.setOutputFileName(fn);
//    // textEdit->print(&pr);

//    printContent(pr);
//#endif

    DB_ActionFilePdf a;
    a.printDialog(this);
}

/**
 * Button save as XHTML clicked
 */
void ACC_ReportPreviewTeDialog::on_pbSaveHtml_clicked() {
    RB_String fn = mFullFileName + ".html";
    RB_String filters = "HTML files (*.html);;All files (*.*)";
    DB_ActionFileSaveAs a;
    a.getSaveFileName(fn, filters);

    if (fn.isEmpty()) return;

    QTextDocumentWriter wr(fn);
    wr.setFormat("HTML");
    bool success = wr.write(textEdit->document());

    if (!success) {
        RB_DEBUG->error("ACC_ReportPreviewTeDialog::on_pbSaveHtml_clicked() ERROR");
    }
}

/**
 * Print content and header, footer on each page
 * TODO: lines are sometimes half print on one page and on the next
 * due to currentRect clipping
 */
void ACC_ReportPreviewTeDialog::printContent(QPrinter* pr) {
#ifndef QT_NO_PRINTER
    prepareContent(pr);

    // Create rectangles to fit on page
    QRect innerRect = pr->pageRect();
    innerRect.setTop(innerRect.top() + mHeaderHeight);
    innerRect.setBottom(innerRect.bottom() - mFooterHeight);
    // Overal content rectangle
    QRect contentRect = QRect(QPoint(0,0), mTdContent->size().toSize());
    // Printable rectangle of content
    QRect currentRect = QRect(QPoint(0,0), innerRect.size());

    // Set painter starting position
    QPainter painter(pr);

    int count = 0;
    painter.save();
    painter.translate(0, mHeaderHeight);

    while (currentRect.intersects(contentRect)) {
        // Print content at translated position
        mTdContent->drawContents(&painter, currentRect);
        painter.restore();

        if (count == 0) {
            // Print report header
            mTdReportHeader->drawContents(&painter);
        } else {
            // Print following page header
            painter.save();
            painter.translate(0, mFooterHeight/2);
            painter.drawText(0, 0, RB_String(tr("Page") + "%1, "
                                             + tr("continued from page") + " %2")
                                                .arg(count+1).arg(count));
            painter.restore();
        }

        // First set new position and part of content
        currentRect.translate(0, innerRect.height());
        ++count;

        if (count == 1) { // only once
            // Next resize innerRect for next page with header height based on footer
            innerRect.setTop(innerRect.top() - mHeaderHeight + mFooterHeight);
            currentRect.setHeight(innerRect.height());
        }

        // Print footer
        if (currentRect.intersects(contentRect)) {
            // Print page following footer
            painter.save();
            painter.translate(0, pr->pageRect().height() - mFooterHeight/2);
            painter.drawText(0, 0, RB_String(tr("Page") + "%1 "
                                             + tr("continued on page") + " %2")
                                                .arg(count).arg(count+1));
            painter.restore();

            // Move painter with text document position to print on new page
            painter.save();
            painter.translate(0, -currentRect.y() + mFooterHeight);
            pr->newPage();
        } else {
            // Print report footer
            painter.save();
            painter.translate(0, pr->pageRect().height() - mFooterHeight);
            mTdReportFooter->drawContents(&painter);
        }
    }

    painter.restore();
    painter.end();
 #endif
}

/**
 * Prepare header footer and content textDocuments,
 * including the height of the header and footer.
 */
void ACC_ReportPreviewTeDialog::prepareContent(QPrinter* pr) {
    if (!mTdReportHeader) {
        mTdReportHeader = new QTextDocument(this);
        mTdReportHeader->setUseDesignMetrics(true);
    }
    if (!mTdContent) {
        mTdContent = new QTextDocument(this);
        mTdContent->setUseDesignMetrics(true);
    }
    if (!mTdReportFooter) {
        mTdReportFooter = new QTextDocument(this);
        mTdReportFooter->setUseDesignMetrics(true);
    }

    QStringList strL = mHtml.split("<!-- section-break -->");
    if (strL.count() != 5) { // 0 and 4 are html style and body
        RB_DEBUG->error("ACC_ReportPreviewTeDialog::prepareContent() ERROR");
        return;
    }

    // Set text documents HTML
    mTdReportHeader->setHtml(strL.at(0) + strL.at(1) + strL.at(4));
    mTdContent->setHtml(strL.at(0) + strL.at(2) + strL.at(4));
    mTdReportFooter->setHtml(strL.at(0) + strL.at(3) + strL.at(4));

    // Set text document page size
    mTdReportHeader->setPageSize(pr->pageRect().size());
    mTdContent->setPageSize(pr->pageRect().size());
    mTdReportFooter->setPageSize(pr->pageRect().size());

    // Get header and footer height
    mHeaderHeight = mTdReportHeader->size().toSize().height();
    mFooterHeight = mTdReportFooter->size().toSize().height();

    mUtilPrintTxtHf.setPageOrientation(QPrinter::Portrait);
    mUtilPrintTxtHf.setPaperMargin(20, 15, 15, 15);
    mUtilPrintTxtHf.clearAllReplace();
}

void ACC_ReportPreviewTeDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
