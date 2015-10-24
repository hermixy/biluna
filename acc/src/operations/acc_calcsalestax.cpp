/*****************************************************************
 * $Id: acc_calcsalestax.cpp 2211 2015-01-24 10:19:33Z rutger $
 * Created: Nov 18, 2010 5:52:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_calcsalestax.h"

#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_CalcSalesTax::ACC_CalcSalesTax() {
    mFromPrd = 0;
    mToPrd = 0;
    mIsSummaryOnly = true;
    mRootId = "";
}

/**
 * Destructor
 */
ACC_CalcSalesTax::~ACC_CalcSalesTax() {

}

/**
 * Create the tax amounts from the ACC_GlTrans table
 * @param taxList container which will contain the amounts related to tax
 * @return true on success
 */
bool ACC_CalcSalesTax::execute(RB_ObjectBase* taxList) {
    if (!taxList) return false;

    taxList->setName("ACC_GlTransList"); // to make sure

    RB_String qStr = "";
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    // 1a
    //
    // Tax of sales payable high, function 22
    setQuery(qStr, ACC2::ControlTaxPayHigh);
    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_CalcSalesTax::execute() "
                        + query.lastError().text() + " ERROR");
        return false;
    }
    addObject(taxList, query, ACC2::ControlTaxPayHigh);

    // Tax of sales payable globalisation high, function 27
    setQuery(qStr, ACC2::ControlTaxPayGlobHigh);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPayGlobHigh);

    // Sales amount with high tax, function 12
    setQuery(qStr, ACC2::ControlSalesHighTax);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesHighTax);

    // Sales globalisation high tax, function 92
    setQuery(qStr, ACC2::ControlSalesGlobHigh);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesGlobHigh);

    // Purchase globalisation high tax, function 95
    setQuery(qStr, ACC2::ControlPurchGlobHigh);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchGlobHigh);

    // 1b
    //
    // Tax payable low, function 21
    setQuery(qStr, ACC2::ControlTaxPayLow);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPayLow);

    // Tax payable globalisation low, function 26
    setQuery(qStr, ACC2::ControlTaxPayGlobLow);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPayGlobLow);

    // Sales tax low, function 11
    setQuery(qStr, ACC2::ControlSalesLowTax);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesLowTax);

    // Sales globalisation low, function 91
    setQuery(qStr, ACC2::ControlSalesGlobLow);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesGlobLow);

    // Purchase globalisation low, function 94
    setQuery(qStr, ACC2::ControlPurchGlobLow);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchGlobLow);

    // 1c
    //
    // Tax payable other, function 23
    setQuery(qStr, ACC2::ControlTaxPayOther);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPayOther);

    // Tax payable globalisation other, function 28
    setQuery(qStr, ACC2::ControlTaxPayGlobOther);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPayGlobOther);

    // Sales tax other, function 13
    setQuery(qStr, ACC2::ControlSalesOtherTax);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesOtherTax);

    // Sales globalisation other, function 93
    setQuery(qStr, ACC2::ControlSalesGlobOther);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesGlobOther);

    // Purchase globalisation other, function 96
    setQuery(qStr, ACC2::ControlPurchGlobOther);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchGlobOther);

    // 1d
    //
    // Tax private use, function 29
    setQuery(qStr, ACC2::ControlTaxPrivateUse);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPrivateUse);

    // Private use, function 19
    setQuery(qStr, ACC2::ControlPrivateUseTax);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPrivateUseTax);

    // 1e
    //
    // Sales tax no tax, function 10
    setQuery(qStr, ACC2::ControlSalesNoTax);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesNoTax);

    // Sales with reversed tax, function 14
    setQuery(qStr, ACC2::ControlSalesRevTax);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesRevTax);

    // 2a
    //
    // Tax purchase, receivable reversed, function 45
    setQuery(qStr, ACC2::ControlTaxPurchRecRev);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPurchRecRev);

    // Purchase tax receivable reversed, function 34
    setQuery(qStr, ACC2::ControlPurchaseRevTax);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchaseRevTax);

    // 3a
    //
    // Sales export outside EU, function 81
    setQuery(qStr, ACC2::ControlSalesOutEu);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesOutEu);

    // 3b
    //
    // Sales export inside EU, function 80
    setQuery(qStr, ACC2::ControlSalesInEu);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlSalesInEu);

    // Sales service inside EU, function 88
    setQuery(qStr, ACC2::ControlServiceInEu);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlServiceInEu);

    // 3c
    //
    // Sales installation/tele sales inside EU, function 84
    setQuery(qStr, ACC2::ControlInstallInEu);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlInstallInEu);

    // 4a
    //
    // Tax Purchase from outside EU, function 76
    setQuery(qStr, ACC2::ControlTaxPurchOutEu);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPurchOutEu);

    // Purchase with low tax from outside EU, function 71
    setQuery(qStr, ACC2::ControlPurchOutEuLow);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchOutEuLow);

    // Purchase with high tax from outside EU, function 72
    setQuery(qStr, ACC2::ControlPurchOutEuHigh);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchOutEuHigh);

    // Purchase with other tax from outside EU, function 73
    setQuery(qStr, ACC2::ControlPurchOutEuOther);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchOutEuOther);

    // 4b
    //
    // Purchase with low tax from inside EU, function 66
    setQuery(qStr, ACC2::ControlTaxPurchInEu);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxPurchInEu);

    // Purchase with low tax from inside EU, function 61
    setQuery(qStr, ACC2::ControlPurchInEuLow);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchInEuLow);

    // Purchase with high tax from outside EU, function 62
    setQuery(qStr, ACC2::ControlPurchInEuHigh);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchInEuHigh);

    // Purchase with other tax from inside EU, function 63
    setQuery(qStr, ACC2::ControlPurchInEuOther);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlPurchInEuOther);

    // 5b
    //
    // Purchase tax low, function 41
    setQuery(qStr, ACC2::ControlTaxRecLow);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxRecLow);

    // Purchase tax high, function 42
    setQuery(qStr, ACC2::ControlTaxRecHigh);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxRecHigh);

    // Purchase tax other, function 43
    setQuery(qStr, ACC2::ControlTaxRecOther);
    query.exec(qStr);
    addObject(taxList, query, ACC2::ControlTaxRecOther);

    return true;
}

