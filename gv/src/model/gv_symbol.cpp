/*****************************************************************
 * $Id: gv_symbol.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Apr 24, 2008 9:42:28 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_symbol.h"

#include <QList>
#include <QtWidgets>
#include "gv_drawing.h"
#include "gv_node.h"
#include "gv_objectfactory.h"
#include "gv_port.h"
#include "gv_graphicsscene.h"
#include "rb_debug.h"


GV_Symbol::GV_Symbol(const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f,
                    QGraphicsItem* parent)
                    : QObject(), QGraphicsPathItem(parent),
                    GV_Component(id, p, n, f) {
    // RB_Component
    setStandardPortCount(1);
    createMembers();

    // Cannot create two standard ports since GV_PortList does not yet exist
    // createStartPort();

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
//  setAcceptDrops(true);
//	setCursor(Qt::OpenHandCursor);
    setToolTip(QString("%1\n%2")
           .arg("GV_Symbol")
           .arg("This could be subtype information!"));

    setPen(mPen);
    mPort0 = NULL;
    mPort1 = NULL;
    mRotLever = NULL;
    mResHandleLT = NULL;
    mResHandleLB = NULL;
    mResHandleRB = NULL;
    mResHandleRT = NULL;
    mGraphicsItem = this;
    mObject = this;

    // For example GV_Symbol or RPT_Symbol
    RB_StringList strL = n.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    mContextPrefix = strL.at(0);
}

GV_Symbol::GV_Symbol(GV_Symbol* symbol) : QObject(),
        QGraphicsPathItem(symbol->parentItem()), GV_Component(symbol) {
    // RB_Component
    setStandardPortCount(1);

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setToolTip(QString("%1\n%2")
           .arg("GV_Symbol")
           .arg("This could be subtype information!"));

    setPen(mPen);
    mPort0 = NULL;
    mPort1 = NULL;
    mRotLever = NULL;
    mResHandleLT = NULL;
    mResHandleLB = NULL;
    mResHandleRB = NULL;
    mResHandleRT = NULL;
    mGraphicsItem = this;
    mObject = this;
    setPath(symbol->path());
}

/**
 * Destructor
 * NOTE: use deleteLater() instead, refer to QObject::~QObject()
 */
GV_Symbol::~GV_Symbol() {
    GV_Component::disconnect();
//	delete mPort0;
    // delete mPort1; not here because port is for symbol part of portlist
    mPort0 = NULL;
    mPort1 = NULL;

//    if (mRotLever) {
//        mRotLever->setParentItem(NULL);
//        delete mRotLever;
//        mRotLever = NULL;
//	}

    RB_DEBUG->print("GV_Symbol::~GV_Symbol() OK");
}

void GV_Symbol::createMembers() {
    // RB_DEBUG->print("EQL_Cylinder::createMembers()");
    // fname is used for functional name, such as shell, pipe, tube, barrel, etc.
    //addMember(this, "fname", "-", "Line", RB2::MemberChar40);
    addMember(this, "path", "-", "", RB2::MemberChar255);				// 0
    addMember(this, "color", "-", 0, RB2::MemberUInteger);
    addMember(this, "style", "-", 0, RB2::MemberInteger);
    addMember(this, "width", "mm", 0.0, RB2::MemberDouble);
}

/**
 * Create start port to graphicsview symbol item. The ports are actualy
 * QGraphicsRectItem's and separate classes as in GV.
 * NOTE: the first port added ALLWAYS has to be the origine port
 * AND at x = 0.0 and y = 0.0!
 */
void GV_Symbol::createPorts() {
    RB_DEBUG->print("GV_Symbol::createPorts()");

    if (!mPort0) {
        RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
        mPort0 = (GV_Port*)getFactory()->newObject("", portList);
        mPort0->setParentItem(this);
        mPort0->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
        mPort0->setPen(mDetailPen);
        mPort0->setVisible(false);
    }
    mPort1 = NULL;
}

/**
 * Create rotation lever and resize handles
 */
