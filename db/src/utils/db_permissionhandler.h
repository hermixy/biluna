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
    void setUserPermission();
    bool isValidDbUser();

    int getPerspectivePermission(const RB_String& perspectiveCode);
    int getCrudxPermission(const RB_String& modelName,
                                  const RB_String& projectId);

private:
    DB_PermissionHandler();
    void setUserCount();
    void handlePermissionSetting(RB_ObjectBase* projectObj,
                                        RB_ObjectBase* permObj);

    static DB_PermissionHandler* mActiveUtility;
    RB_ObjectContainer* mPermissionList;

    QString mUserName;
    QString mPassword;
    int mUserCount;

};

#endif // DB_PERMISSIONHANDLER_H
