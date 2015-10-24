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
    mProjectId = "";
    mDateFrom = "";
    mDateTo = "";
    mBankAccountNumber = "";
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
bool ACC_CreateBankPaymentList::execute(RB_ObjectContainer* paymentList) {
    if (!paymentList || mProjectId.isEmpty() || mDateFrom.isEmpty()
            || mDateTo.isEmpty()) {
        RB_DEBUG->error("ACC_CreateBankPaymentList::execute() ERROR");
        return false;
    }

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    ACC_SqlCommonFunctions sqlFunc;
    sqlFunc.getBankPaymentList(query, mProjectId, mDateFrom, mDateTo,
                               mBankAccountNumber);

     /*
     * SQL:
     * 0 - description
     * 1 - SUBSTR(chartmaster_idx, 39) as GL
     * 2 - amount
     * 3 - transno
     * 4 - amountcleared as cleared
     * 5 - SUBSTR(transdate, 1, 10) as transdate
     */

    RB_String descr = "";
    RB_String gl = "";
    RB_String amount = "";
    RB_String transno = "";
    RB_String cleared = "";
    RB_String transdate = "";

    while (query.next()) {
        descr = query.value(0).toString();
        gl = query.value(1).toString();
        amount = query.value(2).toString();
        transno = query.value(3).toString();
        cleared = query.value(4).toString();
        transdate = query.value(5).toString();

        RB_ObjectBase* obj = new RB_ObjectAtomic("", paymentList, "ACC_BankPayment");
        paymentList->addObject(obj);
        obj->addMember("descr", "-", descr, RB2::MemberChar255);
        obj->addMember("gl", "-", gl, RB2::MemberChar125);
        obj->addMember("amount", "-", amount, RB2::MemberDouble);
        obj->addMember("transno", "-", transno, RB2::MemberChar125);
        obj->addMember("cleared", "-", cleared, RB2::MemberDouble);
        obj->addMember("transdate", "-", transdate, RB2::MemberChar40);
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
