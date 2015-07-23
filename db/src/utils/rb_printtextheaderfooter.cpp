/*****************************************************************
 * $Id: rb_printtextheaderfooter.cpp 2073 2014-01-25 17:07:56Z rutger $
 * Created: 2013-12-19 rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_printtextheaderfooter.h"

#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextFrameFormat>
#include "rb_debug.h"

/**
 * Constructor
 */
RB_PrintTextHeaderFooter::RB_PrintTextHeaderFooter() {
    mDocHeader = NULL;
    mDocContent = NULL;
    mDocFooter = NULL;

    // millimeters
    mDocHeaderHeight = 0.0;
    mDocFooterHeight = 0.0;

    mMarginLeft = -1.0;
    mMarginRight = -1.0;
    mMarginTop = -1.0;
    mMarginBottom = -1.0;

    mIsPrintPageNumbers = false;
    mPageTotal = 0;

    mPrinter = NULL;
    mOrientation = -1;
    mDpiy = -1;
    mContentTopDpi = -1;
    mContentHeightDpi = -1;
    mContentWidthDpi = -1;

    mIsDebug = false;
}

RB_PrintTextHeaderFooter::~RB_PrintTextHeaderFooter() {
    delete mDocHeader;
    delete mDocContent;
    delete mDocFooter;
}

/**
 * Print this textEdit widget
 * @param printer
 */
void RB_PrintTextHeaderFooter::print(QPrinter* printer) {
    // allows printing of selection
    // QTextEdit::print(printer);

    // otherwise:
    // document()->print(printer);

    // HACK: because Qt 5.2 printing problems
    printDocument(printer);
}

/**
 * Set paper margins [mm] from the edge of the paper to starting of the text
 * @param marginLeft
 * @param marginRight
 * @param marginTop
 * @param marginBottom
 */
void RB_PrintTextHeaderFooter::setPaperMargin(qreal marginLeft, qreal marginTop,
                                         qreal marginRight, qreal marginBottom) {
    mMarginLeft = marginLeft;
    mMarginTop = marginTop;
    mMarginRight = marginRight;
    mMarginBottom = marginBottom;
}

void RB_PrintTextHeaderFooter::setPrintPageNumbers(bool isPrintPageNumbers) {
    mIsPrintPageNumbers = isPrintPageNumbers;
}

void RB_PrintTextHeaderFooter::setPageOrientation(QPrinter::Orientation orientation) {
    mOrientation = (int)orientation;
}

/**
 * Set document header text and height
 * @param textDoc text document
 * @param height height of the section in millimeters
 */
void RB_PrintTextHeaderFooter::setDocHeader(QTextDocument* textDoc, qreal height) {
    if (!mDocHeader) {
        mDocHeader = new QTextDocument();
    }
    mDocHeader->setHtml(textDoc->toHtml());
    mDocHeaderHeight = height;
}

/**
 * Set document text
 * @param textDoc text document
 */
void RB_PrintTextHeaderFooter::setDocContent(QTextDocument* textDoc) {
    if (!mDocContent) {
        mDocContent = new QTextDocument();
    }
    mDocContent->setHtml(textDoc->toHtml());
}

/**
 * Set document footer text and height
 * @param textDoc text document
 * @param height height of the section in millimeters
 */
void RB_PrintTextHeaderFooter::setDocFooter(QTextDocument* textDoc, qreal height) {
    if (!mDocFooter) {
        mDocFooter = new QTextDocument();
    }
    mDocFooter->setHtml(textDoc->toHtml());
    mDocFooterHeight = height;
}

void RB_PrintTextHeaderFooter::addHeaderReplace(const QString& key,
                                           const QString& value) {
    mHeaderReplaceMap[key] = value;
}

void RB_PrintTextHeaderFooter::addContentReplace(const QString& key,
                                            const QString& value) {
    mContentReplaceMap[key] = value;
}

void RB_PrintTextHeaderFooter::addFooterReplace(const QString& key,
                                           const QString& value) {
    mFooterReplaceMap[key] = value;
}

void RB_PrintTextHeaderFooter::clearHeaderReplace() {
    mHeaderReplaceMap.clear();
}

void RB_PrintTextHeaderFooter::clearContentReplace() {
    mContentReplaceMap.clear();
}

void RB_PrintTextHeaderFooter::clearFooterReplace() {
    mFooterReplaceMap.clear();
}

