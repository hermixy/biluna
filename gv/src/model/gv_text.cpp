/*****************************************************************
 * $Id: gv_text.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Mar 7, 2008 5:34:25 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_text.h"

#include <QtWidgets>

#include "gv_drawing.h"
#include "gv_graphicsscene.h"
#include "gv_node.h"
#include "gv_objectfactory.h"
#include "gv_port.h"
#include "rb_debug.h"

const double GV_Text::FontScale = 25.4 / 72;

GV_Text::GV_Text(const RB_String& id, RB_ObjectBase* p,
                    const RB_String& n, RB_ObjectFactory* f,
                    QGraphicsItem* parent)
                    : QGraphicsTextItem(parent), GV_Component(id, p, n, f) {
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
           .arg("GV_Text")
           .arg("This could be subtype information!"));

//    setPen(mPen);
    mPort0 = NULL;
    mPort1 = NULL;
    mRotLever = NULL;
    mGraphicsItem = this;
    mObject = this;

    // For example GV_Symbol or RPT_Symbol
    RB_StringList strL = n.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    mContextPrefix = strL.at(0);

    setScale(FontScale);
}

GV_Text::GV_Text(GV_Text* text) : QGraphicsTextItem(text->parentItem()),
                                    GV_Component(text) {
    // RB_Component
    setStandardPortCount(1);

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setToolTip(QString("%1\n%2")
           .arg("GV_Text")
           .arg("This could be subtype information!"));

//    setPen(mPen);
    mPort0 = NULL;
    mPort1 = NULL;
    mRotLever = NULL;
    mGraphicsItem = this;
    mObject = this;
    setHtml(text->toHtml());

    setScale(FontScale);
}

/**
 * Destructor
 * NOTE: use deleteLater() instead, refer to QObject::~QObject()
 */
GV_Text::~GV_Text() {
    GV_Component::disconnect();
    mPort0 = NULL;
    mPort1 = NULL;

    RB_DEBUG->print("GV_Text::~GV_Text() OK");
}

void GV_Text::createMembers() {
    addMember(this, "html", "-", "", RB2::MemberChar255);	  // html text
    addMember(this, "color", "-", 0, RB2::MemberUInteger);  // default color
    addMember(this, "width", "mm", 0.0, RB2::MemberDouble); // text box width
}

/**
 * Create start port to graphicsview symbol item. The ports are actualy
 * QGraphicsRectItem's and separate classes as in GV.
 * NOTE: the first port added ALLWAYS has to be the origine port
 * AND at x = 0.0 and y = 0.0!
 */
void GV_Text::createPorts() {
    if (!mPort0) {
        RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
        mPort0 = (GV_Port*)getFactory()->newObject("", portList);
        mPort0->setParentItem(this);
        mPort0->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
        mPort0->setPen(mDetailPen);
        mPort0->setVisible(false);
        mPort0->setScale(1/FontScale); // compensate text scale
    }
    mPort1 = NULL;
}

/**
 * Create rotation lever and resize handles
 */
void GV_Text::createHandles() {
    mRotLever = new GV_RotationLever(this);
    mRotLever->draw(); // for possible future parameters
    mRotLever->setPen(mDetailPen);
    mRotLever->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mRotLever->setVisible(false);
    mRotLever->setScale(1/FontScale); // compensate text scale

    // Left top
    mResHandleLT = new GV_ResizeHandle(this);
    mResHandleLT->draw(); // for possible future parameters
    mResHandleLT->setPen(mDetailPen);
    mResHandleLT->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mResHandleLT->setVisible(false);
    mResHandleLT->setScale(1/FontScale); // compensate text scale

//    // Left bottom
//    mResHandleLB = new GV_ResizeHandle(this);
//    mResHandleLB->draw();
//    mResHandleLB->setPen(mDetailPen);
//    mResHandleLB->setFlag(QGraphicsItem::ItemIsSelectable, true);
//    mResHandleLB->setVisible(false);
//
//    // Right bottom
//    mResHandleRB = new GV_ResizeHandle(this);
//    mResHandleRB->draw();
//    mResHandleRB->setPen(mDetailPen);
//    mResHandleRB->setFlag(QGraphicsItem::ItemIsSelectable, true);
//    mResHandleRB->setVisible(false);

    // Right bottom
    mResHandleRT = new GV_ResizeHandle(this);
    mResHandleRT->draw();
    mResHandleRT->setPen(mDetailPen);
    mResHandleRT->setFlag(QGraphicsItem::ItemIsSelectable, true);
    mResHandleRT->setVisible(false);
    mResHandleRT->setScale(1/FontScale); // compensate text scale

}

