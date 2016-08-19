/****************************************************************************
** $Id: rs_actionsetsnapmode.cpp 10604 2008-05-01 21:20:50Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "rs_actionsetsnapmode.h"



//RS_Hash<RS2::ActionType, RS_GuiAction*> RS_ActionSetSnapMode::actions;
QActionGroup* RS_ActionSetSnapMode::actionGroup = new QActionGroup(NULL);



/**
 * Constructor.
 *
 * @param snapMode The new snap mode used from now on.
 */
RS_ActionSetSnapMode::RS_ActionSetSnapMode(RS_EntityContainer& container,
        RS_GraphicView& graphicView,
        RS2::ActionType snapMode)
        :RS_ActionInterface( container, graphicView) {

    this->snapMode = snapMode;
    actionGroup->setExclusive(true);
}



/*
void RS_ActionSetSnapMode::updateGuiActions(RS2::ActionType snapMode) {
    RS_DEBUG->print("RS_ActionSetSnapMode::updateGuiActions");
    
    RS_HashIterator<RS2::ActionType, RS_GuiAction*> i(actions);
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


RS2::ActionType RS_ActionSetSnapMode::getSnapMode() {
    RS_HashIterator<RS2::ActionType, RS_GuiAction*> i(actions);
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
RS_ActionInterface* RS_ActionSetSnapMode::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionSetSnapMode(*container, *graphicView,
            type);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}
*/



void RS_ActionSetSnapMode::trigger() {
    RS_DEBUG->print("RS_ActionSetSnapMode::trigger");

    graphicView->setDefaultSnapMode(snapMode);
    //updateGuiActions(snapMode);

    finish();
}



void RS_ActionSetSnapMode::init(int status) {
    RS_DEBUG->print("RS_ActionSetSnapMode::init");
    RS_ActionInterface::init(status);
    trigger();
}

