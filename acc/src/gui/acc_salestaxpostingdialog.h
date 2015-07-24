/*****************************************************************
 * $Id: acc_salestaxpostingdialog.h 1803 2012-11-22 21:43:16Z rutger $
 * Created: Jan 19, 2012 4:25:40 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESTAXPOSTINGDIALOG_H
#define ACC_SALESTAXPOSTINGDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_salestaxpostingdialog.h"

/**
 * Edit dialog for the sales/purchase tax posting accounts
 *
 * TODO: consider separate purchase tax posting to other General Ledger
 * than the sales tax General Ledger.
 * Refer also to ACC_SqlCommonFunctions::getPurchaseTaxes() called
 * ACC_PoTransDoc::execute()
 */
class ACC_SalesTaxPostingDialog : public RB_Dialog, private Ui::ACC_SalesTaxPostingDialog {

    Q_OBJECT

public:
    ACC_SalesTaxPostingDialog(QWidget *parent = 0);
    virtual ~ACC_SalesTaxPostingDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Sales Tax Posting"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_ileSalesTaxAccount_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

protected slots:

private:
    //! Data model
    RB_MmProxy* mModel;
    //! Data widget mapper
    RB_DataWidgetMapper* mMapper;

};

#endif // ACC_SALESTAXPOSTINGDIALOG_H
