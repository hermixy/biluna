/*****************************************************************
 * $Id: db_permissionreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Nov 21, 2015 7:00:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PERMISSIONREPORTWIDGET_H
#define DB_PERMISSIONREPORTWIDGET_H

#include "rb_simplereportwidget.h"
#include "ui_db_permissionreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View allocation report widget
 */
class DB_EXPORT DB_PermissionReportWidget : public RB_SimpleReportWidget,
                            private Ui::DB_PermissionReportWidget {

    Q_OBJECT

public:
    explicit DB_PermissionReportWidget(QWidget *parent = 0);
    virtual ~DB_PermissionReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "DB Permission List Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }
    virtual QWidget* getMainWidget() { return tePermissionList; }

    virtual void init();

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setColumnWidthsAndTitle();
    void setWidgets();
};

#endif // DB_PERMISSIONREPORTWIDGET_H