void GV_Symbol::createHandles() {
    mRotLever = new GV_RotationLever(this);
    mRotLever->draw(); // for possible future parameters
    mRotLever->setPen(mDetailPen);
    mRotLever->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mRotLever->setVisible(false);

    // Left top
    mResHandleLT = new GV_ResizeHandle(this);
    mResHandleLT->draw(); // for possible future parameters
    mResHandleLT->setPen(mDetailPen);
    mResHandleLT->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mResHandleLT->setVisible(false);

    // Left bottom
    mResHandleLB = new GV_ResizeHandle(this);
    mResHandleLB->draw();
    mResHandleLB->setPen(mDetailPen);
    mResHandleLB->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mResHandleLB->setVisible(false);

    // Right bottom
    mResHandleRB = new GV_ResizeHandle(this);
    mResHandleRB->draw();
    mResHandleRB->setPen(mDetailPen);
    mResHandleRB->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mResHandleRB->setVisible(false);

    // Right bottom
    mResHandleRT = new GV_ResizeHandle(this);
    mResHandleRT->draw();
    mResHandleRT->setPen(mDetailPen);
    mResHandleRT->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mResHandleRT->setVisible(false);

}

/**
 * Set handle postion at creation of symbol and after scaling of the symbol
 */
void GV_Symbol::setHandlePosition() {
    double margin = 2.0;
    double x = 0.0; // translateX
    double y = 0.0;
    QTransform m;

    x = boundingRect().x() + boundingRect().width() + margin;
    y = 0.0;
    m.reset();
    m.translate(x, y);
    mRotLever->setTransform(m);

    x = boundingRect().x() - margin;
    y = boundingRect().y() - margin;
    m.reset();
    m.translate(x, y);
    m.rotate(-135);
    mResHandleLT->setTransform(m);

    x = boundingRect().x() - margin;
    y = boundingRect().y() + boundingRect().height() + margin;
    m.reset();
    m.translate(x, y);
    m.rotate(135);
    mResHandleLB->setTransform(m);

    x = boundingRect().x() + boundingRect().width() + margin;
    y = boundingRect().y() + boundingRect().height() + margin;
    m.reset();
    m.translate(x, y);
    m.rotate(45);
    mResHandleRB->setTransform(m);

    x = boundingRect().x() + boundingRect().width() + margin;
    y = boundingRect().y() - margin;
    m.reset();
    m.translate(x, y);
    m.rotate(-45);
    mResHandleRT->setTransform(m);

}

/**
 * Get port data at id
 * @param portId id of the port
 * @param p to contain port data
 */
void GV_Symbol::port(const RB_String& portId, GV_Port& p) {
    RB_DEBUG->print("GV_Symbol::port(RB_String, GV_Port)");
    // RB_DEBUG->print(portId);

    if (portId.startsWith("Port")) {
        RB_String str = portId;
        str.remove("Port");
        int portNumber = str.toInt();

        switch (portNumber) {
            case 0:
                p.setLocation(RVector(0.0, 0.0, 0.0));
                p.setDirection(RVector(-1.0, 0.0, 0.0));
                p.setOrientation(0.0); // default only
                break;
            default:
                break;
        }
    } else {
        RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
        GV_Port* port = (GV_Port*)portList->getObject(portId);

        if (!port) {
            RB_DEBUG->print(RB_Debug::D_ERROR,
                    "GV_Symbol::port() port does not exist ERROR");
        } else {
            ((RB_Port)p) = *((RB_Port*)port);
        }
    }
}


/**
 * Get port data at number, standard ports and custom ports if any.
 * @param portNumber number of the port, starting with 0 for the standard ports
 * @param p to contain port data
 */
void GV_Symbol::port(int portNumber, GV_Port& p) {
    GV_Component::port(portNumber, p);
}


/**
 * Initialization of object for creation of ports, line type etc.
 * Called separately in GV_ObjectFactory to createHandles,
 * ports can read from database, at creation of new object
 * call createPorts() separately
 */
void GV_Symbol::init() {
    RB_DEBUG->print("GV_Symbol::init()");
//    createPorts();
    createHandles();

    // TODO: set line type and color
}

/**
 * Bounding rectangle of symbol
 * @return QRectF rectangle
 */
