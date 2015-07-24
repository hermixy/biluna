/*****************************************************************
 * $Id: acc_salesinvoicehtml.cpp 2186 2014-10-16 14:04:01Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_salesinvoicehtml.h"

#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_SalesInvoiceHtml::ACC_SalesInvoiceHtml() {
    mRoot = NULL;
    mHtml = "";
    mLineItems = "";
    mTransDoc = NULL;
    mDebtorTransList = NULL;
    mCustomer = NULL;
    mSalesOrder = NULL;
    mNetFreight = 0.0;
    mExchangeRate = 1.0;
}

/**
 * Destructor
 */
ACC_SalesInvoiceHtml::~ACC_SalesInvoiceHtml() {
    delete mCustomer;
    delete mSalesOrder;
    RB_DEBUG->print("ACC_SalesInvoiceHtml::~ACC_SalesInvoiceHtml() OK");
}

/**
 * Execute operation, use getHtml() to get the created HTML
 * @param transDoc transaction document (or invoice to be)
 * @param dtList debtor transaction list with line items of document
 */
bool ACC_SalesInvoiceHtml::execute(RB_ObjectBase* transDoc, RB_ObjectContainer* dtList) {
    mTransDoc = transDoc;
    mDebtorTransList = dtList;

    // Get customer data
    RB_String id = transDoc->getValue("debtor_idx").toString();
    mExchangeRate = transDoc->getValue("rate").toDouble();

    if (id.isEmpty() || id == "0") {
        return false;
    }

    mCustomer = ACC_OBJECTFACTORY->newSingleObject("ACC_Customer");
    mCustomer->setId(id);
    mCustomer->dbRead(ACC_MODELFACTORY->getDatabase());

    // Get sales order data
    id = transDoc->getValue("order_id").toString();

    if (id.isEmpty() || id == "0") {
        return false;
    }

    mSalesOrder = ACC_OBJECTFACTORY->newSingleObject("ACC_SalesOrder");
    mSalesOrder->setId(id);
    mSalesOrder->dbRead(ACC_MODELFACTORY->getDatabase());

    mRoot = ACC_MODELFACTORY->getModel(ACC_ModelFactory::ModelCustomer)->getRoot();

    return true;
}

/**
 * Get invoice in HTML
 * @returns HTML table with invoice page(s)
 */
