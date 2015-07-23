/*****************************************************************
 * $Id: db_actiontoolsscreenshot.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Feb 23, 2015 9:38:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSSCREENSHOT_H
#define DB_ACTIONTOOLSSCREENSHOT_H

#include "rb_action.h"

/**
 * Take screenshot action
 */
class DB_EXPORT DB_ActionToolsScreenshot : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsScreenshot();
    virtual ~DB_ActionToolsScreenshot() { }

    static RB_String getName() {
        return "Screenshot dialog";
    }

    virtual RB_String name() {
        return DB_ActionToolsScreenshot::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // DB_ACTIONTOOLSSCREENSHOT_H