QRectF GV_Symbol::boundingRect() const {
    // x, y, width, height, valve = 20x30
    // return QRectF(-5, -15, 40, 30);
    return QGraphicsPathItem::boundingRect();
}

/**
 * Wrapper for QGraphicsItem::setPos(), also set position of symbol or text
 * and the origine node
 * @param p position of origine
 */
void GV_Symbol::setPos(const QPointF& p) {
    GV_Component::setPos(p);
}

/**
 * Scale symbol using the resize handles
 * @param resHandle one of the resizehandles
 * @param delta mouse move delta
 */
void GV_Symbol::scale(GV_ResizeHandle* resHandle, const QPointF& delta) {
    double factorX;
    double factorY;
    double lengthX;
    double lengthY;
    QPointF scaleOrigine;
    QPointF scaleCorner;
    double halfLineThickness = pen().width() / 2;

    // to update boundary area
    prepareGeometryChange();

    if (resHandle == mResHandleLT) {
        // Control points based on bounding rectangle minus half line thickness
        scaleOrigine = QPointF(boundingRect().x() + boundingRect().width(),
                               boundingRect().y() + boundingRect().height());
        scaleOrigine += QPointF(-halfLineThickness, -halfLineThickness);
        scaleCorner = QPointF(boundingRect().x(), boundingRect().y());
        scaleCorner += QPointF(halfLineThickness, halfLineThickness);
        lengthX = std::abs(scaleOrigine.x() - scaleCorner.x());
        factorX = (lengthX + delta.x()) / lengthX; // factor at X border
        lengthY = std::abs(scaleOrigine.y() - scaleCorner.y());
        factorY = (lengthY + delta.y()) / lengthY; // factor at Y border
    } else if (resHandle == mResHandleLB) {
        // Control points based on bounding rectangle minus half line thickness
        scaleOrigine = QPointF(boundingRect().x() + boundingRect().width(),
                               boundingRect().y());
        scaleOrigine += QPointF(-halfLineThickness, halfLineThickness);
        scaleCorner = QPointF(boundingRect().x(),
                              boundingRect().y() + boundingRect().height());
        scaleCorner += QPointF(halfLineThickness, -halfLineThickness);
        lengthX = std::abs(scaleOrigine.x() - scaleCorner.x());
        factorX = (lengthX + delta.x()) / lengthX; // factor at X border
        lengthY = std::abs(scaleOrigine.y() - scaleCorner.y());
        factorY = (lengthY - delta.y()) / lengthY; // factor at Y border
    } else if (resHandle == mResHandleRB) {
        // Control points based on bounding rectangle minus half line thickness
        scaleOrigine = QPointF(boundingRect().x(), boundingRect().y());
        scaleOrigine += QPointF(halfLineThickness, halfLineThickness);
        scaleCorner = QPointF(boundingRect().x() + boundingRect().width(),
                              boundingRect().y() + boundingRect().height());
        scaleCorner += QPointF(-halfLineThickness, -halfLineThickness);
        lengthX = std::abs(scaleOrigine.x() - scaleCorner.x());
        factorX = (lengthX - delta.x()) / lengthX; // factor at X border
        lengthY = std::abs(scaleOrigine.y() - scaleCorner.y());
        factorY = (lengthY - delta.y()) / lengthY; // factor at Y border
    } else if (resHandle == mResHandleRT) {
        // Control points based on bounding rectangle minus half line thickness
        scaleOrigine = QPointF(boundingRect().x(),
                               boundingRect().y() + boundingRect().height());
        scaleOrigine += QPointF(halfLineThickness, -halfLineThickness);
        scaleCorner = QPointF(boundingRect().x() + boundingRect().width(),
                              boundingRect().y());
        scaleCorner += QPointF(-halfLineThickness, halfLineThickness);
        lengthX = std::abs(scaleOrigine.x() - scaleCorner.x());
        factorX = (lengthX - delta.x()) / lengthX; // factor at X border
        lengthY = std::abs(scaleOrigine.y() - scaleCorner.y());
        factorY = (lengthY + delta.y()) / lengthY; // factor at Y border
    } else {
        // no resize handle
        return;
    }

    // Scale (reposition) symbol elements
    QPainterPath p = path();
    int count = p.elementCount();

    for (int i = 0; i < count; i++) {
        double scaledX = p.elementAt(i).x * factorX;
        double scaledY = p.elementAt(i).y * factorY;
        p.setElementPositionAt(i, scaledX, scaledY);
    }

    setPath(p);
    setHandlePosition();

    // Update port positions
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");

    RB_ObjectIterator* iter = portList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
        double scaledX = port->pos().x() * factorX;
        double scaledY = port->pos().y() * factorY;
        port->setPosition(this, scaledX, scaledY);
    }
    delete iter;

    // Reposition origine
    double movedX = (-scaleOrigine.x()) * (factorX - 1);
    double movedY = (-scaleOrigine.y()) * (factorY - 1);
    QPointF moveDir = mapToScene(movedX, movedY) - mapToScene(0.0, 0.0);
    moveBy(moveDir.x(), moveDir.y());
}

