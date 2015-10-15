/*****************************************************************
 * $Id: acc_createbankpaymentlist.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 14, 2015 21:17:25 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "acc_createbankpaymentlist.h"

#include "acc_modelfactory.h"
#include "acc_sqlcommonfunctions.h"


/**
 * Constructor
 */
ACC_CreateBankPaymentList::ACC_CreateBankPaymentList() {
    mObject = NULL;
}

/**
 * Destructor
 */
ACC_CreateBankPaymentList::~ACC_CreateBankPaymentList() {
    // nothing
}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool ACC_CreateBankPaymentList::execute(RB_ObjectContainer* licenseList) {
    ACC_SqlCommonFunctions sqlFunc;

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (mField.contains("last",Qt::CaseInsensitive)) {
//        sqlFunc.selectLastNameData(query, mFieldFrom, mFieldTo);
    } else if (mField.contains("end",Qt::CaseInsensitive)) {
//        sqlFunc.selectLicenseEndData(query, mFieldFrom, mFieldTo);
    } else if (mField.contains("modified",Qt::CaseInsensitive)) {
//        sqlFunc.selectModifiedData(query, mFieldFrom, mFieldTo);
    } else {
        RB_DEBUG->error("ACC_CreateBankPaymentList::execute() error");
        return false;
    }

     /*
     * SQL:
     * 0 - contactname
     * 1 - licensecode
     * 2 - licensestart
     * 3 - licenseend
     * 4 - modified
     * 5 - customer
     * 6 - ordernumber
     * 7 - comment
     */

    RB_String contactName = "";
    RB_String licenseCode = "";
    RB_String licenseStart = "";
    RB_String licenseEnd = "";
    RB_String modified = "";
    RB_String customer = "";
    RB_String orderNumber = "";
    RB_String comment = "";

    while (query.next()) {
        contactName = query.value(0).toString();
        licenseCode = query.value(1).toString();
        licenseStart = query.value(2).toString();
        licenseEnd = query.value(3).toString();
        modified = query.value(4).toString();
        customer = query.value(5).toString();
        orderNumber = query.value(6).toString();
        comment = query.value(7).toString();

        RB_ObjectBase* obj = new RB_ObjectAtomic("", licenseList, "ACC_LicenseData");
        licenseList->addObject(obj);
        obj->addMember("contactname", "-", contactName, RB2::MemberChar125);
        obj->addMember("licensecode", "-", licenseCode, RB2::MemberChar125);
        obj->addMember("licensestart", "-", licenseStart, RB2::MemberChar125);
        obj->addMember("licenseend", "-", licenseEnd, RB2::MemberChar125);
        obj->addMember("modifieddate", "-", modified, RB2::MemberChar125);
        obj->addMember("customer", "-", customer, RB2::MemberChar125);
        obj->addMember("ordernumber", "-", orderNumber, RB2::MemberChar125);
        obj->addMember("comment", "-", comment, RB2::MemberChar255);
    }

    return true;
}

/**
 * Execute operation
 * @param input input object, if applicable
 * @param output result/output object, if applicable
 * @return true on success, default false
 */
bool ACC_CreateBankPaymentList::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "ACC_CreateBankPaymentList::execute() "
                    "not implemented ERROR");
    return false;
}

