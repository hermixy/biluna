/*****************************************************************
 * $Id: acc_salesquotationhtml.cpp 2141 2014-04-07 17:53:21Z rutger $
 * Created: Dec 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesquotationhtml.h"

#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_SalesQuotationHtml::ACC_SalesQuotationHtml() {
    mHtml = "";
    mLineItems = "";
    mCustomer = NULL;
    mSalesOrder = NULL;
    mIsOrder = false; // is quotation
}

/**
 * Destructor
 */
ACC_SalesQuotationHtml::~ACC_SalesQuotationHtml() {
    delete mCustomer;
    // do not delete mSalesOrder, is part of objectfactory
    mSalesOrder->eraseChildren();
    RB_DEBUG->print("ACC_SalesQuotationHtml::~ACC_SalesQuotationHtml() OK");
}

/**
 * Execute operation, use getHtml() to get the created HTML
 * @param transDoc transaction document (or invoice to be)
 * @param ctList creditor transaction list with line items of document
 */
bool ACC_SalesQuotationHtml::execute(RB_ObjectBase* salesOrder) {
    mSalesOrder = salesOrder;

    // Get customer data for reference only
    RB_String id = salesOrder->getValue("parent").toString();

    if (id.isEmpty() || id == "0") {
        return false;
    }

    mCustomer = ACC_OBJECTFACTORY->newSingleObject("ACC_Customer");
    mCustomer->setId(id);
    mCustomer->dbRead(ACC_MODELFACTORY->getDatabase());

    return true;
}

/**
 * Set flag isCheckPrint to true will show the tax and freight as P.M.
 * (Pro Memorie) on the order
 * @param isCheckPrint
 */
void ACC_SalesQuotationHtml::setIsOrder(bool isOrder) {
    mIsOrder = isOrder;
}

/**
 * Get invoice in HTML
 * @returns HTML table with invoice page(s)
 */
RB_String ACC_SalesQuotationHtml::getHtml() {
    if (!mCustomer || !mSalesOrder) {
        mHtml = tr("Not based on purchase order or data error.");
        return mHtml;
    }

    QFile file(":/resources/salesquotation_table_te.html");
    file.open(QIODevice::ReadOnly);
    mHtml = file.readAll();

    setCompanyData();
    setAddress();
    setOrderData();

    return mHtml;
}

/**
 * Set company data in invoice header such as company name.
 * Logo is HTML, can be text or the path to an image <img src="./..">
 */
void ACC_SalesQuotationHtml::setCompanyData() {
    RB_ObjectBase* obj = ACC_MODELFACTORY->getRoot();
    // Logo: html
    mHtml.replace("{$ACC_Project.logo}", obj->getValue("logo").toString());

    if (mIsOrder) {
        mHtml.replace("{$ACC_SalesOrderQuotation}", "SALES ORDER");
    } else {
        mHtml.replace("{$ACC_SalesOrderQuotation}", "QUOTATION");
    }

    // Invoice to be send to branch address
    mHtml.replace("{$ACC_Project.coyname}", obj->getValue("company").toString());
    mHtml.replace("{$ACC_Project.regoffice1}", obj->getValue("regoffice1").toString());
    mHtml.replace("{$ACC_Project.regoffice2}", obj->getValue("regoffice2").toString());
    mHtml.replace("{$ACC_Project.regoffice3}", obj->getValue("regoffice3").toString());
    mHtml.replace("{$ACC_Project.regoffice4}", obj->getValue("regoffice4").toString());
    mHtml.replace("{$ACC_Project.regoffice5}", obj->getValue("regoffice5").toString());
    mHtml.replace("{$ACC_Project.regoffice6}", obj->getValue("regoffice6").toString());
    mHtml.replace("{$ACC_Project.telephone}", obj->getValue("telephone").toString());
    mHtml.replace("{$ACC_Project.fax}", obj->getValue("fax").toString());
    mHtml.replace("{$ACC_Project.email}", obj->getValue("email").toString());
    mHtml.replace("{$ACC_Project.gstno}", obj->getValue("gstno").toString());
}

/**
 * Set customer sold to- and delivered to addresses
 */
