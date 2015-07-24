/*****************************************************************
 * $Id: rb_textpreview.cpp 1965 2013-08-08 16:33:15Z rutger $
 * Created: 2011-05-02 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_textpreview.h"

#include <cmath>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>
#include <QWheelEvent>


/**
 * Constructor
 */
RB_TextPreview::RB_TextPreview(QWidget *parent) : QWidget(parent) {

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mDpiX = QApplication::desktop()->physicalDpiX();
    // int dpiY = QApplication::desktop()->physicalDpiY();

    mScaleFactor = 1.0;
    mMargin = 5.0; // mm

    mPaperSize = QSizeF(210.0, 297.0); // mm
    mPageCount = 1;

    mWidgetWidth = mPaperSize.width() + 2 * mMargin; // mm
    mWidgetHeight = mPaperSize.height() + 2 * mMargin; // mm

    mMarginLeft = 20.0;
    mMarginRight = 15.0;
    mMarginTop = 15.0;
    mMarginBottom = 15.0;

    mPageHeader = NULL;
    mDocContent = NULL;
    mPageFooter = NULL;
    mPageFooterNumber = NULL;
    mPrinter = NULL;

    // debugText();

}

/**
 * Print the document
 * @param pr printer
 */
void RB_TextPreview::print(QPrinter* pr) {
    mPrinter = pr;
    QPainter p(pr);
    printText(&p);
    mPrinter = NULL;
}



/**
 * @returns papersizes from QPrinter as in Qt 4.8
 */
QStringList RB_TextPreview::getPaperSizeList() {
    QStringList items;
    items << "A0" << "A1" << "A2" << "A3" << "A4"
          << "A5" << "A6" << "A7" << "A8" << "A9"
          << "B0" << "B1" << "B2" << "B3" << "B4"
          << "B5" << "B6" << "B7" << "B8" << "B9" << "B10"
          << "C5E" << "Comm10E" << "DLE" << "Executive"
          << "Folio" << "Ledger" << "Legal" << "Letter"
          << "Tabloid"; // << "Custom";
    return items;
}

/**
 * @param paperSize paper size as text, from example A4 or Letter
 * @returns QPrinter::PaperSize
 */
QPrinter::PaperSize RB_TextPreview::getPaperSizeEnum(const QString& paperSize) {
    if (paperSize == "A0") {
        return QPrinter::A0;        //	5	841 x 1189 mm
    } else if (paperSize == "A1") {
        return QPrinter::A1;        //	6	594 x 841 mm
    } else if (paperSize == "A2") {
        return QPrinter::A2;        //	7	420 x 594 mm
    } else if (paperSize == "A3") {
        return QPrinter::A3;        //	8	297 x 420 mm
    } else if (paperSize == "A4") {
        return QPrinter::A4;        //	0	210 x 297 mm, 8.26 x 11.69 inches
    } else if (paperSize == "A5") {
        return QPrinter::A5;        //	9	148 x 210 mm
    } else if (paperSize == "A6") {
        return QPrinter::A6;        //	10	105 x 148 mm
    } else if (paperSize == "A7") {
        return QPrinter::A7;        //	11	74 x 105 mm
    } else if (paperSize == "A8") {
        return QPrinter::A8;        //	12	52 x 74 mm
    } else if (paperSize == "A9") {
        return QPrinter::A9;        //	13	37 x 52 mm
    } else if (paperSize == "B0") {
        return QPrinter::B0;        //	14	1000 x 1414 mm
    } else if (paperSize == "B1") {
        return QPrinter::B1;        //	15	707 x 1000 mm
    } else if (paperSize == "B2") {
        return QPrinter::B2;        //	17	500 x 707 mm
    } else if (paperSize == "B3") {
        return QPrinter::B3;        //	18	353 x 500 mm
    } else if (paperSize == "B4") {
        return QPrinter::B4;        //	19	250 x 353 mm
    } else if (paperSize == "B5") {
        return QPrinter::B5;        //	1	176 x 250 mm, 6.93 x 9.84 inches
    } else if (paperSize == "B6") {
        return QPrinter::B6;        //	20	125 x 176 mm
    } else if (paperSize == "B7") {
        return QPrinter::B7;        //	21	88 x 125 mm
    } else if (paperSize == "B8") {
        return QPrinter::B8;        //	22	62 x 88 mm
    } else if (paperSize == "B9") {
        return QPrinter::B9;        //	23	33 x 62 mm
    } else if (paperSize == "B10") {
        return QPrinter::B10;       //	16	31 x 44 mm
    } else if (paperSize == "C5E") {
        return QPrinter::C5E;       //	24	163 x 229 mm
    } else if (paperSize == "Comm10E") {
        return QPrinter::Comm10E;   //	25	105 x 241 mm, U.S. Common 10 Envelope
    } else if (paperSize == "DLE") {
        return QPrinter::DLE;       //	26	110 x 220 mm
    } else if (paperSize == "Executive") {
        return QPrinter::Executive; //	4	7.5 x 10 inches, 190.5 x 254 mm
    } else if (paperSize == "Folio") {
        return QPrinter::Folio;     //	27	210 x 330 mm
    } else if (paperSize == "Ledger") {
        return QPrinter::Ledger;    //	28	431.8 x 279.4 mm
    } else if (paperSize == "Legal") {
        return QPrinter::Legal;     //	3	8.5 x 14 inches, 215.9 x 355.6 mm
    } else if (paperSize == "Letter") {
        return QPrinter::Letter;    //	2	8.5 x 11 inches, 215.9 x 279.4 mm
    } else if (paperSize == "Tabloid") {
        return QPrinter::Tabloid;   //	29	279.4 x 431.8 mm
    }

    return QPrinter::Custom; //	30	Unknown, or a user defined size.
}

