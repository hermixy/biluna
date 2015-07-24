/*****************************************************************
 * $Id: acc_createcostcenterbalancereport.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Sept 29, 2012 12:15:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_createcostcenterbalancereport.h"

#include "acc_dialogfactory.h" // test only, commandline message
#include "acc_modelfactory.h"

/**
 * Constructor
 */
ACC_CreateCostCenterBalanceReport::ACC_CreateCostCenterBalanceReport() {
    // nothing
}

/**
 * Destructor
 */
ACC_CreateCostCenterBalanceReport::~ACC_CreateCostCenterBalanceReport() {
    // nothing
}

/**
 * Create the cost center report from the ACC_CostSum table. The
 * database is queried four (4) times, for customer and supplier, and
 * whether part assigned to cost group and cost center.
 * @param costCenterSumList container which will contain the cost center summary objects
 * @return true on success
 */
bool ACC_CreateCostCenterBalanceReport::execute(RB_ObjectBase* costCenterBalanceList) {
    if (!costCenterBalanceList) return false;

    /* TREE
    -- up to and including great grandchildren (4 levels) of root
    -- the base list are the parents or level 1 of root,
    -- children are level 2
    -- grandchildren are level 3
    -- great grandchildren are level 4
    SELECT t4.*
    FROM acc_customer as t4
    LEFT JOIN acc_customer as t3 on t4.parent=t3.id
    LEFT JOIN acc_customer as t2 on t3.parent=t2.id
    LEFT JOIN acc_customer as t1 on t2.parent=t1.id
    WHERE t1.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    OR t2.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    OR t3.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    OR t4.parent='{08c87571-20d2-4f38-9e79-c1336755b367}'
    */

    /* TEST
    -- not used
    SELECT acc_customer.mname, acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit, acc_costcenter.centercode, acc_costcenter.centername,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.budget ELSE 0 END) as budget,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.forecast ELSE 0 END) as forecast,
    (SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.credit ELSE 0 END) -
    SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.debit ELSE 0 END)) AS actual,
    acc_custcostpart.weight
    FROM acc_costgroup
    INNER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx, 1, 38)=acc_costgroup.id
    LEFT OUTER JOIN acc_costsum ON acc_costcenter.id=acc_costsum.parent
    LEFT OUTER JOIN acc_costbudget ON acc_costcenter.id=acc_costbudget.parent
    LEFT OUTER JOIN acc_custcostpart ON SUBSTR(acc_custcostpart.costgroup_idx, 1, 38)=acc_costgroup.id
    LEFT OUTER JOIN acc_custcostpart as t1 ON SUBSTR(acc_custcostpart.costcenter_idx, 1, 38)=acc_costcenter.id
    LEFT OUTER JOIN acc_customer ON acc_customer.id=acc_custcostpart.parent
    WHERE acc_costcenter.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}' AND acc_customer.mname<>''
    GROUP BY acc_customer.mname, acc_costgroup.sequenceno, acc_costcenter.centercode
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode;

    SELECT acc_supplier.suppname, acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit, acc_costcenter.centercode, acc_costcenter.centername,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.budget ELSE 0 END) as budget,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.forecast ELSE 0 END) as forecast,
    (SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.credit ELSE 0 END) -
    SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.debit ELSE 0 END)) AS actual,
    acc_supplcostpart.weight
    FROM acc_costgroup
    INNER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx, 1, 38)=acc_costgroup.id
    LEFT OUTER JOIN acc_costsum ON acc_costcenter.id=acc_costsum.parent
    LEFT OUTER JOIN acc_costbudget ON acc_costcenter.id=acc_costbudget.parent
    LEFT OUTER JOIN acc_supplcostpart ON SUBSTR(acc_supplcostpart.costcenter_idx, 1, 38)=acc_costcenter.id
    LEFT OUTER JOIN acc_supplcostpart as t1 ON SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38)=acc_costgroup.id
    LEFT OUTER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent
    WHERE acc_costcenter.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}' AND acc_supplier.suppname<>''
    GROUP BY acc_supplier.suppname, acc_costgroup.sequenceno, acc_costcenter.centercode
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode;

    -- 4 different SQL statements

    -- Customer cost center
    SELECT acc_customer.mname, acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit,
    acc_costcenter.centercode, acc_costcenter.centername,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.budget ELSE 0 END) as budget,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.forecast ELSE 0 END) as forecast,
    (SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.credit ELSE 0 END) -
    SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.debit ELSE 0 END)) AS actual,
    acc_custcostpart.weight
    FROM acc_customer
    INNER JOIN acc_custcostpart ON acc_custcostpart.parent=acc_customer.id
    INNER JOIN acc_costcenter ON acc_costcenter.id=SUBSTR(acc_custcostpart.costcenter_idx, 1, 38)
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id
    LEFT OUTER JOIN acc_costgroup ON acc_costgroup.id=SUBSTR(acc_costcenter.centergroup_idx, 1, 38)
    WHERE acc_customer.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}' AND SUBSTR(acc_custcostpart.costcenter_idx, 39)<>''
    GROUP BY acc_customer.mname, acc_costgroup.sequenceno, acc_costcenter.centercode
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode;

    -- Customer cost group not used refer to supplier group
    SELECT acc_customer.mname, acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit,
    acc_costcenter.centercode, acc_costcenter.centername,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.budget ELSE 0 END) as budget,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.forecast ELSE 0 END) as forecast,
    (SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.credit ELSE 0 END) -
    SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.debit ELSE 0 END)) AS actual,
    acc_custcostpart.weight
    FROM acc_customer
    INNER JOIN acc_custcostpart ON acc_custcostpart.parent=acc_customer.id
    INNER JOIN acc_costgroup ON acc_costgroup.id=SUBSTR(acc_custcostpart.costgroup_idx, 1, 38)
    INNER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx, 1, 38)=acc_costgroup.id
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id
    WHERE acc_customer.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}' AND SUBSTR(acc_custcostpart.costgroup_idx, 39)<>''
    GROUP BY acc_customer.mname, acc_costgroup.sequenceno, acc_costcenter.centercode
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode;

    -- Supplier cost center
    SELECT acc_supplier.suppname, acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit,
    acc_costcenter.centercode, acc_costcenter.centername,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.budget ELSE 0 END) as budget,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.forecast ELSE 0 END) as forecast,
    (SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.credit ELSE 0 END) -
    SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.debit ELSE 0 END)) AS actual,
    acc_supplcostpart.weight
    FROM acc_supplier
    INNER JOIN acc_supplcostpart ON acc_supplcostpart.parent=acc_supplier.id
    INNER JOIN acc_costcenter ON acc_costcenter.id=SUBSTR(acc_supplcostpart.costcenter_idx, 1, 38)
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id
    LEFT OUTER JOIN acc_costgroup ON acc_costgroup.id=SUBSTR(acc_costcenter.centergroup_idx, 1, 38)
    WHERE acc_supplier.parent='{ddfd9618-47e7-455a-993c-a5797dc3d5f9}' AND SUBSTR(acc_supplcostpart.costcenter_idx, 39)<>''
    GROUP BY acc_supplier.suppname, acc_costgroup.sequenceno, acc_costcenter.centercode
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode;

    --
    -- FINAL (supplier group) suppname, sequenceno, groupname, costprofit, centercode, centername, budget, forecast, actual, weight
    --

    SELECT acc_supplier.suppname, t4.sequenceno, t4.groupname, t4.costprofit,
    acc_costcenter.centercode, acc_costcenter.centername,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.budget ELSE 0 END) as budget,
    SUM(CASE WHEN acc_costbudget.period>=201201 AND acc_costbudget.period<=201212 THEN acc_costbudget.forecast ELSE 0 END) as forecast,
    (SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.credit ELSE 0 END) -
    SUM(CASE WHEN acc_costsum.period>=201201 AND acc_costsum.period<=201212 THEN acc_costsum.debit ELSE 0 END)) AS actual,
    acc_supplcostpart.weight
    FROM acc_costgroup AS t4
    LEFT OUTER JOIN acc_costgroup AS t3 ON t3.id=t4.parent
    LEFT OUTER JOIN acc_costgroup AS t2 ON t2.id=t3.parent
    LEFT OUTER JOIN acc_costgroup AS t1 ON t1.id=t2.parent
    LEFT OUTER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx,1,38)=t4.id
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id
    -- only to get the fields
    LEFT OUTER JOIN acc_supplcostpart ON  SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38)=t4.id
    LEFT OUTER JOIN acc_supplier ON  acc_supplcostpart.parent=acc_supplier.id
    WHERE
    (t4.id IN
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx
    FROM acc_supplcostpart
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent
    WHERE acc_supplier.parent='ID0001')
    OR
    t3.id IN
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx
    FROM acc_supplcostpart
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent
    WHERE acc_supplier.parent='ID0001')
    OR
    t2.id IN
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx
    FROM acc_supplcostpart
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent
    WHERE acc_supplier.parent='ID0001')
    OR
    t1.id IN
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx
    FROM acc_supplcostpart
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent
    WHERE acc_supplier.parent='ID0001'))
    AND t4.id<>'0'
    -- required
    GROUP BY t4.sequenceno, acc_costcenter.centercode
    ORDER BY t4.costprofit, t4.sequenceno, acc_costcenter.centercode;
    -- not ORDER BY acc_supplier.suppname, do sort with objects


    */

    mProjectId = ACC_MODELFACTORY->getRootId();
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String qStr = "";
    setCustomerCostGroupSql(qStr); ACC_DIALOGFACTORY->commandMessage(qStr); // test only

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateCostCenterBalanceReport::execute() 1 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
        RB_DEBUG->print(qStr);
    }

    setCostCenterBalanceList(query, costCenterBalanceList);
    setCustomerCostCenterSql(qStr); ACC_DIALOGFACTORY->commandMessage(qStr); // test only

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateCostCenterBalanceReport::execute() 2 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
        RB_DEBUG->print(qStr);
    }

    setCostCenterBalanceList(query, costCenterBalanceList);
    setSupplierCostGroupSql(qStr); ACC_DIALOGFACTORY->commandMessage(qStr); // test only

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateCostCenterBalanceReport::execute() 3 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
        RB_DEBUG->print(qStr);
    }

    setCostCenterBalanceList(query, costCenterBalanceList);
    setSupplierCostCenterSql(qStr); ACC_DIALOGFACTORY->commandMessage(qStr); // test only

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "ACC_CreateCostCenterBalanceReport::execute() 4 "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
        RB_DEBUG->print(qStr);
    }

    setCostCenterBalanceList(query, costCenterBalanceList);
    return true;
}

