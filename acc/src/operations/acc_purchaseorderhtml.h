/*****************************************************************
 * $Id: acc_purchaseorderhtml.h 2010 2013-10-04 20:58:40Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PURCHASEORDERHTML_H
#define ACC_PURCHASEORDERHTML_H

#include <QObject>
#include "acc_operation.h"

/**
 * Class responsible for the preparation of the html
 * for a purchase order preview for checking purposes.
 */
class ACC_PurchaseOrderHtml : public QObject, public ACC_Operation  {

    Q_OBJECT

public:
    ACC_PurchaseOrderHtml();
    virtual ~ACC_PurchaseOrderHtml();

    virtual bool execute(RB_ObjectBase* transDoc, RB_ObjectContainer* dtList);
    virtual void setIsCheckPrint(bool isCheckPrint = true);
    virtual RB_String getHtml();
    virtual RB_String getOrderNumber() {
        return mOrderNumber;
    }

private:
    void setAddress();
    void setCompanyData();
    void setOrderData();
    void setOrderDetails();
    void setLineItem(const RB_String& stockCode, const RB_String& description,
                     double unitPrice, double qtyDispatched,
                     const RB_String& unit, double discount,
                     const RB_String& narrative);

    RB_ObjectBase* mTransDoc;
    RB_ObjectContainer* mCreditorTransList;
    RB_ObjectBase* mSupplier;
    RB_ObjectBase* mPurchaseOrder;
    RB_ObjectBase* mLocation;
    //! Invoice HTML
    RB_String mHtml;
    //! Invoice line items
    RB_String mLineItems;

    int mEndlessLoopCounter;
    bool mIsCheckPrint;

    RB_String mOrderNumber;
};

#endif // ACC_PURCHASEORDERHTML_H
