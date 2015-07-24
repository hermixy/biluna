/*****************************************************************
 * $Id: srm_sqlcommonfunctions.cpp 2073 2014-01-25 17:07:56Z rutger $
 * Created: Mar 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Customer Relation Management (CRM) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "srm_sqlcommonfunctions.h"

#include "acc.h"
#include "srm_dialogfactory.h"
#include "srm_modelfactory.h"
#include "srm_objectfactory.h"
#include "rb_settings.h"

/**
 * Constructor
 */
SRM_SqlCommonFunctions::SRM_SqlCommonFunctions() {

}

/**
 * Destructor
 */
SRM_SqlCommonFunctions::~SRM_SqlCommonFunctions() {

}

void SRM_SqlCommonFunctions::getTargetList(QSqlQuery& query,
                                           const RB_String& groupId) {
    if (!SRM_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error("SRM_SqlCommonFunctions::getTargetList()"
                        " database not open ERROR");
        return;
    }
    /*
    SELECT `acc_contact`.`salutation`, `acc_contact`.`firstname`,
     `acc_contact`.`lastname`, `acc_contact`.`title`, `acc_contact`.`department`,
     `acc_contact`.`phonework`, `acc_contact`.`phonefax`, `acc_contact`.`email1`,
     `acc_contact`.`primaryaddress1`, `acc_contact`.`primaryaddress2`,
     `acc_contact`.`primaryaddress3`, `acc_contact`.`primaryaddress4`,
     `acc_contact`.`primaryaddress5`, `acc_contact`.`primaryaddress6`,
     `acc_customer`.`customerno`, `acc_customer`.`mname`
    FROM `srm_groupcontact`
    INNER JOIN `acc_contact` ON `acc_contact`.`id`=SUBSTR(`srm_groupcontact`.`contact_idx`, 0, 39)
    INNER JOIN `acc_customer` ON `acc_customer`.`id`=`acc_contact`.`parent`
    WHERE `srm_groupcontact`.`parent`='{4408b399-2cb7-491d-81ad-4c676a1c1ef6}'
    ORDER BY `acc_contact`.`lastname`;
    */

    RB_String qStr = "";

    qStr = "SELECT `acc_contact`.`salutation`, `acc_contact`.`firstname`, "
            "`acc_contact`.`lastname`, `acc_contact`.`title`, `acc_contact`.`department`, "
            "`acc_contact`.`phonework`, `acc_contact`.`phonefax`, `acc_contact`.`email1`, "
            "`acc_contact`.`primaryaddress1`, `acc_contact`.`primaryaddress2`, "
            "`acc_contact`.`primaryaddress3`, `acc_contact`.`primaryaddress4`, "
            "`acc_contact`.`primaryaddress5`, `acc_contact`.`primaryaddress6`, "
            "`acc_supplier`.`suppliercode`, `acc_supplier`.`suppname`, "
            "`acc_contact`.`id`, `acc_contact`.`parent` "
            "FROM `srm_groupcontact` "
            "INNER JOIN `acc_contact` ON `acc_contact`.`id`=";
    this->substrIdxId("srm_groupcontact.contact_idx", qStr);
    qStr += " INNER JOIN `acc_supplier` ON `acc_supplier`.`id`=`acc_contact`.`parent` "
            "WHERE `srm_groupcontact`.`parent`='" + groupId + "' "
            "ORDER BY `acc_contact`.`lastname`;";


    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "srm_SqlCommonFunctions::getTargetList() %s ERROR",
                        query.lastError().text().toStdString().c_str());
        RB_String str = " groupId = " + groupId;
        SRM_DIALOGFACTORY->commandMessage("SRM_SqlCommonFunctions::getTargetList() "
                                          + query.lastError().text() + "\n" + str);
    }
}

void SRM_SqlCommonFunctions::getTemplateId(QSqlQuery& query, const QString& targetId) {
    /*
    SELECT  SUBSTR(`srm_campaign`.`document_idx`,0,39)
    FROM `srm_campaigntarget`
    INNER JOIN `srm_campaign` ON `srm_campaign`.`id`=`srm_campaigntarget`.`parent`
    WHERE `srm_campaigntarget`.`id`='{4408b399-2cb7-491d-81ad-4c676a1c1ef6}';
    */

    RB_String qStr = "";

    qStr = "SELECT ";
    this->substrIdxId("srm_campaign.document_idx", qStr);
    qStr += " FROM `srm_campaigntarget` "
            "INNER JOIN `srm_campaign` ON `srm_campaign`.`id`=`srm_campaigntarget`.`parent` "
            "WHERE `srm_campaigntarget`.`id`='" + targetId + "';";

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "srm_SqlCommonFunctions::getDocumentId() %s ERROR",
                        query.lastError().text().toStdString().c_str());
        RB_String str = " targetId = " + targetId;
        SRM_DIALOGFACTORY->commandMessage("SRM_SqlCommonFunctions::getDocumentId() "
                                          + query.lastError().text() + "\n" + str);
    }
}

int SRM_SqlCommonFunctions::getLastUsedSystemSeqNumber(int modelType,
                                                       const QString& modelSubTypeId) {
    return RB_SqlCommonFunctions::getLastUsedSystemSeqNumber(
                "SRM", SRM_MODELFACTORY->getRootId(), modelType, modelSubTypeId);

}

bool SRM_SqlCommonFunctions::setLastSystemSeqNumber(int modelType, int lastNumber,
                                                    const QString& modelSubTypeId) {
    return RB_SqlCommonFunctions::setLastSystemSeqNumber(
                "SRM", SRM_MODELFACTORY->getRootId(), modelType, lastNumber,
                modelSubTypeId);
}

int SRM_SqlCommonFunctions::getNextSystemSeqNumber(int modelType,
                                                   const QString& seqNumberName,
                                                   const QString& modelSubTypeId) {
    return RB_SqlCommonFunctions::getNextSystemSeqNumber(
                "SRM", SRM_MODELFACTORY->getRootId(), modelType,
                seqNumberName, modelSubTypeId);
}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE 'whereStr'
 * @param field field name
 * @param table table name
 * @param whereStr where string
 * @return variant result value
 */
RB_Variant SRM_SqlCommonFunctions::selectFromWhere(const RB_String& field,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    if (!SRM_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "srm_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE " + whereStr + ";";
    QSqlQuery query(SRM_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "srm_SqlCommonFunctionsFunction::selectFromWhereId() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    return RB_Variant();
}

