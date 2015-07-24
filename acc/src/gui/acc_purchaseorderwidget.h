/*****************************************************************
 * $Id: acc_purchaseorderwidget.h 2217 2015-02-13 18:18:14Z rutger $
 * Created: Jan 23, 2010 1:00:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PURCHASEORDERWIDGET_H
#define ACC_PURCHASEORDERWIDGET_H

#include "rb_datawidgetmapper.h"
#include "rb_widget.h"
#include "ui_acc_purchaseorderwidget.h"

class RB_MmObjectInterface;
class RB_ObjectBase;

/**
 * Dialog for sales order create, read, update and delete actions
 */
class ACC_PurchaseOrderWidget : public RB_Widget,
        private Ui::ACC_PurchaseOrderWidget {

    Q_OBJECT

public:
    ACC_PurchaseOrderWidget(QWidget *parent = 0);
    virtual ~ACC_PurchaseOrderWidget();

    virtual RB_String getId() const { return RB_String(); }
    virtual RB_String getName() const { return "ACC Purchase Orders"; }
    virtual RB2::PerspectiveType getPerspectiveType() const {
        return RB2::PerspectiveACC;
    }

    virtual void init();
    virtual bool fileSave(bool withSelect);
    virtual void fileRevert();
    virtual RB_String getHelpSubject() const;
    virtual bool closeWidget();

public slots:
    virtual void on_pbAddOrder_clicked();
    virtual void on_pbDeleteOrder_clicked();
    virtual void on_pbSaveOrder_clicked();
    virtual void on_pbRevertOrder_clicked();
    virtual void on_pbCopyOrder_clicked();

    virtual void on_ileContact_clicked();
    virtual void on_ileContact_clear();

    virtual void on_pbAddDetail_clicked();
    virtual void on_pbDeleteDetail_clicked();

    virtual void on_ileStockCode_clicked();
    virtual void on_ileStockCode_clear();

    virtual void on_pbCheckOrder_clicked();
    virtual void on_pbProcessOrder_clicked();

    virtual void slotParentSelectionChanged(const QModelIndex& current,
                                            const QModelIndex& previous);
    virtual void slotItemSelectionChanged(const QModelIndex& current,
                                          const QModelIndex& previous);
    virtual void slotCalculateItemTotal();

protected:
    void changeEvent(QEvent *e);

private:
    int translatePurchaseOrder(bool isPost = false);
    void createNewOrder(RB_ObjectBase* order);
    void setExchangeRate();

    //! Item parent model, ACC_Supplier
    RB_MmProxy* mParentModel;
    //! Order model
    RB_MmProxy* mModel;
    //! Child orderdetail model
    RB_MmProxy* mChildModel;
    //! Order parent mapper, for supplier name
    RB_DataWidgetMapper* mParentMapper;
    //! Data widget mapper for order model
    RB_DataWidgetMapper* mMapper;
    //! Data widget mapper for orderdetail model
    RB_DataWidgetMapper* mChildMapper;

    /** Customer (debtor) transaction document which is used in the
        processinvoiceoperation and to store the data in the database
        and to show final data in invoice */
    RB_ObjectBase* mTransDoc;
    /** Supplier (creditor) transactions list per purchaseorder lineitem
        which is used in the processinvoiceoperation and to store
        the GL data in the database and to show final data on invoice */
    RB_ObjectContainer* mCreditorTransList;
    /** TODO: Stockmoveslist to store the data in the database in case the material
        management plugin is available */
    RB_ObjectBase* mStockMovesList;

    /** ID of the customer currency. This ID will be used to calculate
        the amounts for the GL bookings with the relevant exchange rate */
    RB_String mCurrencyId;
    double mExchangeRate;
};

#endif // ACC_PURCHASEORDERWIDGET_H
