/*****************************************************************
 * $Id: db_dashboardwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 31, 2011 1:08:25 PM - Rutger Botermans
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_DASHBOARDWIDGET_H
#define DB_DASHBOARDWIDGET_H

#include "rb_string.h"
#include "rb_widget.h"
#include "ui_db_dashboardwidget.h"

class DB_EXPORT DB_DashboardWidget : public RB_Widget,
                                private Ui::DB_DashboardWidget {

    Q_OBJECT

public:
    explicit DB_DashboardWidget(QWidget *parent = 0);
    virtual ~DB_DashboardWidget();

    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveDB; }

    virtual void init();

    virtual void filePrint(QPrinter* printer);
    virtual void filePrintPreview(QPrinter* printer);

    virtual void setHtml(const RB_String& txt);
    virtual QTextBrowser* getTextBrowser();

protected:
    void changeEvent(QEvent *e);

private:
    RB_String mWidgetName;
};

#endif // DB_DASHBOARDWIDGET_H
