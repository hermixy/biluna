/*****************************************************************
 * $Id: gv_actionzoomselection.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:16:28 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomselection.h 8382
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMSELECTION_H
#define GV_ACTIONZOOMSELECTION_H

#include "gv_actionbase.h"


/**
 * This action zooms to the current selection.
 */
class GV_ActionZoomSelection : public GV_ActionBase {
    
	Q_OBJECT
    
public:
    GV_ActionZoomSelection(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionZoomSelection() {}

    static RB_String getName() {
        return "Zoom Selection";
    }

    virtual RB_String name() {
        return GV_ActionZoomSelection::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

//    virtual void init(int status=0);
//    virtual void trigger();
};

#endif