QSizeF RB_TextPreview::getPaperSize(QPrinter::PaperSize size) {
    if (size == QPrinter::A0) {
        return QSizeF(841, 1189); //  mm
    } else if (size == QPrinter::A1) {
        return QSizeF(594, 841); //  mm
    } else if (size == QPrinter::A2) {
        return QSizeF(420, 594); // mm
    } else if (size == QPrinter::A3) {
        return QSizeF(297, 420); // mm
    } else if (size == QPrinter::A4) {
        return QSizeF(210, 297); // mm, 8.26, 11.69 inches
    } else if (size == QPrinter::A5) {
        return QSizeF(148, 210); // mm
    } else if (size == QPrinter::A6) {
        return QSizeF(105, 148); // mm
    } else if (size == QPrinter::A7) {
        return QSizeF(74, 105); // mm
    } else if (size == QPrinter::A8) {
        return QSizeF(52, 74); // mm
    } else if (size == QPrinter::A9) {
        return QSizeF(37, 52); // mm
    } else if (size == QPrinter::B0) {
        return QSizeF(1000, 1414); // mm
    } else if (size == QPrinter::B1) {
        return QSizeF(707, 1000); // mm
    } else if (size == QPrinter::B2) {
        return QSizeF(500, 707); // mm
    } else if (size == QPrinter::B3) {
        return QSizeF(353, 500); // mm
    } else if (size == QPrinter::B4) {
        return QSizeF(250, 353); // mm
    } else if (size == QPrinter::B5) {
        return QSizeF(176, 250); // mm, 6.93, 9.84 inches
    } else if (size == QPrinter::B6) {
        return QSizeF(125, 176); // mm
    } else if (size == QPrinter::B7) {
        return QSizeF(88, 125); // mm
    } else if (size == QPrinter::B8) {
        return QSizeF(62, 88); // mm
    } else if (size == QPrinter::B9) {
        return QSizeF(33, 62); // mm
    } else if (size == QPrinter::B10) {
        return QSizeF(31, 44); // mm
    } else if (size == QPrinter::C5E) {
        return QSizeF(163, 229); // mm
    } else if (size == QPrinter::Comm10E) {
        return QSizeF(105, 241); // mm, U.S. Common 10 Envelope
    } else if (size == QPrinter::DLE) {
        return QSizeF(110, 220); // mm
    } else if (size == QPrinter::Executive) {
        return QSizeF(190.5, 254); // mm 7.5, 10 inches
    } else if (size == QPrinter::Folio) {
        return QSizeF(210, 330); // mm
    } else if (size == QPrinter::Ledger) {
        return QSizeF(431.8, 279.4); // mm
    } else if (size == QPrinter::Legal) {
        return QSizeF(215.9, 355.6); // mm	8.5, 14 inches
    } else if (size == QPrinter::Letter) {
        return QSizeF(215.9, 279.4); // mm 8.5, 11 inches
    } else if (size == QPrinter::Tabloid) {
        return QSizeF(279.4, 431.8); // mm
    }

    return QSize(); //	30	Unknown, or a user defined size.

}

