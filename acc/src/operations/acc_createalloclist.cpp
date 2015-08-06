/*****************************************************************
 * $Id: acc_createalloclist.cpp 2212 2015-01-28 16:45:01Z rutger $
 * Created: June 29, 2011 19:29:42 PM - rutger
 *
 * Copyright (C) 2011 Biluna. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_createalloclist.h"

#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_CreateAllocList::ACC_CreateAllocList() {
    // nothing
}

/**
 * Destructor
 */
ACC_CreateAllocList::~ACC_CreateAllocList() {
    // nothing
}

/**
 * Create the amount allocation list for the given period
 * @param allocList container which will contain the allocation objects
 * @return true on success
 */
bool ACC_CreateAllocList::execute(RB_ObjectBase* allocList) {
    if (!allocList) return false;

    RB_String qStr = "";

    setQuery(qStr, ACC2::ControlDebtor, ACC2::ControlBank);
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateAllocList::execute() 1 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    addObjectsToList(query, allocList);

    setQuery(qStr, ACC2::ControlDebtor, ACC2::ControlMemoBook);
    query.clear();

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateAllocList::execute() 2 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    addObjectsToList(query, allocList);

    setQuery(qStr, ACC2::ControlCreditor, ACC2::ControlBank);
    query.clear();

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateAllocList::execute() 3 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    addObjectsToList(query, allocList);

    setQuery(qStr, ACC2::ControlCreditor, ACC2::ControlMemoBook);
    query.clear();

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateAllocList::execute() 4 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    addObjectsToList(query, allocList);

    return true;
}

/**
 * Set the start and end date to be considered for the creation of the list.
 * This function should be called before execute().
 * @param startDate start date of the period
 * @param endDate end date of the period
 */
void ACC_CreateAllocList::setPeriod(const QDate& startDate,
                                    const QDate& endDate) {
    mStartDate = startDate;
    mEndDate = endDate;
}

/**
 * Create the query. The query is made twice, once for the bank/cash
 * and once for the journal documents. The type calls for the different
 * documents.
 * @todo Improve query, refer below to 'dates'
 * @param qStr query string to be set
 * @param type document type for which the query should be set
 */
void ACC_CreateAllocList::setQuery(RB_String& qStr, ACC2::ControlType docToType,
                                   ACC2::ControlType docFromType) {
    /*
    SELECT acc_transdoc.docname, acc_transdoc.transno as doctotransno,
    acc_transdoc.totalamountrec as totalamountrecpay,
    t2.transdate as allocdate, acc_transallocn.amount as amountalloc,
    acc_banktrans.transno, acc_banktrans.amount as itemamount
    FROM acc_transdoc
    INNER JOIN acc_transallocn ON acc_transallocn.docallocto_id = acc_transdoc.id
    INNER JOIN acc_banktrans ON acc_banktrans.id = acc_transallocn.itemallocfrom_id
    LEFT OUTER JOIN acc_transdoc as t2 ON t2.id=acc_transallocn.docfrom_id
    WHERE acc_transdoc.transdate >= '2001'
    AND acc_transdoc.transdate <= '2012'
    ORDER BY acc_transdoc.transno ASC;
    */
    RB_String recPayField = "";
    RB_String bankMemoSet = "";
    RB_String bankMemoField = "";

    switch (docToType) {
    case ACC2::ControlDebtor :
        recPayField = "totalamountrec";
        break;
    case ACC2::ControlCreditor :
        recPayField = "totalamountpay";
        break;
    default :
        break;
    }

    switch (docFromType) {
    case ACC2::ControlBank :
    case ACC2::ControlCash :
        bankMemoSet = "acc_banktrans.transno, "
                "acc_banktrans.amount as itemamount";
        bankMemoField = "acc_banktrans";
        break;
    case ACC2::ControlMemoBook :
        bankMemoSet = "acc_memotrans.transno, "
                "acc_memotrans.amount as itemamount";
        bankMemoField = "acc_memotrans";
        break;
    default :
        break;
    }

    qStr = "SELECT acc_transdoc.docname, acc_transdoc.transno as doctotransno, ";
    qStr += "acc_transdoc.transdate, acc_transdoc." + recPayField + " as totalamountrecpay, ";
    qStr += "t2.transdate as allocdate, acc_transallocn.amount as amountalloc, ";
    qStr += bankMemoSet + " ";
    qStr += "FROM acc_transdoc ";
    qStr += "INNER JOIN acc_transallocn ON acc_transallocn.docallocto_id = acc_transdoc.id ";
    qStr += "INNER JOIN " + bankMemoField + " ON " + bankMemoField + ".id = acc_transallocn.itemallocfrom_id ";
    qStr += "LEFT OUTER JOIN acc_transdoc as t2 ON t2.id=acc_transallocn.docfrom_id ";
    qStr += "WHERE SUBSTR(acc_transdoc.transdate,1,10) >= '" + mStartDate.toString(Qt::ISODate) + "' ";
    qStr += "AND SUBSTR(acc_transdoc.transdate,1,10) <= '" + mEndDate.toString(Qt::ISODate) + "' ";
    qStr += "AND acc_transdoc." + recPayField + " <> 0 ";
    qStr += "AND " + bankMemoField + ".parent='" + ACC_MODELFACTORY->getRootId() + "' ";
    qStr += "ORDER BY acc_transdoc.transno DESC;";
//    RB_DEBUG->print(qStr);
}

/**
 * Create and add the objects to the allocation list
 * @param q query with records
 * @param allocList alloction list
 */
void ACC_CreateAllocList::addObjectsToList(QSqlQuery& q,
                                           RB_ObjectBase* allocList) {
    /*
    0 docname
    1 doctotransno
    2 transdate
    3 totalamountrecpay
    4 datealloc
    5 amount
    6 transno (doc of item)
    7 itemamount
    */

    while (q.next()) {
        RB_ObjectBase* obj = new RB_ObjectAtomic("", allocList, "ACC_Allocation");
        allocList->addObject(obj);
        obj->addMember("docname", "-", q.value(0), RB2::MemberChar125);
        obj->addMember("doctotransno", "-", q.value(1), RB2::MemberChar40);
        obj->addMember("transdate", "-", q.value(2), RB2::MemberChar40);
        obj->addMember("totalamountrecpay", "-", q.value(3), RB2::MemberDouble);
        obj->addMember("datealloc", "-", q.value(4), RB2::MemberChar40);
        obj->addMember("amount", "-", q.value(5), RB2::MemberDouble);
        obj->addMember("transno", "-", q.value(6), RB2::MemberChar40);
        obj->addMember("itemamount", "-", q.value(7), RB2::MemberDouble);
    }
}
