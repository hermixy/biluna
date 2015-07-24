/*****************************************************************
 * $Id: acc_salesinvoicehtml.h 1952 2013-07-12 17:11:46Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESINVOICEHTML_H
#define ACC_SALESINVOICEHTML_H

#include <QObject>
#include "acc_operation.h"

/**
 * Class responsible for the preparation of the html for a sales invoice
 */
class ACC_SalesInvoiceHtml : public QObject, public ACC_Operation  {

    Q_OBJECT

public:
    ACC_SalesInvoiceHtml();
    virtual ~ACC_SalesInvoiceHtml();

    virtual bool execute(RB_ObjectBase* transDoc, RB_ObjectContainer* dtList);
    virtual RB_String getHtml();

private:
    void setAddress();
    RB_ObjectBase* getInvoiceAddress(RB_ObjectBase* cust);
    void setCompanyData();
    void setOrderData();
    void setOrderDetails();
    void setLineItem(const RB_String& stockCode, const RB_String& description,
                     double unitPrice, double qtyDispatched,
                     const RB_String& unit, double discount,
                     const RB_String& narrative);

    RB_ObjectBase* mRoot;
    RB_ObjectBase* mTransDoc;
    RB_ObjectContainer* mDebtorTransList;
    RB_ObjectBase* mCustomer;
    RB_ObjectBase* mSalesOrder;
    //! Invoice HTML
    RB_String mHtml;
    //! Invoice line items
    RB_String mLineItems;
    double mNetFreight;

    int mEndlessLoopCounter;
    double mExchangeRate;

};

#endif // ACC_SALESINVOICEHTML_H