/**
 * Set distance to corner of the symbol for the resizehandle, used for snap
 * purposes
 * @param resHandle resizehandle
 * @param e mousePoint current location of the mouse in scene coordinates
 */
void GV_Symbol::setDistanceToCorner(GV_ResizeHandle* resHandle,
                                    QGraphicsSceneMouseEvent* e) {
    double halfLineThickness = pen().width() / 2;
    QPointF corner;

    if (resHandle == mResHandleLT) {
        corner = QPointF(boundingRect().x(), boundingRect().y());
        corner += QPointF(halfLineThickness, halfLineThickness);
    } else if (resHandle == mResHandleLB) {
        corner = QPointF(boundingRect().x(),
                         boundingRect().y() + boundingRect().height());
        corner += QPointF(halfLineThickness, -halfLineThickness);
    } else if (resHandle == mResHandleRB) {
        corner = QPointF(boundingRect().x() + boundingRect().width(),
                         boundingRect().y() + boundingRect().height());
        corner += QPointF(-halfLineThickness, -halfLineThickness);
    } else if (resHandle == mResHandleRT) {
        corner = QPointF(boundingRect().x() + boundingRect().width(),
                         boundingRect().y());
        corner += QPointF(-halfLineThickness, halfLineThickness);
    } else {
        return;
    }

    mDistance = mapFromScene(e->scenePos()) - corner;
}

QPointF GV_Symbol::getRelativeCornerPos(QGraphicsSceneMouseEvent* e) {
    QPointF locMousePos = mapFromScene(e->scenePos());
    return mapToScene(locMousePos - mDistance);
}

/**
 * Update the corner position after a resizehandle move action
 * @param resHandle The moved resizehandle
 * @param pos new position for the corner, can be snap position in scene coordinates
 */
void GV_Symbol::updateCornerPosition(GV_ResizeHandle* resHandle, const QPointF& scenePos) {
    double halfLineThickness = pen().width() / 2;

    // scenePos is mousePos translated with distanceToCorner to the new corner position
    QPointF locMousePos = mapFromScene(scenePos);
    QPointF corner;

    if (resHandle == mResHandleLT) {
        corner = QPointF(boundingRect().x(), boundingRect().y());
        corner += QPointF(halfLineThickness, halfLineThickness);
    } else if (resHandle == mResHandleLB) {
        corner = QPointF(boundingRect().x(),
                         boundingRect().y() + boundingRect().height());
        corner += QPointF(halfLineThickness, -halfLineThickness);
    } else if (resHandle == mResHandleRB) {
        corner = QPointF(boundingRect().x() + boundingRect().width(),
                         boundingRect().y() + boundingRect().height());
        corner += QPointF(-halfLineThickness, -halfLineThickness);
    } else if (resHandle == mResHandleRT) {
        corner = QPointF(boundingRect().x() + boundingRect().width(),
                         boundingRect().y());
        corner += QPointF(-halfLineThickness, halfLineThickness);
    } else {
        return;
    }

    scale(resHandle, corner - locMousePos);
}

