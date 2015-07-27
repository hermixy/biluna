/*****************************************************************
 * $Id: gv_actionmodifymergeline.cpp 1260 2010-07-29 17:53:40Z rutger $
 * Created: Feb 18, 2009 2:23:20 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionmodifymergeline.h"

RB_GuiAction* GV_ActionModifyMergeLine::action = NULL;

GV_ActionModifyMergeLine::GV_ActionModifyMergeLine(GV_Drawing& dwg,
        GV_GraphicsView& gv) : GV_ActionBase(dwg, gv) {
    RB_DEBUG->print("GV_ActionModifySplitLine::GV_ActionModifySplitLine()");
}

RB_GuiAction* GV_ActionModifyMergeLine::createGuiAction() {
    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionModifyMergeLine::getName(),
            "modifymergeline",
            tr("&Merge Line"));
        action->setStatusTip(tr("Merge two line segments"));
        action->setFactory(GV_ActionModifyMergeLine::factory);
        action->setCommand("ml");
        action->setKeycode("ml");
    }

    return action;
}


RB_Action* GV_ActionModifyMergeLine::factory() {
    RB_DEBUG->print("GV_ActionModifyMergeLine::factory()");

    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionModifyMergeLine(*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}



void GV_ActionModifyMergeLine::finish() {
    GV_ActionBase::finish();
}


void GV_ActionModifyMergeLine::init(int status) {
    GV_ActionBase::init(status);

    snapMode = GV2::ActionSnapFree;
    snapRes = GV2::ActionRestrictNothing;
}


/**
 * Trigger action
 * @param selPort selected port
 */
void GV_ActionModifyMergeLine::trigger(GV_Port* selPort) {
    RB_DEBUG->print("GV_ActionModifyMergeLine::trigger()");

    if (!selPort) return;
    GV_Node* selNode = selPort->connectNode();
    if (!selNode) return;
    GV_Port* othPort = selNode->otherPort(selPort);

    if (!selNode || !othPort) return;

    GV_Line* selLine = dynamic_cast<GV_Line*>(selPort->parentItem());
    GV_Line* othLine = dynamic_cast<GV_Line*>(othPort->parentItem());

    if (!selLine || !othLine) return;

    // both items are lines
    QPointF p0;
    QPointF p1;

    if (selNode->connectPort() == selPort) {
        p0 = othLine->port0()->scenePos();
        p1 = selLine->port1()->scenePos();
        this->removeComponent(selLine);

        QLineF l(p0, p1);
        othLine->setLine(l);
//        othLine->connectFreePort(othLine->port1());
    } else if (selNode->connectPort() == othPort) {
        p0 = selLine->port0()->scenePos();
        p1 = othLine->port1()->scenePos();
        this->removeComponent(othLine);

        QLineF l(p0, p1);
        selLine->setLine(l);
//        selLine->connectFreePort(selLine->port1());
    }

    finish();
}



void GV_ActionModifyMergeLine::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {

    this->setSelectedItem(e->scenePos());
    GV_Port* selPort = this->getPort();
    if (!selPort) return;

    if (e->button() == Qt::LeftButton) {
        trigger(selPort);
    } else if (e->button() == Qt::RightButton) {
        finish();
    }
}