/**
 * Set period
 * @param fromPrd starting period and including (period = year * 100 + month)
 * @param toPrd ending period and including (period = year * 100 + month)
 */
void ACC_CreateCostCenterBalanceReport::setPeriod(int fromPrd, int toPrd) {
    mFromPrd = fromPrd;
    mToPrd = toPrd;
    mFromDate = QDate(int(fromPrd / 100), int(fromPrd % 100), 1);
    mToDate = QDate(int(toPrd / 100), int(toPrd % 100), 1);
}

/**
 * Set customer cost group sql. Only selects the ACC_CustCostPart
 * allocated to a ACC_CostGroup
 * @param qstr to be set with SQL statement
 */
void ACC_CreateCostCenterBalanceReport::setCustomerCostGroupSql(RB_String& qStr) {
    qStr = "SELECT acc_customer.mname, t4.sequenceno, t4.groupname, t4.costprofit, \
    acc_costcenter.centercode, acc_costcenter.centername, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.budget ELSE 0 END) as budget, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " \
        AND acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.forecast ELSE 0 END) as forecast, \
    (SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
         acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.credit ELSE 0 END) - \
    SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.debit ELSE 0 END)) AS actual, \
    acc_custcostpart.weight, acc_custcostpart.type_id, \
    acc_custcostpart.startdate, acc_custcostpart.enddate, COUNT(acc_costsum.period) as costsumcount \
    FROM acc_costgroup AS t4 \
    LEFT OUTER JOIN acc_costgroup AS t3 ON t3.id=t4.parent \
    LEFT OUTER JOIN acc_costgroup AS t2 ON t2.id=t3.parent \
    LEFT OUTER JOIN acc_costgroup AS t1 ON t1.id=t2.parent \
    LEFT OUTER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx,1,38)=t4.id \
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_custcostpart ON  SUBSTR(acc_custcostpart.costgroup_idx, 1, 38)=t4.id \
    LEFT OUTER JOIN acc_customer ON  acc_custcostpart.parent=acc_customer.id \
    WHERE \
    (t4.id IN \
    (SELECT SUBSTR(acc_custcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_custcostpart \
    INNER JOIN acc_customer ON acc_customer.id=acc_custcostpart.parent \
    WHERE acc_customer.parent='" + mProjectId + "') \
    OR \
    t3.id IN \
    (SELECT SUBSTR(acc_custcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_custcostpart \
    INNER JOIN acc_customer ON acc_customer.id=acc_custcostpart.parent \
    WHERE acc_customer.parent='" + mProjectId + "') \
    OR \
    t2.id IN \
    (SELECT SUBSTR(acc_custcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_custcostpart \
    INNER JOIN acc_customer ON acc_customer.id=acc_custcostpart.parent \
    WHERE acc_customer.parent='" + mProjectId + "') \
    OR \
    t1.id IN \
    (SELECT SUBSTR(acc_custcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_custcostpart \
    INNER JOIN acc_customer ON acc_customer.id=acc_custcostpart.parent \
    WHERE acc_customer.parent='" + mProjectId + "')) \
    AND t4.id<>'0' \
    AND acc_custcostpart.startdate < '" + mToDate.toString(Qt::ISODate) + "' \
    AND acc_custcostpart.enddate > '" + mFromDate.toString(Qt::ISODate) + "' \
    GROUP BY t4.sequenceno, acc_costcenter.centercode, acc_custcostpart.type_id \
    ORDER BY t4.costprofit, t4.sequenceno, acc_costcenter.centercode;";
    // not ORDER BY acc_customer.suppname, sorting to be done with objects
}

/**
 * Set customer cost center sql. Only selects the ACC_CustCostPart
 * allocated to a ACC_CostCenter
 * @param qstr to be set with SQL statement
 */
void ACC_CreateCostCenterBalanceReport::setCustomerCostCenterSql(RB_String& qStr) {
    qStr = "SELECT acc_customer.mname, acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit, \
    acc_costcenter.centercode, acc_costcenter.centername, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.budget ELSE 0 END) as budget, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " \
        AND acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.forecast ELSE 0 END) as forecast, \
    (SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
         acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.credit ELSE 0 END) - \
    SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.debit ELSE 0 END)) AS actual, \
    acc_custcostpart.weight, acc_custcostpart.type_id, \
    acc_custcostpart.startdate, acc_custcostpart.enddate, COUNT(acc_costsum.period) as costsumcount \
    FROM acc_customer \
    INNER JOIN acc_custcostpart ON acc_custcostpart.parent=acc_customer.id \
    INNER JOIN acc_costcenter ON acc_costcenter.id=SUBSTR(acc_custcostpart.costcenter_idx, 1, 38) \
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_costgroup ON acc_costgroup.id=SUBSTR(acc_costcenter.centergroup_idx, 1, 38) \
    WHERE acc_customer.parent='" + mProjectId + "' AND SUBSTR(acc_custcostpart.costcenter_idx, 39)<>'' \
    AND acc_custcostpart.startdate < '" + mToDate.toString(Qt::ISODate) + "' \
    AND acc_custcostpart.enddate > '" + mFromDate.toString(Qt::ISODate) + "' \
    GROUP BY acc_customer.mname, acc_costgroup.sequenceno, acc_costcenter.centercode, acc_custcostpart.type_id \
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode, acc_customer.mname;";
}

