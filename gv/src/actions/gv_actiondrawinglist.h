/*****************************************************************
 * $Id: gv_actiondrawinglist.h 1148 2010-05-08 11:53:42Z rutger $
 * Created: Apr 21, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONDRAWINGLIST_H
#define GV_ACTIONDRAWINGLIST_H

#include "rb_action.h"


/**
 * Action opens drawinglist dockwindow
 */
class GV_ActionDrawingList : public RB_Action {

    Q_OBJECT

public:
    GV_ActionDrawingList();
    virtual ~GV_ActionDrawingList() {}

    static RB_String getName() {
        return "List of drawings";
    }

    virtual RB_String name() {
        return GV_ActionDrawingList::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // GV_ACTIONDRAWING_H
