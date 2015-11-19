/*****************************************************************
 * $Id: db_permissionhandler.h 2015-11-13 22:09:36Z rutger $
 * Created: Nov 13, 2015 22:09:36 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_PERMISSIONHANDLER_H
#define DB_PERMISSIONHANDLER_H

#include "rb_action.h"
#include "rb_objectcontainer.h"
#include "rb_utility.h"

#define DB_PERMISSIONHANDLER DB_PermissionHandler::getInstance()

class DB_EXPORT DB_PermissionHandler : public RB_Utility {

public:
    virtual ~DB_PermissionHandler();
    static DB_PermissionHandler* getInstance();

    virtual void refresh() override;
    void setUserName(const QString& userName);
    void setPassword(const QString& password);
    bool isValidDbUser();

    void conditionalExecute(RB_Action* action,
                            int perspective,
                            const QString& perspectiveProjectId = "",
                            int permission = RB2::PermissionDefault,
                            const QString& tokenList = "");

private:
    DB_PermissionHandler();
    void setUserPermission();
    bool hasPermission(int perspective,
                       const QString& perspectiveProjectId = "",
                       int permission = RB2::PermissionDefault,
                       const QString& tokenList = "");

    static DB_PermissionHandler* mActiveUtility;
    RB_ObjectContainer* mPermissionList;

    QString mUserId;
    QString mUserName;
    QString mPassword;
    int mUserCount;
    bool mIsValidUser;
    bool mIsAdmin;

};

#endif // DB_PERMISSIONHANDLER_H
