/*****************************************************************
 * $Id: acc_purchaseorderhtml.cpp 2061 2014-01-12 16:34:14Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_purchaseorderhtml.h"

#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_PurchaseOrderHtml::ACC_PurchaseOrderHtml() {
    mHtml = "";
    mLineItems = "";
    mTransDoc = NULL;
    mCreditorTransList = NULL;
    mSupplier = NULL;
    mPurchaseOrder = NULL;
    // mNetFreight
    mIsCheckPrint = true;
    mOrderNumber = "";
}

/**
 * Destructor
 */
ACC_PurchaseOrderHtml::~ACC_PurchaseOrderHtml() {
    delete mSupplier;
    delete mPurchaseOrder;
    delete mLocation;
    RB_DEBUG->print("ACC_PurchaseOrderHtml::~ACC_PurchaseOrderHtml() OK");
}

/**
 * Execute operation, use getHtml() to get the created HTML
 * @param transDoc transaction document (or invoice to be)
 * @param ctList creditor transaction list with line items of document
 */
bool ACC_PurchaseOrderHtml::execute(RB_ObjectBase* transDoc, RB_ObjectContainer* ctList) {
    mTransDoc = transDoc;
    mCreditorTransList = ctList;

    // Get supplier data for reference only
    RB_String id = transDoc->getIdValue("creditor_idx").toString();

    if (id.isEmpty() || id == "0") {
        return false;
    }

    mSupplier = ACC_OBJECTFACTORY->newSingleObject("ACC_Supplier");
    mSupplier->setId(id);
    mSupplier->dbRead(ACC_MODELFACTORY->getDatabase());

    // Get purchase order data for reference only
    id = transDoc->getValue("order_id").toString();

    if (id.isEmpty() || id == "0") {
        return false;
    }

    mPurchaseOrder = ACC_OBJECTFACTORY->newSingleObject("ACC_PurchaseOrder");
    mPurchaseOrder->setId(id);
    mPurchaseOrder->dbRead(ACC_MODELFACTORY->getDatabase());

    // Get receipt location data for location name
    mLocation = ACC_OBJECTFACTORY->newSingleObject("ACC_Location");
    mLocation->setId(mPurchaseOrder->getValue("receiptat_id").toString());
    mLocation->dbRead(ACC_MODELFACTORY->getDatabase());

    return true;
}

/**
 * Set flag isCheckPrint to true will show the tax and freight as P.M.
 * (Pro Memorie) on the order
 * @param isCheckPrint
 */
void ACC_PurchaseOrderHtml::setIsCheckPrint(bool isCheckPrint) {
    mIsCheckPrint = isCheckPrint;
}

/**
 * Get invoice in HTML
 * @returns HTML table with invoice page(s)
 */
