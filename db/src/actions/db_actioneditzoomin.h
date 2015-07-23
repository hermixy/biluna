/*****************************************************************
 * $Id: db_actioneditzoomin.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Mar 24, 2010 6:48:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITZOOMIN_H
#define DB_ACTIONEDITZOOMIN_H

#include "rb_action.h"

/**
 * This action class can handle user events for zoom in.
 */
class DB_EXPORT DB_ActionEditZoomIn : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditZoomIn();
    virtual ~DB_ActionEditZoomIn() {}

    static RB_String getName() {
        return "Zoom In";
    }

    virtual RB_String name() {
        return DB_ActionEditZoomIn::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
