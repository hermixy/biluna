/*****************************************************************
 * $Id: gv_actionzoomscroll.h 831 2008-10-07 19:54:45Z rutger $
 * Created: Oct 5, 2008 1:12:50 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based rs_actionzoomscroll.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMSCROLL_H
#define GV_ACTIONZOOMSCROLL_H

#include "gv_actioninterface.h"


/**
 * This action triggers a scrolling.
 */
class GV_ActionZoomScroll : public GV_ActionInterface {
    
	Q_OBJECT
    
public:
    GV_ActionZoomScroll(/* RS2::Direction direction, */
    		GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionZoomScroll() {}

    static RB_String getName() {
        return "Zoom Scroll";
    }

    virtual RB_String name() {
        return GV_ActionZoomScroll::getName();
    }

/*
    virtual void init(int status=0);
    virtual void trigger();
*/
protected:
//    RS2::Direction direction;
};

#endif
