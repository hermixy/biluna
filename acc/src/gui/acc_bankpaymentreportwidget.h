/*****************************************************************
 * $Id: sail_trackdistancereportwidget.h 1744 2012-08-22 17:47:26Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKPAYMENTREPORTWIDGET_H
#define ACC_BANKPAYMENTREPORTWIDGET_H

#include "rb_simplereportwidget.h"
#include "ui_acc_bankpaymentreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View software license report
 */
class ACC_BankPaymentReportWidget : public RB_SimpleReportWidget,
                            private Ui::ACC_BankPaymentReportWidget {

    Q_OBJECT

public:
    explicit ACC_BankPaymentReportWidget(QWidget *parent = 0);
    virtual ~ACC_BankPaymentReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectiveACC;
    }

    virtual void init();

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbRefresh_clicked();
    virtual void slotCustSupplSelectionChanged(const QModelIndex& current,
                                               const QModelIndex& previous);

protected:
    void changeEvent(QEvent *e);

private:
    void setColumnWidthsAndTitle();
    void setSelectionWidgets();
};

#endif // ACC_BANKPAYMENTREPORTWIDGET_H