RB_String ACC_SalesInvoiceHtml::getHtml() {
    if (!mTransDoc || !mDebtorTransList || !mCustomer || !mSalesOrder) {
        mHtml = tr("Not based on sales order or data error.");
        return mHtml;
    }

    QFile file(":/resources/salesinvoice_table_te.html");
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
void ACC_SalesInvoiceHtml::setCompanyData() {
    RB_ObjectBase* obj = ACC_MODELFACTORY->getRoot();
    // Logo: html
    mHtml.replace("{$ACC_Project.logo}", obj->getValue("logo").toString());

    // Invoice to be send to branch address
    mHtml.replace("{$ACC_Project.coyname}", obj->getValue("coyname").toString());
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

    mHtml.replace("{$ACC_Project.bankname}", obj->getValue("bankname").toString());
    mHtml.replace("{$ACC_Project.bankiban}", obj->getValue("bankiban").toString());
    mHtml.replace("{$ACC_Project.bankbicswift}", obj->getValue("bankbicswift").toString());
}

/**
 * Set customer sold to- and delivered to addresses, currency
 */
void ACC_SalesInvoiceHtml::setAddress() {
    if (!mCustomer) return;

    int invoiceBranch = mCustomer->getValue("invaddrbranch").toInt();
    RB_ObjectBase* cust = mCustomer;

    if (invoiceBranch == 1) {
        // Invoice to be send to debtormaster or customer main office
        mEndlessLoopCounter = 0;
        cust = getInvoiceAddress(mCustomer);
    }

    RB_String currencyId = cust->getValue("currency_id").toString();
    ACC_SqlCommonFunctions f;
    RB_String currency = f.selectFromWhereId("currcode", "ACC_Currency", currencyId).toString();
    mHtml.replace("{$ACC_Customer.currency}", currency);

    RB_String contactId = mSalesOrder->getValue("contact_idx").toString();

    if (invoiceBranch == 0 || invoiceBranch == 1
            /*!cust->getValue("brpostaddress1").toString().isEmpty()
            || !cust->getValue("brpostaddress2").toString().isEmpty()
            || !cust->getValue("brpostaddress3").toString().isEmpty()
            || !cust->getValue("brpostaddress4").toString().isEmpty()
            || !cust->getValue("brpostaddress5").toString().isEmpty()
            || !cust->getValue("brpostaddress6").toString().isEmpty()
            || contactId.size() < 38 */ ) {

        // Invoice to be send to branch address
        mHtml.replace("{$ACC_Customer.mname}", cust->getValue("mname").toString());
        mHtml.replace("{$ACC_Customer.address1}", cust->getValue("brpostaddress1").toString());
        mHtml.replace("{$ACC_Customer.address2}", cust->getValue("brpostaddress2").toString());
        mHtml.replace("{$ACC_Customer.address3}", cust->getValue("brpostaddress3").toString());
        mHtml.replace("{$ACC_Customer.address4}", cust->getValue("brpostaddress4").toString());
        mHtml.replace("{$ACC_Customer.address5}", cust->getValue("brpostaddress5").toString());
        mHtml.replace("{$ACC_Customer.address6}", cust->getValue("brpostaddress6").toString());

    } else {
        // invoiceBranch == 2  in exceptional case send directly to contact,
        // a private person or company with department name
        RB_ObjectBase* contactObj = ACC_OBJECTFACTORY->newSingleObject("ACC_Contact");
        contactObj->setId(contactId);
        contactObj->dbRead(ACC_MODELFACTORY->getDatabase());

        mHtml.replace("{$ACC_Customer.mname}", contactObj->getValue("firstname").toString() + " "
                      + contactObj->getValue("lastname").toString());
        mHtml.replace("{$ACC_Customer.address1}", contactObj->getValue("primaryaddress1").toString());
        mHtml.replace("{$ACC_Customer.address2}", contactObj->getValue("primaryaddress2").toString());
        mHtml.replace("{$ACC_Customer.address3}", contactObj->getValue("primaryaddress3").toString());
        mHtml.replace("{$ACC_Customer.address4}", contactObj->getValue("primaryaddress4").toString());
        mHtml.replace("{$ACC_Customer.address5}", contactObj->getValue("primaryaddress5").toString());
        mHtml.replace("{$ACC_Customer.address6}", contactObj->getValue("primaryaddress6").toString());

        delete contactObj;
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

    if (cust != mCustomer) {
        delete cust;
        cust = NULL;
    }


}

/**
 * Get customer invoice address
 * @param cust customer branch
 * @return invoice customer branch
 */
RB_ObjectBase* ACC_SalesInvoiceHtml::getInvoiceAddress(RB_ObjectBase* cust) {
    int invoiceHomeOffice = cust->getValue("invaddrbranch").toInt(); // TODO: invaddrbranch is confusing
    RB_String custParentId = cust->getValue("custparent_idx").toString();

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
 * Set order data, such as customer reference
 */
void ACC_SalesInvoiceHtml::setOrderData() {
    if (!mTransDoc || !mDebtorTransList || !mCustomer || !mSalesOrder) {
        mHtml.replace("{$ACC_TransDoc.transno}", "Not based on sales order or ERROR");
        return;
    }

    mHtml.replace("{$ACC_TransDoc.transno}",
                  RB_String::number(mTransDoc->getValue("transno").toInt()));
    RB_String str = "";

    if (!mTransDoc || mTransDoc->getValue("transno").toInt() < 0) {
        str = tr("(View only)");
    } else {
        str = mTransDoc->getValue("transdate").toDate().toString("yyyy-MM-dd");
    }

    mHtml.replace("{$ACC_TransDoc.transdate}", str);
    RB_String id = mSalesOrder->getValue("shipvia_id").toString();
    ACC_SqlCommonFunctions f;
    str = f.selectFromWhereId("shippername", "ACC_Shipper", id).toString(); // TODO: for mass creation load ACC_Shipper
    mHtml.replace("{$ACC_TransDoc.shipvia_id}", str);
    id = mCustomer->getValue("paymentterm_id").toString();
    str = f.selectFromWhereId("terms", "ACC_PaymentTerm", id).toString(); // TODO: for mass creation load ACC_PaymentTerm
    mHtml.replace("{$ACC_PaymentTerm.terms}", str);

    mHtml.replace("{$ACC_TransDoc.customerref}", mSalesOrder->getValue("customerref").toString());
    mHtml.replace("{$ACC_Customer.taxref}", mCustomer->getValue("taxref").toString());
    mHtml.replace("{$ACC_CustBranch.contactname}", mCustomer->getValue("contactname").toString());

    setOrderDetails();

    double totalGrossAmt = mTransDoc->getValue("totalamountrec").toDouble() * mExchangeRate;
    // mNetFreight set during setOrderDetails()
    double totalTax = mTransDoc->getValue("totaltax").toDouble() * mExchangeRate;
    RB_String comment = mTransDoc->getValue("invoicecomment").toString();
    mNetFreight *= mExchangeRate;

    // total price
    mHtml.replace("{$ACC_TransDoc.totalnetprice}",
                  RB_String::number(totalGrossAmt - totalTax - mNetFreight, 'f', 2));
//    mHtml.replace("{$ACC_TransDoc.freightcost}", RB_String::number(mNetFreight, 'f', 2));
    mHtml.replace("{$ACC_TransDoc.tax}", RB_String::number(totalTax, 'f', 2));
    mHtml.replace("{$ACC_TransDoc.totalgrossprice}", RB_String::number(totalGrossAmt, 'f', 2));
    mHtml.replace("{$ACC_TransDoc.invoicecomment}", comment);
}

/**
 * Set order details (line items) and freight cost
 * @param stockMovesList container of line items with sales order details
 */
void ACC_SalesInvoiceHtml::setOrderDetails() {
    if (!mDebtorTransList) return;

    mLineItems.clear();
    RB_String stockCode = "";
    RB_String stockDescr = "";
    double qtyDisp = 0.0;
    RB_String uom = "";
    double price = 0.0;
    double discount = 0.0;
    RB_String narr = "";

    RB_ObjectIterator* iter = mDebtorTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* dt = iter->currentObject();

        stockCode = dt->getDValue("stk_idx").toString();
        stockDescr = dt->getValue("description").toString();
        price = dt->getValue("unitprice").toDouble() * mExchangeRate;
        qtyDisp = dt->getValue("qtydispatched").toDouble();
        uom = dt->getValue("stkuom").toString();
        discount = dt->getValue("discountpercent").toDouble();
        narr = dt->getValue("narrative").toString();
        if (dt->getValue("orderdetail_id").toString() != "freight") {
            setLineItem(stockCode, stockDescr, price, qtyDisp, uom, discount, narr);
        } else {
            mNetFreight = dt->getValue("amount").toDouble();
        }
    }

    delete iter;

    mHtml.replace("{$ACC_TransDoc.lineitems}", mLineItems);
}

/**
 * Set an invoice line item
 * @param stockCode stock item code
 * @param description description of stock item
 * @param unitPrice unit price of stock item
 * @param qtyDispatched quantity dispatched
 * @param unit unit of measurement
 * @param discount discount on this sales order line
 * @param narrative narrative with sales order line
 */
void ACC_SalesInvoiceHtml::setLineItem(const RB_String& stockCode,
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
}