void RB_PrintTextHeaderFooter::clearAllReplace() {
    clearHeaderReplace();
    clearContentReplace();
    clearFooterReplace();
}

/**
 * HACK: from QTextDocument
 * @param printer
 * @param te
 */
void RB_PrintTextHeaderFooter::printDocument(QPrinter* printer) {

    if (!printer || !mDocContent)
        return;

    mPrinter = printer;

    // Override printer settings
    if (mMarginLeft < 0 || mMarginTop < 0
            || mMarginRight < 0 || mMarginBottom < 0) {
        mMarginLeft = printer->margins().left;
        mMarginRight = printer->margins().right;
        mMarginTop = printer->margins().top;
        mMarginBottom = printer->margins().bottom;
    } else {
        mPrinter->setPageMargins(mMarginLeft, mMarginTop, mMarginRight,
                                 mMarginBottom, QPrinter::Millimeter);
    }

    if (mOrientation < 0) {
        mOrientation = (int)mPrinter->orientation();
    } else {
        mPrinter->setOrientation((QPrinter::Orientation)mOrientation);
    }

    QPainter p(printer); // includes p.begin(), p.end() at destruction

    // Check that there is a valid device to print to.
    if (!p.isActive())
        return;

    // Clone document
    QTextDocument* d = mDocContent;
    (void)d->documentLayout(); // make sure that there is a layout
    QRectF body = QRectF(QPointF(0, 0), d->pageSize());

    QTextDocument* doc = d->clone(d);
    QScopedPointer<QTextDocument> clonedDoc;
    clonedDoc.reset(doc);

    // Copy additional formats
    for (QTextBlock srcBlock = d->firstBlock(), dstBlock = clonedDoc->firstBlock();
         srcBlock.isValid() && dstBlock.isValid();
         srcBlock = srcBlock.next(), dstBlock = dstBlock.next()) {
        dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
    }

    // Text replacement in clone/copy of document
    if (mContentReplaceMap.size() > 0) {
        QString str = doc->toHtml();

        std::map<QString, QString>::const_iterator iter;
        iter = mContentReplaceMap.begin();

        while (iter != mContentReplaceMap.end()) {
            str.replace(iter->first, iter->second, Qt::CaseInsensitive);
            ++iter;
        }

        doc->setHtml(str);
    }

    // Layout
    QAbstractTextDocumentLayout* layout = doc->documentLayout();
    layout->setPaintDevice(p.device());

    int margin = 0;
    QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
    fmt.setMargin(margin);
    doc->rootFrame()->setFrameFormat(fmt);

    mDpiy = p.device()->logicalDpiY();
    mContentWidthDpi = (printer->pageSizeMM().width() - mMarginLeft
                        - mMarginRight) / 25.4 * mDpiy;
    mContentHeightDpi = (printer->pageSizeMM().height() - mMarginTop
                         - mMarginBottom - mDocHeaderHeight
                         - mDocFooterHeight) / 25.4 * mDpiy;
    mContentTopDpi = mDocHeaderHeight / 25.4 * mDpiy;

    if (mOrientation == (int)QPrinter::Landscape) {
        mContentWidthDpi = (printer->pageSizeMM().height() - mMarginLeft
                            - mMarginRight) / 25.4 * mDpiy;
        mContentHeightDpi = (printer->pageSizeMM().width() - mMarginTop
                             - mMarginBottom - mDocHeaderHeight
                             - mDocFooterHeight) / 25.4 * mDpiy;
    }

    body = QRectF(0, mContentTopDpi, mContentWidthDpi, mContentHeightDpi);
    clonedDoc->setPageSize(body.size()); // is actually doc->setPageSize(...)
    clonedDoc->documentLayout();


    // Printing
    QPointF pageNumberPos;
    if (mIsPrintPageNumbers) {
        pageNumberPos = QPointF(body.width() - margin,
                            body.height() - margin
                            + QFontMetrics(doc->defaultFont(), p.device()).ascent()
                            + 5 * mDpiy / 72.0);
    }

    // Pagination and printing
    int fromPage = printer->fromPage();
    int toPage = printer->toPage();
    bool ascending = true;

    if (fromPage == 0 && toPage == 0) {
        fromPage = 1;
        toPage = doc->pageCount();
    }
    // paranoia check
    fromPage = qMax(1, fromPage);
    toPage = qMin(doc->pageCount(), toPage);

    if (toPage < fromPage) {
        // if the user entered a page range outside the actual number
        // of printable pages, just return
        return;
    }

    mPageTotal = toPage;
    int page = fromPage;

    while (true) {
        printHeader(page, &p);
        printPage(page, &p, doc, body, pageNumberPos);
        printFooter(page, &p);

        if (page == toPage)
            break;

        if (ascending)
            ++page;
        else
            --page;

        if (!printer->newPage())
            return;
    }

}

