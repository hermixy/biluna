/*****************************************************************
 * $Id: rb_textpreview.h 1766 2012-09-05 10:02:08Z rutger $
 * Created: 2012-03-07 rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TEXTPREVIEW_H
#define RB_TEXTPREVIEW_H

#include <QPrinter>
#include <QStringList>
#include <QWidget>

class QTextBlock;
class QTextDocument;
class QTextFrame;

/**
 * Preview (widget) for formatted text on selected paper size
 */
class RB_TextPreview : public QWidget {

    Q_OBJECT

public:
    explicit RB_TextPreview(QWidget *parent = 0);
    virtual ~RB_TextPreview() { }

    virtual void print(QPrinter* pr);

    virtual QStringList getPaperSizeList();
    virtual QPrinter::PaperSize getPaperSizeEnum(const QString& paperSize);
    virtual QSizeF getPaperSize(QPrinter::PaperSize size);
    virtual void setPaperMargin(qreal marginLeft, qreal marginRight,
                                qreal marginTop, qreal marginBottom);

    virtual void setPageHeader(QTextDocument* textDoc, qreal height);
    virtual void setDocContent(QTextDocument* textDoc);
    virtual void setPageFooter(QTextDocument* textDoc, qreal height);

    virtual QSize sizeHint() const;

signals:


public slots:
    virtual void slotSetPaperSize(QPrinter::PaperSize size);

protected:
    virtual void wheelEvent(QWheelEvent* e);


protected slots:
    virtual void paintEvent(QPaintEvent* e);

private:
    void paintText(QPainter* p);
    void printText(QPainter* p);
    void scaleWidget(double factor);

    // Debug only
    void debugText();
    void debugRectangle(QPainter* p, const QRectF& rect, const QColor& color);

    QPrinter* mPrinter;
    double mDpiX;

    //! Number of pages
    int mPageCount;

    qreal mMargin;
    qreal mWidgetWidth, mWidgetHeight;
    qreal mScaleFactor;
    QSizeF mPaperSize;
    QPrinter::PaperSize mPaperSizeEnum;

    QTextDocument* mPageHeader;
    QTextDocument* mDocContent;
    QTextDocument* mPageFooter;
    QTextDocument* mPageFooterNumber;

    qreal mMarginLeft;
    qreal mMarginRight;
    qreal mMarginTop;
    qreal mMarginBottom;
    qreal mPageHeaderHeight;
    qreal mPageFooterHeight;

    qreal mScaleFactorCurrent;
    QSizeF mPaperSizeCurrent;
    QSizeF mPageSizeCurrent;
    qreal mMarginLeftCurrent;
    qreal mMarginRightCurrent;
    qreal mMarginTopCurrent;
    qreal mMarginBottomCurrent;


};

#endif // RB_TEXTPREVIEW_H
