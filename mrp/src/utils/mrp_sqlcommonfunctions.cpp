/*****************************************************************
 * $Id: sail_sqlcommonfunctions.cpp 1746 2012-08-23 21:01:10Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "mrp_sqlcommonfunctions.h"

#include "mrp_dialogfactory.h"
#include "mrp_modelfactory.h"
#include "mrp_objectfactory.h"
#include "rb_settings.h"

/**
 * Constructor
 */
MRP_SqlCommonFunctions::MRP_SqlCommonFunctions() {
    // nothing
}

/**
 * Destructor
 */
MRP_SqlCommonFunctions::~MRP_SqlCommonFunctions() {
    // nothing
}

void MRP_SqlCommonFunctions::selectLastNameData(QSqlQuery& query,
                                                const QString& fieldFrom,
                                                const QString& fieldTo) {
    selectLicenseData(query, "contact_idx", fieldFrom, fieldTo);
}

void MRP_SqlCommonFunctions::selectLicenseEndData(QSqlQuery& query,
                                                  const QString& fieldFrom,
                                                  const QString& fieldTo) {
    selectLicenseData(query, "enddate", fieldFrom, fieldTo);
}

void MRP_SqlCommonFunctions::selectModifiedData(QSqlQuery& query,
                                                const QString& fieldFrom,
                                                const QString& fieldTo) {
    selectLicenseData(query, "changed", fieldFrom, fieldTo);
}

/**
 * Create SQL statement and execute select with the return fields in SQL:
 * 0 - contactname (contact_idx=lastname, firstname)
 * 1 - licensecode
 * 2 - licensestart
 * 3 - licenseend (enddate)
 * 4 - modified (changed)
 * 5 - customer (customer_idx=mname)
 * 6 - ordernumber (salesorder_idx=orderno)
 * 7 - comment
 */
void MRP_SqlCommonFunctions::selectLicenseData(QSqlQuery& query,
                                               const QString& fieldName,
                                               const QString& fieldFrom,
                                               const QString& fieldTo) {
    RB_String qStr = "";
    qStr = "SELECT SUBSTR(mrp_softwarelicense.contact_idx, 39), "
            "mrp_softwarelicense.licensecode, mrp_softwarelicense.startdate, "
            "mrp_softwarelicense.enddate, mrp_softwarelicense.changed, "
            "SUBSTR(mrp_softwarelicense.customer_idx, 39), "
            "SUBSTR(mrp_softwarelicense.salesorder_idx, 39), comment "
           "FROM mrp_softwarelicense ";
    if (!fieldName.endsWith("_idx")) {
        qStr += "WHERE " + fieldName + " >= '" + fieldFrom + "' "
                "AND " + fieldName + " <= '" + fieldTo + "' ";
    } else {
        qStr += "WHERE SUBSTR(" + fieldName + ", 39) >= '" + fieldFrom + "' "
                "AND SUBSTR(" + fieldName + ", 39) <= '" + fieldTo + "' ";
    }

    qStr += "AND mrp_softwarelicense.id > '0'; ";

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "MRP_SqlCommonFunctions::selectData() 1 %s ERROR",
                        query.lastError().text().toStdString().c_str());
        MRP_DIALOGFACTORY->commandMessage("MRP_SqlCommonFunctions::selectRouteDistanceData() "
                                          + query.lastError().text() + "\n");
    }
}
