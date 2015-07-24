/*****************************************************************
 * $Id: srm_printtext.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: 2012-03-18 rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_printtext.h"

#include <QDateTime>
#include <QPainter>
#include <QTextCursor>
#include <QTextDocument>
#include <QWidget>
#include "rb_debug.h"
#include "rb_objectbase.h"


/**
 * Constructor
 */
SRM_PrintText::SRM_PrintText() {
    mDocument = NULL;
    mTarget = NULL;

    mPaperSize = QSizeF(210.0, 297.0); // mm
    mPageCount = 1;

    mMarginLeft = 20.0;
    mMarginRight = 15.0;
    mMarginTop = 15.0;
    mMarginBottom = 15.0;

    mPrinter = NULL;
}

SRM_PrintText::~SRM_PrintText() {

}

/**
 * Print the document
 * @param pr printer
 */
void SRM_PrintText::print(QPrinter* pr) {
    if (!mTarget) {
        RB_DEBUG->error("SRM_PrintText::print() contact is NULL ERROR");
        return;
    }

    mPrinter = pr;
    QPainter p(pr);
    printText(&p);
    mPrinter = NULL;
}

/**
 * Set data objects
 * @param doc document object with reference and texts
 * @param cust customer object
 * @param contact contact object
 */
void SRM_PrintText::setData(RB_ObjectBase* doc, RB_ObjectBase *target) {
    mDocument = doc;
    mTarget = target;

    if (doc) {
        mMarginLeft = doc->getValue("marginleft").toDouble();
        mMarginRight = doc->getValue("marginright").toDouble();
        mMarginTop = doc->getValue("margintop").toDouble();
        mMarginBottom = doc->getValue("marginbottom").toDouble();

        int paperSize = doc->getValue("papersize").toInt();
        QPrinter pr;
        pr.setPaperSize((QPrinter::PageSize)paperSize);
        mPaperSize = QSizeF(pr.paperRect(QPrinter::Millimeter).width(),
                            pr.paperRect(QPrinter::Millimeter).height());

        if (doc->getValue("pageheaderheight").toDouble() > 0.0) {
            mPageHeader.setHtml(doc->getValue("pageheader").toString());
            mPageHeaderHeight = doc->getValue("pageheaderheight").toDouble();
        } else {
            mPageHeader.clear();
            mPageHeaderHeight = 0.0;
        }

        mDocContent.setHtml(doc->getValue("doccontent").toString());

        if (doc->getValue("pagefooterheight").toDouble() > 0.0) {
            mPageFooter.setHtml(doc->getValue("pagefooter").toString());
            mPageFooterHeight = doc->getValue("pagefooterheight").toDouble();
        } else {
            mPageFooter.clear();
            mPageFooterHeight = 0.0;
        }
    }
}

/**
 * Print the text
 * @param p painter
 */
void SRM_PrintText::printText(QPainter* p) {
    if (!mPrinter || !p) {
        return;
    }

    replaceFields();
    QWidget wdgt; // only to logicalDpiX and Y

    // scale text dpi to mm
    double mmToDoc = ((double)wdgt.logicalDpiX()) / 25.4; // mm to document logical DPI
    double sourceDpiX = ((double)mPrinter->logicalDpiX() / ((double)wdgt.logicalDpiX()));
    double sourceDpiY = ((double)mPrinter->logicalDpiY() / ((double)wdgt.logicalDpiY()));
    mPageSizeCurrent = QSizeF(mPrinter->pageRect(QPrinter::DevicePixel).width() / sourceDpiX,
                              mPrinter->pageRect(QPrinter::DevicePixel).height() / sourceDpiY);

    // Printable rectangle on paper within print margins
    QRectF printRect;
    printRect.setLeft(0);
    printRect.setTop(0);
    printRect.setRight(mPageSizeCurrent.width());
    printRect.setBottom(mPageSizeCurrent.height());

    // Now printable area calculated in DPI, left and top are already 0
    QRectF innerRect = printRect;
    innerRect.setTop(printRect.top() + mPageHeaderHeight * mmToDoc);
    innerRect.setBottom(printRect.height() - mPageFooterHeight * mmToDoc);

    mPageHeader.setUseDesignMetrics(true);
    mPageHeader.setTextWidth(printRect.width());
    mDocContent.setUseDesignMetrics(true);
    mDocContent.setPageSize(printRect.size());
    mPageFooter.setUseDesignMetrics(true);
    mPageFooter.setTextWidth(printRect.width());

    // Printable document content text on paper minus page header and footer,
    // if applicable. Below is calculated in mm.
    QRectF contentRect = QRectF(QPointF(0.0, 0.0),  mDocContent.size().toSize());

    // Start content rectangle
    QRectF currentRect = QRectF(QPointF(0.0, 0.0),  innerRect.size());

    // Calculate number of pages and reset pageSize
    mDocContent.setPageSize(currentRect.size());
    mPageCount = mDocContent.pageCount();

    // p.begin(this);
    p->setRenderHint(QPainter::Antialiasing);
    p->scale(sourceDpiX, sourceDpiY);

    qreal currentX = 0.0; // screen page mm
    qreal currentY = 0.0; // screen page mm
    int pageCounter = 0;

    while (currentRect.intersects(contentRect)) {
        // Paint page header
        p->save();
        p->translate(currentX, currentY);
        // No print rect, it assumes that headers/footers fit within area
        mPageHeader.drawContents(p);
        p->restore();

        currentY = mPageHeaderHeight * mmToDoc - currentRect.height() * pageCounter;

        // Paint document content, compensated for currentRect top translation
        // and as fits on this page
        p->save();
        p->translate(currentX, currentY);
// debugRectangle(p, currentRect, QColor("#DDEEFF"));
        mDocContent.drawContents(p, currentRect);
        p->restore();

        currentRect.translate(0, currentRect.height());
        ++pageCounter;

        currentY = mPageSizeCurrent.height() - mPageFooterHeight * mmToDoc;

        // Format page footer
        QString str = mPageFooter.toHtml();
        str.replace("{$SRM_Document.reference}", mDocument->getValue("reference").toString(),
                    Qt::CaseInsensitive);
        str.replace("{$pagenumberoftotal}", QString::number(pageCounter) + " of "
                    + QString::number(mPageCount), Qt::CaseInsensitive);
        str.replace("{$pagenumber}", QString::number(pageCounter),
                    Qt::CaseInsensitive);
        mPageFooter.setHtml(str);

        // Paint page footer
        p->save();
        p->translate(currentX, currentY);
        mPageFooter.drawContents(p);
        p->restore();

        // Set X and Y to origine (left-top) of new page
        currentX = 0.0;
        currentY = 0.0;

        if (mPrinter && pageCounter < mPageCount ) {
            mPrinter->newPage();
        }
    }

    p->end();
}

