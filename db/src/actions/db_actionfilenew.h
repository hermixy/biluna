/*****************************************************************
 * $Id: db_actionfilenew.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILENEW_H
#define DB_ACTIONFILENEW_H

#include "rb_action.h"


/**
 * This action class can handle user events to create a new blank text document.
 */
class DB_EXPORT DB_ActionFileNew : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileNew();
    virtual ~DB_ActionFileNew() {}

    static RB_String getName() {
        return "File New rich text document";
    }

    virtual RB_String name() {
        return DB_ActionFileNew::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
