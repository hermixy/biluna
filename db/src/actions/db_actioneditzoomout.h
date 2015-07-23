/*****************************************************************
 * $Id: db_actioneditzoomout.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 24, 2010 6:48:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITZOOMOUT_H
#define DB_ACTIONEDITZOOMOUT_H

#include "rb_action.h"

/**
 * This action class can handle user events for zoom out.
 */
class DB_EXPORT DB_ActionEditZoomOut : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditZoomOut();
    virtual ~DB_ActionEditZoomOut() {}

    static RB_String getName() {
        return "Zoom out";
    }

    virtual RB_String name() {
        return DB_ActionEditZoomOut::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