///**
// * Reimplemented to update the node positions connected to the ports,
// * called from rotationlever.
// */
//void GV_Symbol::setTransform(const QTransform& m) {
//    QGraphicsPathItem::setTransform(m);
//
//    // Update port positions
//    RB_ObjectContainer* portList = getContainer("GV_PortList");
//    RB_ObjectIterator* iter = portList->createIterator();
//    for (iter->first(); !iter->isDone(); iter->next()) {
//        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
//        GV_Node* node = port->connectNode();
//        if (node) {
//            node->updatePosition(this);
//        }
//    }
//    delete iter;
//}
//
///**
// * Reimplemented to update the node positions connected to the ports,
// * called from rotationlever.
// */
//void GV_Symbol::rotate(double angle) {
//    QGraphicsPathItem::rotate(angle);
//
//    // Update port positions
//    RB_ObjectContainer* portList = getContainer("GV_PortList");
//    RB_ObjectIterator* iter = portList->createIterator();
//    for (iter->first(); !iter->isDone(); iter->next()) {
//        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
//        GV_Node* node = port->connectNode();
//        if (node) {
//            node->updatePosition(this);
//        }
//    }
//    delete iter;
//}
//

/**
 * Synchronize the object members with the values of the QGraphicsPathItem
 * such as color
 */
void GV_Symbol::synchronizeObjectData() {
    if ((dynamic_cast<GV_Component*>(this))->getFlags() & RB2::FlagIsDeleted) return;

    if (!mPort0 || !mPort0->connectNode()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_Symbol::synchronizeObjectData() port or "
                        "node, orphan? ERROR");
        return;
    }

    GV_Component::synchronizeObjectData();

    QString str = "";

    for (int i = 0; i < path().elementCount(); ++i) {
        if (i != 0) str += ";";
        int type = path().elementAt(i).type;
        switch (type) {
        case QPainterPath::MoveToElement /*0*/:
            str += "M;"; break;
        case QPainterPath::LineToElement /*1*/:
            str += "L;"; break;
        case QPainterPath::CurveToElement /*2*/:
            str += "C;"; break;
        case QPainterPath::CurveToDataElement /*3*/:
            str += "X;"; break;
        }

        str += QString::number(path().elementAt(i).x) + ";";
        str += QString::number(path().elementAt(i).y);
    }

    setValue("path", str);
    setValue("color", pen().color().rgb()); // is uint
    setValue("style", (int)pen().style());
    setValue("width", pen().widthF());
}

/**
 * Synchronize the the QGraphicsPathItem with the object data
 * such as color
 */
void GV_Symbol::synchronizeSceneData() {
    if (!mPort0 /*|| mPort1*/) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Symbol::synchronizeSceneData() ",
                        "port0 not set INFO");
        return;
    }

    GV_Component::synchronizeSceneData();
    createHandles();

    QString str = getValue("path").toString();
    QStringList strL = str.split(";");
    QPainterPath path;
    QString strType;
    double x0, x1, x2;
    double y0, y1, y2;
    int counter = 0; // to count extra elements for curveToElement()

    for (int i = 0; i < strL.size()-2; ++i) {
        strType = strL.at(i);

        if (strType == "M") {
            x0 = strL.at(++i).toDouble();
            y0 = strL.at(++i).toDouble();
            path.moveTo(x0, y0);
        } else if (strType == "L") {
            x0 = strL.at(++i).toDouble();
            y0 = strL.at(++i).toDouble();
            path.lineTo(x0, y0);
        } else if (strType == "C") {
            x0 = strL.at(++i).toDouble();
            y0 = strL.at(++i).toDouble();
            counter = 1;
        } else if (strType == "X") {
            if (counter == 1) {
                x1 = strL.at(++i).toDouble();
                y1 = strL.at(++i).toDouble();
                ++counter;
            } else if (counter == 2) {
                x2 = strL.at(++i).toDouble();
                y2 = strL.at(++i).toDouble();
                path.cubicTo(x0, y0, x1, y1, x2, y2);
            }
        }
    }

    setPath(path);
    QPen p = pen();
    p.setColor(QColor(getValue("color").toUInt()));
    p.setStyle((Qt::PenStyle)getValue("style").toInt());
    p.setWidthF(getValue("width").toDouble());
    setPen(p);
    setHandlePosition();
}

