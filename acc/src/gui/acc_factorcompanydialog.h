/*****************************************************************
 * $Id: acc_factorcompanydialog.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Mar 18, 2010 11:00:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_FACTORCOMPANYDIALOG_H
#define ACC_FACTORCOMPANYDIALOG_H

#include "rb_datawidgetmapper.h"
#include "rb_dialog.h"
#include "ui_acc_factorcompanydialog.h"

/**
 * Edit factor companies dialog
 */
class ACC_FactorCompanyDialog : public RB_Dialog, private Ui::ACC_FactorCompanyDialog {

    Q_OBJECT

public:
    ACC_FactorCompanyDialog(QWidget *parent = 0);
    virtual ~ACC_FactorCompanyDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Factor Companies"; }
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
    //! Factor companies model
    RB_MmProxy* mModel;
    //! Data widget mapper for factor companies model
    RB_DataWidgetMapper* mMapper;
};

#endif // ACC_FACTORCOMPANYDIALOG_H
