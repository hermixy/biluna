/*****************************************************************
 * $Id: srm_printtext.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: 2012-03-19 rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_PRINTTEXT_H
#define SRM_PRINTTEXT_H

#include <QPrinter>
#include <QStringList>
#include <QTextDocument>
#include <QWidget>

class RB_ObjectBase;


/**
 * Operation for printing formatted text with header/footer database fields
 * @todo copied from rb_textpreview, refactor code
 */
class SRM_PrintText {

public:
    explicit SRM_PrintText();
    virtual ~SRM_PrintText();

    virtual void print(QPrinter* pr);
    virtual void setData(RB_ObjectBase* doc, RB_ObjectBase* target);

private:
    void printText(QPainter* p);
    void replaceFields();
    void setField(QTextDocument* textDoc, const QString& fieldName,
                  const QString& fieldValue);
    // Debug only
    void debugRectangle(QPainter* p, const QRectF& rect, const QColor& color);

    QPrinter* mPrinter;
    RB_ObjectBase* mDocument;
    RB_ObjectBase* mTarget;

    //! Number of pages
    int mPageCount;

    QSizeF mPaperSize;

    QTextDocument mPageHeader;
    QTextDocument mDocContent;
    QTextDocument mPageFooter;

    qreal mMarginLeft;
    qreal mMarginRight;
    qreal mMarginTop;
    qreal mMarginBottom;
    qreal mPageHeaderHeight;
    qreal mPageFooterHeight;

    QSizeF mPageSizeCurrent;
    qreal mMarginLeftCurrent;
    qreal mMarginRightCurrent;
    qreal mMarginTopCurrent;
    qreal mMarginBottomCurrent;
};

#endif // SRM_PRINTTEXT_H
