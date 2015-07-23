/*****************************************************************
 * $Id: rb_sqlcommonfunctions.cpp 2199 2014-12-08 21:41:10Z rutger $
 * Created: Jun 23, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_sqlcommonfunctions.h"

#include <QtSql>
#include "rb.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "rb_settings.h"

/**
 * Constructor
 */
RB_SqlCommonFunctions::RB_SqlCommonFunctions() {
    // nothing
}

/**
 * Destructor
 */
RB_SqlCommonFunctions::~RB_SqlCommonFunctions() {
    // nothing
}

/**
 * Get ID from _idx field
 * IDX fields are as follows:
 *
 * For example: if table acc_project has rows with taxprovince_idx field:
 * {7c6d6bb5-9308-4440-930a-86ff36d90e21}Nederland
 * {7c6d6bb5-9308-4440-930a-86ff36d90e21}Belgie
 *
 * SELECT SUBSTR(taxprovince_idx, 39) as ID
 * FROM acc_project
 * WHERE SUBSTR(taxprovince_idx, 1, 38)='{7c6d6bb5-9308-4440-930a-86ff36d90e21}';
 *
 * returns:
 *    ID
 * Nederland
 * Belgie
 * @param field field or column name
 * @param sql adds SUBSTR(field,1,38) to SQL
 */
void RB_SqlCommonFunctions::substrIdxId(const RB_String& field, RB_String& sql) {
    sql += "SUBSTR(" + field + ",1,38)";
}

/**
 * Get content from _idx field
 * @param field field or column name
 * @param sql add SUBSTR(field,39) to SQL
 */
void RB_SqlCommonFunctions::substrIdxContent(const RB_String& field, RB_String& sql) {
    sql += "SUBSTR(" + field + ",39)";
}

/**
 * @brief RB_SqlCommonFunctions::isPerspectiveVersionExisting
 * @param persp perspective code
 * @param major major version number
 * @param minor minor version number
 * @param build optional build number
 * @returns true if exists
 */
bool RB_SqlCommonFunctions::isPerspectiveVersionExisting(const RB_String& persp,
                                                         int major, int medior,
                                                         int minor, int build) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctions::isPerspectiveVersionExisting()"
                        " database not open ERROR");
        return false;
    }

    RB_String qStr = "SELECT * FROM `db_version` WHERE perspectivecode='"
            + persp + "' AND major=" + RB_String::number(major)
            + " AND medior=" + RB_String::number(medior)
            + " AND minor=" + RB_String::number(minor);
    if (build > 0) {
        qStr += " AND build=" + RB_String::number(build);
    }
    qStr += ";";

    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->warning("RB_SqlCommonFunctionsFunction::isPerspectiveVersionExisting() "
                          + query.lastError().text() + " ERROR");
        return false;
    }

    if (query.first()) {
        // a record exists
        return true;
    }

    return false;
}

void RB_SqlCommonFunctions::insertPerspectiveVersion(const QString& persp,
                                                     int major, int medior,
                                                     int minor, int build,
                                                     const QString& comment) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->error("RB_SqlCommonFunctions::insertPerspectiveVersion()"
                        " database not open ERROR");
        return;
    }

    if (isPerspectiveVersionExisting(persp, major, medior, minor, build)) {
        RB_DEBUG->warning("RB_SqlCommonFunctions::insertPerspectiveVersion() "
                          "is already existing WARNING");
        return;
    }

    RB_String qStr = "INSERT INTO `db_version` "
            "(id, parent, name, status, created, changed, muser, "
            "perspectivecode, major, medior, minor, build, comment) VALUES ('"
            + RB_Uuid::createUuid().toString() + "', 'none', 'DB_Version', 0, '"
            + QDateTime::currentDateTime().toString(Qt::ISODate) + "', '"
            + QDateTime::currentDateTime().toString(Qt::ISODate) + "', 'system', '"
            + persp + "', "
            + RB_String::number(major) + ", "
            + RB_String::number(medior) + ", "
            + RB_String::number(minor) + ", "
            + RB_String::number(build) + ", '"
            + comment + "');";

    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("RB_SqlCommonFunctionsFunction::insertPerspectiveVersion() "
                        + query.lastError().text() + " ERROR");
        return;
    }
}

/**
 * Get last record sequence number base on model (=ACC: transaction) type
 * and the list ID. These sequence numbers are maintained
 * in the table XXX_SysSeqNo. Assumes the availability of XXX_SysSeqNo
 * table where XXX is perspectiveCode
 * @param perspectiveCode letter code of perspective, ACC is accounting
 * @param parentId usually XXX_Project.id (is modelfactory root ID)
 * @param modelType model type from model factory
 * @param modelSubTypeId the ID to differentiate between records in
 * the same table. This table will have a separate subID field to differentiate
 * between groups in the table. In ACC this is the transaction list ID
 * for example ACC_ChartMaster ID to differentiate for example between
 * bank accounts.
 * @returns -1 if the record does not exist.
 */
