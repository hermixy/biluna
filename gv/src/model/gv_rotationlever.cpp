/*****************************************************************
 * $Id: gv_rotationlever.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Apr 2, 2008 9:01:54 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_rotationlever.h"

#include "gv_component.h"
#include "gv_drawing.h"
#include "gv_symbol.h"
#include "gv_text.h"
#include "RVector.h"

/**
 * Constructor
 */
GV_RotationLever::GV_RotationLever(QGraphicsItem* parent)
                            : QGraphicsPolygonItem(parent){
    RB_String name = "";

    if (parent->type() == UserType + 1) {
        // For example GV_Symbol or RPT_Symbol
        GV_Symbol* symbol = dynamic_cast<GV_Symbol*>(parent);
        if (symbol) name = symbol->getName();
    } else if (parent->type() == UserType + 5) {
        GV_Text* text = dynamic_cast<GV_Text*>(parent);
        if (text) name = text->getName();
    }

    RB_StringList strL = name.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    mContextPrefix = strL.at(0);

    setZValue(1000.0);
}

/**
 * Copy constructor
 */
GV_RotationLever::GV_RotationLever(GV_RotationLever* lever)
                                : QGraphicsPolygonItem(lever->parentItem()){
    RB_String name = "";

    if (lever->parentItem()->type() == UserType + 1) {
        // For example GV_Symbol or RPT_Symbol
        GV_Symbol* symbol = dynamic_cast<GV_Symbol*>(lever->parentItem());
        if (symbol) name = symbol->getName();
    } else if (lever->parentItem()->type() == UserType + 5) {
        GV_Text* text = dynamic_cast<GV_Text*>(lever->parentItem());
        if (text) name = text->getName();
    }

    RB_StringList strL = name.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    mContextPrefix = strL.at(0);

    setZValue(1000.0);
}

/**
 * Destructor
 */
GV_RotationLever::~GV_RotationLever() {
    // nothing
    RB_DEBUG->print("GV_RotationLever::~GV_RotationLever() OK");
}

void GV_RotationLever::draw() {
    // draw rotation handle, line with diamond
    double f = 1.0; // scaling factor

    QPolygonF polygon;
    polygon << QPointF(0.0, 0.0) << QPointF(f * 6.0, 0.0);
    polygon << QPointF(f * 8.0, f * 4.0) << QPointF(f * 10.0, 0.0)
                << QPointF(f * 6.0, 0.0) << QPointF(f * 8.0, f * -4.0)
                << QPointF(f * 10.0, 0.0);
    setPolygon(polygon);
}

/**
 * Mouse move event for rotation around origine only
 * @param event mouse move event
 */
void GV_RotationLever::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    // Rotate an item 45 degrees around (x, y). Example:
    // item->setTransform(QTransform().translate(x, y).rotate(45).translate(-x, -y));

    // mouse position in item coordinates to calculate the angle difference
    // with previous position
    GV_Component* comp = dynamic_cast<GV_Component*>(this->parentItem());
    QPointF origPos = comp->scenePos();
    QPointF lastPos = mapToScene(event->lastPos());
    QPointF currentPos = mapToScene(event->pos());
    RVector vectorFrom(lastPos.x() - origPos.x(), lastPos.y() - origPos.y());
    RVector vectorTo(currentPos.x() - origPos.x(), currentPos.y() - origPos.y());

    GV_Drawing* drawing = comp->getDrawing();

    if (drawing && (drawing->getDefaultSnapMode() != GV2::ActionSnapFree)) {
        double m11 = comp->transform().m11();
        double m12 = comp->transform().m12();
        double m21 = comp->transform().m21();
        double m22 = comp->transform().m22();
        double dx = comp->transform().dx(); // usually zero, internal origine location
        double dy = comp->transform().dy(); // usually zero, internal origine location

        // calculate the scaling factors
        double xScale = pow((pow(m11, 2) + pow(m12, 2)), 0.5);
        double yScale = pow((pow(m21, 2) + pow(m22, 2)), 0.5);

        // Degrees
        double angle = vectorTo.getAngle() * 180 / M_PI;

        // set new angle and round to 5 degrees
        int fifths = angle / 5;
        angle = fifths * 5;

        QMatrix matrix;
        matrix.translate(dx, dy);
        matrix.rotate(angle);
        matrix.scale(xScale, yScale);
        comp->setTransform(QTransform(matrix));
    } else {
        // Rotation around item origine, angle is delta with previous position.
        // not the absolute angle! Parent item will also rotate this and other
        // child items

        // Degrees
        double deltaAngle = (vectorTo.getAngle() - vectorFrom.getAngle()) * 180 / M_PI;
        comp->rotate(deltaAngle);
    }

//	comp->updatePosition();
    // QGraphicsPolygonItem::mouseMoveEvent(event);

}

/**
 * Mouse presse event will select an item if applicable and set the
 * relevant pipe line or equipment
 * @param e mouse scene event
 */
void GV_RotationLever::mousePressEvent(QGraphicsSceneMouseEvent* /* e */) {
    RB_DEBUG->print("GV_RotationLever::mousePressEvent()");

    if (parentItem()) {
        parentItem()->setSelected(true);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                  "GV_RotationLever::mousePressEvent() no parentItem ERROR");
    }
}

/**
 * Overridden function to set the location data in the GV_Symbol object after
 * mouse rotation of symbol
 * @param event mouse event
 */
void GV_RotationLever::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    GV_Component* comp = dynamic_cast<GV_Component*>(parentItem());
    GV_Port* port = comp->port0();

    QPointF mousePoint = event->scenePos();
    QPointF originePort = port->scenePos();

    double dX = mousePoint.x() - originePort.x();
    double dY = mousePoint.y() - originePort.y();
    port->setValue("dirX", dX);
    port->setValue("dirY", dY);

    // set GV model data, required here because not so easy at model save action
    RB_ObjectContainer* portList = comp->getContainer(mContextPrefix + "_PortList");
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        port = (GV_Port*)iter->currentObject();

        // set GV model data
        port->setValue("dirX", dX);
        port->setValue("dirY", dY);
    }

    delete iter;
    QGraphicsPolygonItem::mouseReleaseEvent(event);
}
