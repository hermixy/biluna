/*****************************************************************
 * $Id: acc_orderpreviewdialog.cpp 2061 2014-01-12 16:34:14Z rutger $
 * Created: Jul 14, 2011 12:14:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_orderpreviewdialog.h"

#include <QPrintDialog>
#include <QtWidgets>
#include "db_actionfactory.h"
#include "db_actionfilepdf.h"
#include "db_actionfileprint.h"
#include "db_actionfilesaveas.h"


/**
 * Constructor
 */
ACC_OrderPreviewDialog::ACC_OrderPreviewDialog(QWidget *parent) :
                            RB_Dialog(parent) {

    setupUi(this);

    setWindowIcon(QIcon(":/images/biluna16.png"));
    setWindowTitle("ACC Order Preview");

    mTdReportHeader = NULL;
    mTdReportFooter = NULL;
    mTdContent = NULL;
    mHeaderHeight = 0;
    mFooterHeight = 0;

}

/**
 * Destructor
 */
ACC_OrderPreviewDialog::~ACC_OrderPreviewDialog() {
    // nothing
}

QString ACC_OrderPreviewDialog::getSaveAsFileName() {
    return mFullFileName;
}

/**
 * Initialize the widget
 */
void ACC_OrderPreviewDialog::init() {
    readSettings();
}

/**
 * File export PDF.
 */
void ACC_OrderPreviewDialog::filePdf(QPrinter* printer) {
    printContent(printer);
}

/**
 * File Print.
 */
void ACC_OrderPreviewDialog::filePrint(QPrinter* printer) {
    printContent(printer);
}


/**
 * Set html to show with this window,
 * usually ReportHeader+Content+ReportFooter.
 * The PageHeader and PageFooter are only used
 * for printing more than one page.
 * @param content html content
 */
void ACC_OrderPreviewDialog::setHtml(const RB_String& content) {
    mHtml = content;
    textEdit->setHtml(content);
}

/**
 * @brief ACC_OrderPreviewDialog::setPrinterProperties
 * @param pr
 */
void ACC_OrderPreviewDialog::setPrinterProperties(QPrinter* pr) {
    pr->setPageSize(QPrinter::A4);
    pr->setPageMargins(20, 15, 15, 15, QPrinter::Millimeter);
}

/**
 * Button print clicked
 */
void ACC_OrderPreviewDialog::on_pbPrint_clicked() {
    DB_ActionFilePrint a;
    a.printDialog(this);
}

/**
 * Button save PDF clicked
 */
void ACC_OrderPreviewDialog::on_pbSavePdf_clicked() {
    DB_ActionFilePdf a;
    a.printDialog(this);
}

/**
 * Button save as XHTML clicked
 */
void ACC_OrderPreviewDialog::on_pbSaveHtml_clicked() {
    RB_String fn = mFullFileName + ".html";
    RB_String filters = "HTML files (*.html);;All files (*.*)";
    DB_ActionFileSaveAs a;
    a.getSaveFileName(fn, filters);

    if (fn.isEmpty()) return;

    QTextDocumentWriter wr(fn);
    wr.setFormat("HTML");
    bool success = wr.write(textEdit->document());

    if (!success) {
        RB_DEBUG->error("ACC_OrderPreviewDialog::on_pbSaveHtml_clicked() ERROR");
    }
}

/**
 * Print content and header, footer on each page
 * TODO: lines are sometimes half print on one page and on the next
 * due to currentRect clipping
 */
void ACC_OrderPreviewDialog::printContent(QPrinter* pr) {
#ifndef QT_NO_PRINTER
    prepareContent(pr);

    mUtilPrint.setDocHeader(mTdReportHeader, 110);
    mUtilPrint.setDocContent(mTdContent);
    mUtilPrint.setDocFooter(mTdReportFooter, 15);
    mUtilPrint.print(pr);
    return;


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
void ACC_OrderPreviewDialog::prepareContent(QPrinter* /* pr */) {
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
    if (strL.count() != 3) {
        RB_DEBUG->error("ACC_OrderPreviewDialog::prepareContent() ERROR");
        return;
    }

    // Set text documents HTML
    mTdReportHeader->setHtml(strL.at(0));
    mTdContent->setHtml(strL.at(1));
    mTdReportFooter->setHtml(strL.at(2));

    // Set text document page size
//    mTdReportHeader->setPageSize(pr->pageRect().size());
//    mTdContent->setPageSize(pr->pageRect().size());
//    mTdReportFooter->setPageSize(pr->pageRect().size());

    // Get header and footer height
//    mHeaderHeight = mTdReportHeader->size().toSize().height();
//    mFooterHeight = mTdReportFooter->size().toSize().height();
}

void ACC_OrderPreviewDialog::changeEvent(QEvent* e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
