/*****************************************************************
 * $Id: acc_allocnreportwidget.h 2200 2014-12-15 10:44:04Z rutger $
 * Created: Jun 30, 2011 9:00:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ALLOCNREPORTWIDGET_H
#define ACC_ALLOCNREPORTWIDGET_H

#include "rb_simplereportwidget.h"
#include "ui_acc_allocnreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View allocation report widget
 */
class ACC_AllocnReportWidget : public RB_SimpleReportWidget,
                            private Ui::ACC_AllocnReportWidget {

    Q_OBJECT

public:
    explicit ACC_AllocnReportWidget(QWidget *parent = 0);
    virtual ~ACC_AllocnReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Allocation/Reconciliation Report"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }
    virtual QWidget* getMainWidget() { return teAlloc; }

    virtual void init();

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setColumnWidthsAndTitle();
    void setComboBoxes();
};

#endif // ACC_ALLOCNREPORTWIDGET_H