int RB_SqlCommonFunctions::getLastUsedSystemSeqNumber(const QString &perspectiveCode,
                                                      const QString &parentId,
                                                      int modelType,
                                                      const QString &modelSubTypeId) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctions::getLastUsedSystemSeqNumber()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    // Check if XXX_SysSeqNo has been set for this project
    qStr = "SELECT sequencenumber FROM " + perspectiveCode.toLower()
            + "_sysseqno WHERE modeltype="
            + RB_String::number(modelType)
            + " AND modelsubtype_id='" + modelSubTypeId
            + "' AND parent='" + parentId + "'";
    if (query.exec(qStr)) {
        if (!query.first()) {
            return -1;
        } else {
            // return the value
            return query.value(0).toInt();
        }
    }

    RB_DEBUG->print(RB_Debug::D_ERROR,
                    "RB_SqlCommonFunctionsFunction::getLastUsedSystemSeqNumber() %s ERROR",
                    query.lastError().text().toStdString().c_str());
    return -1;
}

/**
 * Set last used sequence. Used when importing bank statements.
 * @param perspectiveCode
 * @param parentId usually XXX_Project.id (is modelfactory root ID)
 * @param modelType
 * @param modelSubTypeId
 * @param lastNumber
 * @return
 */
bool RB_SqlCommonFunctions::setLastSystemSeqNumber(const QString& perspectiveCode,
                                                   const QString& parentId,
                                                   int modelType,
                                                   int lastNumber,
                                                   const QString& modelSubTypeId) {
    RB_String qStr = "";
    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();

    qStr = "UPDATE " + perspectiveCode.toLower() + "_sysseqno SET sequencenumber="
            + RB_String::number(lastNumber)
            + ", changed='" + QDateTime::currentDateTime().toString(Qt::ISODate)
            + "', muser='" + username
            + "' WHERE modeltype=" + RB_String::number(modelType)
            + " AND modelsubtype_id='" + modelSubTypeId
            + "' AND parent='" + parentId + "'";

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctionsFunction::setLastSystemSeqNumber() %s ERROR",
                        query.lastError().text().toStdString().c_str());

        return false;
    }

    return true;
}

/**
 * @brief RB_SqlCommonFunctions::getNextSystemSeqNumber
 * @param perspectiveCode
 * @param parentId usually XXX_Project.id (is modelfactory root ID)
 * @param modelType
 * @param modelSubTypeId
 * @param seqNumberName
 * @return
 */
int RB_SqlCommonFunctions::getNextSystemSeqNumber(const QString& perspectiveCode,
                                                  const QString& parentId,
                                                  int modelType,
                                                  const QString& seqNumberName,
                                                  const QString& modelSubTypeId) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctions::getNextSystemSeqNumber()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    // Check if XXX_SysSeqNo has been set for this project
    qStr = "SELECT sequencenumber FROM " + perspectiveCode.toLower()
            + "_sysseqno WHERE modeltype="
            + RB_String::number(modelType)
            + " AND modelsubtype_id='" + modelSubTypeId
            + "' AND parent='" + parentId + "'";
    if (query.exec(qStr)) {
        if (!query.first()) {
            if (!createSystemSeqNumber(perspectiveCode, parentId, modelType,
                                       seqNumberName, modelSubTypeId)) {
                return -1;
            }
        }
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctionsFunction::getNextSystemSeqNumber() %s ERROR",
                        query.lastError().text().toStdString().c_str());
        return -1;
    }

    // Set new transaction number
    DB_MODELFACTORY->getDatabase().transaction();
    query.clear();
    query.exec(qStr); // rerun query

    if (!query.first()) {
        DB_MODELFACTORY->getDatabase().commit();
        return -1; // ERROR no XXX_SysType
    }

    int nextSeqNo = query.value(0).toInt();
    ++nextSeqNo;
    query.clear();

    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();

    qStr = "UPDATE " + perspectiveCode.toLower() + "_sysseqno SET sequencenumber="
            + RB_String::number(nextSeqNo)
            + ", changed='" + QDateTime::currentDateTime().toString(Qt::ISODate)
            + "', muser='" + username
            + "' WHERE modeltype=" + RB_String::number(modelType)
            + " AND modelsubtype_id='" + modelSubTypeId
            + "' AND parent='" + parentId + "'";

    if (!query.exec(qStr)) {
        DB_MODELFACTORY->getDatabase().rollback();
        return -1;
    }

    // Commit transaction
    DB_MODELFACTORY->getDatabase().commit();
    query.clear();
    return nextSeqNo;
}

