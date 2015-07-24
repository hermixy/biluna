/*****************************************************************
 * $Id: acc_creategltransactions.cpp 2127 2014-03-20 15:16:27Z rutger $
 * Created: Nov 8, 2010 5:52:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_creategltransactions.h"

#include "acc_modelfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_CreateGlTransactions::ACC_CreateGlTransactions() {

}

/**
 * Destructor
 */
ACC_CreateGlTransactions::~ACC_CreateGlTransactions() {

}

/**
 * Create the GL transactions from the ACC_GlTrans table
 * @param glTransList container which will contain the GL transaction objects
 * @return true on success
 */
bool ACC_CreateGlTransactions::execute(RB_ObjectBase* glTransList) {
    if (!glTransList) return false;

    RB_SqlCommonFunctions util;

    RB_String qStr =
    "SELECT acc_chartmaster.accountcode, acc_chartmaster.accountname, "
        "acc_transdoc.parent, acc_gltrans.transdate, acc_gltrans.transdocno, "
        "acc_gltrans.description, acc_transdoc.refno, acc_gltrans.amount "
    "FROM acc_chartmaster INNER JOIN acc_gltrans ON acc_chartmaster.id=";
    util.substrIdxId("acc_gltrans.chartmaster_idx", qStr);
    qStr += " INNER JOIN acc_transdoc ON acc_gltrans.transdoc_id=acc_transdoc.id "
    "WHERE acc_chartmaster.parent='" + ACC_MODELFACTORY->getRootId() + "' "
        "AND SUBSTR(acc_gltrans.transdate,1,10)>='" + mFromDate.toString( Qt::ISODate) + "' "
        "AND SUBSTR(acc_gltrans.transdate,1,10)<='" + mToDate.toString( Qt::ISODate) + "' "
        "AND acc_chartmaster.accountcode>='" + mFromAcct + "' "
        "AND acc_chartmaster.accountcode<='" + mToAcct + "' "
        "ORDER BY acc_chartmaster.accountcode, acc_gltrans.transdate ASC";



    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateGlTransactions::execute() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    //  0 accountcode
    //  1 accountname
    //  2 parent (ACC_ChartMaster)
    //  3 transdate
    //  4 transdocno
    //  5 description
    //  6 refno
    //  7 amount

    while (query.next()) {
        RB_ObjectBase* obj = new RB_ObjectAtomic("", glTransList, "ACC_GlTransactions");
        glTransList->addObject(obj);
        obj->addMember(obj, "accountcode", "-", query.value(0), RB2::MemberChar40);
        obj->addMember(obj, "accountname", "-", query.value(1), RB2::MemberChar255);
        obj->addMember(obj, "docparent", "-", query.value(2), RB2::MemberChar40);
        obj->addMember(obj, "transdate", "-", query.value(3), RB2::MemberChar40);
        obj->addMember(obj, "transdocno", "-", query.value(4), RB2::MemberInteger);
        obj->addMember(obj, "description", "-", query.value(5), RB2::MemberChar255);
        obj->addMember(obj, "refno", "-", query.value(6), RB2::MemberChar40);

        double amount = query.value(7).toDouble();

        if (amount >= 0.0) {
            obj->addMember(obj, "debit", "-", amount, RB2::MemberDouble);
            obj->addMember(obj, "credit", "-", 0.0, RB2::MemberDouble);
        } else {
            obj->addMember(obj, "debit", "-", 0.0, RB2::MemberDouble);
            obj->addMember(obj, "credit", "-", -amount, RB2::MemberDouble);
        }
    }

    return true;
}

/**
 * Set period
 * @param fromDate starting date
 * @param toDate ending date
 */
void ACC_CreateGlTransactions::setPeriod(const QDate& fromDate,
                                         const QDate& toDate) {
    mFromDate = fromDate;
    mToDate = toDate;
}

/**
 * Set account range
 * @param fromAcct starting GL account number from chartmaster
 * @param toAcct ending GL account number
 */
void ACC_CreateGlTransactions::setAccountRange(const RB_String& fromAcct,
                                               const RB_String& toAcct) {
    mFromAcct = fromAcct;
    mToAcct = toAcct;
}
