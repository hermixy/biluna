/*****************************************************************
 * $Id: acc_supplierdetailwidget.h 1845 2012-12-23 14:39:11Z rutger $
 * Created: Feb 10, 2010 3:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SUPPLIERDETAILWIDGET_H
#define ACC_SUPPLIERDETAILWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_supplierdetailwidget.h"


class ACC_SupplierDetailWidget : public RB_Widget, private Ui::ACC_SupplierDetailWidget {

    Q_OBJECT

public:
    ACC_SupplierDetailWidget(QWidget *parent = 0);
    virtual ~ACC_SupplierDetailWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Supplier Details"; }
    virtual RB2::PerspectiveType getPerspectiveType() const { return RB2::PerspectiveACC; }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

protected slots:
    virtual void on_pbPrevious_clicked();
    virtual void on_pbNext_clicked();
    virtual void on_pbSave_clicked();
    virtual void on_pbRevert_clicked();
    virtual void on_pbMapLocation_clicked();
    virtual void on_pbSendEmail_clicked();
    virtual void on_pbAddItem_clicked();
    virtual void on_pbDeleteItem_clicked();
    virtual void on_ileStockCode_clicked();
    virtual void on_pbAddNote_clicked();
    virtual void on_pbDeleteNote_clicked();
    virtual void slotResetWidgets();

protected:
    void changeEvent(QEvent *e);

private:
    //! Supplier data model
    RB_MmProxy* mModel;
    //! Supplier data widget mapper
    RB_DataWidgetMapper* mMapper;
    //! Stock item data model, items that the supplier could deliver
    RB_MmProxy* mItemModel;
    //! Stock item data widget mapper
    RB_DataWidgetMapper* mItemMapper;
    //! Note data model
    RB_MmProxy* mNoteModel;
    //! Note data widget mapper
    RB_DataWidgetMapper* mNoteMapper;
};

#endif // ACC_SUPPLIERDETAILWIDGET_H
