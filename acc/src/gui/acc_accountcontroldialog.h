/*****************************************************************
 * $Id: acc_accountcontroldialog.h 1540 2012-02-08 21:48:49Z rutger $
 * Created: Nov 11, 2010 4:25:40 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACCOUNTCONTROLDIALOG_H
#define ACC_ACCOUNTCONTROLDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_accountcontroldialog.h"

/**
 * Edit dialog for the account control. The system account controls
 * are translated with the user set account control name.
 */
class ACC_AccountControlDialog : public RB_Dialog, private Ui::ACC_AccountControlDialog {

    Q_OBJECT

public:
    ACC_AccountControlDialog(QWidget *parent = 0);
    virtual ~ACC_AccountControlDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Account Control"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void slotSelectionChanged(const QModelIndex&, const QModelIndex&);
    void slotIndexChanged(int);

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;
    //! System account control model
    RB_MmProxy* mSysModel;

};

#endif // ACC_ACCOUNTCONTROLDIALOG_H
