/*****************************************************************
 * $Id: srm_supplierwidget.h 2101 2014-02-19 11:55:29Z rutger $
 * Created: Feb 18, 2012 3:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_SUPPLIERWIDGET_H
#define SRM_SUPPLIERWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_srm_supplierwidget.h"

class RB_MmProxy;

class SRM_SupplierWidget : public RB_Widget, private Ui::SRM_SupplierWidget {

    Q_OBJECT

public:
    SRM_SupplierWidget(QWidget *parent = 0);
    virtual ~SRM_SupplierWidget();

    virtual RB_String getName() const { return "SRM Supplier"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveSRM; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

    virtual QModelIndex getCurrentModelIndex(int modelType) const;

public slots:
    virtual void on_pbAdd_clicked();
    virtual void on_pbDelete_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbFilterOn_clicked();
    virtual void on_pbFilterOff_clicked();

    virtual void on_pbMapLocation_clicked();
    virtual void on_pbSendEmail_clicked();

    virtual void on_pbAddItem_clicked();
    virtual void on_pbDeleteItem_clicked();

    virtual void on_pbSelectCompany_clicked();

protected:
    void changeEvent(QEvent* e);

protected slots:
    void slotParentRowChanged(const QModelIndex& curr,
                              const QModelIndex& prev);
    void slotHandleParentRowChanged();

private:
    //! Customer data model
    RB_MmProxy* mSuppModel;
    //! Customer data widget mapper
    RB_DataWidgetMapper* mSuppMapper;
    //! Supplier detail (SRM) data model
    RB_MmProxy* mSuppDetModel;
    //! Supplier detail (SRM) data widget mapper
    RB_DataWidgetMapper* mSuppDetMapper;
    //! Last selected ID
    RB_String mCurrentId;
    //! Stock item or puchase data model, items that the supplier could deliver
    RB_MmProxy* mItemModel;
    //! Stock item or purchase data widget mapper
    RB_DataWidgetMapper* mItemMapper;

};

#endif // SRM_SUPPLIERWIDGET_H
