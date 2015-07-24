/*****************************************************************
 * $Id: acc_taxgroupauthdialog.h 1719 2012-07-25 10:26:12Z rutger $
 * Created: Feb 24, 2010 12:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_TAXGROUPAUTHDIALOG_H
#define ACC_TAXGROUPAUTHDIALOG_H

#include "rb_datawidgetmapper.h"
#include "rb_dialog.h"
#include "ui_acc_taxgroupauthdialog.h"

/**
 * Edit tax group and tax authority dialog
 */
class ACC_TaxGroupAuthDialog : public RB_Dialog, private Ui::ACC_TaxGroupAuthDialog {

    Q_OBJECT

public:
    ACC_TaxGroupAuthDialog(QWidget *parent = 0);
    virtual ~ACC_TaxGroupAuthDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Tax Group and Tax Authorities"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();

public slots:
    virtual void on_pbAddTaxGroup_clicked();
    virtual void on_pbDeleteTaxGroup_clicked();
    virtual void on_pbAddTaxAuthority_clicked();
    virtual void on_pbDeleteTaxAuthority_clicked();
    virtual void on_pbHelp_clicked();
    virtual void on_pbOk_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! Tax group model
    RB_MmProxy* mModel;
    //! Child tax authority model
    RB_MmProxy* mChildModel;
    //! Data widget mapper for tax group model
    RB_DataWidgetMapper* mMapper;
    //! Data widget mapper for tax authority model
    RB_DataWidgetMapper* mChildMapper;
};

#endif // ACC_TAXGROUPAUTHDIALOG_H