/**
 * From QTextDocument
 * @param index
 * @param p
 * @param doc
 * @param body
 * @param pageNumberPos
 */
void RB_PrintTextHeaderFooter::printPage(int index, QPainter* p, const QTextDocument *doc,
                         const QRectF &body, const QPointF &pageNumberPos) {
    p->save();
    p->translate(body.left(), body.top() - (index - 1) * body.height());
    QRectF view(0, (index - 1) * body.height(), body.width(), body.height());

    // Debug only
    if (mIsDebug) {
        debugRectangle(p, view, QColor("#EEFFDD"));
    }

    QAbstractTextDocumentLayout *layout = doc->documentLayout();
    QAbstractTextDocumentLayout::PaintContext ctx;

    p->setClipRect(view);
    ctx.clip = view;

    // don't use the system palette text as default text color, on HP/UX
    // for example that's white, and white text on white paper doesn't
    // look that nice
    ctx.palette.setColor(QPalette::Text, Qt::black);

    layout->draw(p, ctx);

    if (!pageNumberPos.isNull()) {
        p->setClipping(false);
        p->setFont(QFont(doc->defaultFont()));
        const QString pageString = QString::number(index);

        p->drawText(qRound(pageNumberPos.x() - p->fontMetrics().width(pageString)),
                          qRound(pageNumberPos.y() + view.top()),
                          pageString);
    }

    p->restore();
}

void RB_PrintTextHeaderFooter::printHeader(int pageno, QPainter* p) {
    if (mDocHeaderHeight <= 0 || !mDocHeader) {
        return;
    }

    // Clone document
    QTextDocument* dH = mDocHeader;
    (void)dH->documentLayout(); // make sure that there is a layout
    QRectF bodyH = QRectF(QPointF(0, 0), dH->pageSize());

    QTextDocument* docH = dH->clone((dH));
    QScopedPointer<QTextDocument> clonedDocH; // make sure docF is deleted
    clonedDocH.reset((docH));

    // Copy additional formats
    for (QTextBlock srcBlock = dH->firstBlock(), dstBlock = clonedDocH->firstBlock();
         srcBlock.isValid() && dstBlock.isValid();
         srcBlock = srcBlock.next(), dstBlock = dstBlock.next()) {
        dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
    }

    // Text replacement in clone/copy
    QString str = docH->toHtml();
    str.replace("{$pagenumber}", QString::number(pageno), Qt::CaseInsensitive);
    str.replace("{$pagetotal}", QString::number(mPageTotal), Qt::CaseInsensitive);

    if (mHeaderReplaceMap.size() > 0) {
        std::map<QString, QString>::const_iterator iter;
        iter = mHeaderReplaceMap.begin();

        while (iter != mHeaderReplaceMap.end()) {
            str.replace(iter->first, iter->second, Qt::CaseInsensitive);
            ++iter;
        }
    }

    docH->setHtml(str);

    // Further layout preparation
    QAbstractTextDocumentLayout* layoutH = docH->documentLayout();
    layoutH->setPaintDevice(p->device());

    int margin = 0;
    QTextFrameFormat fmtH = docH->rootFrame()->frameFormat();
    fmtH.setMargin(margin);
    docH->rootFrame()->setFrameFormat(fmtH);

    int widthH = (mPrinter->pageSizeMM().width() - mMarginLeft
                 - mMarginRight) / 25.4 * mDpiy;
    int heightH = (mDocHeaderHeight) / 25.4 * mDpiy;

    if (mPrinter->orientation() == QPrinter::Landscape) {
        widthH = (mPrinter->pageSizeMM().height() - mMarginLeft
                  - mMarginRight) / 25.4 * mDpiy;
    }

    bodyH = QRectF(0, 0, widthH, heightH);
    clonedDocH->setPageSize(bodyH.size());

    // Painting
    p->save();
    p->translate(bodyH.left(), 0);
    QRectF view(0, 0, bodyH.width(), bodyH.height());

    // Debug only
    if (mIsDebug) {
        debugRectangle(p, view, QColor("#FFEEDD"));
    }

    QAbstractTextDocumentLayout* layout = docH->documentLayout();
    QAbstractTextDocumentLayout::PaintContext ctx;

    p->setClipRect(view);
    ctx.clip = view;

    // don't use the system palette text as default text color, on HP/UX
    // for example that's white, and white text on white paper doesn't
    // look that nice
    ctx.palette.setColor(QPalette::Text, Qt::black);

    layout->draw(p, ctx);
    p->restore();
}


