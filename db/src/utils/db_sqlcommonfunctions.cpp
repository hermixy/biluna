/*****************************************************************
 * $Id: db_sqlcommonfunctions.cpp 2222 2015-02-26 19:38:49Z rutger $
 * Created: Sep 2, 2013 15:42:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Accounting (DB) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_sqlcommonfunctions.h"

#include <QtSql>
#include "rb.h"
#include "db_modelfactory.h"
#include "db_objectfactory.h"
#include "db_permissionperspectiveproject.h"
#include "db_utilityfactory.h"
#include "rb_settings.h"


/**
 * Constructor
 */
DB_SqlCommonFunctions::DB_SqlCommonFunctions() : RB_SqlCommonFunctions() {

}

/**
 * Destructor
 */
DB_SqlCommonFunctions::~DB_SqlCommonFunctions() {

}

// NOT USED
void DB_SqlCommonFunctions::createDatabaseIndexList(QStringList& indexList) {
    if (!DB_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_SqlCommonFunctions::createDatabaseIndexList()"
                        " database not open ERROR");
        return;
    }

    RB_String dbName = DB_MODELFACTORY->getDatabase().databaseName();
    QSqlQuery query(DB_MODELFACTORY->getDatabase());
    RB_String qStr = "";

    if (DB_MODELFACTORY->getDatabase().driverName() == "QMYSQL") {
        // MySQL
        qStr = "SELECT INDEX_NAME "
                "FROM information_schema.statistics "
                "WHERE table_schema = '" + dbName + "' "
                "AND INDEX_NAME<>'PRIMARY';";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QSQLITE") {
        // SQLite
        qStr = "SELECT `name` FROM `sqlite_master` "
                "WHERE `sqlite_master`.`type`='index' "
                "AND `sqlite_master`.`sql` LIKE 'CREATE INDEX%'";
    } else if (DB_MODELFACTORY->getDatabase().driverName() == "QODBC") {
        // ODBC
        // TODO
/*
The DESCRIBE INDEXES FOR TABLE command lists the following information about each index of the table or view:

    Index schema
    Index name
    Unique rule
    Column count
*/

    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "DB_SqlCommonFunctions::createDatabaseIndexList()"
                        " driver not supported ERROR");
        return;
    }

    if (!query.exec(qStr)) {
        RB_DEBUG->error("DB_SqlCommonFunctionsFunction::createDatabaseIndexList() "
                        + query.lastError().text() + " ERROR");
        return;
    }

    while (query.next()) {
        RB_String str = query.value(0).toString();
        indexList.append(str);
    }
}

void DB_SqlCommonFunctions::setPerspectiveProjects(RB_ObjectContainer* list) {
    QStringList tableList = DB_MODELFACTORY->getDatabase().tables();

    foreach (const QString& tableName, tableList) {
           if (tableName.endsWith("_Project", Qt::CaseInsensitive)) {
               appendProjects(list, tableName);
           }
    }
}

void DB_SqlCommonFunctions::appendProjects(RB_ObjectContainer* list,
                                           const QString& tableName) {
    QSqlQuery query(DB_MODELFACTORY->getDatabase());
    RB_String qStr = "SELECT * FROM " + tableName + " WHERE `id` <> '0';";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("DB_SqlCommonFunctionsFunction::appendProjects() "
                        + query.lastError().text() + " ERROR");
        return;
    }

    QStringList perspList = tableName.split("_");
    QString perspType = perspList.at(0);
    perspType = perspType.toUpper();

    while (query.next()) {
        RB_ObjectBase* obj = list->newObject("");
        obj->setValue("id", query.value("id").toString());
        obj->setValue("parent", query.value("parent").toString());
        obj->setValue("name", query.value("name").toString());
        obj->setValue("status", query.value("status").toString());
        obj->setValue("created", query.value("created").toString());
        obj->setValue("changed", query.value("changed").toString());
        obj->setValue("muser", query.value("muser").toString());
        obj->setValue("perspective", perspType);
        obj->setValue("number", query.value("number").toString());
        obj->setValue("description", query.value("description").toString());
        obj->setValue("company", query.value("company").toString());
        obj->setValue("location", query.value("location").toString());
    }
}