/**
 * Set period
 * @param fromPrd starting period and including (period = year * 100 * month)
 * @param toPrd ending period and including (period = year * 100 * month)
 */
void ACC_CalcSalesTax::setPeriod(const QDateTime& fromDate,
                                 const QDateTime& toDate, bool sumOnly) {
    mFromPrd = fromDate.date().year() * 100 + fromDate.date().month();
    mToPrd = toDate.date().year() * 100 + toDate.date().month();
    mIsSummaryOnly = sumOnly;
    mRootId = ACC_MODELFACTORY->getRootId();
}

/**
 * Set query for account control item (type of sales tax)
 * TODO: select for taxauth_id, should be Dutch
 * @param qStr query to be set
 * @param type account control number, refer to ACC2::ControlType
 */
void ACC_CalcSalesTax::setQuery(RB_String& qStr, ACC2::ControlType type) {
    ACC_SqlCommonFunctions util;
    if (mIsSummaryOnly) {
        qStr =
            "SELECT acc_gltrans.*, '"
                + RB_String::number((int)type) + "' AS xaccountcontrol, "
                "SUM(acc_gltrans.amount) AS xtotalamount "
            "FROM acc_gltrans INNER JOIN acc_chartmaster "
                "ON acc_chartmaster.id=";
        util.substrIdxId("acc_gltrans.chartmaster_idx", qStr);
        qStr += " "
            "WHERE acc_chartmaster.parent='" + mRootId
                + "' AND acc_chartmaster.accountcontrol=" + RB_String::number((int)type)
                + " AND acc_gltrans.periodno>=" + RB_String::number(mFromPrd)
                + " AND acc_gltrans.periodno<=" + RB_String::number(mToPrd)
                + " GROUP BY acc_chartmaster.parent" + ";";
        // this last (dummy) GROUP BY is required because the SQL is both
        // a non-aggregate (normal acc_gltrans.*) plus aggregate (SUM()) statement
    } else {
        qStr =
            "SELECT acc_gltrans.*, '"
                + RB_String::number((int)type) + "' AS xaccountcontrol "
            "FROM acc_gltrans  INNER JOIN acc_chartmaster "
                "ON acc_chartmaster.id=";
        util.substrIdxId("acc_gltrans.chartmaster_idx", qStr);
        qStr += " "
            "WHERE acc_chartmaster.parent='" + mRootId
                + "' AND acc_chartmaster.accountcontrol=" + RB_String::number((int)type)
                + " AND acc_gltrans.periodno>=" + RB_String::number(mFromPrd)
                + " AND acc_gltrans.periodno<=" + RB_String::number(mToPrd) + ";";
    }
}

/**
 * Add tax (report) object based on the query of the GL transactions
 * @param taxList container with all GL transactions
 * @param q query with the relevant records
 */
void ACC_CalcSalesTax::addObject(RB_ObjectBase *taxList, QSqlQuery &query,
                                 ACC2::ControlType type) {
    while (query.next()) {
        RB_ObjectBase* obj = ACC_OBJECTFACTORY->newObject("", taxList);
        obj->setValue("id", query.value("id"));
        obj->setValue("parent", query.value("parent"));
        obj->setValue("name", query.value("name"));
        obj->setValue("type", query.value("type"));
        obj->setValue("transdocno", query.value("transdocno"));
        obj->setValue("chequeno", query.value("chequeno"));
        obj->setValue("transdate", query.value("transdate"));
        obj->setValue("periodno", query.value("periodno"));
        obj->setValue("description", query.value("description"));
        obj->setValue("chartmaster_idx", query.value("chartmaster_idx"));
        // Do not use query.value(14) in below setValue()
        // because the line will not show on the tax report if no record
        // 21 is extra field, see above creation of query
        obj->setValue("accountcontrol", query.value("xaccountcontrol"));
        if (mIsSummaryOnly) {
            // 22 is extra field, see above creation of query
            obj->setValue("amount", query.value("xtotalamount"));
        } else {
            obj->setValue("amount", query.value("amount"));
        }
        obj->setValue("posted", query.value("posted"));
        obj->setValue("jobref_id", query.value("jobref_id"));
        obj->setValue("tag_id", query.value("tag_id"));
        obj->setValue("transdoc_id", query.value("transdoc_id"));
        obj->setValue("itemtrans_id", query.value("itemtrans_id"));
        obj->setValue("transallocn_idx", query.value("transallocn_idx"));
        obj->setValue("costcenter_idx", query.value("costcenter_idx"));
    }

    // Make sure one object exists
    if (!query.first()) {
        addDummyObject(taxList, type);
    }
}

/**
 * Add dummy (report) object based with control type for tax summary
 * @param taxList container with all GL transactions
 * @param q query with the relevant records
 */
void ACC_CalcSalesTax::addDummyObject(RB_ObjectBase *taxList, ACC2::ControlType type) {
    if (!mIsSummaryOnly) {
        return;
    }

    RB_ObjectBase* obj = ACC_OBJECTFACTORY->newObject("", taxList);
    obj->setValue("accountcontrol", (int)type);
    obj->setValue("amount", 0);
}
