/*****************************************************************
 * $Id: srm_reportwidget.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Jun 1, 2012 9:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_REPORTWIDGET_H
#define SRM_REPORTWIDGET_H

#include <QtSql>
#include "rb_widget.h"


class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View report widget
 */
class SRM_ReportWidget : public RB_Widget {

    Q_OBJECT

public:
    explicit SRM_ReportWidget(QWidget *parent = 0);
    virtual ~SRM_ReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "SRM Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual bool saveFile(const RB_String &fn);

    virtual void setReportTitle(const RB_String title) { mReportTitle = title; }
    virtual void setCompanyName(const RB_String coyName) { mCompanyName = coyName; }
    virtual void setReportLimit(const RB_String limit) { mReportLimit = limit; }
    virtual void setDataList(RB_ObjectContainer* dataList) { mDataList = dataList; }

    /**
     * Set column width per column, total should be 100%
     * @param percent width in percentage
     */
    virtual void setColumnWidth(double percent) {
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


public slots:


protected:
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

    QTextEdit* mTeReport;

    // Report
    int mReportColCount;
    RB_String mReportTitle;
    RB_String mCompanyName;
    RB_String mReportLimit;
    RB_ObjectContainer* mDataList;

    std::vector<double> mColumnWidthVector;
    QVector<QTextLength> mColWidthVector;
    int mMemCount;
    int mNoDataRow;
    bool mEvenRow;
    bool mIsAlternatingRow;
    bool mIsFirstDataFullRow;

};

#endif // SRM_REPORTWIDGET_H