void ACC_SalesQuotationHtml::setAddress() {
    if (!mCustomer) return;

    int invoiceBranch = mCustomer->getValue("invaddrbranch").toInt();
    RB_ObjectBase* cust = mCustomer;

    if (invoiceBranch != 0) {
        // Invoice to be send to debtormaster or customer main office
        mEndlessLoopCounter = 0;
        cust = getInvoiceAddress(mCustomer);
    }

    // Invoice to be send to branch address
    mHtml.replace("{$ACC_Customer.mname}", cust->getValue("mname").toString());
    mHtml.replace("{$ACC_Customer.address1}", cust->getValue("brpostaddress1").toString());
    mHtml.replace("{$ACC_Customer.address2}", cust->getValue("brpostaddress2").toString());
    mHtml.replace("{$ACC_Customer.address3}", cust->getValue("brpostaddress3").toString());
    mHtml.replace("{$ACC_Customer.address4}", cust->getValue("brpostaddress4").toString());
    mHtml.replace("{$ACC_Customer.address5}", cust->getValue("brpostaddress5").toString());
    mHtml.replace("{$ACC_Customer.address6}", cust->getValue("brpostaddress6").toString());

    RB_String currencyId = cust->getValue("currency_id").toString();
    ACC_SqlCommonFunctions f;
    RB_String currency = f.selectFromWhereId("currcode", "ACC_Currency", currencyId).toString();
    mHtml.replace("{$ACC_Customer.currency}", currency);

    if (cust != mCustomer) {
        delete cust;
        cust = NULL;
    }

    // Delivery address, deliverto etc. are not part of ACC_Customer but required
    // to make a difference with the above address lines
    mHtml.replace("{$ACC_Customer.deliverto}", mSalesOrder->getValue("deliverto").toString());
    mHtml.replace("{$ACC_Customer.deladd1}", mSalesOrder->getValue("deladd1").toString());
    mHtml.replace("{$ACC_Customer.deladd2}", mSalesOrder->getValue("deladd2").toString());
    mHtml.replace("{$ACC_Customer.deladd3}", mSalesOrder->getValue("deladd3").toString());
    mHtml.replace("{$ACC_Customer.deladd4}", mSalesOrder->getValue("deladd4").toString());
    mHtml.replace("{$ACC_Customer.deladd5}", mSalesOrder->getValue("deladd5").toString());
    mHtml.replace("{$ACC_Customer.deladd6}", mSalesOrder->getValue("deladd6").toString());
}

/**
 * Get customer invoice address
 * @param cust customer branch
 * @return invoice customer branch
 */
RB_ObjectBase* ACC_SalesQuotationHtml::getInvoiceAddress(RB_ObjectBase* cust) {
    int invoiceHomeOffice = cust->getValue("invaddrbranch").toInt(); // TODO: invaddrbranch is confusing
    RB_String custParentId = cust->getIdValue("custparent_idx").toString();

    if (invoiceHomeOffice != 1 || custParentId.size() < 38 /* is ID size */) {
        return cust;
    }

    if (cust == mCustomer) {
        cust = ACC_OBJECTFACTORY->newSingleObject("ACC_Customer");
    }

    cust->setId(custParentId);
    cust->dbRead(ACC_MODELFACTORY->getDatabase());

    if (mEndlessLoopCounter < 10) {
        ++mEndlessLoopCounter;
        return getInvoiceAddress(cust);
    }

    return NULL;
}

/**
 * Set order data, such as supplier reference
 */
void ACC_SalesQuotationHtml::setOrderData() {
    if (!mCustomer || !mSalesOrder) {
        mHtml.replace("{$ACC_TransDoc.transno}", "ERROR");
        return;
    }

    RB_String str = mSalesOrder->getValue("orderno").toString();
    mHtml.replace("{$ACC_SalesOrder.orderno}", str);

    str = mSalesOrder->getValue("orddate").toString();
    mHtml.replace("{$ACC_SalesOrder.orddate}", str);

    RB_String id = mSalesOrder->getValue("shipvia_id").toString();
    ACC_SqlCommonFunctions f;
    str = f.selectFromWhereId("shippername", "ACC_Shipper", id).toString();
    mHtml.replace("{$ACC_SalesOrder.shipvia_id}", str);
    id = mCustomer->getValue("paymentterm_id").toString();
    str = f.selectFromWhereId("terms", "ACC_PaymentTerm", id).toString();
    mHtml.replace("{$ACC_PaymentTerm.terms}", str);

    mHtml.replace("{$ACC_SalesOrder.customerref}",
                  mSalesOrder->getValue("customerref").toString());
    mHtml.replace("{$ACC_SalesOrder.customerref}", mSalesOrder->getValue("customerref").toString());
    mHtml.replace("{$ACC_Customer.taxref}", mCustomer->getValue("taxref").toString());
    mHtml.replace("{$ACC_Customer.contactname}", mCustomer->getValue("contactname").toString());

    setOrderDetails();

    // total price
    mHtml.replace("{$ACC_SalesOrder.totalnetprice}",
                  RB_String::number(mTotalNettPrice, 'f', 2));
    mHtml.replace("{$ACC_SalesOrder.freightcost}", "P.M.");
    mHtml.replace("{$ACC_SalesOrder.tax}", "P.M.");
    mHtml.replace("{$ACC_SalesOrder.totalgrossprice}",
                  RB_String::number(mTotalNettPrice, 'f', 2));

    RB_String comment = mSalesOrder->getValue("invoicecomment").toString();
    mHtml.replace("{$ACC_SalesOrder.invoicecomment}", comment);
}

