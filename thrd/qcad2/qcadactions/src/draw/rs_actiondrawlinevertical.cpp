/****************************************************************************
** $Id: rs_actiondrawlineangle.cpp 8357 2008-01-21 21:59:39Z andrew $
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

#include "rs_actiondrawlinevertical.h"



RS_ActionDrawLineVertical::RS_ActionDrawLineVertical(
    RS_EntityContainer& container, RS_GraphicView& graphicView) :
        RS_ActionDrawLineAngle(container, graphicView, M_PI/2.0, true) {
}



RS_ActionDrawLineVertical::~RS_ActionDrawLineVertical() {
}



RS_GuiAction* RS_ActionDrawLineVertical::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawLineVertical::getName(),
            "linesver",
            tr("&Vertical"));
        action->setStatusTip(tr("Draw vertical lines"));
        action->setCommand("linever,lv");
        action->setKeycode("lv");
        action->setFactory(RS_ActionDrawLineVertical::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionDrawLineVertical::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = 
            new RS_ActionDrawLineVertical(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}
