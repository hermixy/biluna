/*****************************************************************
 * $Id: db_actiontoolstextfilelist.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jul 13, 2011 3:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSTEXTFILELIST_H
#define DB_ACTIONTOOLSTEXTFILELIST_H

#include "rb_action.h"


/**
 * This action class creates text of a file list
 */
class DB_EXPORT DB_ActionToolsTextFileList : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsTextFileList();
    virtual ~DB_ActionToolsTextFileList() {}

    static RB_String getName() {
        return "Create text file list";
    }

    virtual RB_String name() {
        return DB_ActionToolsTextFileList::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