/**
 * @brief RB_SqlCommonFunctions::createSystemSeqNumber
 * @param perspectiveCode
 * @param parentId usually XXX_Project.id (is modelfactory root ID)
 * @param modelType
 * @param modelSubTypeId
 * @param seqNumberName
 * @return
 */
bool RB_SqlCommonFunctions::createSystemSeqNumber(const QString& perspectiveCode,
                                                  const QString& parentId,
                                                  int modelType,
                                                  const QString& seqNumberName,
                                                  const QString& modelSubTypeId) {
    // Create new record because record does not exists
    int seqNumber = RB2::modelTypeToPerspective(modelType); // min 11 max 99
    seqNumber *= RB2::MODELPREFIX_TO_MODELTYPE_MULTIPLIER; // is 1000
    seqNumber += (int) modelType % (RB2::MODELPREFIX_TO_MODELTYPE_MULTIPLIER);
    seqNumber *= 10000; // to largest integer in spinbox (9 digits)

    RB_String dt = QDateTime::currentDateTime().toString(Qt::ISODate);
    RB_SETTINGS->beginGroup("database");
    RB_String username = RB_SETTINGS->value("username", "unknown").toString();
    RB_SETTINGS->endGroup();

    RB_ObjectBase* obj = new RB_ObjectAtomic(RB_Uuid::createUuid().toString(),
                                             NULL, perspectiveCode + "_SysSeqNo");
    obj->setValue("parent", parentId);
    obj->setValue("created", dt);
    obj->setValue("changed", dt);
    obj->setValue("muser", username);
    obj->addMember(obj, "modeltype", "-", modelType, RB2::MemberInteger);
    obj->addMember(obj, "seqnumbername", "-", seqNumberName, RB2::MemberChar125);
    obj->addMember(obj, "sequencenumber", "-", seqNumber, RB2::MemberInteger);
    obj->addMember(obj, "modelsubtype_id", "-", modelSubTypeId, RB2::MemberChar40);
    obj->setFlag(RB2::FlagIsDirty);

    if (!obj->dbUpdate(DB_MODELFACTORY->getDatabase())) {
        RB_DEBUG->debug("RB_SqlCommonFunctions::createSystemSeqNumber() sql ERROR:");
        RB_DEBUG->print(" " + DB_MODELFACTORY->getDatabase().lastError().text());
        delete obj;
        return false;
    }

    delete obj;
    return true;
}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE id='id'
 * @return sql result value
 */
RB_Variant RB_SqlCommonFunctions::selectFromWhereId(const RB_String& field,
                                                     const RB_String& table,
                                                     const RB_String& id) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE id='" + id + "';";
    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctionsFunction::selectFromWhereId() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    return RB_Variant();
}

/**
 * Simple query execution UPDATE 'table' SET 'field'='value' WHERE id='id'
 * @returns true on success
 */
bool RB_SqlCommonFunctions::update(const RB_String& table,
                                    const RB_String& field,
                                    const RB_String& value,
                                    const RB_String& id) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctions::updateStringWhereId()"
                        " database not open ERROR");
        return false;
    }

    RB_String qStr = "";
    qStr = "UPDATE " + table.toLower() + " SET " + field + "='" + value + "' WHERE id='" + id + "';";
    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    bool success = query.exec(qStr);
    if (!success) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctionsFunction::updateStringWhereId() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    return success;
}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE 'whereStr'
 * @param field field name
 * @param table table name
 * @param whereStr where string
 * @return variant result value
 */
RB_Variant RB_SqlCommonFunctions::selectFromWhere(const RB_String& field,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE " + whereStr + ";";
    QSqlQuery query(DB_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctionsFunction::selectFromWhereId() "
                        "%s ERROR", query.lastError().text().toStdString().c_str());
    }

    return RB_Variant();
}

/**
 * Simple query execution SELECT * FROM 'table' WHERE 'whereStr'
 * @param query query that will contain the result record set
 * @param field field name
 * @param table table name
 * @param whereStr where string
 */
void RB_SqlCommonFunctions::selectAllFromWhere(QSqlQuery& query,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    RB_String qStr = "";
    qStr = "SELECT * FROM " + table.toLower() + " WHERE " + whereStr + ";";

    if (!query.exec(qStr)) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "RB_SqlCommonFunctionsFunction::getSelectAllFromWhere() %s ERROR",
                        query.lastError().text().toStdString().c_str());
    }
}