/**
 * Set supplier cost group sql. Only selects the ACC_SupplCostPart
 * allocated to a ACC_CostGroup
 * @param qstr to be set with SQL statement
 */
void ACC_CreateCostCenterBalanceReport::setSupplierCostGroupSql(RB_String& qStr) {
    qStr = "SELECT acc_supplier.suppname, t4.sequenceno, t4.groupname, t4.costprofit, \
    acc_costcenter.centercode, acc_costcenter.centername, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.budget ELSE 0 END) as budget, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " \
        AND acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.forecast ELSE 0 END) as forecast, \
    (SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
         acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.credit ELSE 0 END) - \
    SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.debit ELSE 0 END)) AS actual, \
    acc_supplcostpart.weight, acc_supplcostpart.type_id, \
    acc_supplcostpart.startdate, acc_supplcostpart.enddate, COUNT(acc_costsum.period) as costsumcount \
    FROM acc_costgroup AS t4 \
    LEFT OUTER JOIN acc_costgroup AS t3 ON t3.id=t4.parent \
    LEFT OUTER JOIN acc_costgroup AS t2 ON t2.id=t3.parent \
    LEFT OUTER JOIN acc_costgroup AS t1 ON t1.id=t2.parent \
    LEFT OUTER JOIN acc_costcenter ON SUBSTR(acc_costcenter.centergroup_idx,1,38)=t4.id \
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_supplcostpart ON  SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38)=t4.id \
    LEFT OUTER JOIN acc_supplier ON  acc_supplcostpart.parent=acc_supplier.id \
    WHERE \
    (t4.id IN \
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_supplcostpart \
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent \
    WHERE acc_supplier.parent='" + mProjectId + "') \
    OR \
    t3.id IN \
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_supplcostpart \
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent \
    WHERE acc_supplier.parent='" + mProjectId + "') \
    OR \
    t2.id IN \
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_supplcostpart \
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent \
    WHERE acc_supplier.parent='" + mProjectId + "') \
    OR \
    t1.id IN \
    (SELECT SUBSTR(acc_supplcostpart.costgroup_idx, 1, 38) as costgroup_idx \
    FROM acc_supplcostpart \
    INNER JOIN acc_supplier ON acc_supplier.id=acc_supplcostpart.parent \
    WHERE acc_supplier.parent='" + mProjectId + "')) \
    AND t4.id<>'0' \
    AND acc_supplcostpart.startdate < '" + mToDate.toString(Qt::ISODate) + "' \
    AND acc_supplcostpart.enddate > '" + mFromDate.toString(Qt::ISODate) + "' \
    GROUP BY t4.sequenceno, acc_costcenter.centercode, acc_supplcostpart.type_id \
    ORDER BY t4.costprofit, t4.sequenceno, acc_costcenter.centercode;";
    // not ORDER BY acc_supplier.suppname, sorting to be done with objects
}