RB_String ACC_PurchaseOrderHtml::getHtml() {
    if (!mTransDoc || !mCreditorTransList || !mSupplier || !mPurchaseOrder) {
        mHtml = tr("Not based on purchase order or data error.");
        return mHtml;
    }

    QFile file(":/resources/purchaseorder_table_te.html");
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
void ACC_PurchaseOrderHtml::setCompanyData() {
    RB_ObjectBase* obj = ACC_MODELFACTORY->getRoot();
    // Logo: html
    mHtml.replace("{$ACC_Project.logo}", obj->getValue("logo").toString());

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

//    mHtml.replace("{$ACC_Project.bankname}", obj->getValue("bankname").toString());
//    mHtml.replace("{$ACC_Project.bankiban}", obj->getValue("bankiban").toString());
//    mHtml.replace("{$ACC_Project.bankbicswift}", obj->getValue("bankbicswift").toString());
}

/**
 * Set customer sold to- and delivered to addresses
 */
void ACC_PurchaseOrderHtml::setAddress() {
    RB_ObjectBase* obj = mSupplier;

    // Purchase order to be send to supplier address
    mHtml.replace("{$ACC_Supplier.suppname}", obj->getValue("suppname").toString());
    mHtml.replace("{$ACC_Supplier.address1}", obj->getValue("address1").toString());
    mHtml.replace("{$ACC_Supplier.address2}", obj->getValue("address2").toString());
    mHtml.replace("{$ACC_Supplier.address3}", obj->getValue("address3").toString());
    mHtml.replace("{$ACC_Supplier.address4}", obj->getValue("address4").toString());
    mHtml.replace("{$ACC_Supplier.address5}", obj->getValue("address5").toString());
    mHtml.replace("{$ACC_Supplier.address6}", obj->getValue("address6").toString());

    RB_String currencyId = obj->getValue("currency_id").toString();
    ACC_SqlCommonFunctions f;
    RB_String currency = f.selectFromWhereId("currcode", "ACC_Currency", currencyId).toString();
    mHtml.replace("{$ACC_Supplier.currency}", currency);

    // Receipt address
    mHtml.replace("{$ACC_Location.locationname}", mLocation->getValue("locationname").toString());
    mHtml.replace("{$ACC_PurchaseOrder.deladd1}", mPurchaseOrder->getValue("deladd1").toString());
    mHtml.replace("{$ACC_PurchaseOrder.deladd2}", mPurchaseOrder->getValue("deladd2").toString());
    mHtml.replace("{$ACC_PurchaseOrder.deladd3}", mPurchaseOrder->getValue("deladd3").toString());
    mHtml.replace("{$ACC_PurchaseOrder.deladd4}", mPurchaseOrder->getValue("deladd4").toString());
    mHtml.replace("{$ACC_PurchaseOrder.deladd5}", mPurchaseOrder->getValue("deladd5").toString());
    mHtml.replace("{$ACC_PurchaseOrder.deladd6}", mPurchaseOrder->getValue("deladd6").toString());
}

/**
 * Set order data, such as supplier reference
 */
void ACC_PurchaseOrderHtml::setOrderData() {
    if (!mTransDoc || !mCreditorTransList || !mSupplier || !mPurchaseOrder) {
        mHtml.replace("{$ACC_TransDoc.transno}", "ERROR");
        return;
    }

    mOrderNumber = mPurchaseOrder->getValue("orderno").toString();
    mHtml.replace("{$ACC_PurchaseOrder.orderno}", mOrderNumber);

    RB_String str = mPurchaseOrder->getValue("orderdate").toString();
    mHtml.replace("{$ACC_PurchaseOrder.orderdate}", str);

    RB_String id = mPurchaseOrder->getValue("shipvia_id").toString();
    ACC_SqlCommonFunctions f;
    str = f.selectFromWhereId("shippername", "ACC_Shipper", id).toString();
    mHtml.replace("{$ACC_TransDoc.shipvia_id}", str);
    id = mSupplier->getValue("paymentterm_id").toString();
    str = f.selectFromWhereId("terms", "ACC_PaymentTerm", id).toString();
    mHtml.replace("{$ACC_PaymentTerm.terms}", str);

    mHtml.replace("{$ACC_PurchaseOrder.requisitionno}", mPurchaseOrder->getValue("requisitionno").toString());
    RB_ObjectBase* obj = ACC_MODELFACTORY->getRoot();
    mHtml.replace("{$ACC_Project.gstno}", obj->getValue("gstno").toString());
    // TODO: responsible Procurement Manager
//    mHtml.replace("{$ACC_PurchaseOrder.contactname}", mPurchaseOrder->getValue("contactname").toString());

    setOrderDetails();

    double totalGrossAmt = mTransDoc->getValue("totalamountpay").toDouble();
//    double netFreight = 0.0;
    double totalTax = mTransDoc->getValue("totaltax").toDouble();
    RB_String comment = mTransDoc->getValue("invoicecomment").toString();

    // total price
    mHtml.replace("{$ACC_TransDoc.totalnetprice}",
                  RB_String::number(totalGrossAmt - totalTax /*- netFreight*/, 'f', 2));
    if (!mIsCheckPrint) {
        mHtml.replace("{$ACC_TransDoc.freightcost}", "-" /*RB_String::number(netFreight, 'f', 2)*/);
        mHtml.replace("{$ACC_TransDoc.tax}", RB_String::number(totalTax, 'f', 2));
    } else {
        mHtml.replace("{$ACC_TransDoc.freightcost}", "P.M.");
        mHtml.replace("{$ACC_TransDoc.tax}", "P.M.");
    }
    mHtml.replace("{$ACC_TransDoc.totalgrossprice}",
                  /* RB_String::number(totalGrossAmt, 'f', 2)); */
                  RB_String::number(totalGrossAmt - totalTax /*- netFreight*/, 'f', 2));
    mHtml.replace("{$ACC_TransDoc.invoicecomment}", comment);
}

/**
 * Set order details (line items) and freight cost
 * @param stockMovesList container of line items with sales order details
 */
void ACC_PurchaseOrderHtml::setOrderDetails() {
    if (!mCreditorTransList) return;

    mLineItems.clear();
    RB_String stockCode = "";
    RB_String stockDescr = "";
    double qtyDisp = 0.0;
    RB_String uom = "";
    double price = 0.0;
    double discount = 0.0;
    RB_String narr = "";

    RB_ObjectIterator* iter = mCreditorTransList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* ct = iter->currentObject();

        stockCode = ct->getDValue("stk_idx").toString(); // _idx
        stockDescr = ct->getValue("description").toString();
        price = ct->getValue("unitprice").toDouble();
        qtyDisp = ct->getValue("qtyreceived").toDouble();
        uom = ct->getValue("stkuom").toString();
        discount = ct->getValue("discountpercent").toDouble();
        narr = ct->getValue("narrative").toString();

        setLineItem(stockCode, stockDescr, price, qtyDisp, uom, discount, narr);
    }

    delete iter;

    mHtml.replace("{$ACC_TransDoc.lineitems}", mLineItems);
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
void ACC_PurchaseOrderHtml::setLineItem(const RB_String& stockCode,
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
    double itemPrice = unitPrice * (1.0 - discount/100.0);
    mLineItems += RB_String::number(itemPrice, 'f', 2);
    mLineItems += "</td><td width=\"7%\" align=\"right\">";
    mLineItems += RB_String::number(qtyDispatched);
    mLineItems += "</td><td width=\"7%\" align=\"center\">";
    mLineItems += unit;
    mLineItems += "</td><td width=\"17%\" align=\"right\">";
    double linePrice = unitPrice * qtyDispatched * (1.0 - discount/100.0);
    mLineItems += RB_String::number(linePrice, 'f', 2);
    mLineItems += "</td></tr><tr><td width=\"20%\">&nbsp;<br />&nbsp;</td><td width=\"*\" colspan=\"4\">";
    mLineItems += narrative;
    mLineItems += "</td><td width=\"15%\">&nbsp;</td></tr></table>";
}

