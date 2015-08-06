/*****************************************************************
 * $Id: acc_createdoctrans.cpp 2127 2014-03-20 15:16:27Z rutger $
 * Created: Nov 10, 2010 5:52:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_createdoctrans.h"

#include "acc_modelfactory.h"
#include "acc_sqlcommonfunctions.h"

/**
 * Constructor
 */
ACC_CreateDocTrans::ACC_CreateDocTrans() {

}

/**
 * Destructor
 */
ACC_CreateDocTrans::~ACC_CreateDocTrans() {

}

/**
 * Create the documents with the GL transactions from the ACC_GlTrans table
 * @param docList container which will contain the document and transactions
 * @return true on success
 */
bool ACC_CreateDocTrans::execute(RB_ObjectBase* docList) {
    if (!docList) return false;

    /*
    SELECT acc_chartmaster.accountcode, acc_chartmaster.accountname, acc_transdoc.transno,
        acc_gltrans.chartmaster_id, acc_gltrans.transdate,
        acc_gltrans.description, acc_transdoc.refno, acc_gltrans.amount
    FROM acc_chartmaster INNER JOIN acc_transdoc ON acc_chartmaster.id=acc_transdoc.parent
                        INNER JOIN acc_gltrans ON acc_transdoc.id=acc_gltrans.transdoc_id
    WHERE acc_chartmaster.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
        AND acc_gltrans.transdate>='2010-01-01'
        AND acc_gltrans.transdate<='2010-12-31'
        ORDER BY acc_chartmaster.accountcode, acc_transdoc.transno;
     */


    RB_SqlCommonFunctions util;
    RB_String qStr =
    "SELECT acc_transdoc.id, acc_transdoc.docname, acc_chartmaster.accountcode, "
        "acc_chartmaster.accountname, acc_transdoc.transno, ";
    util.substrIdxId("acc_gltrans.chartmaster_idx", qStr);
    qStr += ", acc_gltrans.transdate, acc_gltrans.description, acc_transdoc.refno, acc_gltrans.amount "
    "FROM acc_chartmaster INNER JOIN acc_transdoc ON acc_chartmaster.id=acc_transdoc.parent "
                        "INNER JOIN acc_gltrans ON acc_transdoc.id=acc_gltrans.transdoc_id "
    "WHERE acc_chartmaster.parent='" + ACC_MODELFACTORY->getRootId() + "' "
        "AND acc_gltrans.transdate>='" + mFromDate.toString(Qt::ISODate) + "' "
        "AND acc_gltrans.transdate<='" + mToDate.toString(Qt::ISODate) + "' "
        "ORDER BY acc_chartmaster.accountcode, acc_transdoc.transno ASC";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateDocTrans::execute() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    //  0 transdoc_id
    //  1 docname
    //  2 accountcode
    //  3 accountname
    //  4 transno
    //  5 chartmaster_id
    //  6 transdate
    //  7 description
    //  8 refno
    //  9 amount

    while (query.next()) {
        RB_ObjectBase* obj = new RB_ObjectAtomic("", docList, "ACC_GlTransactions");
        docList->addObject(obj);
        obj->addMember("transdocid", "-", query.value(0), RB2::MemberChar40);
        obj->addMember("transdocname", "-", query.value(1), RB2::MemberChar125);
        obj->addMember("accountcode", "-", query.value(2), RB2::MemberChar40);
        obj->addMember("accountname", "-", query.value(3), RB2::MemberChar255);
        obj->addMember("transno", "-", query.value(4), RB2::MemberChar40);
        obj->addMember("chartmasterid", "-", query.value(5), RB2::MemberChar40);
        obj->addMember("transdate", "-", query.value(6), RB2::MemberChar40);
        obj->addMember("description", "-", query.value(7), RB2::MemberChar255);
        obj->addMember("refno", "-", query.value(8), RB2::MemberChar40);

        double amount = query.value(9).toDouble();

        if (amount >= 0.0) {
            obj->addMember("debit", "-", amount, RB2::MemberDouble);
            obj->addMember("credit", "-", 0.0, RB2::MemberDouble);
        } else {
            obj->addMember("debit", "-", 0.0, RB2::MemberDouble);
            obj->addMember("credit", "-", -amount, RB2::MemberDouble);
        }
    }

    return true;
}

/**
 * Set period
 * @param fromDate starting date
 * @param toDate ending date
 */
void ACC_CreateDocTrans::setPeriod(const QDate& fromDate,
                                   const QDate& toDate) {
    mFromDate = fromDate;
    mToDate = toDate;
}