void DB_SqlCommonFunctions::getPermissionList(RB_ObjectContainer* list,
                                              const QString& userId) {
    /*
SELECT suser.username as username,
suser.firstname as firstname,
suser.lastname as lastname,
suser.start AS userstart,
suser.end AS userend,
sgroup.permission_id AS crudx,
sgroup.tokenlist AS tokenlist,
pproject.persproject_idx AS persproject,
pproject.mstatus_id AS persprojectstatus,
pproject.start AS persprojectstart,
pproject.end AS persprojectend
FROM db_systemuser AS suser
LEFT JOIN db_systemusergroup AS sugroup ON sugroup.parent=suser.id
LEFT JOIN db_systemgroup AS sgroup ON sgroup.id=SUBSTR(sugroup.group_idx,1,38)
LEFT JOIN db_permissiongroup AS pgroup ON SUBSTR(pgroup.group_idx,1,38)=sgroup.id
LEFT JOIN db_permissionproject AS pproject ON pgroup.parent=pproject.id
WHERE suser.parent='{4ea4abad-f86a-4749-8016-acfe53171f82}'
AND suser.id='XXX'
AND userstart<=SUBSTR(CURRENT_TIMESTAMP, 1, 10)
AND userend>=SUBSTR(CURRENT_TIMESTAMP, 1, 10)
ORDER BY SUBSTR(persproject_idx, 39);
     */

    if (list->getName() != "DB_SystemUserPermissionList") {
        RB_DEBUG->error("DB_SqlCommonFunctions::getPermissionList() object ERROR");
        return;
    }

    QSqlQuery query(DB_MODELFACTORY->getDatabase());
    QString qStr = "SELECT suser.username as username, "
            "suser.firstname as firstname, "
            "suser.lastname as lastname, "
            "suser.start AS userstart, "
            "suser.end AS userend, "
            "sgroup.permission_id AS crudx_id, "
            "sgroup.tokenlist AS tokenlist, "
            "pproject.perspective AS perspective, "
            "pproject.persproject_idx AS persproject_idx, "
            "pproject.mstatus_id AS persprojectstatus_id, "
            "pproject.start AS persprojectstart, "
            "pproject.end AS persprojectend "
            "FROM db_systemuser AS suser "
            "LEFT JOIN db_systemusergroup AS sugroup ON sugroup.parent=suser.id "
            "LEFT JOIN db_systemgroup AS sgroup ON sgroup.id=SUBSTR(sugroup.group_idx,1,38) "
            "LEFT JOIN db_permissiongroup AS pgroup ON SUBSTR(pgroup.group_idx,1,38)=sgroup.id "
            "LEFT JOIN db_permissionproject AS pproject ON pgroup.parent=pproject.id "
            "WHERE suser.id='";
    qStr += userId + "' AND userstart<=SUBSTR(CURRENT_TIMESTAMP, 1, 10) "
                     "AND userend>=SUBSTR(CURRENT_TIMESTAMP, 1, 10) "
                     "ORDER BY SUBSTR(persproject_idx, 39);";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("DB_SqlCommonFunctionsFunction::getPermissionList() "
                        + query.lastError().text() + " ERROR");
        return;
    }

    while (query.next()) {
        RB_ObjectBase* obj = list->newObject("");
        obj->setValue("username", query.value("username"));
        obj->setValue("firstname", query.value("firstname"));
        obj->setValue("lastname", query.value("lastname"));
        obj->setValue("userstart", query.value("userstart"));
        obj->setValue("userend", query.value("userend"));
        obj->setValue("crudx_id", query.value("crudx_id"));
        obj->setValue("tokenlist", query.value("tokenlist"));
        obj->setValue("perspective", query.value("perspective"));
        obj->setValue("persproject_idx", query.value("persproject_idx")); // id + Project number
        obj->setValue("persprojectstatus_id", query.value("persprojectstatus_id"));
        obj->setValue("persprojectstart", query.value("persprojectstart"));
        obj->setValue("persprojectend", query.value("persprojectend"));
    }
}

