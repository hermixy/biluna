/*****************************************************************
 * $Id: rb_printtextheaderfooter.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: 2013-12-19 rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_PRINTTEXTHEADERFOOTER_H
#define RB_PRINTTEXTHEADERFOOTER_H

#include <map>
#include <QPointF>
#include <Qprinter>
#include <QRectF>
#include <QScopedPointer>
#include "db_global.h"
#include "rb_string.h"

class QPainter;
class QTextDocument;

/**
 * Class that prints headers and footers to paged documents, and
 * replacement of text such as {$firstname} by firstname. The
 * {$pagenumber} and {$pagetotal} are reserved for printing the
 * page number and the total number of pages
 * Based on qtextdocument.cpp static void printPage() and void QTextDocument::print()
 * @brief The RB_PrintTextHeaderFooter class
 * \TODO needs to be refactored, lot of code duplication, header/footer/content
 */
class DB_EXPORT RB_PrintTextHeaderFooter {

public:
    explicit RB_PrintTextHeaderFooter();
    virtual ~RB_PrintTextHeaderFooter();

    virtual void print(QPrinter* printer);
    virtual void setPaperMargin(qreal marginLeft, qreal marginTop,
                                qreal marginRight, qreal marginBottom);
    virtual void setPrintPageNumbers(bool isPrintPageNumbers = true);
    virtual void setPageOrientation(QPrinter::Orientation orientation);

    virtual void setDocHeader(QTextDocument* textDoc, qreal height);
    virtual void setDocContent(QTextDocument* textDoc);
    virtual void setDocFooter(QTextDocument* textDoc, qreal height);

    virtual void addHeaderReplace(const RB_String& key, const RB_String& value);
    virtual void addContentReplace(const RB_String& key, const RB_String& value);
    virtual void addFooterReplace(const RB_String& key, const RB_String& value);

    virtual void clearHeaderReplace();
    virtual void clearContentReplace();
    virtual void clearFooterReplace();
    virtual void clearAllReplace();

    virtual void setIsDebug(bool isDebug = true) {
        mIsDebug = isDebug;
    }

private:
    void printDocument(QPrinter* printer);
    void printPage(int index, QPainter* painter, const QTextDocument* doc,
                   const QRectF& body, const QPointF& pageNumberPos);
    void printHeader(int pageno, QPainter* painter);
    void printFooter(int pageno, QPainter* p);

    // Debug only
    void debugRectangle(QPainter* p, const QRectF& rect, const QColor& color);
    bool mIsDebug;

    QTextDocument* mDocHeader;
    QTextDocument* mDocContent;
    QTextDocument* mDocFooter;

    qreal mDocHeaderHeight;
    qreal mDocFooterHeight;

    qreal mMarginLeft;
    qreal mMarginRight;
    qreal mMarginTop;
    qreal mMarginBottom;

    bool mIsPrintPageNumbers;
    int mPageTotal;

    QPrinter* mPrinter;
    int mOrientation; // 0 is portrait and 1 landscape
    int mDpiy;
    int mContentTopDpi;
    int mContentHeightDpi;
    int mContentWidthDpi;

    std::map<RB_String, RB_String> mHeaderReplaceMap;
    std::map<RB_String, RB_String> mContentReplaceMap;
    std::map<RB_String, RB_String> mFooterReplaceMap;
};

#endif // RB_PRINTTEXTHEADERFOOTER_H
