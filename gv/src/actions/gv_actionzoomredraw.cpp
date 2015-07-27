/*****************************************************************
 * $Id: gv_actionzoomredraw.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 5, 2008 1:08:27 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionzoomredraw.cpp 9172
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionzoomredraw.h"
//Added by qt3to4:
//#include <QPixmap>


/**
 * Constructor.
 *
 * @param keepAspectRatio true: keep same zoom value for x/y.
 *                        false: adjust both x and y individually
 */
GV_ActionZoomRedraw::GV_ActionZoomRedraw(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {
}


RB_GuiAction* GV_ActionZoomRedraw::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionZoomRedraw::getName(),
        			"zoomredraw",
                    tr("&Redraw"));
        action->setStatusTip(tr("Redraws the screen"));
        action->setCommand("regen,rg,zr,rd");
        action->setKeycode("rg,zr,rd");
        action->setFactory(GV_ActionZoomRedraw::factory);
        
    }                                  
    return action;
}

/*
void GV_ActionZoomRedraw::init(int status) {
    GV_ActionBase::init(status);
    trigger();
}
*/


RB_Action* GV_ActionZoomRedraw::factory() {
//    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
//    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
//    
//    if (container!=NULL && graphicView!=NULL) {
//        RS_ActionInterface* a = new RS_ActionZoomRedraw(*container, *graphicView);
//        graphicView->setCurrentAction(a);
//        return a;
//    }

    return NULL;
}


/*
void RS_ActionZoomRedraw::trigger() {
    graphicView->redraw();
    finish();
}
*/
// EOF