/**
 * Draw control point, origine and rotation handle
 */
void GV_Symbol::drawNodePorts() {
    if (!(QGraphicsItem::flags() & QGraphicsItem::ItemIsSelectable)) return;

    GV_Node* node = NULL;
    GV_Port* port = NULL;
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");

    // show ports if item selected or rotation lever is selected
    if (isSelected() || mRotLever->isSelected() || isBeingHovered()
        || mResHandleLT->isSelected() || mResHandleLB->isSelected()
        || mResHandleRB->isSelected() || mResHandleRT->isSelected()) {

        if (!isBeingHovered() && !getDrawing()->isSelectionOnly()) {
            // rotation lever is not drawn if item is being hovered by the mouse
            // or for selection only
            mRotLever->setVisible(true);
            mResHandleLT->setVisible(true);
            mResHandleLB->setVisible(true);
            mResHandleRB->setVisible(true);
            mResHandleRT->setVisible(true);
        }

        mPort0->setVisible(true);
        node = mPort0->connectNode();
        if (node) node->setVisible(true);

        RB_ObjectIterator* iter = portList->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            port = (GV_Port*)iter->currentObject();
            port->setVisible(true);
            node = port->connectNode();
            if (node) node->setVisible(true);
        }
        delete iter;
    } else {
        mRotLever->setVisible(false);
        mResHandleLT->setVisible(false);
        mResHandleLB->setVisible(false);
        mResHandleRB->setVisible(false);
        mResHandleRT->setVisible(false);

        mPort0->setVisible(false);
        node = mPort0->connectNode();
        if (node) node->setVisible(false);

        RB_ObjectIterator* iter = portList->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            port = (GV_Port*)iter->currentObject();
            port->setVisible(false);
            node = port->connectNode();
            if (node) node->setVisible(false);
        }
        delete iter;
    }
}

/**
 * Get foreground color
 * @return color
 */
QColor GV_Symbol::getForegroundColor() const {
    QPen pen = this->pen();
    return pen.color();
}

/**
 * Set color
 * @param color
 */
void GV_Symbol::setForegroundColor(const QColor& color) {
    QPen pen = this->pen();
    pen.setColor(color);
    this->setPen(pen);
}

/**
 * Overriden function QGraphicsPathItem::setPath() to set rotationlever
 * and scalehandles
 * @param path full path to create symbol
 */
void GV_Symbol::setPath(const QPainterPath& path) {
    QGraphicsPathItem::setPath(path);
    setHandlePosition();
}

/**
 * Add port to graphicsview item. The ports are actualy
 * QGraphicsRectItem's and separate classes as in GL.
 * NOTE: the first port added ALLWAYS has to be the origine port
 * AND at x = 0.0 and y = 0.0!
 * @param port port data of model, not of scene!
 */
GV_Port* GV_Symbol::addPort(const GV_Port& port) {
    RB_DEBUG->print("GV_Symbol::addPort()");
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");

    if (!portList) {
        RB_DEBUG->printObject(this);
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Symbol::addPort() ERROR");
        return NULL;
    }

    int x = (int)port.location().x;
    int y = (int)port.location().y;

    RB_ObjectBase* obj = getFactory()->newObject("", portList);
    GV_Port* newPort = dynamic_cast<GV_Port*>(obj);
    newPort->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
    newPort->setPos(x, y);                  // scene location
    newPort->setLocation(port.location());	// model location
    newPort->setPen(mDetailPen);
    newPort->setParentItem(this);
    newPort->setVisible(false);

    // set standard start and end point of default flow, done by createStartPort
    if (!mPort0) {
        // mPort0 done by createStartPort()
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Symbol::addPort() "
                "mPort0 does not exist ERROR");
        return NULL;
    } else if (!mPort1) {
        mPort1 = newPort;
    }

    RB_DEBUG->print(" newPort x,y = %f,%f", newPort->pos().x(), newPort->pos().y());
    return newPort;
}

/**
 * Set port object, the port is already created and part of the GV_PortList. The
 * port will be set to mPort1 (GV_Symbol only) and drawn as a rectangle.
 * mPort0 is not part of the portlist. The main flow Port1 has always to be
 * done first. This function is used by GV_Drawing
 */
