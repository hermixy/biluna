/*****************************************************************
 * $Id: gv_actionzoompan.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:03:00 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoompan.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMPAN_H
#define GV_ACTIONZOOMPAN_H

#include "gv_actionbase.h"


/**
 * This action class can handle user events to zoom in a window.
 */
class GV_ActionZoomPan : public GV_ActionBase {
	
    Q_OBJECT
    
public:
    GV_ActionZoomPan(GV_Drawing& dwg, GV_GraphicsView& gv);
    ~GV_ActionZoomPan() {}

    static RB_String getName() {
        return "Zoom Pan";
    }

    virtual RB_String name() {
        return GV_ActionZoomPan::getName();
    }


    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
/*
    virtual void init(int status=0);
    virtual void trigger();
    virtual void mouseMoveEvent(RS_MouseEvent* e);
    virtual void mousePressEvent(RS_MouseEvent* e);
    virtual void mouseReleaseEvent(RS_MouseEvent* e);
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
*/
protected:
    //RS_Vector v1;
    //RS_Vector v2;
    int x1;
    int y1;
    int x2;
    int y2;
};

#endif