/**
 * Set supplier cost center sql. Only selects the ACC_SupplCostPart
 * allocated to a ACC_CostCenter
 * @param qstr to be set with SQL statement
 */
void ACC_CreateCostCenterBalanceReport::setSupplierCostCenterSql(RB_String& qStr) {
    qStr = "SELECT acc_supplier.suppname, acc_costgroup.sequenceno, acc_costgroup.groupname, acc_costgroup.costprofit, \
    acc_costcenter.centercode, acc_costcenter.centername, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.budget ELSE 0 END) as budget, \
    SUM(CASE WHEN acc_costbudget.period>=" + RB_String::number(mFromPrd) + " \
        AND acc_costbudget.period<=" + RB_String::number(mToPrd) + " THEN acc_costbudget.forecast ELSE 0 END) as forecast, \
    (SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
         acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.credit ELSE 0 END) - \
    SUM(CASE WHEN acc_costsum.period>=" + RB_String::number(mFromPrd) + " AND \
        acc_costsum.period<=" + RB_String::number(mToPrd) + " THEN acc_costsum.debit ELSE 0 END)) AS actual, \
    acc_supplcostpart.weight, acc_supplcostpart.type_id, \
    acc_supplcostpart.startdate, acc_supplcostpart.enddate, COUNT(acc_costsum.period) as costsumcount \
    FROM acc_supplier \
    INNER JOIN acc_supplcostpart ON acc_supplcostpart.parent=acc_supplier.id \
    INNER JOIN acc_costcenter ON acc_costcenter.id=SUBSTR(acc_supplcostpart.costcenter_idx, 1, 38) \
    LEFT OUTER JOIN acc_costsum ON acc_costsum.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_costbudget ON acc_costbudget.parent=acc_costcenter.id \
    LEFT OUTER JOIN acc_costgroup ON acc_costgroup.id=SUBSTR(acc_costcenter.centergroup_idx, 1, 38) \
    WHERE acc_supplier.parent='" + mProjectId + "' AND SUBSTR(acc_supplcostpart.costcenter_idx, 39)<>'' \
    AND acc_supplcostpart.startdate < '" + mToDate.toString(Qt::ISODate) + "' \
    AND acc_supplcostpart.enddate > '" + mFromDate.toString(Qt::ISODate) + "' \
    GROUP BY acc_supplier.suppname, acc_costgroup.sequenceno, acc_costcenter.centercode, acc_supplcostpart.type_id \
    ORDER BY acc_costgroup.costprofit, acc_costgroup.sequenceno, acc_costcenter.centercode, acc_supplier.suppname;";
}

