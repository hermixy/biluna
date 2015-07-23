/*****************************************************************
 * $Id: db_actionfilesaveas.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILESAVEAS_H
#define DB_ACTIONFILESAVEAS_H

#include "rb_action.h"

/**
 * Action to save the document to a file with another name
 */
class DB_EXPORT DB_ActionFileSaveAs : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileSaveAs();
    ~DB_ActionFileSaveAs() {}

    static RB_String getName() {
        return "File save as";
    }

    virtual RB_String name() {
        return DB_ActionFileSaveAs::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

    void getSaveFileName(RB_String& fn, const RB_String& filters = RB_String());
};

#endif // DB_ACTIONFILESAVEAS_H
