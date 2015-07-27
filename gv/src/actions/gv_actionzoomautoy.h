/*****************************************************************
 * $Id: gv_actionzoomautoy.h 831 2008-10-07 19:54:45Z rutger $
 * Created: Oct 5, 2008 12:48:07 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomautoy.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMAUTOY_H
#define GV_ACTIONZOOMAUTOY_H

#include "gv_actioninterface.h"


/**
 * This action triggers an auto zoom in Y only.
 */
class GV_ActionZoomAutoY : public GV_ActionInterface {
	
    Q_OBJECT
    
public:
    GV_ActionZoomAutoY(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionZoomAutoY() {}

    static RB_String getName() {
        return "Zoom Auto Y";
    }

    virtual RB_String name() {
        return GV_ActionZoomAutoY::getName();
    }


//    virtual void init(int status=0);
//    virtual void trigger();
};

#endif
