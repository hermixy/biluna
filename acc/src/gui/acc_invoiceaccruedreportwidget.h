/*****************************************************************
 * $Id: acc_invoiceaccruedreportwidget.h 2202 2015-01-11 19:39:36Z rutger $
 * Created: Jan 11, 2015 21:17:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_INVOICEACCRUEDREPORTWIDGET_H
#define ACC_INVOICEACCRUEDREPORTWIDGET_H

#include "rb_simplereportwidget.h"
#include "ui_acc_invoiceaccruedreportwidget.h"

class RB_ObjectAtomic;
class RB_ObjectContainer;

/**
 * View invoice accrued (open at year ending) report
 */
class ACC_InvoiceAccruedReportWidget : public RB_SimpleReportWidget,
                            private Ui::ACC_InvoiceAccruedReportWidget {

    Q_OBJECT

public:
    explicit ACC_InvoiceAccruedReportWidget(QWidget *parent = 0);
    virtual ~ACC_InvoiceAccruedReportWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const;
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();

    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbRefresh_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setColumnWidthsAndTitle();
    void setWidgets();

    QDate mFiscalYearStart;
    QDate mFiscalYearEnd;
};

#endif // ACC_INVOICEACCRUEDREPORTWIDGET_H
