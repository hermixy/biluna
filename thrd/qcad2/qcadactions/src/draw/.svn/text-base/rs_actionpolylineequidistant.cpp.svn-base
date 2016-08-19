/****************************************************************************
** $Id: rs_actiondrawlineparallel.cpp 1134 2004-07-13 23:26:13Z andrew $
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

#include "rs_actionpolylineequidistant.h"

#include "rs_creation.h"
#include "rs_snapper.h"



RS_ActionPolylineEquidistant::RS_ActionPolylineEquidistant(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_ActionBase( container, graphicView) {

    entity = NULL;
    distance = 1.0;
    number = 1;
    useArcs = true;
    coord = RS_Vector(false);
}

RS_GuiAction* RS_ActionPolylineEquidistant::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionPolylineEquidistant::getName(), 
            "polylineequidistant",
            tr("E&quidistant"));
        action->setStatusTip(tr("Draw equidistant polylines"));
        action->setCommand("oq");
        action->setKeycode("oq");
        action->setFactory(RS_ActionPolylineEquidistant::factory);
    }
    return action;
}


RS_ActionInterface* RS_ActionPolylineEquidistant::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionPolylineEquidistant(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void RS_ActionPolylineEquidistant::trigger() {
    RS_ActionBase::trigger();

    if (entity!=NULL) {
        RS_Entity* parent = entity->getParentContainer();
        if (parent!=NULL && parent->rtti()==RS2::EntityPolyline) {
            RS_Polyline* pl = dynamic_cast<RS_Polyline*>(parent);
            RS_Creation creation(container, graphicView);
            creation.createEquidistant(coord,
                              distance, 
                              number,
                              pl,
                              useArcs);
        }
    }
}



void RS_ActionPolylineEquidistant::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionPolylineEquidistant::mouseMoveEvent begin");

    coord = RS_Vector(graphicView->toGraphX(e->x()),
                      graphicView->toGraphY(e->y()));

    entity = catchEntity(e, RS2::ResolveAll);
    
    RS_Polyline* pl = NULL;
    if (entity!=NULL) {
        RS_Entity* parent = entity->getParentContainer();
        if (parent!=NULL && parent->rtti()==RS2::EntityPolyline) {
            pl = dynamic_cast<RS_Polyline*>(parent);
        }
    }

    clearPreview();

    switch (getStatus()) {
    case SetEntity:
        if (pl!=NULL) {
            RS_Creation creation(preview, NULL, false);
            creation.createEquidistant(coord,
                                    distance, 1,
                                    pl, useArcs);
        }
        break;

    default:
        break;
    }

    drawPreview();

    RS_DEBUG->print("RS_ActionPolylineEquidistant::mouseMoveEvent end");
}



void RS_ActionPolylineEquidistant::mouseReleaseEvent(RS_MouseEvent* e) {
    if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    } else {
        trigger();
    }
}



void RS_ActionPolylineEquidistant::updateMouseButtonHints() {
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
void RS_ActionPolylineEquidistant::showOptions() {
    RS_ActionBase::showOptions();

    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestOptions(this, true);
    }
    updateMouseButtonHints();
}
*/



void RS_ActionPolylineEquidistant::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        if (RS_DIALOGFACTORY!=NULL) {
            RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                             + getAvailableCommands().join(", "));
        }
        return;
    }
    /*

    switch (getStatus()) {
    case SetEntity: {
            if (checkCommand("through", c)) {
                finish();
                graphicView->setCurrentAction(
                    new RS_ActionPolylineEquidistantThrough(*container,
                                                         *graphicView));
            } else if (checkCommand("number", c)) {
                deleteSnapper();
                deletePreview();
                clearPreview();
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
    */
}



RS_StringList RS_ActionPolylineEquidistant::getAvailableCommands() {
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



void RS_ActionPolylineEquidistant::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionPolylineEquidistant::updateToolBar() {
    if (RS_DIALOGFACTORY!=NULL) {
        RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarPolylines);
    }
}

