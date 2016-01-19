/*****************************************************************
 * $Id: acc_qacostcenter.cpp 2202 2016-01-07 19:39:36Z rutger $
 * Created: Jan 07, 2016 5:34:25 PM - rutger
 *
 * Copyright (C) 2016 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "acc_qacostcenter.h"

#include "acc.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "acc_sqlcommonfunctions.h"
#include "acc_utilityfactory.h"


ACC_QaCostCenter* ACC_QaCostCenter::mActiveUtility = 0;


/**
 * Constructor
 */
ACC_QaCostCenter::ACC_QaCostCenter() : RB_Utility() {
    RB_DEBUG->print("ACC_QaCostCenter::ACC_QaCostCenter()");
    initMemberValues();
    ACC_UTILITYFACTORY->addUtility(this);
}

void ACC_QaCostCenter::initMemberValues() {
    mCostCenterDefaultId = "";
    mCostCenterDefaultName = "";
}

/**
 * Destructor
 */
ACC_QaCostCenter::~ACC_QaCostCenter() {
    ACC_UTILITYFACTORY->removeUtility(this);
    mActiveUtility = NULL;
    RB_DEBUG->print("ACC_QaCostCenter::~ACC_QaCostCenter() OK");
}

/**
 * Create or get instance
 * @return factory
 */
ACC_QaCostCenter* ACC_QaCostCenter::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new ACC_QaCostCenter();
        mActiveUtility->refresh();
    }
    return mActiveUtility;
}

/**
 * Get account object based on account ID
 * @param ccId account ID
 * @return account object
 */
RB_ObjectBase* ACC_QaCostCenter::getCostCenterObj(const RB_String& costCenterId) {
    RB_ObjectBase* ccObj = NULL;

    std::map<RB_String, RB_ObjectBase*>::iterator iterMap
            = mCostCenterMap.find(costCenterId);
    if (iterMap != mCostCenterMap.end()) {
        ccObj = iterMap->second;
    }

    return ccObj;
}

/**
 * @returns GL accounts/chartmaster list
 */
RB_ObjectContainer* ACC_QaCostCenter::getCostCenterList() {
    return mCostCenterList;
}

/**
 * Refresh GL accounts or chartmaster list,
 * called after changes to database table ACC_ChartMaster
 */
void ACC_QaCostCenter::refresh() {
    initMemberValues();
    setCostCenterList();
}

/**
 * Set the standard accounts such as Account Payable, -Receivable and
 * various tas ID and name for later use.
 * These values are frequently used when the user edits the accounting of
 * the sales- and purchase invoices.
 */
void ACC_QaCostCenter::setCostCenterList() {
    mCostCenterList = ACC_MODELFACTORY->getRoot()->getContainer("ACC_CostCenterList");
    mCostCenterList->erase();
    mCostCenterMap.clear();

    // Create the cost center map
    QSqlQuery query(ACC_MODELFACTORY->getDatabase());
    RB_String sqlStr = "SELECT * FROM acc_costcenter WHERE ";
    sqlStr += "parent='" + ACC_MODELFACTORY->getRootId() + "' ORDER BY centercode ASC;";
    query.exec(sqlStr);

    while (query.next()) {
        RB_ObjectBase* obj = ACC_OBJECTFACTORY->newObject("", mCostCenterList, "");
        obj->setValue("id", query.record().value(query.record().indexOf("id")));
        obj->setValue("parent", query.record().value(query.record().indexOf("parent")));
        obj->setValue("name", query.record().value(query.record().indexOf("name")));
        obj->setValue("status", query.record().value(query.record().indexOf("status")));
        obj->setValue("created", query.record().value(query.record().indexOf("created")));
        obj->setValue("changed", query.record().value(query.record().indexOf("changed")));
        obj->setValue("muser", query.record().value(query.record().indexOf("muser")));
        obj->setValue("centercode", query.record().value(query.record().indexOf("centercode")));
        obj->setValue("centername", query.record().value(query.record().indexOf("centername")));
        obj->setValue("description", query.record().value(query.record().indexOf("description")));
        obj->setValue("centergroup_idx", query.record().value(query.record().indexOf("centergroup_idx")));
        mCostCenterMap[query.record().value(0).toString()]= obj;
    }

    std::map<RB_String, RB_ObjectBase*>::iterator iter;

    for (iter = mCostCenterMap.begin(); iter != mCostCenterMap.end(); ++iter) {
        RB_ObjectBase* actObj = iter->second;
        QString centerName = actObj->getValue("centername").toString();

        if (centerName.contains("default", Qt::CaseInsensitive)) {
            mCostCenterDefaultId = actObj->getId();
            mCostCenterDefaultName = actObj->getValue("centercode").toString()
                    + " - " + actObj->getValue("centername").toString();
        }
    }
}

