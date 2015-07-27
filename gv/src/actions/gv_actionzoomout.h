/*****************************************************************
 * $Id: gv_actionzoomout.h 1263 2010-07-30 14:07:45Z rutger $
 * Created: Oct 5, 2008 12:58:23 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomout.h 7068
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMOUT_H
#define GV_ACTIONZOOMOUT_H

#include "gv_actionzoomin.h"


/**
 * This action triggers zoom decrease.
 */
class GV_ActionZoomOut : public GV_ActionZoomIn {

    Q_OBJECT

public:
    GV_ActionZoomOut(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionZoomOut() {}

    static RB_String getName() {
        return "Zoom Out";
    }

    virtual RB_String name() {
        return GV_ActionZoomIn::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

    virtual void init(int status=0);
    virtual void trigger();
};

#endif
