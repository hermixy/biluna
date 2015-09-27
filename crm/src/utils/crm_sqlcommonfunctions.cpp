/*****************************************************************
 * $Id: crm_sqlcommonfunctions.cpp 2199 2014-12-08 21:41:10Z rutger $
 * Created: Mar 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Customer Relation Management (CRM) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "crm_sqlcommonfunctions.h"

#include "acc.h"
#include "crm_dialogfactory.h"
#include "crm_modelfactory.h"
#include "crm_objectfactory.h"
#include "rb_settings.h"

/**
 * Constructor
 */
CRM_SqlCommonFunctions::CRM_SqlCommonFunctions()
                        : RB_SqlCommonFunctions() {

}

/**
 * Destructor
 */
CRM_SqlCommonFunctions::~CRM_SqlCommonFunctions() {

}

void CRM_SqlCommonFunctions::getTargetList(QSqlQuery& query,
                                           const RB_String& groupId) {
    if (!CRM_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error("CRM_SqlCommonFunctions::getTargetList()"
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
    FROM `crm_groupcontact`
    INNER JOIN `acc_contact` ON `acc_contact`.`id`=SUBSTR(`crm_groupcontact`.`contact_idx`, 0, 39)
    INNER JOIN `acc_customer` ON `acc_customer`.`id`=`acc_contact`.`parent`
    WHERE `crm_groupcontact`.`parent`='{4408b399-2cb7-491d-81ad-4c676a1c1ef6}'
    ORDER BY `acc_contact`.`lastname`;
    */

    RB_String qStr = "";

    qStr = "SELECT `acc_contact`.`salutation`, `acc_contact`.`firstname`, "
            "`acc_contact`.`lastname`, `acc_contact`.`title`, `acc_contact`.`department`, "
            "`acc_contact`.`phonework`, `acc_contact`.`phonefax`, `acc_contact`.`email1`, "
            "`acc_contact`.`primaryaddress1`, `acc_contact`.`primaryaddress2`, "
            "`acc_contact`.`primaryaddress3`, `acc_contact`.`primaryaddress4`, "
            "`acc_contact`.`primaryaddress5`, `acc_contact`.`primaryaddress6`, "
            "`acc_customer`.`customerno`, `acc_customer`.`mname`, "
            "`acc_contact`.`id`, `acc_contact`.`parent` "
            "FROM `crm_groupcontact` "
            "INNER JOIN `acc_contact` ON `acc_contact`.`id`=";
    this->substrIdxId("crm_groupcontact.contact_idx", qStr);
    qStr += " INNER JOIN `acc_customer` ON `acc_customer`.`id`=`acc_contact`.`parent` "
            "WHERE `crm_groupcontact`.`parent`='" + groupId + "' "
            "ORDER BY `acc_contact`.`lastname`;";


    if (!query.exec(qStr)) {
        RB_DEBUG->error("CRM_SqlCommonFunctions::getTargetList() "
                        + query.lastError().text() + " ERROR");
        RB_String str = " groupId = " + groupId;
        CRM_DIALOGFACTORY->commandMessage("CRM_SqlCommonFunctions::getTargetList() "
                                          + query.lastError().text() + "\n" + str);
    }
}

void CRM_SqlCommonFunctions::getTemplateId(QSqlQuery& query, const QString& targetId) {
    /*
    SELECT  SUBSTR(`crm_campaign`.`document_idx`,0,39)
    FROM `crm_campaigntarget`
    INNER JOIN `crm_campaign` ON `crm_campaign`.`id`=`crm_campaigntarget`.`parent`
    WHERE `crm_campaigntarget`.`id`='{4408b399-2cb7-491d-81ad-4c676a1c1ef6}';
    */

    RB_String qStr = "";

    qStr = "SELECT ";
    this->substrIdxId("crm_campaign.document_idx", qStr);
    qStr += " FROM `crm_campaigntarget` "
            "INNER JOIN `crm_campaign` ON `crm_campaign`.`id`=`crm_campaigntarget`.`parent` "
            "WHERE `crm_campaigntarget`.`id`='" + targetId + "';";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("CRM_SqlCommonFunctions::getDocumentId() "
                        + query.lastError().text() + " ERROR");
        RB_String str = " targetId = " + targetId;
        CRM_DIALOGFACTORY->commandMessage("CRM_SqlCommonFunctions::getDocumentId() "
                                          + query.lastError().text() + "\n" + str);
    }
}

int CRM_SqlCommonFunctions::getLastUsedSystemSeqNumber(int modelType,
                                                       const QString& modelSubTypeId) {
    return RB_SqlCommonFunctions::getLastUsedSystemSeqNumber(
                "CRM", CRM_MODELFACTORY->getRootId(), modelType, modelSubTypeId);

}

bool CRM_SqlCommonFunctions::setLastSystemSeqNumber(int modelType, int lastNumber,
                                                    const QString& modelSubTypeId) {
    return RB_SqlCommonFunctions::setLastSystemSeqNumber(
                "CRM", CRM_MODELFACTORY->getRootId(), modelType, lastNumber,
                modelSubTypeId);
}

int CRM_SqlCommonFunctions::getNextSystemSeqNumber(int modelType,
                                                   const QString& seqNumberName,
                                                   const QString& modelSubTypeId) {
    return RB_SqlCommonFunctions::getNextSystemSeqNumber(
                "CRM", CRM_MODELFACTORY->getRootId(), modelType,
                seqNumberName, modelSubTypeId);
}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE 'whereStr'
 * @param field field name
 * @param table table name
 * @param whereStr where string
 * @return variant result value
 */
/*RB_Variant CRM_SqlCommonFunctions::selectFromWhere(const RB_String& field,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    if (!CRM_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE " + whereStr + ";";
    QSqlQuery query(CRM_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "CRM_SqlCommonFunctionsFunction::selectFromWhereId() "
                        + query.lastError().text() + " ERROR");
    }

    return RB_Variant();
}
*/