/**
 * Set order details (line items) and freight cost
 * @param stockMovesList container of line items with sales order details
 */
void ACC_SalesQuotationHtml::setOrderDetails() {
    RB_ObjectContainer* orderItemList = mSalesOrder->getContainer("ACC_SalesOrderDetailList");
    if (!orderItemList) return;

    mLineItems.clear();
    RB_String stockCode = "";
    RB_String stockDescr = "";
    double qtyDisp = 0.0;
    RB_String uom = "";
    double price = 0.0;
    double discount = 0.0;
    RB_String narr = "";

    mTotalNettPrice = 0.0;

    RB_ObjectIterator* iter = orderItemList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* item = iter->currentObject();

        stockCode = item->getDValue("stk_idx").toString(); // _idx
        stockDescr = item->getValue("stkdescription").toString();
        price = item->getValue("unitprice").toDouble();
        qtyDisp = item->getValue("quantity").toDouble();
        uom = item->getValue("stkuom").toString();
        discount = item->getValue("discountpercent").toDouble();
        narr = item->getValue("narrative").toString();

        setLineItem(stockCode, stockDescr, price, qtyDisp, uom, discount, narr);
    }

    delete iter;

    mHtml.replace("{$ACC_SalesOrder.lineitems}", mLineItems);
}

/**
 * Set an invoice line item
 * TODO: currency rate
 * @param stockCode stock item code
 * @param description description of stock item
 * @param unitPrice unit price of stock item
 * @param qtyDispatched quantity dispatched
 * @param unit unit of measurement
 * @param discount discount on this sales order line
 * @param narrative narrative with sales order line
 */
void ACC_SalesQuotationHtml::setLineItem(const RB_String& stockCode,
                                     const RB_String& description,
                                     double unitPrice,
                                     double qtyDispatched,
                                     const RB_String& unit,
                                     double discount,
                                     const RB_String& narrative) {
    // Use % operator for fast concatenation of strings instead of +, not for +=
    // Use single quotes to pass a character instead of double quotes which is a const char*
    mLineItems += "<table width=\"100%\"><tr><td width=\"20%\">";
    mLineItems += stockCode;
    mLineItems += "</td><td width=\"37%\">";
    mLineItems += description;
    mLineItems += "</td><td width=\"12%\" align=\"right\">";
    mLineItems += RB_String::number(unitPrice, 'f', 2);
    mLineItems += "</td><td width=\"5%\" align=\"right\">";
    mLineItems += RB_String::number(qtyDispatched);
    mLineItems += "</td><td width=\"5%\" align=\"center\">";
    mLineItems += unit;
    mLineItems += "</td><td width=\"5%\" align=\"right\">";
    mLineItems += RB_String::number(discount); // percent
    mLineItems += "</td><td width=\"16%\" align=\"right\">";
    double linePrice = unitPrice * qtyDispatched * (1.0 - discount/100.0);
    mLineItems += RB_String::number(linePrice, 'f', 2);
    mLineItems += "</td></tr><tr><td width=\"20%\">&nbsp;<br />&nbsp;</td><td width=\"*\" colspan=\"5\">";
    mLineItems += narrative;
    mLineItems += "</td><td width=\"15%\">&nbsp;</td></tr></table>";

    mTotalNettPrice += linePrice;
}

