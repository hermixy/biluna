/*****************************************************************
 * $Id: acc_selectcustbranchdialog.h 1540 2012-02-08 21:48:49Z rutger $
 * Created: Jan 25, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTCUSTBRANCHDIALOG_H
#define ACC_SELECTCUSTBRANCHDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_selectcustbranchdialog.h"


/**
 * Dialog to select customer and branch
 */
class ACC_SelectCustBranchDialog : public RB_Dialog,
        private Ui::ACC_SelectCustBranchDialog {

    Q_OBJECT

public:
    ACC_SelectCustBranchDialog(QWidget *parent = 0);
    virtual ~ACC_SelectCustBranchDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Select Customer Branch"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();

    virtual RB_ObjectBase* getCurrentObject();
    virtual QModelIndex getCurrentModelIndex() const;
    virtual RB_String getBranchId() const;

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbSelect_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void setCurrentObject();
    void changeEvent(QEvent *e);

private:
    //! Customer model
    RB_MmProxy* mCustModel;
    //! Selected index
    QModelIndex mCurrentModelIndex;
    //! Selected/current object
    RB_ObjectBase* mCurrentObject;
};

#endif // ACC_SELECTCUSTBRANCHDIALOG_H
