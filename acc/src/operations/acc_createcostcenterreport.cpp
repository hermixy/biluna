/*****************************************************************
 * $Id: acc_createcostcenterreport.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Jul 9, 2012 6:48:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_createcostcenterreport.h"

#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_CreateCostCenterReport::ACC_CreateCostCenterReport() {
    // nothing
}

/**
 * Destructor
 */
ACC_CreateCostCenterReport::~ACC_CreateCostCenterReport() {
    // nothing
}

/**
 * Create the cost center report from the ACC_CostSum table
 * @param costCenterSumList container which will contain the cost center summary objects
 * @return true on success
 */
bool ACC_CreateCostCenterReport::execute(RB_ObjectBase* costCenterSumList) {
    if (!costCenterSumList) return false;

    /*
    TEST

    SELECT acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit, acc_costcenter.centercode, acc_costcenter.centername,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.budget ELSE 0 END) as budget,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.forecast ELSE 0 END) as forecast,
    (SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.credit ELSE 0 END) -
    SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.debit ELSE 0 END)) AS actual,
    COUNT(acc_costsum.period) as costsumcount
     FROM acc_costgroup
    INNER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx, 1, 38)=acc_costgroup.id
    LEFT OUTER JOIN acc_costsum ON acc_costcenter.id=acc_costsum.parent
    LEFT OUTER JOIN acc_costbudget ON acc_costcenter.id=acc_costbudget.parent
    WHERE acc_costcenter.parent='ID0001'
    GROUP BY acc_costgroup.sequenceno, acc_costcenter.centercode
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode;

    */


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
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateCostCenterReport::execute() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());

    }

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
        // Correct the budget and forecast for the amount/count of acc_costsum in SQL datasets
        obj->addMember("budget", "-", query.value(5).toDouble() / costsumcount, RB2::MemberDouble);
        obj->addMember("forecast", "-", query.value(6).toDouble() / costsumcount, RB2::MemberDouble);
        obj->addMember("actual", "-", query.value(7), RB2::MemberDouble);
    }

    return true;
}

/**
 * Set period
 * @param fromPrd starting period and including (period = year * 100 + month)
 * @param toPrd ending period and including (period = year * 100 + month)
 */
void ACC_CreateCostCenterReport::setPeriod(int fromPrd, int toPrd) {
    mFromPrd = fromPrd;
    mToPrd = toPrd;
}
