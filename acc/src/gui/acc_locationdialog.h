/*****************************************************************
 * $Id: acc_locationdialog.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Feb 18, 2010 8:44:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_LOCATIONDIALOG_H
#define ACC_LOCATIONDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_locationdialog.h"

/**
 * Edit dialog for warehouse or stock location
 */
class ACC_LocationDialog : public RB_Dialog, private Ui::ACC_LocationDialog {

    Q_OBJECT

public:
    ACC_LocationDialog(QWidget *parent = 0);
    virtual ~ACC_LocationDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Edit stock location"; }
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

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // ACC_LOCATIONDIALOG_H