/**
 * Set handle postion at creation of symbol and after scaling of the symbol
 */
void GV_Text::setHandlePosition() {
    double margin = 2.0;
    double x = 0.0; // translateX
    double y = 0.0;
    QTransform m;

    x = boundingRect().x() + boundingRect().width() + margin;
    y = boundingRect().y() + boundingRect().height() / 2; // origine is left-top
    m.reset();
    m.translate(x, y);
    mRotLever->setTransform(m);

    x = boundingRect().x() - margin;
    y = boundingRect().y() - margin;
    m.reset();
    m.translate(x, y);
    m.rotate(-135);
    mResHandleLT->setTransform(m);

//    x = boundingRect().x() - margin;
//    y = boundingRect().y() + boundingRect().height() + margin;
//    m.reset();
//    m.translate(x, y);
//    m.rotate(135);
//    mResHandleLB->setTransform(m);
//
//    x = boundingRect().x() + boundingRect().width() + margin;
//    y = boundingRect().y() + boundingRect().height() + margin;
//    m.reset();
//    m.translate(x, y);
//    m.rotate(45);
//    mResHandleRB->setTransform(m);

    x = boundingRect().x() + boundingRect().width() + margin;
    y = boundingRect().y() - margin;
    m.reset();
    m.translate(x, y);
    m.rotate(-45);
    mResHandleRT->setTransform(m);

}

/**
 * Initialization of object for creation of ports, line type etc.
 * Called separately in GV_ObjectFactory because GV_PortList does not
 * exist in constructor
 */
void GV_Text::init() {
    RB_DEBUG->print("GV_Text::init()");
//    createPorts();
    createHandles();
    QGraphicsTextItem::setHtml(tr("Your text ..."));
    setHandlePosition();


    // TODO: set line type and color
}

/**
 * Bounding rectangle of text or dragged rectangle
 * @return QRectF rectangle
 */
QRectF GV_Text::boundingRect() const {
    // x, y, width, height, valve = 20x30
    // return QRectF(-5, -15, 40, 30);
    return QGraphicsTextItem::boundingRect();
}

/**
 * Wrapper for QGraphicsItem::setPos(), also set position of symbol or text
 * and the origine node
 * @param p position of origine
 */
void GV_Text::setPos(const QPointF& p) {
    GV_Component::setPos(p);
//    QRectF rect = QGraphicsTextItem::boundingRect();
}

/**
 * Scale symbol using the resize handles
 * @param resHandle one of the resizehandles
 * @param delta mouse move delta
 */
