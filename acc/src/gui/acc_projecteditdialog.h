/*****************************************************************
 * $Id: acc_projecteditdialog.h 2163 2014-09-04 08:03:35Z rutger $
 * Created: Jan 23, 2010 1:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PROJECTEDITDIALOG_H
#define ACC_PROJECTEDITDIALOG_H

#include "rb_datawidgetmapper.h"
#include "rb_dialog.h"
#include "ui_acc_projecteditdialog.h"

// class RB_MmObjectInterface;

/**
 * Edit project (company) data and settings. A XXX_Project class is the root
 * of the datamodel and in ACC it is at the same time the current company.
 */
class ACC_ProjectEditDialog : public RB_Dialog, private Ui::ACC_ProjectEditDialog {

    Q_OBJECT

public:
    ACC_ProjectEditDialog(QWidget *parent = 0);
    virtual ~ACC_ProjectEditDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Company Edit"; }
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

    virtual void on_ilePayroll_clicked();
    virtual void on_ileGoodsReceived_clicked();
    virtual void on_ileRetainedEarning_clicked();
    virtual void on_ileFreight_clicked();
    virtual void on_ileSalesExchVar_clicked();
    virtual void on_ilePurchExchVar_clicked();
    virtual void on_ilePaymentDiscount_clicked();

protected:
    virtual void changeEvent(QEvent *e);

private:
    void setAccount(const RB_String& fieldName, RB_ObjectBase* obj);

    //! Company (project) model
    RB_MmProxy* mModel;
    //! Data widget mapper for company model
    RB_DataWidgetMapper* mMapper;

};

#endif // ACC_PROJECTEDITDIALOG_H
