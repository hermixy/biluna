/*****************************************************************
 * $Id: rb_simplereportwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sept 26, 2012 9:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_SIMPLEREPORTWIDGET_H
#define RB_SIMPLEREPORTWIDGET_H

#include <QtSql>
#include "rb_widget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;
class RB_TextEdit;


/**
 * View report widget
 */
class DB_EXPORT RB_SimpleReportWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit RB_SimpleReportWidget(QWidget *parent = 0);
    virtual ~RB_SimpleReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "RB Simple Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editSelectAll();
    virtual void editInsertImage(const RB_String& fn);
    virtual void editZoomIn(double factor = 1.0);
    virtual void editZoomOut(double factor = -1.0);

    virtual void formatBold();
    virtual void formatItalic();
    virtual void formatUnderline();
    virtual void formatStrikethrough();
    virtual void formatAlignLeft();
    virtual void formatAlignCenter();
    virtual void formatAlignRight();
    virtual void formatAlignJustify();
    virtual void formatIncreaseIndent();
    virtual void formatDecreaseIndent();
    virtual void formatNumberedList();
    virtual void formatBulletedList();
    virtual void formatFontName();
    virtual void formatFontSize();
    virtual void formatTextColor();
    virtual void formatBackgroundColor();
    virtual void formatRemove();

    virtual bool saveFile(const RB_String &fn);


public slots:


protected:
    virtual void setReportTitle(const RB_String title) { mReportTitle = title; }
    virtual void setReportSubTitle(const RB_String subTitle) { mReportSubTitle = subTitle; }
    virtual void setReportLimit(const RB_String limit) { mReportLimit = limit; }

    /**
     * Set column width per column, total should be 100%
     * @param percent width in percentage
     */
    virtual void setColumnWidth(int percent) {
        mColumnWidthVector.push_back(percent);
        mColWidthVector.append(QTextLength(QTextLength::PercentageLength, percent));
    }
    /**
     * Clear column width when refreshing the report
     */
    virtual void clearColumnWidths() {
        mColumnWidthVector.clear();
        mColWidthVector.clear();
    }
    /**
     * Set the alternating color for each row for ease of reading,
     * default is true.
     * @param alternating true if alternating color is to be used
     */
    virtual void setAlternatingRow(bool alternating = true) {
        mIsAlternatingRow = alternating;
    }
    /**
     * Set the first data over the full first row,
     * default is false.
     * @param fullRow true if full row is to be used
     */
    virtual void setFirstDataFullRow(bool fullRow = true) {
        mIsFirstDataFullRow = fullRow;
    }

    virtual void createReport(RB_String& html, RB_ObjectContainer* list);
    virtual void createReport(RB_String& html, QSqlQuery &query);
    virtual void setReportHeader(RB_String& html);
    virtual void setTableHeader(RB_String& html, RB_ObjectBase* obj);
    virtual void setTableHeader(RB_String& html, const QSqlRecord& rec);
    virtual void setDataHeader(RB_String& html);
    virtual void setDataRow(RB_String& html, RB_ObjectBase* obj);
    virtual void setDataRow(RB_String& html, const QSqlRecord& rec);
    virtual void setDataFooter(RB_String& html);
    virtual void setTableFooter(RB_String& html);
    virtual void setReportFooter(RB_String& html);

    virtual void setColumnProperties(int dataFieldCount);
    virtual void setColumnName(RB_String& html, const RB_String& fieldName,
                               int col);
    virtual void setColumnData(RB_String& html, const RB_String& colData,
                               int col);

    RB_TextEdit* mTeReport;

    // Report
    int mReportColCount;
    RB_String mReportTitle;
    RB_String mReportSubTitle;
    RB_String mReportLimit;

    std::vector<int> mColumnWidthVector;
    QVector<QTextLength> mColWidthVector;
    int mMemCount;
    int mNoDataRow;
    bool mEvenRow;
    bool mIsAlternatingRow;
    bool mIsFirstDataFullRow;
    int mColRunner;
    int mRowRunner;

    RB_String mOnePixelTransparentImage;
};

#endif // RB_SIMPLEREPORTWIDGET_H
