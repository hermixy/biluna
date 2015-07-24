/*****************************************************************
 * $Id: acc_taxauthratedialog.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Feb 22, 2010 12:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXAUTHRATEDIALOG_H
#define ACC_TAXAUTHRATEDIALOG_H

#include "rb_datawidgetmapper.h"
#include "rb_dialog.h"
#include "ui_acc_taxauthratedialog.h"

/**
 * Edit tax authority and rates dialog
 */
class ACC_TaxAuthRateDialog : public RB_Dialog, private Ui::ACC_TaxAuthRateDialog {

    Q_OBJECT

public:
    ACC_TaxAuthRateDialog(QWidget *parent = 0);
    virtual ~ACC_TaxAuthRateDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Tax Authority Rates"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAddAuthority_clicked();
    virtual void on_pbDeleteAuthority_clicked();
    virtual void on_pbAddAuthRate_clicked();
    virtual void on_pbDeleteAuthRate_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    void setAccount(const RB_String& fieldName, RB_MmProxy* model,
                    RB_ObjectBase* obj);

    //! Authority model
    RB_MmProxy* mModel;
    //! Child authority rate model
    RB_MmProxy* mChildModel;
    //! Data widget mapper for authority model
    RB_DataWidgetMapper* mMapper;
    //! Data widget mapper for authority rate model
    RB_DataWidgetMapper* mChildMapper;
};

#endif // ACC_TAXAUTHRATEDIALOG_H