void GV_Text::scale(GV_ResizeHandle* resHandle, const QPointF& delta) {
    double factorX;
    double factorY;
    double lengthX;
    double lengthY;
    QPointF scaleOrigine;
    QPointF scaleCorner;
    double halfLineThickness = 0.0; // pen().width() / 2;

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
//    } else if (resHandle == mResHandleLB) {
//        // Control points based on bounding rectangle minus half line thickness
//        scaleOrigine = QPointF(boundingRect().x() + boundingRect().width(),
//                               boundingRect().y());
//        scaleOrigine += QPointF(-halfLineThickness, halfLineThickness);
//        scaleCorner = QPointF(boundingRect().x(),
//                              boundingRect().y() + boundingRect().height());
//        scaleCorner += QPointF(halfLineThickness, -halfLineThickness);
//        lengthX = std::abs(scaleOrigine.x() - scaleCorner.x());
//        factorX = (lengthX + delta.x()) / lengthX; // factor at X border
//        lengthY = std::abs(scaleOrigine.y() - scaleCorner.y());
//        factorY = (lengthY - delta.y()) / lengthY; // factor at Y border
//    } else if (resHandle == mResHandleRB) {
//        // Control points based on bounding rectangle minus half line thickness
//        scaleOrigine = QPointF(boundingRect().x(), boundingRect().y());
//        scaleOrigine += QPointF(halfLineThickness, halfLineThickness);
//        scaleCorner = QPointF(boundingRect().x() + boundingRect().width(),
//                              boundingRect().y() + boundingRect().height());
//        scaleCorner += QPointF(-halfLineThickness, -halfLineThickness);
//        lengthX = std::abs(scaleOrigine.x() - scaleCorner.x());
//        factorX = (lengthX - delta.x()) / lengthX; // factor at X border
//        lengthY = std::abs(scaleOrigine.y() - scaleCorner.y());
//        factorY = (lengthY - delta.y()) / lengthY; // factor at Y border
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
    //    QPainterPath p = path();
    //    int count = p.elementCount();
    //
    //    for (int i = 0; i < count; i++) {
    //        double scaledX = p.elementAt(i).x * factorX;
    //        double scaledY = p.elementAt(i).y * factorY;
    //        p.setElementPositionAt(i, scaledX, scaledY);
    //    }
    //
    //    setPath(p);

    // Scale text bounding rectangle length
    setTextWidth(lengthX * factorX);

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
void GV_Text::setDistanceToCorner(GV_ResizeHandle* resHandle,
                                    QGraphicsSceneMouseEvent* e) {
    double halfLineThickness = 0.0; // pen().width() / 2;
    QPointF corner;

    if (resHandle == mResHandleLT) {
        corner = QPointF(boundingRect().x(), boundingRect().y());
        corner += QPointF(halfLineThickness, halfLineThickness);
//    } else if (resHandle == mResHandleLB) {
//        corner = QPointF(boundingRect().x(),
//                         boundingRect().y() + boundingRect().height());
//        corner += QPointF(halfLineThickness, -halfLineThickness);
//    } else if (resHandle == mResHandleRB) {
//        corner = QPointF(boundingRect().x() + boundingRect().width(),
//                         boundingRect().y() + boundingRect().height());
//        corner += QPointF(-halfLineThickness, -halfLineThickness);
    } else if (resHandle == mResHandleRT) {
        corner = QPointF(boundingRect().x() + boundingRect().width(),
                         boundingRect().y());
        corner += QPointF(-halfLineThickness, halfLineThickness);
    } else {
        return;
    }

    mDistance = mapFromScene(e->scenePos()) - corner;
}

QPointF GV_Text::getRelativeCornerPos(QGraphicsSceneMouseEvent* e) {
    QPointF locMousePos = mapFromScene(e->scenePos());
    return mapToScene(locMousePos - mDistance);
}

/**
 * Update the corner position after a resizehandle move action
 * @param resHandle The moved resizehandle
 * @param pos new position for the corner, can be snap position in scene coordinates
 */
void GV_Text::updateCornerPosition(GV_ResizeHandle* resHandle, const QPointF& scenePos) {
    double halfLineThickness = 0.0; // pen().width() / 2;

    // scenePos is mousePos translated with distanceToCorner to the new corner position
    QPointF locMousePos = mapFromScene(scenePos);
    QPointF corner;

    if (resHandle == mResHandleLT) {
        corner = QPointF(boundingRect().x(), boundingRect().y());
        corner += QPointF(halfLineThickness, halfLineThickness);
//    } else if (resHandle == mResHandleLB) {
//        corner = QPointF(boundingRect().x(),
//                         boundingRect().y() + boundingRect().height());
//        corner += QPointF(halfLineThickness, -halfLineThickness);
//    } else if (resHandle == mResHandleRB) {
//        corner = QPointF(boundingRect().x() + boundingRect().width(),
//                         boundingRect().y() + boundingRect().height());
//        corner += QPointF(-halfLineThickness, -halfLineThickness);
    } else if (resHandle == mResHandleRT) {
        corner = QPointF(boundingRect().x() + boundingRect().width(),
                         boundingRect().y());
        corner += QPointF(-halfLineThickness, halfLineThickness);
    } else {
        return;
    }

    scale(resHandle, corner - locMousePos);
}

/**
 * Synchronize the object members with the values of the QGraphicsTextItem
 * such as color
 */
void GV_Text::synchronizeObjectData() {
    if ((dynamic_cast<GV_Component*>(this))->getFlags() & RB2::FlagIsDeleted) return;

    if (!mPort0 || !mPort0->connectNode()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_Text::synchronizeObjectData() port or "
                        "node, orphan? ERROR");
        return;
    }

    GV_Component::synchronizeObjectData();

    QString str = toHtml();

    // From RB_XmlWriter to have proper text in SQL
    // check if ampersand is already used first
    QRegExp rx("&(?!#x)");
    str.replace( rx, "&amp;" );
    // check for the rest
    str.replace( ">", "&gt;" );
    str.replace( "<", "&lt;" );
    str.replace( "\"", "&quot;" );
    str.replace( "\'", "&apos;" );

    setValue("html", str);
    setValue("color", defaultTextColor().rgb());
    setValue("width", textWidth());
}

