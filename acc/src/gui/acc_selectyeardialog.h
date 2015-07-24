/*****************************************************************
 * $Id: acc_selectyeardialog.h 1540 2012-02-08 21:48:49Z rutger $
 * Created: Jan 09, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTYEARDIALOG_H
#define ACC_SELECTYEARDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_selectyeardialog.h"


/**
 * Dialog to select the relevant year
 */
class ACC_SelectYearDialog : public RB_Dialog,
        private Ui::ACC_SelectYearDialog {

    Q_OBJECT

public:
    ACC_SelectYearDialog(QWidget *parent = 0);
    virtual ~ACC_SelectYearDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Select Year"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();

    virtual int getYear() const;

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbSelect_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:
};

#endif // ACC_SELECTYEARDIALOG_H
