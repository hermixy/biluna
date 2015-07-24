/*****************************************************************
 * $Id: acc_salesglpostingdialog.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Apr 05, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESGLPOSTINGDIALOG_H
#define ACC_SALESGLPOSTINGDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_salesglpostingdialog.h"


/**
 * Dialog to edit sales GL postings
 */
class ACC_SalesGlPostingDialog : public RB_Dialog, private Ui::ACC_SalesGlPostingDialog {

    Q_OBJECT

public:
    ACC_SalesGlPostingDialog(QWidget *parent = 0);
    virtual ~ACC_SalesGlPostingDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Edit Sales GL Postings"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_ileDiscountGlAccount_clicked();
    virtual void on_ileSalesGlAccount_clicked();
    virtual void on_ileStockCategory_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! Sales GL postings data model
    RB_MmProxy* mModel;
    //! Data widget mapper for COGS GL Postings model
    RB_DataWidgetMapper* mMapper;
};

#endif // ACC_SALESGLPOSTINGDIALOG_H
