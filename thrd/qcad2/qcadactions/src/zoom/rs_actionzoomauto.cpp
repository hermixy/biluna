/****************************************************************************
** $Id: rs_actionzoomauto.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actionzoomauto.h"
//Added by qt3to4:
#include <QPixmap>


/**
 * Constructor.
 *
 * @param keepAspectRatio true: keep same zoom value for x/y.
 *                        false: adjust both x and y individually
 */
RS_ActionZoomAuto::RS_ActionZoomAuto(RS_EntityContainer& container,
                                     RS_GraphicView& graphicView,
                                     bool keepAspectRatio)
        :RS_ActionInterface( container, graphicView) {

    this->keepAspectRatio = keepAspectRatio;
}


RS_GuiAction* RS_ActionZoomAuto::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionZoomAuto::getName(), 
            "zoomauto",
                             tr("&Auto Zoom"));
        action->setStatusTip(tr("Zooms automatic"));
        action->setCommand("zoomauto,za");
        action->setKeycode("za");
        action->setFactory(RS_ActionZoomAuto::factory);
    }
    return action;
}


void RS_ActionZoomAuto::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}



RS_ActionInterface* RS_ActionZoomAuto::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionZoomAuto(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionZoomAuto::trigger() {
    graphicView->zoomAuto(false, keepAspectRatio);
    finish();
}

