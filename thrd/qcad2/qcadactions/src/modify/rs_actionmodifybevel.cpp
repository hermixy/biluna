/****************************************************************************
** $Id: rs_actionmodifybevel.cpp 10540 2008-04-21 10:27:43Z andrew $
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

#include "rs_actionmodifybevel.h"

#include "rs_snapper.h"
#include "rs_information.h"


RS_ActionModifyBevel::RS_ActionModifyBevel(RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionBase(container, graphicView) {

    entity1 = NULL;
    coord1 = RS_Vector(false);
    entity2 = NULL;
    coord2 = RS_Vector(false);
}


RS_GuiAction* RS_ActionModifyBevel::createGuiAction() {
    static RS_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RS_GuiAction(RS_ActionModifyBevel::getName(), 
            "modifybevel",
            tr("&Chamfer / Bevel"));
        action->setStatusTip(tr("Chamfer / bevel corners"));
        action->setCommand("bevel,chamfer,ch");
        action->setKeycode("ch");
        action->setFactory(RS_ActionModifyBevel::factory);
    }                                   
    return action;
}


RS_ActionInterface* RS_ActionModifyBevel::factory() {
    RS_EntityContainer* container = RS_DIALOGFACTORY->getDocument();
    RS_GraphicView* graphicView = RS_DIALOGFACTORY->getGraphicView();
    
    if (container!=NULL && graphicView!=NULL) {
        RS_ActionInterface* a = new RS_ActionModifyBevel(*container, *graphicView);
        graphicView->setCurrentAction(a);
        return a;
    }

    return NULL;
}


void RS_ActionModifyBevel::init(int status) {
    RS_ActionBase::init(status);

    snapMode = RS2::ActionSnapFree;
    snapRes = RS2::ActionRestrictNothing;
}



void RS_ActionModifyBevel::trigger() {

    RS_DEBUG->print("RS_ActionModifyBevel::trigger()");

    if (entity1!=NULL && entity1->isAtomic() &&
            entity2!=NULL && entity2->isAtomic()) {

        RS_Modification m(*container, graphicView);
        m.bevel(coord1, dynamic_cast<RS_AtomicEntity*>(entity1),
                coord2, dynamic_cast<RS_AtomicEntity*>(entity2),
                data);

        entity1 = NULL;
        entity2 = NULL;
        setStatus(SetEntity1);

        RS_DIALOGFACTORY->updateSelectionWidget(container->countSelected());
    }
    else {
        RS_DEBUG->print("RS_ActionModifyBevel::trigger: "
            "Entities are NULL or not atomic");
    }
}



void RS_ActionModifyBevel::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionModifyBevel::mouseMoveEvent begin");

    RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
    RS_Entity* se = catchEntity(e, RS2::ResolveAll);

    switch (getStatus()) {
    case SetEntity1:
        coord1 = mouse;
        entity1 = se;
        break;

    case SetEntity2:
        coord2 = mouse;
        entity2 = se;

        if (entity1!=NULL && entity2!=NULL && entity2->isAtomic() &&
                RS_Information::isTrimmable(entity1, entity2)) {
            
            deletePreview();
            clearPreview();
            RS_Entity* tmp1 = entity1->clone();
            RS_Entity* tmp2 = entity2->clone();
            tmp1->reparent(preview);
            tmp2->reparent(preview);
            preview->addEntity(tmp1);
            preview->addEntity(tmp2);

            bool trim = data.trim;
            data.trim = false;
            RS_Modification m(*preview, NULL, false);
            m.bevel(coord1, dynamic_cast<RS_AtomicEntity*>(entity1),
                    coord2, dynamic_cast<RS_AtomicEntity*>(entity2),
                    data);
            data.trim = trim;

            preview->removeEntity(tmp1);
            preview->removeEntity(tmp2);
            drawPreview();
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionModifyBevel::mouseMoveEvent end");
}



void RS_ActionModifyBevel::mouseReleaseEvent(RS_MouseEvent* e) {
    RS_Vector mouse = graphicView->toGraph(e->x(), e->y());
    RS_Entity* se = catchEntity(e, RS2::ResolveAll);

    if (RS2::qtToRsButtonState(e->button())==RS2::LeftButton) {
        switch (getStatus()) {
        case SetEntity1:
            entity1 = se;
            coord1 = mouse;
            if (entity1!=NULL && entity1->isAtomic()) {
                setStatus(SetEntity2);
            }
            else {
                RS_DEBUG->print("RS_ActionModifyBevel::mouseReleaseEvent: "
                    "no bevelable entity found for 1st click");
            }
            break;

        case SetEntity2:
            entity2 = se;
            coord2 = mouse;
            if (entity2!=NULL && entity2->isAtomic() &&
                RS_Information::isTrimmable(entity1, entity2)) {
                trigger();
            }
            else {
                RS_DEBUG->print("RS_ActionModifyBevel::mouseReleaseEvent: "
                    "no bevelable entity found for 2nd click");
            }
            break;

        default:
            break;
        }
    } else if (RS2::qtToRsButtonState(e->button())==RS2::RightButton) {
        clearPreview();
        graphicView->restore();
        init(getStatus()-1);
    }
}



void RS_ActionModifyBevel::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetEntity1:
    case SetEntity2:
        if (checkCommand("length1", c)) {
            clearPreview();
            graphicView->restore();
            lastStatus = (Status)getStatus();
            setStatus(SetLength1);
        } else if (checkCommand("length2", c)) {
            clearPreview();
            graphicView->restore();
            lastStatus = (Status)getStatus();
            setStatus(SetLength2);
        } else if (checkCommand("trim", c)) {
            //deleteSnapper();
            //deletePreview();
            //clearPreview();
            //lastStatus = (Status)getStatus();
            //setStatus(SetTrim);
            data.trim = !data.trim;
            RS_DIALOGFACTORY->requestOptions(this, true, true);
        }
        break;

    case SetLength1: {
            bool ok;
            double l = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.length1 = l;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetLength2: {
            bool ok;
            double l = RS_Math::eval(c, &ok);
            if (ok==true) {
                data.length2 = l;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

        /*case SetTrim: {
        if (checkCommand()) {
        data.trim = true;
    } else if (c==cmdNo.toLower() || c==cmdNo2) {
        data.trim = false;
                } else {
                    RS_DIALOGFACTORY->commandMessage(tr("Please enter 'Yes' "
               "or 'No'"));
                }
                RS_DIALOGFACTORY->requestOptions(this, true, true);
                setStatus(lastStatus);
            }
            break;*/

    default:
        break;
    }
}



RS_StringList RS_ActionModifyBevel::getAvailableCommands() {
    RS_StringList cmd = RS_ActionBase::getAvailableCommands();
    
    switch (getStatus()) {
    case SetEntity1:
    case SetEntity2:
        cmd += command("length1");
        cmd += command("length2");
        cmd += command("trim");
        break;
    default:
        break;
    }
    return cmd;
}



void RS_ActionModifyBevel::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetEntity1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select first entity"),
                                            tr("Cancel"));
        break;
    case SetEntity2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select second entity"),
                                            tr("Back"));
        break;
    case SetLength1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter length 1:"),
                                            tr("Back"));
        break;
    case SetLength2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter length 2:"),
                                            tr("Back"));
        break;
        /*case SetTrim:
            RS_DIALOGFACTORY->updateMouseWidget(tr("Trim on? (yes/no):"),
                                                "");
            break;*/
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionModifyBevel::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionModifyBevel::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
}

