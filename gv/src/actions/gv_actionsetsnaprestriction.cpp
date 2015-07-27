/*****************************************************************
 * $Id: gv_actionsetsnaprestriction.cpp 1135 2010-04-19 21:19:17Z rutger $
 * Created: Nov 7, 2008 4:36:53 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionsetsnaprestriction.h 10309
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionsetsnaprestriction.h"



//RS_Hash<RS2::ActionType, RS_GuiAction*> RS_ActionSetSnapRestriction::actions;
QActionGroup* GV_ActionSetSnapRestriction::actionGroup = new QActionGroup(NULL);



/**
 * Constructor.
 * @param snapRes The new restriction mode used from now on.
 */
GV_ActionSetSnapRestriction::GV_ActionSetSnapRestriction(
		GV_Drawing& dwg, GV_GraphicsView& gv,
        GV2::ActionType snapRes)
        :GV_ActionBase(dwg, gv) {

    this->snapRes = snapRes;
    actionGroup->setExclusive(true);
}


/*
void GV_ActionSetSnapRestriction::updateGuiActions(RS2::ActionType snapRes) {
    GV_HashIterator<RS2::ActionType, RB_GuiAction*> i(actions);
    while (i.hasNext()) {
        i.next();
        if (i.value()!=NULL) {
            i.value()->setChecked(false);
        }
    }
        
    if (actions.contains(snapRes)) {
        actions[snapRes]->setChecked(true);
    }
}


RS2::ActionType GV_ActionSetSnapRestriction::getSnapRestriction() {
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


void GV_ActionSetSnapRestriction::trigger() {
    getDrawing()->setSnapRestriction(snapRes);
    //updateGuiActions(snapRes);
    
    finish();
}



void GV_ActionSetSnapRestriction::init(int status) {
    GV_ActionBase::init(status);
    trigger();
}

/*
RS_ActionInterface* RS_ActionSetSnapRestriction::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionSetSnapRestriction(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}
*/


