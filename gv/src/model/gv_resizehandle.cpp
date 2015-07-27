/*****************************************************************
 * $Id: gv_resizehandle.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Apr 2, 2008 9:01:54 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_resizehandle.h"

#include "gv_coordinateevent.h"
#include "gv_drawing.h"
#include "gv_symbol.h"
#include "gv_text.h"
#include "RVector.h"

/**
 * Constructor
 */
GV_ResizeHandle::GV_ResizeHandle(QGraphicsItem* parent)
                                : QGraphicsPolygonItem(parent){
    mDeltaPos = QPointF(0.0,0.0);
    mMousePoint = QPointF(0.0,0.0);
    setZValue(1000.0);
}

/**
 * Copy constructor
 */
GV_ResizeHandle::GV_ResizeHandle(GV_ResizeHandle* lever)
                                : QGraphicsPolygonItem(lever->parentItem()){
    mDeltaPos = lever->getDeltaPos();
    mMousePoint = lever->getMousePoint();
    setZValue(1000.0);
}

/**
 * Destructor
 */
GV_ResizeHandle::~GV_ResizeHandle() {
    // nothing
    RB_DEBUG->print("GV_ResizeHandle::~GV_ResizeHandle() OK");
}

void GV_ResizeHandle::draw() {
    // draw resize handle, four two-way arrows
    double f = 1.0; // scaling factor

    QPolygonF polygon;
    polygon << QPointF(0.0, 0.0) << QPointF(f * 2.0, 2.0) << QPointF(f * 2.0, f * 1.0);
    polygon << QPointF(f * 4.0, f * 1.0) << QPointF(f * 4.0, f * 2.0) << QPointF(f * 6.0, 0.0);
    polygon << QPointF(f * 4.0, f * -2.0) << QPointF(f * 4.0, f * -1.0) << QPointF(f * 2.0, f * -1.0);
    polygon << QPointF(f * 2.0, f * -2.0) << QPointF(0.0, 0.0);
    setPolygon(polygon);

}

/**
 * Mouse move event for resizing with opposite corner as origine
 * @param event mouse move event
 */
//void GV_ResizeHandle::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
//
//    // parent item needs to be a symbol
//    if (parentItem()->type() == UserType + 1) {
//        // In GV_Symbol: friend class GV_Port;
//        GV_Symbol* symbol = getParentSymbol();
//        QPointF currentPos = mapToParent(e->pos());
//        QPointF lastPos = mapToParent(e->lastPos());
//        symbol->scale(this, currentPos - lastPos);
//    } else if (parentItem()->type() == UserType + 5) {
//        // In GV_Text: friend class GV_Port;
//        GV_Text* text = getParentText();
//        QPointF currentPos = mapToParent(e->pos());
//        QPointF lastPos = mapToParent(e->lastPos());
//        text->scale(this, currentPos - lastPos);
//
//    }
//}

/**
 * Mouse presse event will select an item if applicable and set the
 * relevant pipe line or equipment
 * @param e mouse scene event
 */
void GV_ResizeHandle::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    RB_DEBUG->print("GV_ResizeHandle::mousePressEvent()");
    QGraphicsPolygonItem::mousePressEvent(e);

    if (parentItem()) {
        parentItem()->setSelected(true);

    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                  "GV_ResizeHandle::mousePressEvent() no parentItem ERROR");
    }
}

/**
 * Overridden function to set the dimension data in the GV_Symbol object after
 * mouse scaling of symbol
 * @param event mouse event
 */
void GV_ResizeHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    RB_DEBUG->print("GV_ResizeHandle::mouseReleaseEvent()");
    QGraphicsPolygonItem::mouseReleaseEvent(e);
}

/**
 * Update position of resizehandle, called by move action. The corresponding
 * corner of th symbol can snap to grid.
 * @param e coordinate event
 */
void GV_ResizeHandle::updatePosition(GV_CoordinateEvent* e) {
    RB_DEBUG->print("GV_ResizeHandle::updatePosition()");

    QPointF mousePoint = e->pos();

    if (parentItem()->type() == UserType + 1) {
        // parent item is GV_Symbol
        GV_Symbol* symbol = getParentSymbol();
        symbol->updateCornerPosition(this, mousePoint);
    } else if (parentItem()->type() == UserType + 5) {
        // In GV_Text: friend class GV_Port;
        GV_Text* text = getParentText();
        text->updateCornerPosition(this, mousePoint);
    }
}

/**
 * @return parent symbol
 */
GV_Symbol* GV_ResizeHandle::getParentSymbol() {

    if (parentItem()->type() == UserType + 1) {
        // In GV_Symbol: friend class GV_ResizeHandle;
        GV_Symbol* symbol = dynamic_cast<GV_Symbol*>(parentItem());
        return symbol;
    }

    return NULL;
}

/**
 * @return parent text
 */
GV_Text* GV_ResizeHandle::getParentText() {

    if (parentItem()->type() == UserType + 5) {
        // In GV_Text: friend class GV_ResizeHandle;
        GV_Text* text = dynamic_cast<GV_Text*>(parentItem());
        return text;
    }

    return NULL;
}

/**
 * Set distance of mouse press event to applicable corner of symbol
 * @param e mouse event
 */
void GV_ResizeHandle::setDistanceToCorner(QGraphicsSceneMouseEvent* e) {
    if (parentItem()->type() == UserType + 1) {
        getParentSymbol()->setDistanceToCorner(this, e);
    } else if (parentItem()->type() == UserType + 5) {
        getParentText()->setDistanceToCorner(this, e);
    }
}

/**
 * Get applicable corner position relative to the resizehandle
 * @param e mous event
 * @return corner position in scene coordinates
 */
QPointF GV_ResizeHandle::getRelativeCornerPos(QGraphicsSceneMouseEvent* e) {
    if (parentItem()->type() == UserType + 1) {
        return getParentSymbol()->getRelativeCornerPos(e);
    } else if (parentItem()->type() == UserType + 5) {
        return getParentText()->getRelativeCornerPos(e);
    }

    return QPointF();
}

