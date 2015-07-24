/*****************************************************************
 * $Id: sail_trackoperation.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "mrp_softwarelicenseoperation.h"

#include "mrp_modelfactory.h"
#include "mrp_sqlcommonfunctions.h"


/**
 * Constructor
 */
MRP_SoftwareLicenseOperation::MRP_SoftwareLicenseOperation() {
    mObject = NULL;
}

/**
 * Destructor
 */
MRP_SoftwareLicenseOperation::~MRP_SoftwareLicenseOperation() {
    // nothing
}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool MRP_SoftwareLicenseOperation::execute(RB_ObjectContainer* licenseList) {
    MRP_SqlCommonFunctions sqlFunc;

    QSqlQuery query(MRP_MODELFACTORY->getDatabase());

    if (mField.contains("last",Qt::CaseInsensitive)) {
        sqlFunc.selectLastNameData(query, mFieldFrom, mFieldTo);
    } else if (mField.contains("end",Qt::CaseInsensitive)) {
        sqlFunc.selectLicenseEndData(query, mFieldFrom, mFieldTo);
    } else if (mField.contains("modified",Qt::CaseInsensitive)) {
        sqlFunc.selectModifiedData(query, mFieldFrom, mFieldTo);
    } else {
        RB_DEBUG->error("MRP_SoftwareLicenseOperation::execute() error");
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

        RB_ObjectBase* obj = new RB_ObjectAtomic("", licenseList, "MRP_LicenseData");
        licenseList->addObject(obj);
        obj->addMember(obj, "contactname", "-", contactName, RB2::MemberChar125);
        obj->addMember(obj, "licensecode", "-", licenseCode, RB2::MemberChar125);
        obj->addMember(obj, "licensestart", "-", licenseStart, RB2::MemberChar125);
        obj->addMember(obj, "licenseend", "-", licenseEnd, RB2::MemberChar125);
        obj->addMember(obj, "modifieddate", "-", modified, RB2::MemberChar125);
        obj->addMember(obj, "customer", "-", customer, RB2::MemberChar125);
        obj->addMember(obj, "ordernumber", "-", orderNumber, RB2::MemberChar125);
        obj->addMember(obj, "comment", "-", comment, RB2::MemberChar255);
    }

    return true;
}

/**
 * Execute operation
 * @param input input object, if applicable
 * @param output result/output object, if applicable
 * @return true on success, default false
 */
bool MRP_SoftwareLicenseOperation::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "MRP_SoftwareLicenseOperation::execute() "
                    "not implemented ERROR");
    return false;
}

