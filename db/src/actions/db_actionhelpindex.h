/*****************************************************************
 * $Id: db_actionhelpindex.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 3, 2009 3:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONHELPINDEX_H
#define DB_ACTIONHELPINDEX_H

#include "rb_action.h"

/**
 * This action class opens the help index
 */
class DB_EXPORT DB_ActionHelpIndex : public RB_Action {

    Q_OBJECT

public:
    DB_ActionHelpIndex();
    virtual ~DB_ActionHelpIndex() { }

    static RB_String getName() {
        return "Help Index";
    }

    virtual RB_String name() {
        return DB_ActionHelpIndex::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif // DB_ACTIONHELPINDEX_H
