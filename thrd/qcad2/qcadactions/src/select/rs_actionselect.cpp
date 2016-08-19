/****************************************************************************
** $Id: rs_actionselect.cpp 10350 2008-04-06 19:23:35Z andrew $
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

#include "rs_actionselect.h"

#include "rs_snapper.h"
#include "rs_actionselectsingle.h"


RS_ActionSelect::RS_ActionSelect(RS_EntityContainer& container,
                                 RS_GraphicView& graphicView,
                                 RS_ActionInterface* (*f)())
        : RS_ActionInterface(container, graphicView) {

    RS_DEBUG->print("RS_ActionSelect::RS_ActionSelect");

    this->nextActionFactory = f;
}



void RS_ActionSelect::init(int status) {
    RS_ActionInterface::init(status);
    if (status>=0) {
        graphicView->setCurrentAction(
            new RS_ActionSelectSingle(*container, *graphicView));
    }
    finish();
}

void RS_ActionSelect::finish() {
    RS_ActionInterface::finish();
}



void RS_ActionSelect::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        init(getStatus()-1);
    }
}



void RS_ActionSelect::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        //if (!isFinished()) {
            // request toolbar with select tools and next button  that
            //  triggers next action
            RS_DIALOGFACTORY->requestToolBarSelect(nextActionFactory);
        //} else {
        //    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarMain);
        //}
    }
}

