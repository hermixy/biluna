/*****************************************************************
 * $Id: acc_costcenterbudgetdialog.h 1681 2012-06-25 21:36:26Z rutger $
 * Created: Feb 22, 2010 12:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COSTCENTERBUDGETDIALOG_H
#define ACC_COSTCENTERBUDGETDIALOG_H

#include "rb_datawidgetmapper.h"
#include "rb_dialog.h"
#include "ui_acc_costcenterbudgetdialog.h"

/**
 * Edit tax authority and rates dialog
 */
class ACC_CostCenterBudgetDialog : public RB_Dialog, private Ui::ACC_CostCenterBudgetDialog {

    Q_OBJECT

public:
    ACC_CostCenterBudgetDialog(QWidget *parent = 0);
    virtual ~ACC_CostCenterBudgetDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Cost Center and Budget"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAddCenter_clicked();
    virtual void on_pbDeleteCenter_clicked();
    virtual void on_ileGroup_clicked();
    virtual void on_pbAddBudget_clicked();
    virtual void on_pbDeleteBudget_clicked();
    virtual void on_pbCalculateActual_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:

    //! Cost center model
    RB_MmProxy* mModel;
    //! Budget model
    RB_MmProxy* mChildModel;
    //! Data widget mapper for cost center model
    RB_DataWidgetMapper* mMapper;
    //! Data widget mapper for budget model
    RB_DataWidgetMapper* mChildMapper;
};

#endif // ACC_COSTCENTERBUDGETDIALOG_H