/**
 * Set paper margins from the edge of the paper to starting of the text
 * @param marginLeft
 * @param marginRight
 * @param marginTop
 * @param marginBottom
 */
void RB_TextPreview::setPaperMargin(qreal marginLeft, qreal marginRight,
                                    qreal marginTop, qreal marginBottom) {
    mMarginLeft = marginLeft;
    mMarginRight = marginRight;
    mMarginTop = marginTop;
    mMarginBottom = marginBottom;
}

/**
 * Set page header text and height
 * @param textDoc text document
 * @param height height of the section in millimeters
 */
void RB_TextPreview::setPageHeader(QTextDocument* textDoc, qreal height) {
    if (!mPageHeader) {
        mPageHeader = new QTextDocument(this);
    }
    mPageHeader->setHtml(textDoc->toHtml());
    mPageHeaderHeight = height;
}

/**
 * Set document text
 * @param textDoc text document
 */
void RB_TextPreview::setDocContent(QTextDocument* textDoc) {
    if (!mDocContent) {
        mDocContent = new QTextDocument(this);
    }
    mDocContent->setHtml(textDoc->toHtml());
}

/**
 * Set page footer text and height
 * @param textDoc text document
 * @param height height of the section in millimeters
 */
void RB_TextPreview::setPageFooter(QTextDocument* textDoc, qreal height) {
    if (!mPageFooter) {
        mPageFooter = new QTextDocument(this);
    }
    mPageFooter->setHtml(textDoc->toHtml());
    mPageFooterHeight = height;
}

void RB_TextPreview::slotSetPaperSize(QPrinter::PaperSize size) {
    mPaperSizeEnum = size;
    mPaperSize = getPaperSize(size);

    mWidgetWidth = mPaperSize.width() + 2 * mMargin; // mm
    mWidgetHeight = mPaperSize.height() + 2 * mMargin; // mm

    update();
}

/**
 * @param e wheel event
 */
void RB_TextPreview::wheelEvent(QWheelEvent* e) {
    scaleWidget(1 / (pow((double)2, - e->delta() / 240.0)) );

    // force update of parent by call this resize
    QWidget* p = dynamic_cast<QWidget*>(parent());
    if (p) {
        resize(p->width(), p->height());
    }

    e->accept();
}

/**
 * @param e paint event
 */
void RB_TextPreview::paintEvent(QPaintEvent* /*e*/) {
    QPainter p;
    paintText(&p);
}

/**
 * Paint the text
 * \todo Now also works for the printer, can be removed after printText() is OK
 * @param p painter
 */
