/*****************************************************************
 * $Id: acc_invoiceaccrued.cpp 2210 2015-01-22 14:59:25Z rutger $
 * Created: Jan 11, 2015 - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "acc_invoiceaccrued.h"

#include "acc_modelfactory.h"
#include "acc_sqlcommonfunctions.h"


/**
 * Constructor
 */
ACC_InvoiceAccrued::ACC_InvoiceAccrued() {
    mIsDebtor = true;
    mEndDate.setDate(1970, 1, 1);
}

/**
 * Destructor
 */
ACC_InvoiceAccrued::~ACC_InvoiceAccrued() {
    // nothing
}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool ACC_InvoiceAccrued::execute(RB_ObjectContainer* invoiceAccruedList) {
    ACC_SqlCommonFunctions sqlFunc;

    QDate startDate = mEndDate;
    startDate = startDate.addYears(-1);
    startDate = startDate.addDays(1);

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (mIsDebtor) {
        sqlFunc.getDebtorAccrued(query, startDate, mEndDate);
    } else {
        sqlFunc.getCreditorAccrued(query, startDate, mEndDate);
    }

     /*
     * SQL:
     * 0 - transdocdate
     * 1 - transno
     * 2 - description
     * 3 - totalamountrec
     * 4 - totalamountpay
     * 5 - dateallocdoc
     * 6 - transnoallocdoc
     * 7 - allocatedamount
     */

    RB_String transDocDate = "";
    RB_String transNo = "";
    RB_String description = "";
    RB_String totalAmountRec = "";
    RB_String totalAmountPay = "";
    RB_String dateallocdoc = "";
    RB_String transnoallocdoc = "";
    RB_String allocatedAmount = "";

    while (query.next()) {
        transDocDate = query.value(0).toString();
        transNo = query.value(1).toString();
        description = query.value(2).toString();
        totalAmountRec = query.value(3).toString();
        totalAmountPay = query.value(4).toString();
        dateallocdoc = query.value(5).toString();
        transnoallocdoc = query.value(6).toString();
        allocatedAmount = query.value(7).toString();

        RB_ObjectBase* obj = new RB_ObjectAtomic("", invoiceAccruedList, "ACC_InvoiceAccrued");
        invoiceAccruedList->addObject(obj);
        obj->addMember("transdocdate", "-", transDocDate, RB2::MemberChar125);
        obj->addMember("transno", "-", transNo, RB2::MemberChar125);
        obj->addMember("description", "-", description, RB2::MemberChar125);
        obj->addMember("totalamountrec", "-", totalAmountRec, RB2::MemberChar125);
        obj->addMember("totalamountpay", "-", totalAmountPay, RB2::MemberChar125);
        obj->addMember("dateallocdoc", "-", dateallocdoc, RB2::MemberChar125);
        obj->addMember("transnoallocdoc", "-", transnoallocdoc, RB2::MemberChar125);
        obj->addMember("allocatedamount", "-", allocatedAmount, RB2::MemberChar125);
    }

    return true;
}

/**
 * Execute operation
 * @param input input object, if applicable
 * @param output result/output object, if applicable
 * @return true on success, default false
 */
bool ACC_InvoiceAccrued::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "ACC_InvoiceAccrued::execute() "
                    "not implemented ERROR");
    return false;
}

