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
    mUserName = "";
    mPassword = "";
    mUserCount = 0;
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
    // SQLite SELECT datetime('now');
    // MySQL SELECT NOW(); or CURRDATE() CURTIME()
    if (!mActiveUtility) {
        mActiveUtility = new DB_PermissionHandler();
    }
    return mActiveUtility;
}

void DB_PermissionHandler::refresh() {
    setUserPermission();
}

void DB_PermissionHandler::setUserName(const QString& userName) {
    mUserName = userName;
}

void DB_PermissionHandler::setPassword(const QString& password) {
    mPassword = password;
}

void DB_PermissionHandler::setUserPermission() {
    setUserCount();

    // create per actual project with the user permission
    if (mPermissionList) {
        delete mPermissionList;
    }

    mPermissionList = new RB_ObjectContainer("", NULL,
                                             "DB_SystemUserPermissionList",
                                             DB_OBJECTFACTORY);
    // set user list
    DB_SqlCommonFunctions f;
    f.getPermissionList(mPermissionList, mUserName, mPassword);
}

bool DB_PermissionHandler::isValidDbUser() {
    if (mUserCount < 2) {
        // only one user (= default admin) in database, no system users
        return true;
    }

    if (mPermissionList->countObject() < 1) {
        // system users but no with username and password
        return false;
    }

    QDate today = QDate::currentDate();
    RB_ObjectIterator* iter = mPermissionList->createIterator();
    iter->first();
    RB_ObjectBase* user = iter->currentObject();
    QDate startDate = user->getValue("startdate").toDate();
    QDate endDate = user->getValue("enddate").toDate();

    if (startDate <= today && today >= endDate) {
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
                      permission, tokenList)) {
        action->trigger();
    } else {
        DB_DIALOGFACTORY->requestWarningDialog("No permission to execute");
    }
}

void DB_PermissionHandler::setUserCount() {
    DB_SqlCommonFunctions f;
    mUserCount = f.getUserCount();
}

bool DB_PermissionHandler::hasPermission(int perspective,
                                         const QString& perspectiveProjectId,
                                         int permission,
                                         const QString& tokenList) {
    RB_ObjectIterator* iter = mPermissionList->createIterator();

    for (iter->first(); !iter->isDone() && mIsAdmin; iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        QString str = obj->getValue("tokenlist").toString();

        if (str.contains("BILUNA_ADMINISTRATOR")) {
            mIsAdmin = true;
        }
    }

    delete iter;
    return true;
}
