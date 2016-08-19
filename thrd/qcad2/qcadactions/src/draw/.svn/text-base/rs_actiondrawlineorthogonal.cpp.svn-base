/****************************************************************************
** $Id: rs_actiondrawlinerelangle.cpp 7451 2007-12-02 22:28:36Z andrew $
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

#include "rs_actiondrawlineorthogonal.h"



RS_ActionDrawLineOrthogonal::RS_ActionDrawLineOrthogonal(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView) 
        :RS_ActionDrawLineRelAngle(container, graphicView, M_PI/2.0, true) {

}


RS_GuiAction* RS_ActionDrawLineOrthogonal::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(
            RS_ActionDrawLineOrthogonal::getName(), 
            "linesorthogonal",
            tr("&Orthogonal"));
        action->setStatusTip(tr("Draw orthogonal line"));
        action->setCommand("lineorthogonal,lo");
        action->setKeycode("lo");
        action->setFactory(RS_ActionDrawLineOrthogonal::factory);
    }

    return action;
}

RS_ActionInterface* RS_ActionDrawLineOrthogonal::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawLineOrthogonal(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