void RB_TextPreview::paintText(QPainter* p) {
    if (!p) {
        return;
    }

    // scale text dpi to mm, mDpiX is usually 96
    double dpiToMM = 25.4 / ((double)mDpiX);
    double mmToDpi = ((double)mDpiX) / 25.4;
    double mmToDoc = 1.0;

    if (mPrinter) {
        dpiToMM = 1.0;
        mmToDpi = 1.0;
        mmToDoc = mPrinter->paperSize(QPrinter::DevicePixel).height()
                / mPrinter->paperSize(QPrinter::Millimeter).height();

        mScaleFactorCurrent = 1.0;
        mMargin = 0.0;

        mPaperSizeCurrent = QSizeF(mPrinter->pageRect().width(),
                            mPrinter->pageRect().height());
        mMarginLeftCurrent = 0.0;
        mMarginRightCurrent = 0.0;
        mMarginTopCurrent = 0.0;
        mMarginBottomCurrent = 0.0;
    } else {
        mScaleFactorCurrent = mScaleFactor;
        mMargin = 5.0; // mm

        mPaperSizeCurrent = mPaperSize;
        mMarginLeftCurrent = mMarginLeft;
        mMarginRightCurrent = mMarginRight;
        mMarginTopCurrent = mMarginTop;
        mMarginBottomCurrent = mMarginBottom;
    }

    mWidgetWidth = mPaperSizeCurrent.width() + 2 * mMargin; // mm
    mWidgetHeight = mPaperSizeCurrent.height() + 2 * mMargin; // mm

    // Printable rectangle on paper within print margins
    QRectF printRect;
    printRect.setLeft(0);
    printRect.setTop(0);
    printRect.setRight((mPaperSizeCurrent.width() - mMarginLeftCurrent
                        - mMarginRightCurrent) * mmToDpi);
    printRect.setBottom((mPaperSizeCurrent.height() - mMarginTopCurrent
                         - mMarginBottomCurrent) * mmToDpi);

    mPageHeader->setUseDesignMetrics(true);
    mPageHeader->setTextWidth(printRect.width());
    mDocContent->setUseDesignMetrics(true);
    mDocContent->setPageSize(printRect.size());
    mPageFooter->setUseDesignMetrics(true);
    mPageFooter->setTextWidth(printRect.width());

    // Printable document content text on paper minus page header and footer,
    // if applicable. Below is calculated in mm.
    QRectF contentRect = QRectF(QPointF(0.0, 0.0),  mDocContent->size().toSize());

    // Now printable area calculated in DPI, left and top are already 0
    QRectF innerRect = printRect;
    innerRect.setTop(printRect.top() + mPageHeaderHeight * mmToDpi * mmToDoc);
    innerRect.setBottom(printRect.height() - mPageFooterHeight * mmToDpi * mmToDoc);
    QRectF currentRect = QRectF(QPointF(0.0, 0.0),  innerRect.size());

    // Calculate number of pages and reset pageSize
    mDocContent->setPageSize(currentRect.size());
    mPageCount = mDocContent->pageCount();

    p->begin(this);
    p->setRenderHint(QPainter::Antialiasing);

    // Paint background including margin, if not printer
    for (int i = 0; i < mPageCount && !mPrinter; ++i) {
        p->save();

        p->translate(0.0, mWidgetHeight * mScaleFactorCurrent * i);
        p->scale(mScaleFactorCurrent, mScaleFactorCurrent);
        p->fillRect(QRectF(0.0, 0.0, mWidgetWidth, mWidgetHeight),
                         QColor(Qt::gray));

        // Move painter to paper origine and paint paper outline
        p->translate(mMargin, mMargin);
        p->setPen(QPen(Qt::black));
        p->setBrush(QBrush(Qt::white));
        p->drawRect(0, 0, mPaperSizeCurrent.width(), mPaperSizeCurrent.height());

        p->restore(); // at origine of widget = page origine - margin
    }

    qreal currentX = 0.0; // screen page mm
    qreal currentY = 0.0; // screen page mm
    int pageCounter = 0;

    while (currentRect.intersects(contentRect)) {
        currentX += mMargin + mMarginLeftCurrent;
        currentY += mMargin + mMarginTopCurrent;

        // Paint page header
        p->save();
        p->translate(currentX * mScaleFactorCurrent, currentY * mScaleFactorCurrent);
        p->scale(mScaleFactorCurrent * dpiToMM, mScaleFactorCurrent * dpiToMM);
        p->setFont(mPageHeader->defaultFont());
        // No print rect assumes that headers/footers fit within area
        mPageHeader->drawContents(p);
        p->restore();

        if (!mPrinter) {
            currentY += mPageHeaderHeight - currentRect.height()
                    * dpiToMM * pageCounter;
        } else {
            currentY = mPageHeaderHeight * mmToDoc
                    - currentRect.height() * pageCounter;
        }

        // Paint document content, compensated for currentRect top translation
        // and as fits on this page
        p->save();
        p->translate(currentX * mScaleFactorCurrent, currentY * mScaleFactorCurrent);
        p->scale(mScaleFactorCurrent * dpiToMM, mScaleFactorCurrent * dpiToMM);
        p->setFont(mDocContent->defaultFont());
// debugRectangle(p, currentRect, QColor("#DDEEFF"));
        mDocContent->drawContents(p, currentRect);
        p->restore();

        currentRect.translate(0, currentRect.height());
        ++pageCounter;

        if (!mPrinter) {
            currentY = mWidgetHeight * pageCounter - mMargin
                    - mMarginBottomCurrent - mPageFooterHeight;
        } else {
            currentY = mWidgetHeight - (mMargin + mMarginBottomCurrent
                    + mPageFooterHeight) * mmToDoc;
        }

        // Format page footer
        if (!mPageFooterNumber) {
            mPageFooterNumber = new QTextDocument(this);
        }

        if (pageCounter == 1) {
            mPageFooterNumber->setUseDesignMetrics(true);
            mPageFooterNumber->setTextWidth(printRect.width());
        }

        QString str = mPageFooter->toHtml();
        str.replace("{$pagenumberoftotal}", QString::number(pageCounter) + " of "
                    + QString::number(mPageCount), Qt::CaseInsensitive);
        str.replace("{$pagenumber}", QString::number(pageCounter),
                    Qt::CaseInsensitive);
        mPageFooterNumber->setHtml(str);

        // Paint page footer
        p->save();
        p->translate(currentX * mScaleFactorCurrent, currentY * mScaleFactorCurrent);
        p->scale(mScaleFactorCurrent * dpiToMM, mScaleFactorCurrent * dpiToMM);
        p->setFont(mPageFooterNumber->defaultFont());
        mPageFooterNumber->drawContents(p);
        p->restore();

        // Set X and Y to origine (left-top) of new page
        currentX = 0.0;
        currentY = mWidgetHeight * pageCounter;

        if (mPrinter && pageCounter < mPageCount ) {
            mPrinter->newPage();
            currentY = 0.0;
        }
    }

    p->end();

    if (mPrinter) {
        mPrinter = NULL;
    }
}

