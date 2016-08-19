/****************************************************************************
** $Id: rs_actiondrawlineparallel.cpp 8821 2008-02-02 17:31:21Z andrew $
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

#include "rs_actiondrawlineparallel.h"

#include "rs_creation.h"
#include "rs_snapper.h"
#include "rs_actiondrawlineparallelthrough.h"



RS_ActionDrawLineParallel::RS_ActionDrawLineParallel(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    parallel = NULL;
    entity = NULL;
    distance = 1.0;
    number = 1;
    coord = RS_Vector(false);
}

RS_GuiAction* RS_ActionDrawLineParallel::createGuiAction() {

    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionDrawLineParallel::getName(), 
            "linespara",
             tr("Para&llel"));
        action->setStatusTip(tr("Draw parallels to existing lines, "
                                 "arcs, circles"));
        action->setCommand("lineoffset,lineparallel,offset,o,parallel,par");
        action->setKeycode("lp,pa,of");
        action->setFactory(RS_ActionDrawLineParallel::factory);
    }

    return action;
}


RS_ActionInterface* RS_ActionDrawLineParallel::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionDrawLineParallel(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionDrawLineParallel::trigger() {
    RS_ActionBase::trigger();

    RS_Creation creation(container, graphicView);
    RS_Entity* e = creation.createParallel(coord,
                                           distance, number,
                                           entity);

    if (e==NULL) {
        RS_DEBUG->print("RS_ActionDrawLineParallel::trigger:"
                        " No parallels added\n");
    }
}



void RS_ActionDrawLineParallel::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineParallel::mouseMoveEvent begin");

    coord = RS_Vector(graphicView->toGraphX(e->x()),
                      graphicView->toGraphY(e->y()));

    entity = catchEntity(e, RS2::ResolveAll);

    clearPreview();
    switch (getStatus()) {
    case SetEntity: {

            RS_Creation creation(preview, NULL, false);
            creation.createParallel(coord,
                                    distance, number,
                                    entity);

        }
        break;

    default:
        break;
    }
    drawPreview();

    RS_DEBUG->print("RS_ActionDrawLineParallel::mouseMoveEvent end");
}



void RS_ActionDrawLineParallel::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    } else {
        trigger();
    }
}



void RS_ActionDrawLineParallel::updateMouseButtonHints() {
    if (RS_DIALOGFACTORY!=NULL) {
        switch (getStatus()) {
        case SetEntity:
            RS_DIALOGFACTORY->updateMouseWidget(
                tr("Specify Distance <%1> or select entity or [%2]")
                .arg(distance).arg(command("through")),
                tr("Cancel"));
            break;

        case SetNumber:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Enter number:"), "");
            break;

        default:
            RS_DIALOGFACTORY->updateMouseWidget("", "");
            break;
        }
    }
}



/*
void RS_ActionDrawLineParallel::showOptions() {
    RS_ActionInterface::showOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true);
    }
    updateMouseButtonHints();
}
*/



void RS_ActionDrawLineParallel::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }

    switch (getStatus()) {
    case SetEntity: {
            if (checkCommand("through", c)) {
                finish();
                graphicView->setCurrentAction(
                    new RS_ActionDrawLineParallelThrough(*container,
                                                         *graphicView));
            } else if (checkCommand("number", c)) {
                clearPreview();
                graphicView->restore();
                setStatus(SetNumber);
            } else {
                bool ok;
                double d = RS_Math::eval(c, &ok);
                if (ok==true && d>1.0e-10) {
                    distance = d;
                } else {
                    if (RS_DIALOGFACTORY!=NULL) {
                        RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                    }
                }
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->requestOptions(this, true, true);
                }
                updateMouseButtonHints();
                //setStatus(SetEntity);
            }
        }
        break;

    case SetNumber: {
            bool ok;
            int n = c.toInt(&ok);
            if (ok==true) {
                if (n>0 && n<100) {
                    number = n;
                } else {
                    if (RS_DIALOGFACTORY!=NULL) {
                        RS_DIALOGFACTORY->commandMessage(tr("Not a valid number. "
                                                            "Try 1..99"));
                    }
                }
            } else {
                if (RS_DIALOGFACTORY!=NULL) {
                    RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
                }
            }
            if (RS_DIALOGFACTORY!=NULL) {
                RS_DIALOGFACTORY->requestOptions(this, true, true);
            }
            setStatus(SetEntity);
        }
        break;

    default:
        break;
    }
}



RS_StringList RS_ActionDrawLineParallel::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case SetEntity:
        cmd += command("number");
        cmd += command("through");
        break;
    default:
        break;
    }

    return cmd;
}



void RS_ActionDrawLineParallel::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineParallel::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
    }
}

