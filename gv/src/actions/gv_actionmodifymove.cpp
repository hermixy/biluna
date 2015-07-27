/*****************************************************************
 * $Id$
 * Created: Nov 12, 2008 1:22:33 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionmodifymove.cpp 10309
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_actionmodifymove.h"

#include "gv_port.h"
#include "gv_snapper.h"



GV_ActionModifyMove::GV_ActionModifyMove(GV_Drawing& dwg, GV_GraphicsView& gv)
        :GV_ActionBase(dwg, gv) {

}


RB_GuiAction* GV_ActionModifyMove::createGuiAction() {
    static RB_GuiAction* action = NULL;

    if (action==NULL) {
        action = new RB_GuiAction(GV_ActionModifyMove::getName(),
            "modifymove",
            tr("&Move / Copy"));
        action->setStatusTip(tr("Move or copy entities one or multiple times"));
        action->setCommand("move,mv");
        action->setKeycode("mv");
        action->setFactory(GV_ActionModifyMove::factory);
    }
    return action;
}



void GV_ActionModifyMove::init(int status) {
    GV_ActionBase::init(status); // does nothing
}



RB_Action* GV_ActionModifyMove::factory() {
    RB_DEBUG->print("GV_ActionModifyMove::factory()");

    GV_Drawing* drawing = GV_DIALOGFACTORY->getDrawing();
    GV_GraphicsView* graphicsView = GV_DIALOGFACTORY->getGraphicsView();

    if (drawing != NULL && graphicsView != NULL) {
        GV_ActionBase* a = new GV_ActionModifyMove(*drawing, *graphicsView);
        drawing->setCurrentAction(a);
        return a;
    }

    return NULL;
}

/**
 * Mouse move event, allows snap to grid if port was selected while draging
 * @param e QGraphicsSceneMouseEvent used for position
 */
void GV_ActionModifyMove::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (!(e->buttons() & Qt::LeftButton) || !getSelectedComponent()) {
        GV_ActionBase::mouseMoveEvent(e);
        return;
    }

    if (getResizeHandle()) {
        GV_ResizeHandle* resHandle = getResizeHandle();
        GV_CoordinateEvent ce(snapPoint(resHandle->getRelativeCornerPos(e).x(),
                                        resHandle->getRelativeCornerPos(e).y()));
        if (ce.isValid()) {
            coordinateEvent(&ce);
        }
        e->accept();
    } else if (getPort()) {
        GV_ActionBase::mouseMoveEvent(e);
        GV_CoordinateEvent ce(snapPoint(e));

        if (ce.isValid()) {
            coordinateEvent(&ce);
        }

        // Hack: because hoverEvent does not work with left mousebutton down
        if (e->buttons() & Qt::LeftButton) {
            getDrawing()->clearSelection();
            GV_Port* port = getPort();
            QList<QGraphicsItem*> itemList = port->collidingItems(Qt::IntersectsItemShape);
            for (int i = 0; i < itemList.size(); i++) {
                QGraphicsItem* item = itemList.at(i);
                item->setSelected(true);
            }
            if (port->connectNode()) {
                GV_Port* otherPort = port->connectNode()->otherPort(port);
                if (otherPort) {
                    otherPort->parentItem()->setSelected(false);
                }
            }
        }

    } else {
        GV_ActionBase::mouseMoveEvent(e);
    }
}

/**
 * Mouse press event, overriden here to set distance to corner for scaling of
 * symbol
 * @param e mouse event
 */
void GV_ActionModifyMove::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    GV_ActionBase::mousePressEvent(e);
    GV_ResizeHandle* resHandle = getResizeHandle();
    if (resHandle) {
        resHandle->setDistanceToCorner(e);
    }
}

void GV_ActionModifyMove::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    // RB_DEBUG->print("GV_ActionModifyMove::mouseReleaseEvent()");
    if (getPort()) {
        GV_Component* comp = dynamic_cast<GV_Component*>(getPort()->parentItem());
        if (comp) {
//			comp->connectFreePorts();
        }
    }

    GV_ActionBase::mouseReleaseEvent(e);
    setPort(NULL);
    init(getStatus()-1);
    finish();
}

void GV_ActionModifyMove::coordinateEvent(GV_CoordinateEvent* ce) {
    // calculate the local position on the parent item if parent item exist.
    QGraphicsItem* compItem = NULL;
    QGraphicsItem* parentItem = NULL;
    GV_CoordinateEvent locCe;

    if (getPort()) {
        compItem = getPort()->parentItem();
        parentItem = compItem->parentItem();
    }

    if (parentItem) {
        QPointF p(ce->getCoordinate().x, ce->getCoordinate().y);
        QPointF locP = parentItem->mapFromScene(p);
        locCe.setPos(locP);
    }

    // Set global or local position
    if (ce != NULL && getPort() != NULL && getResizeHandle() == NULL) {
        if (parentItem) {
            getPort()->updatePosition(&locCe);
        } else {
            getPort()->updatePosition(ce);
        }
    } else if (ce != NULL && getPort() == NULL && getResizeHandle() != NULL) {
        // ce already compensated for distance to corner
        if (parentItem) {
            getResizeHandle()->updatePosition(&locCe);
        } else {
            getResizeHandle()->updatePosition(ce);
        }
    }
}


void GV_ActionModifyMove::updateMouseButtonHints() {
//    switch (getStatus()) {
//        /*case Select:
//            GV_DIALOGFACTORY->updateMouseWidget(tr("Pick entities to move"),
//                                           tr("Cancel"));
//            break;*/
//    case SetReferencePoint:
//        GV_DIALOGFACTORY->updateMouseWidget(tr("Specify reference point"),
//                                            tr("Cancel"));
//        break;
//    case SetTargetPoint:
//        GV_DIALOGFACTORY->updateMouseWidget(tr("Specify target point"),
//                                            tr("Back"));
//        break;
//    default:
//        GV_DIALOGFACTORY->updateMouseWidget("", "");
//        break;
//    }
}



void GV_ActionModifyMove::updateMouseCursor() {
//    graphicView->setMouseCursor(RS2::CadCursor);
}



void GV_ActionModifyMove::updateToolBar() {
//    switch (getStatus()) {
//        /*case Select:
//            GV_DIALOGFACTORY->requestToolBar(RS2::ToolBarSelect);
//            break;*/
//    case SetReferencePoint:
//    case SetTargetPoint:
//        GV_DIALOGFACTORY->requestToolBar(RS2::ToolBarSnap);
//        break;
//    default:
//        GV_DIALOGFACTORY->requestToolBar(RS2::ToolBarModify);
//        break;
//    }
}

