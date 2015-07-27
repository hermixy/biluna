/*****************************************************************
 * $Id: gv_actionfileclose.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 12, 2008 2:07:27 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFILECLOSE_H
#define GV_ACTIONFILECLOSE_H

#include "gv_actionbase.h"

class GV_ActionFileClose : public GV_ActionBase {
	
	Q_OBJECT
	
public:
	GV_ActionFileClose(GV_Drawing& dwg, GV_GraphicsView& gv);
	virtual ~GV_ActionFileClose() { }
	
    static RB_String getName() {
        return "Drawings Close";
    }

    virtual RB_String name() {
        return GV_ActionFileClose::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif /*GV_ACTIONFILECLOSE_H*/
