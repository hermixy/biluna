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

    while (query.next()) {
        RB_ObjectBase* obj = list->newObject("");
        obj->setValue("id", query.value("id").toString());
        obj->setValue("parent", query.value("parent").toString());
        obj->setValue("name", query.value("name").toString());
        obj->setValue("status", query.value("status").toString());
        obj->setValue("created", query.value("created").toString());
        obj->setValue("changed", query.value("changed").toString());
        obj->setValue("muser", query.value("muser").toString());
        obj->setValue("description", query.value("name").toString()
                       + " - " + query.value(7).toString());
    }
}

void DB_SqlCommonFunctions::getPermissionList(RB_ObjectContainer* list,
                                              const QString& userName,
                                              const QString& password) {
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
INNER JOIN db_systemusergroup AS sugroup ON sugroup.parent=suser.id
INNER JOIN db_systemgroup AS sgroup ON sgroup.id=SUBSTR(sugroup.group_idx,1,38)
INNER JOIN db_permissiongroup AS pgroup ON SUBSTR(pgroup.group_idx,1,38)=sgroup.id
INNER JOIN db_permissionproject AS pproject ON pgroup.parent=pproject.id
WHERE suser.parent='{4ea4abad-f86a-4749-8016-acfe53171f82}'
ORDER BY persproject_idx;
     */

    if (list->getName() != "DB_SystemUserPermissionList") {
        RB_DEBUG->error("DB_SqlCommonFunctions::getPermissionList() object ERROR");
        return;
    }

    QSqlQuery query(DB_MODELFACTORY->getDatabase());
    RB_String qStr = "SELECT suser.username as username, "
            "suser.firstname as firstname, "
            "suser.lastname as lastname, "
            "suser.start AS userstart, "
            "suser.end AS userend, "
            "sgroup.permission_id AS crudx_id, "
            "sgroup.tokenlist AS tokenlist, "
            "pproject.persproject_idx AS persproject_idx, "
            "pproject.mstatus_id AS persprojectstatus_id, "
            "pproject.start AS persprojectstart, "
            "pproject.end AS persprojectend "
            "FROM db_systemuser AS suser "
            "INNER JOIN db_systemusergroup AS sugroup ON sugroup.parent=suser.id "
            "INNER JOIN db_systemgroup AS sgroup ON sgroup.id=SUBSTR(sugroup.group_idx,1,38) "
            "INNER JOIN db_permissiongroup AS pgroup ON SUBSTR(pgroup.group_idx,1,38)=sgroup.id "
            "INNER JOIN db_permissionproject AS pproject ON pgroup.parent=pproject.id "
            "WHERE suser.parent='";
    qStr += DB_MODELFACTORY->getRootId() + "' AND suser.username='";
    qStr += userName + "' AND suser.password='";
    qStr += password + "' ORDER BY persproject_idx;";

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
        obj->setValue("persproject_idx", query.value("persproject_idx")); // PENG_Project - project number
        obj->setValue("persprojectstatus_id", query.value("persprojectstatus_id"));
        obj->setValue("persprojectstart", query.value("persprojectstart"));
        obj->setValue("persprojectend", query.value("persprojectend"));
    }
}

int DB_SqlCommonFunctions::getUserCount() {
    QSqlQuery query(DB_MODELFACTORY->getDatabase());
    RB_String qStr = "SELECT id FROM db_systemuser WHERE id <> '0';";

    if (!query.exec(qStr)) {
        RB_DEBUG->error("DB_SqlCommonFunctionsFunction::getUserCount() "
                        + query.lastError().text() + " ERROR");
        return 0;
    }

    return query.size();
}