void GV_Symbol::setPort(GV_Port* portItem) {
    portItem->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
//	portItem->moveBy(x, y);
    portItem->setPen(mDetailPen);
    dynamic_cast<QGraphicsItem*>(portItem)->setFlag(QGraphicsItem::ItemIsMovable, true);
    dynamic_cast<QGraphicsItem*>(portItem)->setFlag(QGraphicsItem::ItemIsSelectable, true);

    // set standard start and end point of default flow
    if (!mPort0) {
        // mPort0 done by createStartPort()
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Symbol::setPort() "
                "mPort0 does not exist ERROR");
        return;
    } else if (!mPort1) {
        mPort1 = portItem;
    }

    portItem->setParentItem(this);
}

/**
 * Mouse press click event, set hovering = false
 * @param e mouse event
 */
void GV_Symbol::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    RB_DEBUG->print("GV_Symbol::mousePressEvent()");

    setBeingHovered(false);
    QGraphicsPathItem::mousePressEvent(e);
}

/**
 * Mouse move event for move symbol without snap to grid or other snap
 * @param e mouse move event
 */
void GV_Symbol::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    // TODO hack for: QGraphicsPathItem::mouseMoveEvent(e);
    // Do not use QPointF delta = - e->lastPos() + e->pos();
    // Old hack not required anymore
    // QPointF delta = - mapToScene(e->lastPos()) + mapToScene(e->pos());
    // moveBy(delta.x(), delta.y());
    QGraphicsPathItem::mouseMoveEvent(e);
    updatePosition();
}

/**
 * Mouse release event to make a connection between to items, by checking first
 * whether this item free port is over another item free port.
 */
//void GV_Symbol::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
//    // RB_DEBUG->print("GV_Symbol::mouseReleaseEvent()");
//
//	setBeingHovered(false);
//	bool success = connectFreePort(mPort0);
//
//	RB_ObjectContainer* portList = getContainer("GV_PortList");
//	RB_ObjectIterator* iter = portList->createIterator();
//
//	for (iter->first(); !iter->isDone() && !success; iter->next()) {
//		GV_Port* itemPort = (GV_Port*)iter->currentObject();
//
//		// in GV_Component
//		success = connectFreePort(itemPort);
//	}
//
//	delete iter;
//
//    QGraphicsPathItem::mouseReleaseEvent(e);
//}

/**
 * Show ports and node when mouse enters item
 * @param e graphics scene hover event
 */
void GV_Symbol::hoverEnterEvent(QGraphicsSceneHoverEvent* e) {
    setBeingHovered(true);
    drawNodePorts();
    QGraphicsPathItem::hoverEnterEvent(e);
}

/**
 * Show ports and node when mouse hovers over item
 * @param e graphics scene hover event
 */
void GV_Symbol::hoverMoveEvent(QGraphicsSceneHoverEvent* e) {
    setBeingHovered(true);
    drawNodePorts();
    QGraphicsPathItem::hoverMoveEvent(e);
}

/**
 * Hide ports and node when mouse leaves item,
 * unless item is selected.
 * @param e graphics scene hover event
 */
void GV_Symbol::hoverLeaveEvent(QGraphicsSceneHoverEvent* e) {
    setBeingHovered(false);
    drawNodePorts();
    QGraphicsPathItem::hoverLeaveEvent(e);
}

/**
 * TODO: Does not work
 * Enter item when dragging a legend symbol
 * Show ports and node when mouse hovers over and enters item
 * @param e graphics scene hover event
 */
void GV_Symbol::dragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    // RB_DEBUG->print("GV_Symbol::dragEnterEvent()");
    setBeingHovered(true);
    drawNodePorts();
    QGraphicsPathItem::dragEnterEvent(e);
}

/**
 * TODO: Does not work
 * Leave item when dragging a legend symbol
 * Hide ports and node when mouse hovers over and leaves item,
 * unless item is selected.
 * @param e graphics scene hover event
 */
