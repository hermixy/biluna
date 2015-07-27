/*****************************************************************
 * $Id: gv_actionviewgrid.cpp 1148 2010-05-08 11:53:42Z rutger $
 * Created: Oct 5, 2008 12:30:02 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionviewgrid.h 3703
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionviewgrid.h"

//#include <QPixmap>

RB_GuiAction* GV_ActionViewGrid::mGuiAction = NULL;

/**
 * Constructor.
 */
GV_ActionViewGrid::GV_ActionViewGrid(GV_Drawing& dwg, GV_GraphicsView& gv)
                :GV_ActionBase(dwg, gv) {
	
}


RB_GuiAction* GV_ActionViewGrid::createGuiAction() {
    if (mGuiAction == NULL) {
        mGuiAction = new RB_GuiAction(GV_ActionViewGrid::getName(),
                "viewgrid",
                tr("&Grid"));
        mGuiAction->setStatusTip(tr("Shows/hides the grid"));
        mGuiAction->setCheckable(true);
        mGuiAction->setChecked(true);
        mGuiAction->setCommand("grid,gr");
        mGuiAction->setShortcut("Ctrl+G");
        mGuiAction->setKeycode("gr");
        mGuiAction->setFactory(GV_ActionViewGrid::factory);
    }
    return mGuiAction;
}


void GV_ActionViewGrid::init(int status) {
    RB_DEBUG->print("GV_ActionViewGrid::init");

    GV_ActionBase::init(status);
    trigger();
}


RB_Action* GV_ActionViewGrid::factory() {
    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();
    
    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionViewGrid(*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void GV_ActionViewGrid::trigger() {
    RB_DEBUG->print("GV_ActionViewGrid::trigger");
    
    // switch grid on or off
    GV_Drawing* drawing = getDrawing();
	drawing->setGridOn(!drawing->isGridOn());

    if (mGuiAction != NULL) {
        mGuiAction->setChecked(drawing->isGridOn());
	}
    
    // repaint viewport including background
    GV_GraphicsView* gv = getGraphicsView();
    gv->viewport()->repaint(gv->viewport()->rect());
    
    finish();
}

