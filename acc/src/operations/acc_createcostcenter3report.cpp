/*****************************************************************
 * $Id: acc_createcostcenter3report.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 12, 2012 2:48:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_createcostcenter3report.h"

#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_CreateCostCenter3Report::ACC_CreateCostCenter3Report() {
    // nothing
}

/**
 * Destructor
 */
ACC_CreateCostCenter3Report::~ACC_CreateCostCenter3Report() {
    // nothing
}

/**
 * Create the cost center report from the ACC_CostSum table
 * @param costCenterSumList container which will contain the cost center summary objects
 * @return true on success
 */
bool ACC_CreateCostCenter3Report::execute(RB_ObjectBase* costCenterSumList) {
    if (!costCenterSumList) return false;

    if (mIsPeriodYear) {
        int year = int(mPrd2 / 100) - 1;
        mFromPrd = year * 100 + 1;
        mToPrd = year * 100 + 12;

        execPeriod(costCenterSumList, 1);

        ++year;
        mFromPrd = year * 100 + 1;
        mToPrd = year * 100 + 12;

        execPeriod(costCenterSumList, 2);

        ++year;
        mFromPrd = year * 100 + 1;
        mToPrd = year * 100 + 12;

        execPeriod(costCenterSumList, 3);

    } else { // months
        mFromPrd = mPrd1;
        mToPrd = mPrd1;

        execPeriod(costCenterSumList, 1);

        mFromPrd = mPrd2;
        mToPrd = mPrd2;

        execPeriod(costCenterSumList, 2);

        mFromPrd = mPrd3;
        mToPrd = mPrd3;

        execPeriod(costCenterSumList, 3);

    }

    return true;
}

/**
 * Set period
 * @param prd1 first period and including (period = year * 100 + month)
 * @param prd2 second period and including (period = year * 100 + month)
 * @param prd3 third period and including (period = year * 100 + month)
 */
void ACC_CreateCostCenter3Report::setPeriod(int prd1, int prd2, int prd3) {
    mPrd1 = prd1;
    mPrd2 = prd2;
    mPrd3 = prd3;
    mIsPeriodYear = 100 < prd3 - prd1;
}

/**
 * execute the query of the period
 * @param costCenterSumList list with objects to be set with data
 * @param period period 1, 2 or 3
 */
void ACC_CreateCostCenter3Report::execPeriod(RB_ObjectBase* costCenterSumList,
                                             int period) {
    // same query as in ACC_CreateCostCenterReport
    RB_String qStr = "";
    qStr = "SELECT acc_costgroup.sequenceno, acc_costgroup.groupname, "
                "acc_costgroup.costprofit, acc_costcenter.centercode, acc_costcenter.centername, "
                "SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd)
                + " AND acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.budget ELSE 0 END) AS budget, "
                "SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd)
                + " AND acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.forecast ELSE 0 END) AS forecast, "
                "(SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd)
                + " AND acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.credit ELSE 0 END) - "
                "SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd)
                + " AND acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.debit ELSE 0 END)) AS actual, "
                "COUNT(acc_costsum.period) as costsumcount "
            "FROM acc_costgroup "
                "INNER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx, 1, 38)=acc_costgroup.id "
                "LEFT OUTER JOIN acc_costsum ON acc_costcenter.id=acc_costsum.parent "
                "LEFT OUTER JOIN acc_costbudget ON acc_costcenter.id=acc_costbudget.parent "
            "WHERE acc_costcenter.parent='" + ACC_MODELFACTORY->getRootId() + "' "
            "GROUP BY acc_costgroup.sequenceno, acc_costcenter.centercode "
            "ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode;";

    QSqlQuery query(ACC_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("ACC_CreateCostCenter3Report::execPeriod() "
                        + query.lastError().text() + " ERROR");
        return;
    }

    switch (period) {
    case 1:
        setPeriod1(costCenterSumList, query);
        break;
    case 2:
        setPeriod2(costCenterSumList, query);
        break;
    case 3:
        setPeriod3(costCenterSumList, query);
        break;
    default:
        break;
    }
}

/**
 * Set period 1 data
 * @param costCenterSumList list with cost center period data to be set
 * @param query database query with data
 */
