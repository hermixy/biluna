/****************************************************************************
** $Id: rs_actionsetsnaprestriction.cpp 10309 2008-04-05 13:31:40Z andrew $
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

#include "rs_actionsetsnaprestriction.h"



//RS_Hash<RS2::ActionType, RS_GuiAction*> RS_ActionSetSnapRestriction::actions;
QActionGroup* RS_ActionSetSnapRestriction::actionGroup = new QActionGroup(NULL);



/**
 * Constructor.
 *
 * @param snapRes The new restriction mode used from now on.
 */
RS_ActionSetSnapRestriction::RS_ActionSetSnapRestriction(
        RS_EntityContainer& container,
        RS_GraphicView& graphicView,
        RS2::ActionType snapRes)
        :RS_ActionInterface( container, graphicView) {

    this->snapRes = snapRes;
    actionGroup->setExclusive(true);
}


/*
void RS_ActionSetSnapRestriction::updateGuiActions(RS2::ActionType snapRes) {
    RS_HashIterator<RS2::ActionType, RS_GuiAction*> i(actions);
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


RS2::ActionType RS_ActionSetSnapRestriction::getSnapRestriction() {
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


void RS_ActionSetSnapRestriction::trigger() {
    graphicView->setSnapRestriction(snapRes);
    //updateGuiActions(snapRes);
    
    finish();
}



void RS_ActionSetSnapRestriction::init(int status) {
    RS_ActionInterface::init(status);
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


