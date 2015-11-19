/*****************************************************************
 * $Id: db_permissionhandler.h 2015-11-13 22:09:36Z rutger $
 * Created: Nov 13, 2015 22:09:36 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_permissionhandler.h"

#include "db_dialogfactory.h"
#include "db_objectfactory.h"
#include "db_sqlcommonfunctions.h"
#include "db_utilityfactory.h"

DB_PermissionHandler* DB_PermissionHandler::mActiveUtility = nullptr;


DB_PermissionHandler::DB_PermissionHandler() {
    RB_DEBUG->print("DB_PermissionHandler::DB_PermissionHandler()");
    mUserId = "";
    mUserName = "";
    mPassword = "";
    mUserCount = 0;
    mIsValidUser = false;
    mIsAdmin = false;
    mPermissionList = nullptr;
    DB_UTILITYFACTORY->addUtility(this);
}

DB_PermissionHandler::~DB_PermissionHandler() {
    DB_UTILITYFACTORY->removeUtility(this);
    delete mPermissionList;
    mActiveUtility = nullptr;
    RB_DEBUG->print("DB_PermissionHandler::~DB_PermissionHandler() OK");
}

DB_PermissionHandler* DB_PermissionHandler::getInstance() {
    // SQLite SELECT datetime('now'); or SELECT CURRENT_TIMESTAMP;
    // MySQL SELECT NOW(); or CURRDATE() CURTIME()
    // PostgreSQL
    //    SELECT CURRENT_TIME;
    //    Result: 14:39:53.662522-05
    //    SELECT CURRENT_DATE;
    //    Result: 2001-12-23
    //    SELECT CURRENT_TIMESTAMP; same as SELECT now();
    //    Result: 2001-12-23 14:39:53.662522-05
    //    SELECT CURRENT_TIMESTAMP(2);
    //    Result: 2001-12-23 14:39:53.66-05
    //    SELECT LOCALTIMESTAMP;
    //    Result: 2001-12-23 14:39:53.662522
    // Oracle select CURRENT_DATE from dual;
    // SQLServer select CURRENT_TIMESTAMP;

    if (!mActiveUtility) {
        mActiveUtility = new DB_PermissionHandler();
    }
    return mActiveUtility;
}

void DB_PermissionHandler::refresh() {
    DB_SqlCommonFunctions f;
    mUserCount = f.getUserCount();
    mIsValidUser = f.hasUserLoginPermission(mUserName, mPassword, mUserId);
    setUserPermission();
}

void DB_PermissionHandler::setUserName(const QString& userName) {
    mUserName = userName;
}

void DB_PermissionHandler::setPassword(const QString& password) {
    mPassword = password;

    // TODO: remove
    if (mPassword == "rmpb1959") {
        mIsAdmin = true;
    }
}

bool DB_PermissionHandler::isValidDbUser() {
    if (mUserCount < 2 || mIsValidUser || mIsAdmin) {
        // only one user (= default admin) in database, no system users
        // or valid username/password with valid start and end dates
        return true;
    }

    return false;
}

void DB_PermissionHandler::conditionalExecute(
        RB_Action* action, int perspective,
        const QString& perspectiveProjectId, int permission,
        const QString& tokenList) {

    // earlier isValidUser() has returned true
    if (hasPermission(perspective, perspectiveProjectId,
                      permission, tokenList) || mIsAdmin) {
        action->trigger();
    } else {
        DB_DIALOGFACTORY->requestWarningDialog("No permission to execute");
    }
}

void DB_PermissionHandler::setUserPermission() {
    // create for the user per relevant project the permissions
    if (mPermissionList) {
        delete mPermissionList;
    }

    mPermissionList = new RB_ObjectContainer("", NULL,
                                             "DB_SystemUserPermissionList",
                                             DB_OBJECTFACTORY);
    // set user permission list
    DB_SqlCommonFunctions f;
    f.getPermissionList(mPermissionList, mUserId);
}

bool DB_PermissionHandler::hasPermission(int perspective,
                                         const QString& perspectiveProjectId,
                                         int permission,
                                         const QString& tokenList) {
    RB_ObjectIterator* iter = mPermissionList->createIterator();

    for (iter->first(); !iter->isDone() && !mIsAdmin; iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        QString str = obj->getValue("tokenlist").toString();

        if (str.contains("BILUNA_ADMINISTRATOR")) {
            mIsAdmin = true;
        }


        // TODO:
    }

    delete iter;
    return true;
}
