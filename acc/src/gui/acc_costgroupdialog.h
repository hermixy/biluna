/*****************************************************************
 * $Id: acc_costgroupdialog.h 1797 2012-10-26 11:23:34Z rutger $
 * Created: Jun 20, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COSTGROUPDIALOG_H
#define ACC_COSTGROUPDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_costgroupdialog.h"


class ACC_CostGroupDialog : public RB_Dialog, private Ui::ACC_CostGroupDialog {

    Q_OBJECT

public:
    ACC_CostGroupDialog(QWidget *parent = 0);
    virtual ~ACC_CostGroupDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Cost Groups"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAddGroup_clicked();
    virtual void on_pbAddSubGroup_clicked();
    virtual void on_pbDeleteGroup_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();
    virtual void slotRowChanged(const QModelIndex& current,
                                const QModelIndex& previous);

protected:
    void changeEvent(QEvent *e);

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // ACC_COSTGROUPDIALOG_H