/**
 * Replace the fields with data
 */
void SRM_PrintText::replaceFields() {
    if (!mTarget) return;

    setField(&mDocContent, "{$ACC_Contact.salutation}", mTarget->getValue("salutation").toString());
    setField(&mDocContent, "{$ACC_Contact.firstname}", mTarget->getValue("firstname").toString());
    setField(&mDocContent, "{$ACC_Contact.lastname}", mTarget->getValue("lastname").toString());
    setField(&mDocContent, "{$ACC_Contact.title}", mTarget->getValue("title").toString());
    setField(&mDocContent, "{$ACC_Contact.department}", mTarget->getValue("department").toString());
    setField(&mDocContent, "{$ACC_Contact.phonework}", mTarget->getValue("phonework").toString());
    setField(&mDocContent, "{$ACC_Contact.phonefax}", mTarget->getValue("phonefax").toString());
    setField(&mDocContent, "{$ACC_Contact.email1}", mTarget->getValue("email1").toString());
    setField(&mDocContent, "{$ACC_Contact.primaryaddress1}", mTarget->getValue("primaryaddress1").toString());
    setField(&mDocContent, "{$ACC_Contact.primaryaddress2}", mTarget->getValue("primaryaddress2").toString());
    setField(&mDocContent, "{$ACC_Contact.primaryaddress3}", mTarget->getValue("primaryaddress3").toString());
    setField(&mDocContent, "{$ACC_Contact.primaryaddress4}", mTarget->getValue("primaryaddress4").toString());
    setField(&mDocContent, "{$ACC_Contact.primaryaddress5}", mTarget->getValue("primaryaddress5").toString());
    setField(&mDocContent, "{$ACC_Contact.primaryaddress6}", mTarget->getValue("primaryaddress6").toString());

    setField(&mDocContent, "{$ACC_Supplier.suppname}", mTarget->getValue("suppname").toString());
    setField(&mDocContent, "{$ACC_Supplier.suppliercode}", mTarget->getValue("suppliercode").toString());

    setField(&mDocContent, "{$SRM_Template.reference}", mDocument->getValue("reference").toString());

    setField(&mDocContent, "{$date}", QDateTime::currentDateTime().date().toString(Qt::ISODate));
}

/**
 * Replace fieldName by fieldValue in text
 * @param textDoc text document
 * @param fieldName field name with {$ and }
 * @param fieldValue field value
 */
void SRM_PrintText::setField(QTextDocument* textDoc, const QString& fieldName,
                             const QString& fieldValue) {
    QTextCursor tc = textDoc->find(fieldName, Qt::CaseInsensitive);

    if (tc.hasSelection()) {
        tc.insertText(fieldValue);
    }
}

/**
 * Debug function only, to highlight the various page areas
 */
void SRM_PrintText::debugRectangle(QPainter* p, const QRectF& rect,
                                    const QColor& color) {
    p->fillRect(rect, color);
    p->setPen(QPen(Qt::red));
    p->drawLine(-5, 0, 5, 0);
    p->drawLine(0, 5, 0, -5);
}

