/*****************************************************************
 * $Id: db_actionstylepreformatted.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSTYLEPREFORMATTED_H
#define DB_ACTIONSTYLEPREFORMATTED_H

#include "rb_action.h"

/**
 * This action class can handle user events for text to appear
 * exactly as it is typed. Spaces, tabs, and line breaks that exist
 * in the actual code will be preserved with the pre tag
 */
class DB_EXPORT DB_ActionStylePreformatted : public RB_Action {

    Q_OBJECT

public:
    DB_ActionStylePreformatted();
    virtual ~DB_ActionStylePreformatted() {}

    static RB_String getName() {
        return "Style Preformatted";
    }

    virtual RB_String name() {
        return DB_ActionStylePreformatted::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
