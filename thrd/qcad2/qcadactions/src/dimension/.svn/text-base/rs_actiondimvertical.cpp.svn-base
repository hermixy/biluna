/****************************************************************************
** $Id: rs_actiondimlinear.cpp 7839 2008-01-11 19:33:27Z andrew $
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

#include "rs_actiondimvertical.h"


/**
 * Constructor.
 */
RS_ActionDimVertical::RS_ActionDimVertical(RS_EntityContainer& container,
                                       RS_GraphicView& graphicView)
    : RS_ActionDimLinear(container, graphicView, M_PI/2.0, true) {
}



RS_ActionDimVertical::~RS_ActionDimVertical() {}


RS_GuiAction* RS_ActionDimVertical::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDimVertical::getName(), 
            
                "dimver",
            tr("&Vertical"));
        action->setStatusTip(tr("Vertical dimension"));
        action->setCommand("dimver,dv");
        action->setKeycode("dv");
        action->setFactory(RS_ActionDimVertical::factory);
    }

    return action;
}



RS_ActionInterface* RS_ActionDimVertical::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionDimVertical* a = new RS_ActionDimVertical(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}

