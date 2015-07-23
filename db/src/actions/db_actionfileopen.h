/*****************************************************************
 * $Id: db_actionfileopen.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILEOPEN_H
#define DB_ACTIONFILEOPEN_H

#include "rb_action.h"


/**
 * This action class can handle user events to open files.
 */
class DB_EXPORT DB_ActionFileOpen : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileOpen();
    virtual ~DB_ActionFileOpen() {}

    static RB_String getName() {
        return "File Open";
    }

    virtual RB_String name() {
        return DB_ActionFileOpen::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

    void getOpenFileName(RB_String& fn,
                         const RB_String& filters = RB_String());
    void setOpenFilename(const RB_String& fn);

private:
    bool activateExistingWindow(const RB_String& fn);

    RB_String mFileName;

};

#endif