/**
 * Synchronize the the QGraphicsTextItem with the object data
 * such as color
 */
void GV_Text::synchronizeSceneData() {
    if (!mPort0) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Text::synchronizeSceneData() ",
                        "port0 not set ERROR");
      return;
    }

    GV_Component::synchronizeSceneData();
    createHandles();

    QString str = getValue("html").toString();
    str.replace("&amp;", "&");
    str.replace("&gt;", ">");
    str.replace("&lt;", "<");
    str.replace("&quot;", "\"");
    str.replace("&apos;", "\'");

    setHtml(str);
    QColor color;
    color.setRgb(getValue("color").toUInt());
    setDefaultTextColor(color);
    setTextWidth(getValue("width").toDouble());

    setHandlePosition();

    // Compensate text scale factor
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
    RB_ObjectIterator* iter = portList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
        port->setScale(1/FontScale);
    }
    delete iter;
}

/**
 * Draw control point, origine and rotation handle
 */
void GV_Text::drawNodePorts() {
    if (!(QGraphicsItem::flags() & QGraphicsItem::ItemIsSelectable)) return;

    GV_Node* node = NULL;
    GV_Port* port = NULL;
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");

    // show ports if item selected or rotation lever is selected
    if (isSelected() || mRotLever->isSelected() || isBeingHovered()
        || mResHandleLT->isSelected() /* || mResHandleLB->isSelected()
        || mResHandleRB->isSelected() */ || mResHandleRT->isSelected()) {

        if (!isBeingHovered() && !getDrawing()->isSelectionOnly()) {
            // rotation lever is not drawn if item is being hovered by the mouse
            // or for selection only
            mRotLever->setVisible(true);
            mResHandleLT->setVisible(true);
//            mResHandleLB->setVisible(true);
//            mResHandleRB->setVisible(true);
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
//        mResHandleLB->setVisible(false);
//        mResHandleRB->setVisible(false);
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
QColor GV_Text::getForegroundColor() const {
    if ((QGraphicsTextItem::flags() & Qt::TextEditorInteraction)
                && !textCursor().selection().isEmpty()) {
        return textCursor().charFormat().foreground().color();
    } else {
        return this->defaultTextColor();
    }

    return QColor();
}

/**
 * Set color
 * @param color
 */
void GV_Text::setForegroundColor(const QColor& color) {
    if ((QGraphicsTextItem::flags() & Qt::TextEditorInteraction)
                && !textCursor().selection().isEmpty()) {
        if (color.isValid()) {
            QTextCharFormat fmt = textCursor().charFormat();
            fmt.setForeground(QBrush(color));
            textCursor().setCharFormat(fmt);
        }
    } else {
        this->setDefaultTextColor(color);
    }
}

/**
 * Add port to graphicsview item. The ports are actualy
 * QGraphicsRectItem's and separate classes as in GL.
 * NOTE: the first port added ALLWAYS has to be the origine port
 * AND at x = 0.0 and y = 0.0!
 * @param port port data of model, not of scene!
 */
GV_Port* GV_Text::addPort(const GV_Port& port) {
    RB_DEBUG->print("GV_Text::addPort()");

    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");

    if (!portList) {
        RB_DEBUG->printObject(this);
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Text::addPort() ERROR");
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
    newPort->setScale(1/FontScale); // compensate text scale

    // set standard start and end point of default flow, done by createStartPort
    if (!mPort0) {
        // mPort0 done by createStartPort()
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Text::addPort() "
                "mPort0 does not exist ERROR");
        return NULL;
    } else if (!mPort1) {
        mPort1 = newPort;
    }

    return newPort;
}

/**
 * Set port object, the port is already created and part of the GV_PortList. The
 * port will be set to mPort1 (GV_Text only) and drawn as a rectangle.
 * The main flow Port1 has always to be done first. This function is used by GV_Drawing
 */
void GV_Text::setPort(GV_Port* portItem) {
    portItem->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
//	portItem->moveBy(x, y);
    portItem->setPen(mDetailPen);
    dynamic_cast<QGraphicsItem*>(portItem)->setFlag(QGraphicsItem::ItemIsMovable, true);
    dynamic_cast<QGraphicsItem*>(portItem)->setFlag(QGraphicsItem::ItemIsSelectable, true);
    portItem->setScale(1/FontScale); // compensate text scale

    // set standard start and end point of default flow
    if (!mPort0) {
        // mPort0 done by createStartPort()
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Text::setPort() "
                "mPort0 does not exist ERROR");
        return;
    } else if (!mPort1) {
        mPort1 = portItem;
    }

    portItem->setParentItem(this);
}

/**
 * Overriden function QGraphicsPathItem::setPath() to set rotationlever
 * and scalehandles
 * @param str HTML text
 */
void GV_Text::setHtml(const QString& str) {
    QGraphicsTextItem::setHtml(str);
    QString pStr = QGraphicsTextItem::toPlainText();
    if (pStr.isEmpty()) {
        QGraphicsTextItem::setHtml(tr("Your text ..."));
    }
    setHandlePosition();
}

/**
 * Update position of this item and connecting items
 * @param node origine node position change, if NULL move has been done by
 * mouse move event of this item
 */
//void GV_Text::updatePosition(GV_Node* movedNode) {
//    // RB_DEBUG->print("GV_Text::updatePosition()");
//
//    // Rotate an item 45 degrees around (x, y). Example:
//    // item->setTransform(QTransform().translate(x, y).rotate(45).translate(-x, -y));
//    if (movedNode == mOrigineNode) {
//        // if parent component changes location
//        setPos(movedNode->scenePos());
//    } else if (movedNode != mOrigineNode && movedNode != NULL) {
//        // if a connecting component changes location and node is not origineNode
//        QPointF nodeLoc = movedNode->scenePos();
//        QPointF portLoc;
//
//        if (movedNode->parentComponent() == this) {
//            portLoc= movedNode->parentPort()->scenePos();
//        } else {
//            portLoc= movedNode->connectPort()->scenePos();
//        }
//
//        moveBy(nodeLoc.x() - portLoc.x(), nodeLoc.y() - portLoc.y());
//    }
//
//    // update all ports and connecting nodes, except of movedNode
//    // which can be the origineNode or NULL
//    GV_Node* node = NULL;
//    GV_Port* port = NULL;
//
//    node = mPort0->connectNode();
//
//    if (node && movedNode != node) {
//        // same as movedNode != mOrigineNode
//        node->updatePosition(this);
//    }
//
//    RB_ObjectContainer* portList = getContainer("GV_PortList");
//    RB_ObjectIterator* iter = portList->createIterator();
//
//    for (iter->first(); !iter->isDone(); iter->next()) {
//        port = (GV_Port*)iter->currentObject();
//        node = port->connectNode();
//
//        if (node && movedNode != node) {
//            node->updatePosition(this);
//        }
//
//    }
//
//    delete iter;
//}

/**
 * Update port position based on mouse move event on an individual port. Called
 * by move action
 * @param port port to be updated
 * @param posAtParent position at parent where symbol port location is to be located
 */
void GV_Text::updatePortPosition(GV_Port* port, const QPointF& posAtParent) {
    if (!port) return;
    QPointF dist = port->scenePos() - mPort0->scenePos();
    this->setPos(posAtParent - dist);
    updatePosition();
}

void GV_Text::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    setBeingHovered(false);
    QGraphicsTextItem::mousePressEvent(event);
}

void GV_Text::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (textInteractionFlags() & Qt::TextEditable) {
        QGraphicsTextItem::mouseMoveEvent(event);
    } else {
        QGraphicsItem::mouseMoveEvent(event);
        updatePosition();
    }
}

