/*****************************************************************
 * $Id: acc_purchaseglpostingdialog.h 1888 2013-01-31 17:21:31Z rutger $
 * Created: Jan 28, 2013 17:34:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PURCHASEGLPOSTINGDIALOG_H
#define ACC_PURCHASEGLPOSTINGDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_purchaseglpostingdialog.h"


/**
 * Dialog to edit purchase GL postings
 */
class ACC_PurchaseGlPostingDialog : public RB_Dialog, private Ui::ACC_PurchaseGlPostingDialog {

    Q_OBJECT

public:
    ACC_PurchaseGlPostingDialog(QWidget *parent = 0);
    virtual ~ACC_PurchaseGlPostingDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Edit Purchase GL Postings"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_ilePurchaseGlAccount_clicked();
    virtual void on_ileStockCategory_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! Purchase GL postings data model
    RB_MmProxy* mModel;
    //! Data widget mapper for purchase GL Postings model
    RB_DataWidgetMapper* mMapper;
};

#endif // ACC_PURCHASEGLPOSTINGDIALOG_H
