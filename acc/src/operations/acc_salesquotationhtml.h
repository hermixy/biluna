/*****************************************************************
 * $Id: acc_salesquotationhtml.h 1839 2012-12-19 20:54:54Z rutger $
 * Created: Dec 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_SALESQUOTATIONHTML_H
#define ACC_SALESQUOTATIONHTML_H

#include <QObject>
#include "acc_operation.h"

/**
 * Class responsible for the preparation of the html
 * for a sales quotation preview checking and printing purposes.
 */
class ACC_SalesQuotationHtml : public QObject, public ACC_Operation  {

    Q_OBJECT

public:
    ACC_SalesQuotationHtml();
    virtual ~ACC_SalesQuotationHtml();

    virtual bool execute(RB_ObjectBase* salesOrder);
    virtual void setIsOrder(bool isOrder = true);
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

    RB_ObjectBase* mCustomer;
    RB_ObjectBase* mSalesOrder;

    //! Invoice HTML
    RB_String mHtml;
    //! Invoice line items
    RB_String mLineItems;

    double mTotalNettPrice;
    int mEndlessLoopCounter;
    bool mIsOrder;

};

#endif // ACC_SALESQUOTATIONHTML_H