void ACC_CreateCostCenter3Report::setPeriod1(RB_ObjectBase *costCenterSumList,
                                             QSqlQuery &query) {
    int costsumcount = 1;

    //  0 groupsequenceno
    //  1 groupname
    //  2 groupcostprofit
    //  3 centercode
    //  4 centername
    //  5 budget
    //  6 forecast
    //  7 actual
    //  8 costsumcount
    while (query.next()) {
        if (query.value(8).toInt() > 1) {
            costsumcount = query.value(8).toInt();
        } else {
            costsumcount = 1; // reset
        }

        RB_ObjectBase* obj = new RB_ObjectAtomic("", costCenterSumList, "ACC_CostCenterSummary");
        costCenterSumList->addObject(obj);
        obj->addMember("groupsequenceno", "-", query.value(0), RB2::MemberDouble);
        obj->addMember("groupname", "-", query.value(1), RB2::MemberDouble);
        obj->addMember("groupcostprofit", "-", query.value(2), RB2::MemberDouble);
        obj->addMember("centercode", "-", query.value(3), RB2::MemberDouble);
        obj->addMember("centername", "-", query.value(4), RB2::MemberDouble);
        obj->addMember("budget", "-", query.value(5).toDouble() / costsumcount, RB2::MemberDouble);
        obj->addMember("forecast", "-", query.value(6).toDouble() / costsumcount, RB2::MemberDouble);
        obj->addMember("actual", "-", query.value(7), RB2::MemberDouble);
        obj->addMember("budget2", "-", 0.0);
        obj->addMember("forecast2", "-", 0.0);
        obj->addMember("actual2", "-", 0.0);
        obj->addMember("budget3", "-", 0.0);
        obj->addMember("forecast3", "-", 0.0);
        obj->addMember("actual3", "-", 0.0);
    }
}

/**
 * Set period 1 data
 * @param costCenterSumList list with cost center period data to be set
 * @param query database query with data
 */
void ACC_CreateCostCenter3Report::setPeriod2(RB_ObjectBase *costCenterSumList,
                                             QSqlQuery& query) {
    int costsumcount = 1;

    while (query.next()) {
        if (query.value(8).toInt() > 1) {
            costsumcount = query.value(8).toInt();
        } else {
            costsumcount = 1; // reset
        }

        RB_ObjectBase* obj = costCenterSumList->getObject("centercode", query.value(3));

        if (!obj) {
            obj = new RB_ObjectAtomic("", costCenterSumList, "ACC_CostCenterSummary");
            costCenterSumList->addObject(obj);
            obj->addMember("groupsequenceno", "-", query.value(0), RB2::MemberDouble);
            obj->addMember("groupname", "-", query.value(1), RB2::MemberDouble);
            obj->addMember("groupcostprofit", "-", query.value(2), RB2::MemberDouble);
            obj->addMember("centercode", "-", query.value(3), RB2::MemberDouble);
            obj->addMember("centername", "-", query.value(4), RB2::MemberDouble);
            obj->addMember("budget", "-", 0.0);
            obj->addMember("forecast", "-", 0.0);
            obj->addMember("actual", "-", 0.0);
            obj->addMember("budget2", "-", query.value(5).toDouble() / costsumcount, RB2::MemberDouble);
            obj->addMember("forecast2", "-", query.value(6).toDouble() / costsumcount, RB2::MemberDouble);
            obj->addMember("actual2", "-", query.value(7), RB2::MemberDouble);
            obj->addMember("budget3", "-", 0.0);
            obj->addMember("forecast3", "-", 0.0);
            obj->addMember("actual3", "-", 0.0);
        } else {
            obj->setValue("budget2", query.value(5).toDouble() / costsumcount);
            obj->setValue("forecast2", query.value(6).toDouble() / costsumcount);
            obj->setValue("actual2", query.value(7));
        }
    }
}

/**
 * Set period 3 data
 * @param costCenterSumList list with cost center period data to be set
 * @param query database query with data
 */
void ACC_CreateCostCenter3Report::setPeriod3(RB_ObjectBase *costCenterSumList,
                                             QSqlQuery& query) {
    int costsumcount = 1;

    while (query.next()) {
        if (query.value(8).toInt() > 1) {
            costsumcount = query.value(8).toInt();
        } else {
            costsumcount = 1; // reset
        }

        RB_ObjectBase* obj = costCenterSumList->getObject("centercode", query.value(3));

        if (!obj) {
            obj = new RB_ObjectAtomic("", costCenterSumList, "ACC_CostCenterSummary");
            costCenterSumList->addObject(obj);
            obj->addMember("groupsequenceno", "-", query.value(0), RB2::MemberDouble);
            obj->addMember("groupname", "-", query.value(1), RB2::MemberDouble);
            obj->addMember("groupcostprofit", "-", query.value(2), RB2::MemberDouble);
            obj->addMember("centercode", "-", query.value(3), RB2::MemberDouble);
            obj->addMember("centername", "-", query.value(4), RB2::MemberDouble);
            obj->addMember("budget", "-", 0.0);
            obj->addMember("forecast", "-", 0.0);
            obj->addMember("actual", "-", 0.0);
            obj->addMember("budget2", "-", 0.0);
            obj->addMember("forecast2", "-", 0.0);
            obj->addMember("actual2", "-", 0.0);
            obj->addMember("budget3", "-", query.value(5).toDouble() / costsumcount, RB2::MemberDouble);
            obj->addMember("forecast3", "-", query.value(6).toDouble() / costsumcount, RB2::MemberDouble);
            obj->addMember("actual3", "-", query.value(7), RB2::MemberDouble);
        } else {
            obj->setValue("budget3", query.value(5).toDouble() / costsumcount);
            obj->setValue("forecast3", query.value(6).toDouble() / costsumcount);
            obj->setValue("actual3", query.value(7));
        }
    }
}