void RB_PrintTextHeaderFooter::printFooter(int pageno, QPainter* p) {
    if (mDocFooterHeight <= 0 || !mDocFooter) {
        return;
    }

    // Clone document
    QTextDocument* dF = mDocFooter;
    (void)dF->documentLayout(); // make sure that there is a layout
    QRectF bodyF = QRectF(QPointF(0, 0), dF->pageSize());

    QTextDocument* docF = dF->clone((dF));
    QScopedPointer<QTextDocument> clonedDocF; // make sure docF is deleted
    clonedDocF.reset((docF));

    // Copy additional formats
    for (QTextBlock srcBlock = dF->firstBlock(), dstBlock = clonedDocF->firstBlock();
         srcBlock.isValid() && dstBlock.isValid();
         srcBlock = srcBlock.next(), dstBlock = dstBlock.next()) {
        dstBlock.layout()->setAdditionalFormats(srcBlock.layout()->additionalFormats());
    }

    // Text replacement in clone/copy
    QString str = docF->toHtml();
    str.replace("{$pagenumber}", QString::number(pageno), Qt::CaseInsensitive);
    str.replace("{$pagetotal}", QString::number(mPageTotal), Qt::CaseInsensitive);

    if (mFooterReplaceMap.size() > 0) {
        std::map<QString, QString>::const_iterator iter;
        iter = mFooterReplaceMap.begin();

        while (iter != mFooterReplaceMap.end()) {
            str.replace(iter->first, iter->second, Qt::CaseInsensitive);
            ++iter;
        }
    }

    docF->setHtml(str);

    QAbstractTextDocumentLayout* layoutF = docF->documentLayout();
    layoutF->setPaintDevice(p->device());

    int margin = 0;
    QTextFrameFormat fmtF = docF->rootFrame()->frameFormat();
    fmtF.setMargin(margin);
    docF->rootFrame()->setFrameFormat(fmtF);

    int widthF = (mPrinter->pageSizeMM().width() - mMarginLeft
                 - mMarginRight) / 25.4 * mDpiy;
    int heightF = mDocFooterHeight / 25.4 * mDpiy;
    int topF = mContentHeightDpi + mDocHeaderHeight / 25.4 * mDpiy;

    if (mOrientation == (int)QPrinter::Landscape) {
        widthF = (mPrinter->pageSizeMM().height() - mMarginLeft
                  - mMarginRight) / 25.4 * mDpiy;
    }

    bodyF = QRectF(0, topF, widthF, heightF);
    clonedDocF->setPageSize(bodyF.size());

    // Painting
    p->save();
    p->translate(bodyF.left(), bodyF.top());
    QRectF view(0, 0, bodyF.width(), bodyF.height());

    // Debug only
    if (mIsDebug) {
        debugRectangle(p, view, QColor("#DDEEFF"));
    }

    QAbstractTextDocumentLayout* layout = docF->documentLayout();
    QAbstractTextDocumentLayout::PaintContext ctx;

    p->setClipRect(view);
    ctx.clip = view;

    // don't use the system palette text as default text color, on HP/UX
    // for example that's white, and white text on white paper doesn't
    // look that nice
    ctx.palette.setColor(QPalette::Text, Qt::black);

    layout->draw(p, ctx);
    p->restore();
}

/**
 * Debug function only, to highlight the various page areas
 */
void RB_PrintTextHeaderFooter::debugRectangle(QPainter* p, const QRectF& rect,
                                              const QColor& color) {
    p->fillRect(rect, color);
    p->setPen(QPen(Qt::red));
    p->drawLine(-5, 0, 5, 0);
    p->drawLine(0, 5, 0, -5);
}
