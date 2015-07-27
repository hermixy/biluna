/*****************************************************************
 * $Id: gv_actionzoomauto.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 12:43:03 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on gv_actionzoomauto.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMAUTO_H
#define GV_ACTIONZOOMAUTO_H

#include "gv_actionbase.h"


/**
 * This action triggers an auto zoom.
 */
class GV_ActionZoomAuto : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionZoomAuto(GV_Drawing& dwg, GV_GraphicsView& gv,
                      bool keepAspectRatio=true);
    ~GV_ActionZoomAuto() {}

    static RB_String getName() {
        return "Zoom Auto";
    }

    virtual RB_String name() {
        return GV_ActionZoomAuto::getName();
    }


    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

//    virtual void init(int status=0);
//    virtual void trigger();

protected:
    bool keepAspectRatio;
};

#endif
