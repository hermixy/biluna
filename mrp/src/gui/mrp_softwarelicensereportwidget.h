/*****************************************************************
 * $Id: sail_trackdistancereportwidget.h 1744 2012-08-22 17:47:26Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_SOFTWARELICENSEREPORTWIDGET_H
#define MRP_SOFTWARELICENSEREPORTWIDGET_H

#include "rb_simplereportwidget.h"
#include "ui_mrp_softwarelicensereportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View software license report
 */
class MRP_SoftwareLicenseReportWidget : public RB_SimpleReportWidget,
                            private Ui::MRP_SoftwareLicenseReportWidget {

    Q_OBJECT

public:
    explicit MRP_SoftwareLicenseReportWidget(QWidget *parent = 0);
    virtual ~MRP_SoftwareLicenseReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveMRP; }

    virtual void init();

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setColumnWidthsAndTitle();
    void setComboBoxes();
};

#endif // MRP_SOFTWARELICENSEREPORTWIDGET_H