void GV_Text::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
    if (textInteractionFlags() == Qt::NoTextInteraction) {
        setTextInteractionFlags(Qt::TextEditorInteraction);
    }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

/**
 * Show ports and node when mouse enters item
 * @param e graphics scene hover event
 */
void GV_Text::hoverEnterEvent(QGraphicsSceneHoverEvent* e) {
    setBeingHovered(true);
    drawNodePorts();
    QGraphicsTextItem::hoverEnterEvent(e);
}

/**
 * Show ports and node when mouse hovers over item
 * @param e graphics scene hover event
 */
void GV_Text::hoverMoveEvent(QGraphicsSceneHoverEvent* e) {
    setBeingHovered(true);
    drawNodePorts();
    QGraphicsTextItem::hoverMoveEvent(e);
}

/**
 * Hide ports and node when mouse leaves item,
 * unless item is selected.
 * @param e graphics scene hover event
 */
void GV_Text::hoverLeaveEvent(QGraphicsSceneHoverEvent* e) {
    setBeingHovered(false);
    drawNodePorts();
    QGraphicsTextItem::hoverLeaveEvent(e);
}

void GV_Text::keyReleaseEvent(QKeyEvent* event) {
    if (textInteractionFlags() & Qt::TextEditable) {
        setHandlePosition();
    }
    QGraphicsTextItem::keyReleaseEvent(event);
}

/**
 * Overrides QGraphicsItem::itemChange().
 * Item change is called by QGraphicsItem to notify custom items or settings
 * of a change to this item. It is used here to notify ports and node that
 * the selection has changed and that the visibility of the ports and node
 * need to change also.
 */
QVariant GV_Text::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        drawNodePorts();
        emit selectedChange(this);
    }
    return value;
}

/**
 * Overrides QGraphicsItem::focusOutEvent().
 * Sets interaction flags for text and emits lostFocus() to update user actions
 */
void GV_Text::focusOutEvent(QFocusEvent* e) {
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(e);
}

/**
 * Overrides QGraphicsItem::contextMenuEvent().
 * Clears selection, set this item as selected and show context menu
 */
void GV_Text::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    scene()->clearSelection();
    setSelected(true);
//    contextMenu->exec(e->screenPos());
}

