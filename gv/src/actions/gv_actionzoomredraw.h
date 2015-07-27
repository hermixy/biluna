/*****************************************************************
 * $Id: gv_actionzoomredraw.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:10:15 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomredraw.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMREDRAW_H
#define GV_ACTIONZOOMREDRAW_H

#include "gv_actionbase.h"


/**
 * This action triggers an auto zoom.
 */
class GV_ActionZoomRedraw : public GV_ActionBase {
    
	Q_OBJECT
    
public:
    GV_ActionZoomRedraw(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionZoomRedraw() {}

    static RB_String getName() {
        return "Zoom Redraw";
    }

    virtual RB_String name() {
        return GV_ActionZoomRedraw::getName();
    }


    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

//    virtual void init(int status=0);
//    virtual void trigger();
};

#endif
