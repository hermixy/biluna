/*****************************************************************
 * $Id: gv_actionmodifysplitline.cpp 1169 2010-05-26 11:53:18Z rutger $
 * Created: Feb 18, 2009 1:07:28 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionmodifysplitline.h"

#include "gv_objectfactory.h"


RB_GuiAction* GV_ActionModifySplitLine::action = NULL;

GV_ActionModifySplitLine::GV_ActionModifySplitLine(GV_Drawing& dwg, 
		GV_GraphicsView& gv) : GV_ActionBase(dwg, gv) {
	RB_DEBUG->print("GV_ActionModifySplitLine::GV_ActionModifySplitLine()");
}

RB_GuiAction* GV_ActionModifySplitLine::createGuiAction() {
    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionModifySplitLine::getName(),
            "modifysplitline",
            tr("&Split Line"));
        action->setStatusTip(tr("Split a line in two segments"));
        action->setFactory(GV_ActionModifySplitLine::factory);
        action->setCommand("sl");
        action->setKeycode("sl");
    }                          

    return action;
}


RB_Action* GV_ActionModifySplitLine::factory() {
    RB_DEBUG->print("GV_ActionModifySplitLine::factory()");

    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionModifySplitLine(*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void GV_ActionModifySplitLine::finish() {
    GV_ActionBase::finish();
}


void GV_ActionModifySplitLine::init(int status) {
    GV_ActionBase::init(status);

    snapMode = GV2::ActionSnapFree;
    snapRes = GV2::ActionRestrictNothing;
}



void GV_ActionModifySplitLine::trigger() {
    RB_DEBUG->print("GV_ActionModifySplitLine::trigger()");

    GV_Line* line = getLine();
    if (!line) return;

    // remember connection, node can be deleted in case of interconnecting node
    GV_Node* endNode = line->port1()->connectNode();
    GV_Port* endPort = NULL;
    if (endNode) {
        endPort = endNode->otherPort(line->port1());
    }

    // disconnect Port1 and half the length
    line->port1()->disconnect();
    QPointF p0 = line->port0()->scenePos();
    QPointF p1 = line->port1()->scenePos();
    QPointF pDelta = p1 - p0;
    QPointF midP = p0 + pDelta / 2;
    QLineF l(p0, midP);
    line->setLine(l);

    // create new line
    GV_Line* newLine = addNewLine(line);
    newLine->setLine(QLineF(midP, p1));

    // create connection mid point
    GV_Node* midNode = newLine->origineNode();
    midNode->connect(newLine->port0(), line->port1());
    midNode->updatePosition(newLine->port0());

    // create connection with end point, TODO: connectFreePort() easier?
    if (endPort && endPort->connectNode() /*endNode could have been deleted*/) {
        // origine node available of connecting item
        endNode = endPort->connectNode(); // not realy necassary
        endNode->connect(endPort, newLine->port1());
    } else if (endPort) {
        // no node, create new interconnecting node
        RB_ObjectContainer* objList = NULL;
        objList = mDrawing->getContainer("GV_NodeList");

        endNode = (GV_Node*)GV_OBJECTFACTORY->newObject("", objList);
        objList->addObject(endNode);
        getDrawing()->addItem(endNode);

        endNode->connect(endPort, newLine->port1());
        endNode->setInterconnecting(true);
    }

    finish();
}



void GV_ActionModifySplitLine::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {

    this->setSelectedItem(e->scenePos());
    GV_Line* line = dynamic_cast<GV_Line*>(this->getSelectedComponent());
    if (!line) return;

    if (e->button() == Qt::LeftButton) {
        trigger();
    } else if (e->button() == Qt::RightButton) {
        finish();
    }
}


/*
void GV_ActionModifySplitLine::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().toLower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case ChooseDivideEntity: {
        }
        break;

    default:
        break;
    }
}



RS_StringList GV_ActionModifySplitLine::getAvailableCommands() {
    RS_StringList cmd = GV_ActionBase::getAvailableCommands();

    switch (getStatus()) {
    case ChooseDivideEntity:
        break;
    default:
        break;
    }

    return cmd;
}


void GV_ActionModifySplitLine::updateMouseButtonHints() {
    switch (getStatus()) {
    case ChooseDivideEntity:
        RS_DIALOGFACTORY->updateMouseWidget(
            tr("Select entity to break out segment"),
            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void GV_ActionModifySplitLine::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void GV_ActionModifySplitLine::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
}
*/


