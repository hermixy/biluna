/*****************************************************************
 * $Id: acc_accountgroupdialog.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Jan 17, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACCOUNTGROUPDIALOG_H
#define ACC_ACCOUNTGROUPDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_accountgroupdialog.h"


class ACC_AccountGroupDialog : public RB_Dialog, private Ui::ACC_AccountGroupDialog {

    Q_OBJECT

public:
    ACC_AccountGroupDialog(QWidget *parent = 0);
    virtual ~ACC_AccountGroupDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Account Groups"; }
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

protected:
    void changeEvent(QEvent *e);

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // ACC_ACCOUNTGROUPDIALOG_H
