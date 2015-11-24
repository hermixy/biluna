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
    mPermissionList = nullptr;
    mUserId = "";
    mUserName = "";
    mPassword = "";
    mUserCount = 0;
    mIsValidUser = false;
    mIsAdmin = false;
    mToday = QDate(1970,1,1);

    DB_UTILITYFACTORY->addUtility(this);
}

DB_PermissionHandler::~DB_PermissionHandler() {
    DB_UTILITYFACTORY->removeUtility(this);
    delete mPermissionList;
    mActiveUtility = nullptr;
    RB_DEBUG->print("DB_PermissionHandler::~DB_PermissionHandler() OK");
}

DB_PermissionHandler* DB_PermissionHandler::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new DB_PermissionHandler();
    }
    return mActiveUtility;
}

void DB_PermissionHandler::refresh() {
    mIsAdmin = false;
    DB_SqlCommonFunctions f;
    mUserCount = f.getUserCount();
    mIsValidUser = f.hasUserLoginPermission(mUserName, mPassword, mUserId);
    setUserPermission();

    // TODO:
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

    mToday = QDate::currentDate();
}

void DB_PermissionHandler::setUserName(const QString& userName) {
    mUserName = userName;
}

void DB_PermissionHandler::setPassword(const QString& password) {
    mPassword = password;
}

bool DB_PermissionHandler::isValidDbUser() {
    if (mUserCount < 2) {
        // only one user (= default admin) in database, no system users
        // or valid username/password with valid start and end dates
        mIsAdmin = true;
        return true;
    } else if (mIsValidUser || mIsAdmin) {
        return true;
    }

    return false;
}

bool DB_PermissionHandler::isAdmin() {
    return mIsAdmin;
}

int DB_PermissionHandler::getUserCount() {
    return mUserCount;
}

void DB_PermissionHandler::getProjectIdList(const QString& perspective,
                                            QStringList& projectIdList) {
    projectIdList.clear();
    QString userPersp;
    int status;
    QDate start;
    QDate end;

    RB_ObjectIterator* iter = mPermissionList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        userPersp = obj->getValue("perspective").toString();
        status = obj->getValue("persprojectstatus_id").toInt() - 2; // refer RB2
        start = obj->getValue("persprojectstart").toDate();
        end = obj->getValue("persprojectend").toDate();

        if (userPersp.compare(perspective, Qt::CaseInsensitive) == 0
                && status >= RB2::ProjectLive
                && start <= mToday && mToday <= end) {
            QString idStr = obj->getIdValue("persproject_idx").toString();
            projectIdList.append(idStr);
        }
    }

    delete iter;
}

QDate DB_PermissionHandler::getToday() const {
    return mToday;
}

bool DB_PermissionHandler::loadPermissionPlugin(const QString &pluginToken) {
    return mIsAdmin || hasPermission("", RB2::PermissionDefault, pluginToken);
}

void DB_PermissionHandler::conditionalPlugin(RB_Action* action,
                                             const QString& pluginToken) {
    // earlier isValidUser() has returned true
    if (mIsAdmin || hasPermission("", RB2::PermissionDefault, pluginToken)) {
        if (action) {
            action->trigger();
        }
    } else {
        DB_DIALOGFACTORY->requestWarningDialog("No permission to execute");
    }
}

void DB_PermissionHandler::conditionalExecute(
        RB_Action* action, const QString& perspectiveProjectId,
        int permission, const QString& tokenList) {

    // earlier isValidUser() has returned true
    if (mIsAdmin || hasPermission(perspectiveProjectId,
                                  permission, tokenList)) {
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

    // set administrator if applicable
    RB_ObjectIterator* iter = mPermissionList->createIterator();

    for (iter->first(); !iter->isDone() && !mIsAdmin; iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        QString str = obj->getValue("tokenlist").toString();

        if (str.contains("BILUNA_ADMINISTRATOR")) {
            mIsAdmin = true;
        }
    }

    delete iter;
}

bool DB_PermissionHandler::hasPermission(const QString& perspectiveProjectId,
                                         int permission,
                                         const QString& tokenList) {
    if (mIsAdmin) {
        return mIsAdmin;
    }

    bool validPermission = false;
    RB_ObjectIterator* iter = mPermissionList->createIterator();

    for (iter->first(); !iter->isDone() && !mIsAdmin && !validPermission;
         iter->next()) {
        bool tokenListValid = false;

        RB_ObjectBase* obj = iter->currentObject();
        QString str = obj->getValue("tokenlist").toString();

        if (str.contains("BILUNA_ADMINISTRATOR")) {
            mIsAdmin = true;
            return mIsAdmin;
        }

        if (!tokenList.isEmpty()) {
            tokenListValid = validTokenList(str, tokenList);
        } else {
            tokenListValid = true;
        }

        bool crudxValid = false;

        if (permission == (int)RB2::PermissionDefault
                || permission <= obj->getValue("crudx_id").toInt()) {
            crudxValid = true;
        }

        bool projectValid = false;
        str = obj->getValue("persproject_idx").toString();

        if (str.isEmpty() || str.contains(perspectiveProjectId)) {
            // Perspective project id is empty when starting perspective plugin
            projectValid = true;
        }

        bool datesValid = false;
        QDate today = QDate::currentDate();

        if (obj->getValue("persprojectstart").toDate() <= today
            && today <= obj->getValue("persprojectend").toDate()) {
            datesValid = true;
        }


        validPermission = tokenListValid && crudxValid
                && projectValid && datesValid;
    }

    delete iter;
    return validPermission;
}

bool DB_PermissionHandler::validTokenList(const QString& userTokens,
                                          const QString& tokenList) {
    QStringList userList = userTokens.split(";", QString::SkipEmptyParts);
    QStringList reqList = tokenList.split(";", QString::SkipEmptyParts);

    int reqCount = reqList.size();
    int validTokenCount = 0;

    for (int i = 0; i < reqCount; ++i) {
        if (userList.contains(reqList.at(i))) {
            ++validTokenCount;
        }
    }

    return validTokenCount == reqCount;
}
