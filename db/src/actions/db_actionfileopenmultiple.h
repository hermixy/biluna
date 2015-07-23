/*****************************************************************
 * $Id: db_actionfileopenmultiple.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 21, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILEOPENMULTIPLE_H
#define DB_ACTIONFILEOPENMULTIPLE_H

#include "rb_action.h"


/**
 * This action class can handle user events to open multiple files.
 */
class DB_EXPORT DB_ActionFileOpenMultiple : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileOpenMultiple();
    virtual ~DB_ActionFileOpenMultiple() {}

    static RB_String getName() {
        return "File Open";
    }

    virtual RB_String name() {
        return DB_ActionFileOpenMultiple::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

    void getOpenFileNames(RB_StringList& fns,
                          const RB_String& filters = RB_String());

private:
    bool activateExistingWindow(const RB_String& fn);

};

#endif
