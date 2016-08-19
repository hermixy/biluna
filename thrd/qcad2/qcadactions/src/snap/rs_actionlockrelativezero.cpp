/****************************************************************************
** $Id: rs_actionlockrelativezero.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actionlockrelativezero.h"


RS_GuiAction* RS_ActionLockRelativeZero::action = NULL;
bool RS_ActionLockRelativeZero::on = false;


RS_ActionLockRelativeZero::RS_ActionLockRelativeZero(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView,
    bool on, bool toggle)
        :RS_ActionInterface(
                    container, graphicView) {

    if (toggle) {
        this->on = !this->on;
    }
    else {
        this->on = on;
    }
}


RS_GuiAction* RS_ActionLockRelativeZero::createGuiAction() {
    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionLockRelativeZero::getName(),
            "relzerolock", 
            tr("(Un-)&Lock Relative Zero"));
        action->setStatusTip(tr("(Un-)Lock relative Zero"));
        action->setCheckable(true);
        action->setChecked(false);
        action->setCommand("lockrelativezero,unlockrelativezero,lrel,rl");
        action->setKeycode("rl");
        action->setFactory(RS_ActionLockRelativeZero::factory);
    }                                  
    return action;
}


void RS_ActionLockRelativeZero::updateGuiAction() {
    if (action!=NULL) {
        action->setChecked(on);
    }
}


bool RS_ActionLockRelativeZero::isLocked() {
    if (action!=NULL) {
        return action->isChecked();
    }
    else {
        return false;
    }
}

void RS_ActionLockRelativeZero::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
    finish();
}


RS_ActionInterface* RS_ActionLockRelativeZero::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionLockRelativeZero(*container, *graphicView, 
            false, true);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

void RS_ActionLockRelativeZero::trigger() {
    graphicView->lockRelativeZero(on);

    if (action!=NULL) {
        action->setChecked(on);
    }
}