bool DB_SqlCommonFunctions::hasUserLoginPermission(const QString& userName,
                                                   const QString& password,
                                                   QString& userId) {
    /*
SELECT suser.username as username,
suser.password as password,
suser.firstname as firstname,
suser.lastname as lastname,
suser.start AS userstart,
suser.end AS userend
FROM db_systemuser AS suser
WHERE suser.username='rutger'
AND userstart<=SUBSTR(CURRENT_TIMESTAMP, 1, 10)
AND userend>=SUBSTR(CURRENT_TIMESTAMP, 1, 10);
    */

    QSqlQuery query(DB_MODELFACTORY->getDatabase());
    QString qStr = "SELECT suser.id as id, "
            "suser.username as username, "
            "suser.password as password, "
            "suser.firstname as firstname, "
            "suser.lastname as lastname, "
            "suser.start AS userstart, "
            "suser.end AS userend "
            "FROM db_systemuser AS suser "
            "WHERE suser.username='";
    qStr += userName + "' AND userstart<=SUBSTR(CURRENT_TIMESTAMP, 1, 10) "
                       "AND userend>=SUBSTR(CURRENT_TIMESTAMP, 1, 10);";

    if (!query.exec(qStr)) {
        RB_DEBUG->error(
                    "DB_SqlCommonFunctionsFunction::getUserLoginPermission() "
                    + query.lastError().text() + " ERROR");
        return false;
    }

    userId = "";

    while (query.next()) {
        // should only by one data set or row
        QVariant encryptedPwd = query.value("password");

        RB_DEBUG->print(DB_UTILITYFACTORY->decrypt(encryptedPwd));
        RB_DEBUG->print(password);

        if (DB_UTILITYFACTORY->decrypt(encryptedPwd) == password) {
            userId = query.value("id").toString();
            return true;
        }
    }

    return false;
}

int DB_SqlCommonFunctions::getUserCount() {
    QSqlQuery query(DB_MODELFACTORY->getDatabase());
    RB_String qStr = "SELECT id FROM db_systemuser WHERE id <> '0';";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("DB_SqlCommonFunctionsFunction::getUserCount() "
                        + query.lastError().text() + " ERROR");
        return 0;
    }

    int rows = 0;
    while (query.next()) {
        ++rows;
    }
    return rows; // query.size(); does not work with SQLite
}

void DB_SqlCommonFunctions::getPermissionReportList(QSqlQuery& query) {
    /*
SELECT db_project.number as db_number,
suser.username as username,
suser.firstname as firstname,
suser.lastname as lastname,
suser.start AS userstart,
suser.end AS userend,
sgroup.permission_id AS crudx,
sgroup.tokenlist AS tokenlist,
SUBSTR(pproject.persproject_idx, 39) AS project,
pproject.mstatus_id AS projstatus,
pproject.start AS projstart,
pproject.end AS projend
FROM db_systemuser AS suser
LEFT JOIN db_systemusergroup AS sugroup ON sugroup.parent=suser.id
LEFT JOIN db_systemgroup AS sgroup ON sgroup.id=SUBSTR(sugroup.group_idx,1,38)
LEFT JOIN db_permissiongroup AS pgroup ON SUBSTR(pgroup.group_idx,1,38)=sgroup.id
LEFT JOIN db_permissionproject AS pproject ON pgroup.parent=pproject.id
LEFT JOIN db_project ON db_project.id=suser.parent
WHERE suser.id <> '0'
ORDER BY db_number, username, project;
     */

    RB_String qStr = "SELECT db_project.number as db_number, "
            "suser.username as username, "
            "suser.firstname as firstname, "
            "suser.lastname as lastname, "
            "suser.start AS userstart, "
            "suser.end AS userend, "
            "sgroup.permission_id AS crudx, "
            "sgroup.tokenlist AS tokenlist, "
            "pproject.perspective AS perspective, "
            "SUBSTR(pproject.persproject_idx, 39) AS project, "
            "pproject.mstatus_id AS projstatus, "
            "pproject.start AS projstart, "
            "pproject.end AS projend "
            "FROM db_systemuser AS suser "
            "LEFT JOIN db_systemusergroup AS sugroup ON sugroup.parent=suser.id "
            "LEFT JOIN db_systemgroup AS sgroup ON sgroup.id=SUBSTR(sugroup.group_idx,1,38) "
            "LEFT JOIN db_permissiongroup AS pgroup ON SUBSTR(pgroup.group_idx,1,38)=sgroup.id "
            "LEFT JOIN db_permissionproject AS pproject ON pgroup.parent=pproject.id "
            "LEFT JOIN db_project ON db_project.id=suser.parent "
            "WHERE suser.id <> '0' "
            "ORDER BY db_number, username, perspective, project;";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("DB_SqlCommonFunctionsFunction::getPermissionReport() "
                        + query.lastError().text() + " ERROR");
    }

}