/**
 * Set the cost center balance details in the list container
 * of the customer center and group, supplier center and group.
 * The query data objects will have:
 * 0 suppname_mname (ACC_Supplier/ACC_Customer)
 * 1 sequenceno (ACC_CostGroup)
 * 2 groupname (ACC_CostGroup)
 * 3 costprofit (ACC_CostGroup)
 * 4 centercode (ACC_CostCenter)
 * 5 centername (ACC_CostCenter)
 * 6 budget (ACC_CostBudget)
 * 7 forecast (ACC_CostBudget)
 * 8 actual (ACC_CostSum)
 * 9 weight (ACC_SupplCostPart/ACC_CustCostPart)
 * 10 type_id (ACC_SupplCostPart/ACC_CustCostPart) related to budget,
 * forecast or actual value
 * 11 startdate (ACC_SupplCostPart/ACC_CustCostPart)
 * 12 enddate (ACC_SupplCostPart/ACC_CustCostPart)
 * 13 costsumcount to compensate budget and forecast for amount acc_costsum count
 *
 * Weight will not be separate in the list objects but already multiplied
 * with the relevant budget, forecast or actual value
 * @param q query with result sets
 * @param costCenterBalanceList cost center balance list with data objects
 */
void ACC_CreateCostCenterBalanceReport
    ::setCostCenterBalanceList(QSqlQuery& query, RB_ObjectBase* costCenterBalanceList) {
    RB_Variant suppname_mname = "";
    double weight = 0.0;
    int budgetForecastActual = 0; // 0=budget 1=forecast 2=actual
    int costsumcount = 1;
    RB_ObjectBase* obj = NULL;

    while (query.next()) {
        if (!obj || !query.value(0).toString().isEmpty()) {
            obj = new RB_ObjectAtomic("", costCenterBalanceList, "ACC_CostCenterBalance");
            costCenterBalanceList->addObject(obj);
            suppname_mname = query.value(0);
            weight = query.value(9).toDouble() / 100.0; // percent
            budgetForecastActual = query.value(10).toInt();
        }

        if (query.value(13).toInt() > 1) {
            costsumcount = query.value(13).toInt();
        } else {
            costsumcount = 1;
        }

        if (!query.value(0).toString().isEmpty()) {
            // cost center or first of cost center group (centercode and centername are empty)
            obj->addMember(obj, "suppname_mname", "-", suppname_mname, RB2::MemberChar125);
            obj->addMember(obj, "sequenceno", "-", query.value(1), RB2::MemberInteger);
            obj->addMember(obj, "groupname", "-", query.value(2), RB2::MemberChar125);
            obj->addMember(obj, "costprofit", "-", query.value(3), RB2::MemberInteger);
            obj->addMember(obj, "centercode", "-", query.value(4), RB2::MemberChar125);
            obj->addMember(obj, "centername", "-", query.value(5), RB2::MemberChar125);

            switch (budgetForecastActual) {
            case 0 :
                obj->addMember(obj, "budget", "-", query.value(6).toDouble() / costsumcount * weight, RB2::MemberDouble);
                obj->addMember(obj, "forecast", "-", 0.0, RB2::MemberDouble);
                obj->addMember(obj, "actual", "-", 0.0, RB2::MemberDouble);
                break;
            case 1 :
                obj->addMember(obj, "budget", "-", 0.0, RB2::MemberDouble);
                obj->addMember(obj, "forecast", "-", query.value(7).toDouble() / costsumcount * weight, RB2::MemberDouble);
                obj->addMember(obj, "actual", "-", 0.0, RB2::MemberDouble);
                break;
            case 2 :
                obj->addMember(obj, "budget", "-", 0.0, RB2::MemberDouble);
                obj->addMember(obj, "forecast", "-", 0.0, RB2::MemberDouble);
                obj->addMember(obj, "actual", "-", query.value(8).toDouble() * weight, RB2::MemberDouble);
                break;
            default :
                obj->addMember(obj, "budget", "-", query.value(6).toDouble() / costsumcount, RB2::MemberDouble);
                obj->addMember(obj, "forecast", "-", query.value(7).toDouble() / costsumcount, RB2::MemberDouble);
                obj->addMember(obj, "actual", "-", query.value(8), RB2::MemberDouble);
                break;
            }
        } else {
            // second and next group records
            switch (budgetForecastActual) {
            case 0 :
                obj->setValue("budget", obj->getValue("budget").toDouble() + query.value(6).toDouble() / costsumcount * weight);
                break;
            case 1 :
                obj->setValue("forecast", obj->getValue("forecast").toDouble() + query.value(7).toDouble() / costsumcount * weight);
                break;
            case 2 :
                obj->setValue("actual", obj->getValue("actual").toDouble() + query.value(8).toDouble() * weight);
                break;
            default :
                break;
            }
        }

        obj->addMember(obj, "startdate", "-", query.value(11), RB2::MemberDateTime);
        obj->addMember(obj, "enddate", "-", query.value(12), RB2::MemberDateTime);
    }
}
