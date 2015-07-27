/*****************************************************************
 * $Id: gv_actionzoomprevious.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:07:13 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actionzoomprevious.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMPREVIOUS_H
#define GV_ACTIONZOOMPREVIOUS_H

#include "gv_actionbase.h"


/**
 * This action switches back to the previously used view..
 */
class GV_ActionZoomPrevious : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionZoomPrevious(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionZoomPrevious() {}

    static RB_String getName() {
        return "Zoom Previous";
    }

    virtual RB_String name() {
        return GV_ActionZoomPrevious::getName();
    }


    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

//    virtual void init(int status=0);
//    virtual void trigger();

protected:
	
};

#endif
