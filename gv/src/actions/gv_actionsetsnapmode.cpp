/*****************************************************************
 * $Id: gv_actionsetsnapmode.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Oct 23, 2008 2:09:24 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnapmode.cpp 10604
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsetsnapmode.h"

//RS_Hash<RS2::ActionType, RS_GuiAction*> RS_ActionSetSnapMode::actions;
QActionGroup* GV_ActionSetSnapMode::actionGroup = new QActionGroup(NULL);


/**
 * Constructor.
 *
 * @param snapMode The new snap mode used from now on.
 */
GV_ActionSetSnapMode::GV_ActionSetSnapMode(GV_Drawing& dwg, GV_GraphicsView& gv,
        GV2::ActionType snapMode)
        :GV_ActionBase(dwg, gv) {

    this->snapMode = snapMode;
    actionGroup->setExclusive(true);
}

/*
void GV_ActionSetSnapMode::updateGuiActions(RS2::ActionType snapMode) {
    RB_DEBUG->print("GV_ActionSetSnapMode::updateGuiActions");
    
    GV_HashIterator<RS2::ActionType, RB_GuiAction*> i(actions);
    while (i.hasNext()) {
        i.next();
        if (i.value()!=NULL) {
            i.value()->setChecked(false);
        }
    }
        
    if (actions.contains(snapMode)) {
        actions[snapMode]->setChecked(true);
    }
}


RS2::ActionType GV_ActionSetSnapMode::getSnapMode() {
    GV_HashIterator<RS2::ActionType, RB_GuiAction*> i(actions);
    while (i.hasNext()) {
        i.next();
        if (i.value()!=NULL) {
            if (i.value()->isChecked()) {
                return i.key();
            }
        }
    }

    return RS2::ActionNone;
}
*/

/*
RB_Action* GV_ActionSetSnapMode::factory() {
    GV_EntityContainer* container = GV_DIALOGFACTORY->getDocument();
    GV_GraphicView* graphicView = GV_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        GV_ActionBase* a = new GV_ActionSetSnapMode(*container, *graphicView,
            type);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}
*/


void GV_ActionSetSnapMode::trigger() {
    RB_DEBUG->print("GV_ActionSetSnapMode::trigger");

    getDrawing()->setDefaultSnapMode(snapMode);
    //updateGuiActions(snapMode);

    finish();
}


void GV_ActionSetSnapMode::init(int status) {
    RB_DEBUG->print("GV_ActionSetSnapMode::init");
    GV_ActionBase::init(status);
    trigger();
}

