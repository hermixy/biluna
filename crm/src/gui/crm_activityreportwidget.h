/*****************************************************************
 * $Id: crm_activityreportwidget.h 2132 2014-03-29 09:59:36Z rutger $
 * Created: Jun 7, 2012 9:00:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIVITYREPORTWIDGET_H
#define CRM_ACTIVITYREPORTWIDGET_H

#include "crm_reportwidget.h"
#include "ui_crm_activityreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View activity report widget
 */
class CRM_ActivityReportWidget : public CRM_ReportWidget, private Ui::CRM_ActivityReportWidget {

    Q_OBJECT

public:
    explicit CRM_ActivityReportWidget(QWidget *parent = 0);
    virtual ~CRM_ActivityReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "Activity Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveCRM; }
    virtual QWidget* getMainWidget() { return mTeReport; }

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
    virtual void editZoomIn(double factor);
    virtual void editZoomOut(double factor);

    virtual bool saveFile(const RB_String& fn);

    virtual RB_String getHelpSubject() const;

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void setDataRow(RB_String& html, RB_ObjectBase* obj);
    void setDataRow(RB_String& html, const QSqlRecord& rec);

    void changeEvent(QEvent *e);

private:
    void setLists();

    std::vector<RB_String> mTypeVector;
    std::vector<RB_String> mPriorityVector;
    std::vector<RB_String> mStatusVector;
};

#endif // CRM_ACTIVITYREPORTWIDGET_H
