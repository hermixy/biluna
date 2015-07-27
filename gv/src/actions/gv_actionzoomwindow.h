/*****************************************************************
 * $Id: gv_actionzoomwindow.h 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:20:36 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomwindow.h 10343
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONZOOMWINDOW_H
#define GV_ACTIONZOOMWINDOW_H

#include "gv_actionbase.h"

/**
 * This action class can handle user events to zoom in a window.
 */
class GV_ActionZoomWindow : public GV_ActionBase {
    
	Q_OBJECT
    
public:
    GV_ActionZoomWindow(GV_Drawing& dwg, GV_GraphicsView& gv,
                        bool keepAspectRatio = true);

    ~GV_ActionZoomWindow() {}

    static RB_String getName() {
        return "Zoom Window";
    }

    virtual RB_String name() {
        return GV_ActionZoomWindow::getName();
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
//    RS_Vector v1;
//    RS_Vector v2;

    bool keepAspectRatio;
};

#endif
