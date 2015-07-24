/*****************************************************************
 * $Id: acc_cogsglpostingdialog.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Apr 05, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_COGSGLPOSTINGDIALOG_H
#define ACC_COGSGLPOSTINGDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_cogsglpostingdialog.h"


/**
 * Dialog to edit cost of goods sold GL postings
 */
class ACC_CogsGlPostingDialog : public RB_Dialog, private Ui::ACC_CogsGlPostingDialog {

    Q_OBJECT

public:
    ACC_CogsGlPostingDialog(QWidget *parent = 0);
    virtual ~ACC_CogsGlPostingDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Edit COGS GL Postings"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_ileStockCategory_clicked();
    virtual void on_ileGlAccount_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! COGS GL postings data model
    RB_MmProxy* mModel;
    //! Data widget mapper for COGS GL Postings model
    RB_DataWidgetMapper* mMapper;
};

#endif // ACC_COGSGLPOSTINGDIALOG_H
