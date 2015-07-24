/*****************************************************************
 * $Id: acc_selectstockitemdialog.h 2027 2013-11-08 19:05:44Z rutger $
 * Created: Jan 25, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SELECTSTOCKITEMDIALOG_H
#define ACC_SELECTSTOCKITEMDIALOG_H

#include "rb_dialog.h"
#include "ui_acc_selectstockitemdialog.h"


/**
 * Dialog to select stock item/master
 */
class ACC_SelectStockItemDialog : public RB_Dialog, private Ui::ACC_SelectStockItemDialog {

    Q_OBJECT

public:
    ACC_SelectStockItemDialog(QWidget *parent = 0);
    virtual ~ACC_SelectStockItemDialog();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Select Stock Item"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();

    virtual QModelIndex getCurrentChild1ModelIndex() const;
    virtual RB_String getStockId() const;
    virtual RB_String getCode() const;
    virtual RB_String getSerialNumber() const;
    virtual RB_String getDescription() const;
    virtual RB_String getUnitOfMeasurement() const;
    virtual double getPrice() const;
    virtual double getMaterialCost() const;
    virtual double getLabourCost() const;
    virtual double getOverheadCost() const;
    virtual RB_String getTaxCatId() const;
    virtual RB_String getCostCenterIdx() const;

public slots:
    virtual void on_pbHelp_clicked();
    virtual void on_pbSelect_clicked();
    virtual void on_pbCancel_clicked();

protected:
    void changeEvent(QEvent *e);

private:
    //! Stock Category Data model
    RB_MmProxy* mModel;
    //! Child stock item model
    RB_MmProxy* mChildModel;
    //! Grandchild Price data model
    RB_MmProxy* mGrChildModel;
};

#endif // ACC_SELECTSTOCKITEMDIALOG_H
