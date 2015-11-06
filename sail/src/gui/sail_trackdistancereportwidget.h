/*****************************************************************
 * $Id: sail_trackdistancereportwidget.h 1744 2012-08-22 17:47:26Z rutger $
 * Created: Aug 11, 2012 8:58:25 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_TRACKDISTANCEREPORTWIDGET_H
#define SAIL_TRACKDISTANCEREPORTWIDGET_H

#include "rb_widget.h"
#include "sail_coordcalc.h"
#include "ui_sail_trackdistancereportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View track distance report
 */
class SAIL_TrackDistanceReportWidget : public RB_Widget,
        private Ui::SAIL_TrackDistanceReportWidget {

    Q_OBJECT

public:
    explicit SAIL_TrackDistanceReportWidget(QWidget *parent = 0);
    virtual ~SAIL_TrackDistanceReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSAIL; }

    virtual void init();

    virtual bool fileSave(bool withSelect);
    virtual bool fileSaveAs();
    virtual void filePdf(QPrinter* printer);
    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual bool saveFile(const RB_String &fn);

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:

    void createTable(RB_ObjectContainer* trackList);
    void addHeaderRow();
    void addDataRow(RB_ObjectBase* obj);
    void addFooterRow();
    void addFooter();

    //! Report HTML
    RB_String mHtml;

    SAIL_CoordCalc mCalc;
};

#endif // SAIL_TRACKDISTANCEREPORTWIDGET_H
