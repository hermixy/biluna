/*****************************************************************
 * $Id: acc_createorderreport.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Feb 22, 2012 17:00:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_createorderreport.h"

#include "acc_modelfactory.h"
#include "acc_sqlcommonfunctions.h"


/**
 * Constructor
 */
ACC_CreateOrderReport::ACC_CreateOrderReport() {
    mIsPurchaseOrder = false;
    mIsAllOrders = false;
}

/**
 * Destructor
 */
ACC_CreateOrderReport::~ACC_CreateOrderReport() {

}

void ACC_CreateOrderReport::setPurchaseOrder() {
    mIsPurchaseOrder = true;
}

/**
 * Set to show all order, default is open orders only
 */
void ACC_CreateOrderReport::setShowAllOrders() {
    mIsAllOrders = true;
}

/**
 * Create the order list
 * @param orderList container which will contain the order (details) objects
 * @return true on success
 */
bool ACC_CreateOrderReport::execute(RB_ObjectBase* orderList) {
    if (!orderList) return false;

    /*
    INCLUDED: discountpercent for correct calculation of total price value
    SELECT `acc_salesorder`.`orderno`,  `acc_salesorder`.`deliverto`,  `acc_salesorder`.`customerref`, `acc_salesorder`.`orddate`, `acc_salesorder`.`deliverydate`,
    SUBSTR(`acc_salesorderdetail`.`stk_idx`, 39) as stk_idx, `acc_salesorderdetail`.`stkdescription`, `acc_salesorderdetail`.`stkuom`, `acc_salesorderdetail`.`unitprice`,
    `acc_salesorderdetail`.`discountpercent`, `acc_salesorderdetail`.`quantity`, `acc_salesorderdetail`.`qtyinvoiced`, `acc_salesorderdetail`.`qtydispatched`, `acc_salesorderdetail`.`itemdue`
    FROM `acc_salesorder` INNER JOIN `acc_salesorderdetail` ON `acc_salesorder`.`id`=`acc_salesorderdetail`.`parent`
    WHERE `acc_salesorder`.`orddate`>='0000-00-00 00:00:00'
    AND `acc_salesorder`.`orddate`<='0000-00-00T00:00:00'
    AND  `acc_salesorder`.`orderno`>='13000039' // for testing only
    ORDER BY `acc_salesorder`.`orderno` ASC;

    orderno
    deliverto
    customerref
    orddate
    deliverydate
    stk_idx
    stkdescription
    stkuom
    unitprice
    discountpercent
    quantity
    qtyinvoiced
    qtydispatched
    itemdue
    */
    RB_String onlyOpenOrders = "";

    if (!mIsAllOrders) {
        if (!mIsPurchaseOrder) {
            onlyOpenOrders = "AND `acc_salesorderdetail`.`quantity` "
                    "- `acc_salesorderdetail`.`qtyinvoiced` <> 0 ";
        } else {
            onlyOpenOrders = "AND `acc_purchaseorderdetail`.`quantity` "
                    "- `acc_purchaseorderdetail`.`qtyinvoiced` <> 0 ";
        }
    }

    ACC_SqlCommonFunctions util;
    RB_String qStr = "";

    if (!mIsPurchaseOrder) {
        qStr = "SELECT `acc_salesorder`.`orderno`,  `acc_salesorder`.`deliverto`, "
                    "`acc_salesorder`.`customerref`, `acc_salesorder`.`orddate`, "
                    "`acc_salesorder`.`deliverydate`, ";
        util.substrIdxContent("acc_salesorderdetail.stk_idx", qStr);
        qStr += " as stk_idx, "
                    "`acc_salesorderdetail`.`stkdescription`, "
                    "`acc_salesorderdetail`.`stkuom`, `acc_salesorderdetail`.`unitprice`, "
                    "`acc_salesorderdetail`.`discountpercent`, `acc_salesorderdetail`.`quantity`, "
                    "`acc_salesorderdetail`.`qtyinvoiced`, `acc_salesorderdetail`.`qtydispatched`, "
                    "`acc_salesorderdetail`.`itemdue` "
                "FROM `acc_salesorder` "
                "INNER JOIN `acc_salesorderdetail` ON `acc_salesorder`.`id`=`acc_salesorderdetail`.`parent` "
                "INNER JOIN `acc_customer` ON `acc_customer`.`id`=`acc_salesorder`.`parent` "
                "WHERE `acc_salesorder`.`orddate`>='" + mFromPrd + "' "
                    "AND `acc_salesorder`.`orddate`<='" + mToPrd + "' "
                    "AND `acc_customer`.`parent`='" + ACC_MODELFACTORY->getRootId() + "' "
                    + onlyOpenOrders +
                "ORDER BY `acc_salesorder`.`orderno` ASC; ";
    } else {
        // NOTE: if acc_location for the purchase order is not completed, the row will not show

        qStr = "SELECT `acc_purchaseorder`.`orderno`,  `acc_supplier`.`suppname`, "
                    "`acc_purchaseorder`.`supplierref`, `acc_purchaseorder`.`orderdate`, "
                    "`acc_purchaseorder`.`receiptdate`, ";
        util.substrIdxContent("acc_purchaseorderdetail.stk_idx", qStr);
        qStr += " as stk_idx, "
                    "`acc_purchaseorderdetail`.`stkdescription`, "
                    "`acc_purchaseorderdetail`.`stkuom`, `acc_purchaseorderdetail`.`unitprice`, "
                    "`acc_purchaseorderdetail`.`discountpercent`, `acc_purchaseorderdetail`.`quantity`, "
                    "`acc_purchaseorderdetail`.`qtyinvoiced`, `acc_purchaseorderdetail`.`qtyreceived`, "
                    "`acc_purchaseorderdetail`.`itemdue` "
                "FROM `acc_purchaseorder` "
                "INNER JOIN `acc_purchaseorderdetail` ON `acc_purchaseorder`.`id`=`acc_purchaseorderdetail`.`parent` "
                "INNER JOIN `acc_location` ON `acc_purchaseorder`.`receiptat_id`=`acc_location`.`id` "
                "INNER JOIN `acc_supplier` ON `acc_supplier`.`id`=`acc_purchaseorder`.`parent` "
                "WHERE `acc_purchaseorder`.`orderdate`>='" + mFromPrd + "' "
                    "AND `acc_purchaseorder`.`orderdate`<='" + mToPrd + "' "
                    "AND `acc_supplier`.`parent`='" + ACC_MODELFACTORY->getRootId() + "' "
                    + onlyOpenOrders +
                "ORDER BY `acc_purchaseorder`.`orderno` ASC; ";
    }


    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateOrderReport::execute() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    while (query.next()) {
        RB_ObjectBase* obj = new RB_ObjectAtomic("", orderList, "ACC_Order");
        orderList->addObject(obj);
        obj->addMember("orderno", "-", query.value(0), RB2::MemberChar125);
        obj->addMember("delivertofrom", "-", query.value(1), RB2::MemberChar125);
        obj->addMember("custsuppref", "-", query.value(2), RB2::MemberChar125);
        obj->addMember("orddate", "-", query.value(3), RB2::MemberChar125);
        obj->addMember("deliverydate", "-", query.value(4), RB2::MemberChar125);
        obj->addMember("stk_idx", "-", query.value(5), RB2::MemberChar125);
        obj->addMember("stkdescription", "-", query.value(6), RB2::MemberChar125);
        obj->addMember("stkuom", "-", query.value(7), RB2::MemberChar125);
        obj->addMember("unitprice", "-", query.value(8), RB2::MemberDouble);
        obj->addMember("discountpercent", "-", query.value(9), RB2::MemberDouble);
        obj->addMember("quantity", "-", query.value(10), RB2::MemberDouble);
        obj->addMember("qtyinvoiced", "-", query.value(11), RB2::MemberDouble);
        obj->addMember("qtydispatched", "-", query.value(12), RB2::MemberDouble);
        obj->addMember("itemdue", "-", query.value(13), RB2::MemberChar125);
    }

    return true;
}

/**
 * Set period
 * @param fromPrd starting period and including (period = year * 100 * month)
 * @param toPrd ending period and including (period = year * 100 * month)
 */
void ACC_CreateOrderReport::setPeriod(const RB_String& fromPrd, const RB_String& toPrd) {
    mFromPrd = fromPrd;
    mToPrd = toPrd + "-32"; // to make sure all days of the month are included
}