/**
 * Print the text
 * @param p painter
 */
void RB_TextPreview::printText(QPainter* p) {
    if (!mPrinter || !p) {
        return;
    }

    // scale text dpi to mm, mDpiX is usually 96
//    double mmToDoc = mPrinter->paperSize(QPrinter::DevicePixel).height()
//            / mPrinter->paperSize(QPrinter::Millimeter).height();
//    mPageSizeCurrent = QSizeF(mPrinter->pageRect(QPrinter::DevicePixel).width(),
//                        mPrinter->pageRect(QPrinter::DevicePixel).height());
//    mPageSizeCurrent = QSizeF(mPrinter->pageRect(QPrinter::Millimeter).width(),
//                              mPrinter->pageRect(QPrinter::Millimeter).height());

    double mmToDoc = ((double)this->logicalDpiX()) / 25.4; // mm to document logical DPI
    double sourceDpiX = ((double)mPrinter->logicalDpiX() / ((double)this->logicalDpiX()));
    double sourceDpiY = ((double)mPrinter->logicalDpiY() / ((double)this->logicalDpiY()));
    mPageSizeCurrent = QSizeF(mPrinter->pageRect(QPrinter::DevicePixel).width() / sourceDpiX,
                              mPrinter->pageRect(QPrinter::DevicePixel).height() / sourceDpiY);

//    int printerRes = mPrinter->resolution(); // dpi
//    double heightInPoint = mPrinter->pageRect(QPrinter::Point).height();
//    double heightInDevPx = mPrinter->pageRect(QPrinter::DevicePixel).height();
//    int physDpiX = mPrinter->physicalDpiX(); // printer 600, PDF 1200
//    int physDpiY = mPrinter->physicalDpiY(); // printer 600, PDF 1200
//    int logDpiX = mPrinter->logicalDpiX(); // printer 600, PDF 1200
//    int logDpiY = mPrinter->logicalDpiY(); // printer 600, PDF 1200
//    int logDpiXte = this->logicalDpiX(); // 96
//    int logDpiYte = this->logicalDpiY(); // 96

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

//    double scaleFactor = mPrinter->pageRect(QPrinter::DevicePixel).height()
//            / mPrinter->pageRect(QPrinter::Point).height();
    mPageHeader->setUseDesignMetrics(true);
    mPageHeader->setTextWidth(printRect.width());
    mPageHeader->documentLayout();
    mDocContent->setUseDesignMetrics(true);
    mDocContent->setPageSize(innerRect.size());
    mDocContent->documentLayout();
    mPageFooter->setUseDesignMetrics(true);
    mPageFooter->setTextWidth(printRect.width());
    mPageFooter->documentLayout();

    // Printable document content text on paper minus page header and footer,
    // if applicable. Below is calculated in mm.
    QRectF contentRect = QRectF(QPointF(0.0, 0.0),  mDocContent->size().toSize());

    // Start content rectangle
    QRectF currentRect = QRectF(QPointF(0.0, 0.0),  innerRect.size());

    // Calculate number of pages and reset pageSize
    mDocContent->setPageSize(currentRect.size());
    mPageCount = mDocContent->pageCount();

    // p->begin(this);
    p->setRenderHint(QPainter::Antialiasing);
    p->scale(sourceDpiX, sourceDpiY);
// debugRectangle(p, printRect, QColor("#DDEEFF"));
    qreal currentX = 0.0; // screen page mm
    qreal currentY = 0.0; // screen page mm
    int pageCounter = 0;

    while (currentRect.intersects(contentRect)) {
        // Paint page header
        p->save();
        p->translate(currentX, currentY);
        // No print rect, it assumes that headers/footers fit within area
        mPageHeader->drawContents(p);
        p->restore();

        currentY = mPageHeaderHeight * mmToDoc - currentRect.height() * pageCounter;

        // Paint document content, compensated for currentRect top translation
        // and as fits on this page
        p->save();
        p->translate(currentX, currentY);
// debugRectangle(p, currentRect, QColor("#DDEEFF"));
        mDocContent->drawContents(p, currentRect);
        p->restore();

        currentRect.translate(0, currentRect.height());
        ++pageCounter;

        currentY = mPageSizeCurrent.height() - mPageFooterHeight * mmToDoc;

        // Format page footer
        if (!mPageFooterNumber) {
            mPageFooterNumber = new QTextDocument(this);
        }

        if (pageCounter == 1) {
            mPageFooterNumber->setUseDesignMetrics(true);
            mPageFooterNumber->setTextWidth(printRect.width());
        }

        QString str = mPageFooter->toHtml();
        str.replace("{$pagenumberoftotal}", QString::number(pageCounter) + " of "
                    + QString::number(mPageCount), Qt::CaseInsensitive);
        str.replace("{$pagenumber}", QString::number(pageCounter),
                    Qt::CaseInsensitive);
        mPageFooterNumber->setHtml(str);

        // Paint page footer
        p->save();
        p->translate(currentX, currentY);
        mPageFooterNumber->drawContents(p);
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
 * Scale this widget for zoom in or -out based on wheel event
 * @param factor incremental scale factor
 */
void RB_TextPreview::scaleWidget(double factor) {
    double totalScaleFactor = mScaleFactor * factor;

    if (totalScaleFactor < 0.36 || totalScaleFactor > 28) {
        return;
    }

    mScaleFactor = totalScaleFactor;
    update();
}

/**
 * Debug only, create text documents with text and header/footer heights
 */
void RB_TextPreview::debugText() {
    mPageHeader = new QTextDocument(this);
    mPageHeader->setHtml("<b>Red-Bag</b><br/>Engineers Consultants Software");
    mDocContent = new QTextDocument(this);
    mDocContent->setHtml("<p  STYLE='font-size: 12pt;  font-family: courier;'><i><b>Document CONTENT and some more text why is this broken in more lines than necessary. "
                         "This should wrap as is <span STYLE='color:green;'>part</span> of the printable page.</b></i></p> "
                         "<p align='right' STYLE='font-size: 12pt;'>1 Some paragraphs to test the page</p>"
                         "<p align='right'>2 Some paragraphs to test the page</p>"
                         "<p style='background:#80BFFF'>3 Some paragraphs to test the page and this is again a long line to test the wrapping of the text. "
                         " Before this line a new line was started in the C++ source code but that should not matter to much.</p>"
                         "<p style='background:#80BFFF'>4 Some paragraphs to test the page</p>"
                         "<p>5 Some paragraphs to test the page</p>"
                         "<table border='1'><tbody><tr><td width='50%'>col 1</td><td width='50%'>col 2</td></tr>"
                            "<tr><td>Rutger</td><td>Botermans</td></tr></tbody></table>"
                         "<ol><li>Gudula</li><li>van Nispen</li><li>Beatrijs</li><li>Voorneman</li></ol>"
                         "<p>6 Some paragraphs to test the page</p>"
                         "<p>7 Some paragraphs to test the page</p>"
                         "<p>8 Some paragraphs to test the page</p>"
                         "<p>9 Some paragraphs to test the page</p>"
                         "<p>10 Some paragraphs to test the page</p>"
                         "<p>11 Some paragraphs to test the page</p>"
                         "<p>12 Some paragraphs to test the page</p>"
                         "<p>13 Some paragraphs to test the page</p>"
                         "<p>14 Some paragraphs to test the page</p>"
                         "<p>15 Some paragraphs to test the page</p>"
                         "<p>16 Some paragraphs to test the page</p>"
                         "<p>17 Some paragraphs to test the page</p>"
                         "<p>18 Some paragraphs to test the page</p>"
                         "<p>19 Some paragraphs to test the page</p>"
                         "<p>20 Some paragraphs to test the page</p>"
                         "<p>21 Some paragraphs to test the page<br/>Rutger Botermans :-)</p>"
                         "<p>22 Some paragraphs to test the page</p>"
                         "<p>23 Some paragraphs to test the page</p>"
                         "<p>24 Some paragraphs to test the page</p>"
                         "<p>25 Some paragraphs to test the page</p>"
                         "<p>26 Some paragraphs to test the page</p>"
                         "<p>27 Some paragraphs to test the page</p>"
                         "<p>28 Some paragraphs to test the page</p>"
                         "<p>29 Some paragraphs to test the page</p>"
                         "<p>30 Some paragraphs to test the page</p>"
                         "<p>31 Some paragraphs to test the page</p>"
                         "<p>32 Some paragraphs to test the page</p>"
                         "<p>33 Some paragraphs to test the page</p>"
                         "<p>34 Some paragraphs to test the page</p>"
                         "<p>35 Some paragraphs to test the page</p>"
                         "<p>36 Some paragraphs to test the page</p>"
                         "<p>37 Some paragraphs to test the page</p>"
                         "<p>38 Some paragraphs to test the page</p>"
                         "<p>39 Some paragraphs to test the page<br/>Rutger Rutger :-)<br/>Rutger Rutger :-)</p>"
                         "<p>40 Some paragraphs to test the page</p>"
                         "<p>41 Some paragraphs to test the page</p>"
                         "<p>42 Some paragraphs to test the page</p>"
                         "<p>43 Some paragraphs to test the page</p>"
                         "<p>44 Some paragraphs to test the page</p>"
                         "<p>45 Some paragraphs to test the page</p>"
                         "<p>46 Some paragraphs to test the page</p>"
                         "<p>47 Some paragraphs to test the page</p>"
                         "<p>48 Some paragraphs to test the page</p>"
                         "<p>49 Some paragraphs to test the page</p>"
                         "<p>50 Some paragraphs to test the page</p>"
                         "<p>51 Some paragraphs to test the page</p>"
                         "<p>52 Some paragraphs to test the page</p>"
                         "<p>53 Some paragraphs to test the page</p>"
                         "<p>54 Some paragraphs to test the page</p>"
                         "<p>55 Some paragraphs to test the page</p>"
                         "<p>56 Some paragraphs to test the page</p>"
                         "<p>57 Some paragraphs to test the page</p>"
                         "<p>58 Some paragraphs to test the page</p>"
                         "<p>59 Some paragraphs to test the page</p>"
                         "<p>60 Some paragraphs to test the page</p>"
                         "<p>61 Some paragraphs to test the page</p>"
                         "<p>62 Some paragraphs to test the page</p>"
                         "<p>63 Some paragraphs to test the page</p>"
                         "<p>64 Some paragraphs to test the page</p>"
                         "<p>65 Some paragraphs to test the page</p>"
                         "<p>66 Some paragraphs to test the page</p>"
                         "<p>67 Some paragraphs to test the page</p>"
                         "<p>68 Some paragraphs to test the page</p>"
                         "<p>69 Some paragraphs to test the page</p>"
                         "<p>70 Some paragraphs to test the page</p>"
                         "<p>71 Some paragraphs to test the page</p>"
                         "<p>72 Some paragraphs to test the page</p>"
                         "<p>73 Some paragraphs to test the page</p>"
                         "<p>74 Some paragraphs to test the page</p>"
                         "<p>75 Some paragraphs to test the page</p>"
                         "<p>76 Some paragraphs to test the page</p>"
                         "<p>77 Some paragraphs to test the page</p>"
                         "<p>78 Some paragraphs to test the page</p>"
                         "<p>79 Some paragraphs to test the page</p>"
                         "<p>80 Some paragraphs to test the page</p>"
                         "<p>81 Some paragraphs to test the page</p>");
    mPageFooter = new QTextDocument(this);
    mPageFooter->setHtml("Page FOOTER ggg {$pagenumberoftotal}");

    mPageHeaderHeight = 40;
    mPageFooterHeight = 60;
}

/**
 * Debug function only, to highlight the various page areas
 */
void RB_TextPreview::debugRectangle(QPainter *p, const QRectF &rect,
                                    const QColor &color) {
    p->fillRect(rect, color);
    p->setPen(QPen(Qt::red));
    p->drawLine(-5, 0, 5, 0);
    p->drawLine(0, 5, 0, -5);
}

QSize RB_TextPreview::sizeHint() const {
    return QSize(int((mWidgetWidth) * mScaleFactor),
                 int((mWidgetHeight) * mScaleFactor * mPageCount));
}