void GV_Symbol::dragLeaveEvent(QGraphicsSceneDragDropEvent* e) {
    // RB_DEBUG->print("GV_Symbol::dragLeaveEvent()");
    setBeingHovered(false);
    drawNodePorts();
    QGraphicsPathItem::dragLeaveEvent(e);
}

/**
 * Overrides QGraphicsItem::itemChange().
 * Item change is called by QGraphicsItem to notify custom items or settings
 * of a change to this item. It is used here to notify ports and node that
 * the selection has changed and that the visibility of the ports and node
 * need to change also.
 */
QVariant GV_Symbol::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        drawNodePorts();
        emit selectedChange(this);
    }
    return value;
}

/**
 * Overrides QGraphicsItem::focusOutEvent().
 * Emits lostFocus() to update user actions
 */
void GV_Symbol::focusOutEvent(QFocusEvent* e) {
    emit lostFocus(this);
    QGraphicsPathItem::focusOutEvent(e);
}

/**
 * Overrides QGraphicsItem::contextMenuEvent().
 * Clears selection, set this item as selected and show context menu
 */
void GV_Symbol::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    scene()->clearSelection();
    setSelected(true);
//    contextMenu->exec(e->screenPos());
}

/**
 * Update position of this item and connecting items
 * @param node origine node position change, if NULL move has been done by
 * mouse move event of this item
 */
//void GV_Symbol::updatePosition(GV_Node* movedNode) {
//	// RB_DEBUG->print("GV_Symbol::updatePosition()");
//
//	// Rotate an item 45 degrees around (x, y). Example:
//	// item->setTransform(QTransform().translate(x, y).rotate(45).translate(-x, -y));
//	if (movedNode == mOrigineNode) {
//		// if parent component changes location
//        setPos(movedNode->scenePos());
//	} else if (movedNode != mOrigineNode && movedNode != NULL) {
//		// if a connecting component changes location and node is not origineNode
//		QPointF nodeLoc = movedNode->scenePos();
//		QPointF portLoc;
//
//		if (movedNode->parentComponent() == this) {
//			portLoc= movedNode->parentPort()->scenePos();
//		} else {
//			portLoc= movedNode->connectPort()->scenePos();
//		}
//
//		moveBy(nodeLoc.x() - portLoc.x(), nodeLoc.y() - portLoc.y());
//	}
//
//	// update all ports and connecting nodes, except of movedNode
//	// which can be the origineNode or NULL
//	GV_Node* node = NULL;
//	GV_Port* port = NULL;
//
//	node = mPort0->connectNode();
//
//	if (node && movedNode != node) {
//		// same as movedNode != mOrigineNode
//		node->updatePosition(this);
//	}
//
//	RB_ObjectContainer* portList = getContainer("GV_PortList");
//	RB_ObjectIterator* iter = portList->createIterator();
//
//	for (iter->first(); !iter->isDone(); iter->next()) {
//		port = (GV_Port*)iter->currentObject();
//		node = port->connectNode();
//
//		if (node && movedNode != node) {
//    		node->updatePosition(this);
//    	}
//
//	}
//
//	delete iter;
//}

/**
 * Update port position based on mouse move event on an individual port. Called
 * by move action or via a node
 * @param port port to be updated
 * @param posAtParent position at parent where symbol port location is to be located
 */
void GV_Symbol::updatePortPosition(GV_Port* port, const QPointF& posAtParent) {
    if (!port) return;
    // Port0 is always at position 0, 0
    QPointF dist = port->scenePos() - mPort0->scenePos();
    this->setPos(posAtParent - dist);
    updatePosition();
}

/**
 * @return scene position of start port (normally port0) which can be port1
 * in case of reversed order
 */
QPointF GV_Symbol::point0() {
    if (isReverseOrder()) {
        if (!mPort1) {
            // cap or plug
            return QPointF();
        }

        return mPort1->scenePos();
    }

    return mPort0->scenePos();
}

/**
 * @return scene position of start port (normally port0) which can be port1
 * in case of reversed order
 */
QPointF GV_Symbol::point1() {
    if (isReverseOrder()) {
        return mPort0->scenePos();
    }

    if (!mPort1) {
        // cap or plug
        return QPointF();
    }

    return mPort1->scenePos();
}
