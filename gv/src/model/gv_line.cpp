/*****************************************************************
 * $Id: gv_line.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Apr 10, 2008 10:07:15 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_line.h"

#include "gv_objectfactory.h"
#include "gv_graphicsscene.h"

GV_Line::GV_Line(const RB_String& id, RB_ObjectBase* p,
        const RB_String& n, RB_ObjectFactory* f,
        QGraphicsItem* parent)
        : QObject(), QGraphicsLineItem(parent), GV_Component(id, p, n, f) {

    // RB_Component
    setStandardPortCount(2);
    createMembers();

    // Cannot create two standard ports since GV_PortList does not yet exist
    // createStartEndPort();

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setAcceptDrops(true);
//	setCursor(Qt::OpenHandCursor);
    setToolTip(QString("%1\n%2")
           .arg("GV_Line")
           .arg("This could be subtype information!"));

    setPen(mPen);
    // rotLever = NULL;

    mGraphicsItem = this;
    mObject = this;

    // For example GV_Symbol or RPT_Symbol
    RB_StringList strL = n.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    mContextPrefix = strL.at(0);
}

GV_Line::GV_Line(GV_Line* line) : QGraphicsLineItem(line->parentItem()),
                                    GV_Component(line) {
    // RB_Component
    setStandardPortCount(2);
    createMembers();
    *((RB_Component*)this) = *((RB_Component*)line);

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setToolTip(QString("%1\n%2")
           .arg("GV_Line")
           .arg("This could be subtype information!"));

    setPen(mPen);
    mGraphicsItem = this;
    mObject = this;
    setPath(line->path());
}

/**
 * Destructor
 * NOTE: use deleteLater() instead, refer to QObject::~QObject()
 */
GV_Line::~GV_Line() {
    GV_Component::disconnect();
//	delete mPort0; part of portList
//	delete mPort1;
    mPort0 = NULL;
    mPort1 = NULL;

    RB_DEBUG->print("GV_Line::~GV_Line() OK");
}

void GV_Line::createMembers() {
    // RB_DEBUG->print("EQL_Cylinder::createMembers()");
    // fname is used for functional name, such as shell, pipe, tube, barrel, etc.
    //addMember(this, "fname", "-", "Line", RB2::MemberChar40);
    addMember(this, "L0", "mm", 0.0, RB2::MemberDouble);				// 0
    addMember(this, "color", "-", 0, RB2::MemberUInteger);
    addMember(this, "style", "-", 0, RB2::MemberInteger);
    addMember(this, "width", "mm", 0.0, RB2::MemberDouble);

//    mPen.setBrush(pen.brush());
//    mPen.setCapStyle(pen.capStyle());
//    mPen.setColor(pen.color());
//    mPen.setCosmetic(pen.isCosmetic());
//    mPen.setDashOffset(pen.dashOffset());
//    mPen.setDashPattern(pen.dashPattern());
//    mPen.setJoinStyle(pen.joinStyle());
//    mPen.setMiterLimit(pen.miterLimit());
//    mPen.setStyle(pen.style());
//    mPen.setWidthF(pen.widthF());

}

/**
 * Create start and end ports to graphicsview line item. The ports are actualy
 * QGraphicsRectItem's and separate classes as in GV.
 * NOTE: the first port added ALLWAYS has to be the origine port
 * AND at x = 0.0 and y = 0.0!
 */
void GV_Line::createPorts() {
    RB_DEBUG->print("GV_Line::createPorts()");

    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
    if (!mPort0) {
        // standard ports are also part of GV_PortList
        mPort0 = (GV_Port*)getFactory()->newObject("", portList, "", true);
        mPort0->setParentItem(this);
        mPort0->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
        mPort0->setPen(mDetailPen);
        mPort0->setVisible(false);
    }
    if (!mPort1) {
        mPort1 = (GV_Port*)getFactory()->newObject("", portList, "", true);
        mPort1->setParentItem(this);
        mPort1->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
        mPort1->moveBy(20.0, 10.0);
        mPort1->setPen(mDetailPen);
        mPort1->setVisible(false);
    }
}

/**
 * Get port data at id
 * @param portId id of the port
 * @param p to contain port data
 */
void GV_Line::port(const RB_String& portId, GV_Port& p) {
    RB_DEBUG->print("GV_Line::port(RB_String, GV_Port)");

    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
    GV_Port* port = (GV_Port*)portList->getObject(portId);

    if (!port) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_Line::port() port does not exist ERROR");
    } else {
        ((RB_Port)p) = *((RB_Port*)port);
    }
}


/**
 * Get port data at number, standard ports and custom ports if any.
 * @param portNumber number of the port, starting with 0 for the standard ports
 * @param p to contain port data
 */
void GV_Line::port(int portNumber, GV_Port& p) {
    // old not required anymore
//	int stdCount = standardPortCount();
//
//	if (portNumber < stdCount) {
//		switch (portNumber) {
//			case 0:
//				this->port("Port0", p);
//				return;
//			case 1:
//				this->port("Port1", p);
//				return;
//			default:
//				RB_DEBUG->print(RB_Debug::D_ERROR,
//								"GV_Line::port() port number ERROR");
//				break;
//		}
//	} else {
//		// actually RB_Component
//		GV_Component::port(portNumber - stdCount, p);
//	}

    GV_Component::port(portNumber, p);
}


/**
 * Initialization of object for creation of ports, line type etc.
 * Called separately because GV_PortList does not exist in constructor
 */
void GV_Line::init() {
    RB_DEBUG->print("GV_Line::init()");
    //	createPort();
    // TODO: set line type etc.
}

/**
 * Set line based on start (Port0) and end (Port1) port position
 */
//void GV_Line::setLine() {
//	// RB_DEBUG->print("GV_Line::setLine()");
//	if (!mPort0 || !mPort1) {
//		RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Line::setLine()"
//				"no Port0 or Port1 ERROR");
//		return;
//	}
//
//	QLineF tmpL = QLineF(mPort0->scenePos(), mPort1->scenePos());
//	double angleDeg = 0.0;
//
//	if (tmpL.length() > 0.0) {
//		angleDeg = acos(tmpL.dx() / tmpL.length()) * 180.0/M_PI;
//
//		if (tmpL.y1() > tmpL.y2()) {
//			angleDeg *= -1.0;
//		}
//	}
//
//	// horizontal line
//	QPointF point1(tmpL.p1().x(), tmpL.p1().y());
//	QPointF point2(tmpL.length() + tmpL.p1().x(), tmpL.p1().y());
//
//	// revise line to horizontal with an angle
//	QLineF revLine(point1, point2);
//	QGraphicsLineItem::setLine(revLine);
//	resetTransform();
//
//    GV_Component::setTransform(QTransform().translate(point1.x(), point1.y())
//			.rotate(angleDeg).translate(-point1.x(), -point1.y()));
//}


/**
 * Set line and port position
 * @param l line data in scene coordinates
 */
//void GV_Line::setLine(const QLineF& l) {
//	// RB_DEBUG->print("GV_Line::setLine(const QLineF& l)");
//
//	QLineF tmpL;
//	double angleDeg = 0.0;
//
//	if (!isReverseOrder()) {
//		tmpL = l;
//	} else {
//		// tmpL = QLineF(l.p2(), l.p1());
//		// TODO: resolve HACK, why does l.p1() not give the correct y coordinate?
//		tmpL = QLineF(l.p2(), mPort1->connectNode()->scenePos());
//	}
//
//	if (tmpL.length() > 0.0) {
//		angleDeg = acos(tmpL.dx() / tmpL.length()) * 180.0/M_PI;
//
//		if (tmpL.y1() > tmpL.y2()) {
//			angleDeg *= -1.0;
//		}
//	}
//
//	// horizontal line
//	QPointF point1(tmpL.p1().x(), tmpL.p1().y());
//	QPointF point2(tmpL.length() + tmpL.p1().x(), tmpL.p1().y());
//
//	// revise line to horizontal with an angle
//	QLineF revLine(point1, point2);
//	QGraphicsLineItem::setLine(revLine);
//	resetTransform();
//
//    // Do not use below, that function is for rotation of symbol and text:
//    //    GV_Component::setTransform(QTransform().translate(point1.x(), point1.y())
//    //			.rotate(angleDeg).translate(-point1.x(), -point1.y()));
//    QGraphicsLineItem::setTransform(QTransform().translate(point1.x(), point1.y())
//            .rotate(angleDeg).translate(-point1.x(), -point1.y()));
//
//	// set port to start and end point of line because line already rotated
//	if (mPort0 && mPort1) {
//		// QGraphicsScene coordinates
//        mPort0->setPos(line().p1());
//        mPort1->setPos(line().p2());
//        // no need to update node here, is done below
//    }
//
//    // New line and node has to be set at Port0 position
//    if (revLine.length() < 0.000001) {
//        revLine = true;
//    }
//
//    // move Port0, Port1 and custom ports that are copies of Port0 and Port1
//    GV_Port* port = NULL;
//    RB_ObjectContainer* portList = getContainer("GV_PortList");
//    RB_ObjectIterator* iter = portList->createIterator();
//
//    for (iter->first(); !iter->isDone(); iter->next()) {
//        port = dynamic_cast<GV_Port*>(iter->currentObject());
//
//        if (port->getValue("basePort").toString() == "Port0" && revLine) {
//            // port is copy of Port0
//            port->setPos(line().p1());
//        } else if (port->getValue("basePort").toString() == "Port1" && mPort1isMoved) {
//            // port is copy of Port1
//            port->setPos(line().p2());
//        }
//
//        // including nodes at Port0 and Port1
//        if ((port == mPort0 && revLine) || (port == mPort1 && mPort1isMoved)
//                                        || (port != mPort0 && port != mPort1)) {
//            GV_Node* node = port->connectNode();
//            if (node) node->updatePosition(port);
//        }
//    }
//
//    delete iter;
//
//    revLine = false;
//    mPort1isMoved = false;
//}

/**
 * @return bounding rectangle
 */
QRectF GV_Line::boundingRect() const {
    qreal extra = (pen().width() + 6) / 2.0;
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath GV_Line::shape() const {
    QPainterPath path = QGraphicsLineItem::shape();
    path.addEllipse(mPort0->boundingRect());
    path.addEllipse(mPort1->boundingRect());
    return path;
}

/**
 * Synchronize the object members with the values of the QGraphicsLineItem
 * such as line width and color
 */
void GV_Line::synchronizeObjectData() {
    if ((dynamic_cast<GV_Component*>(this))->getFlags() & RB2::FlagIsDeleted) return;

    if (!mPort0 || !mPort0->connectNode() || !mPort1) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_Line::synchronizeObjectData() port or "
                        "node, orphan? ERROR");
        return;
    }

    GV_Component::synchronizeObjectData();

    double x0 = mPort0->pos().x();
    double y0 = mPort0->pos().y();
    double x1 = mPort1->pos().x();
    double y1 = mPort1->pos().y();
    double length = pow((pow((x0-x1), 2) + pow((y0-y1), 2)), 0.5);
    setValue("L0", length);

    setValue("color", pen().color().rgb());
    setValue("style", (int)pen().style());
    setValue("width", pen().widthF());
}

/**
 * Synchronize the the QGraphicsLineItem with the object data
 * such as line width and color
 */
void GV_Line::synchronizeSceneData() {
    if (!mPort0 || !mPort1) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Line::synchronizeSceneData() ",
                        "port0 or port1 not set ERROR");
        return;
    }

    GV_Component::synchronizeSceneData();

    QPen p = pen();
    p.setColor(QColor(getValue("color").toUInt()));
    p.setStyle((Qt::PenStyle)getValue("style").toInt());
    p.setWidthF(getValue("width").toDouble());
    setPen(p);

    updatePortPosition(mPort0, QPointF(mPort0->getValue("locX").toDouble(),
                                       mPort0->getValue("locY").toDouble()));
    updatePortPosition(mPort1, QPointF(mPort1->getValue("locX").toDouble(),
                                       mPort1->getValue("locY").toDouble()));
}

/**
 * Draw control point, origine and rotation handle
 */
void GV_Line::drawNodePorts() {
    if (!(QGraphicsItem::flags() & QGraphicsItem::ItemIsSelectable)) return;

    GV_Node* node = NULL;
    GV_Port* port = NULL;
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");

    // show ports if item selected
    if (isSelected() || isBeingHovered()) {
        mPort0->setVisible(true);
        mPort1->setVisible(true);
        node = mPort0->connectNode();
        if (node) {
            node->setVisible(true);
        }

        node = mPort1->connectNode();

        if (node) {
            node->setVisible(true);
        }

        RB_ObjectIterator* iter = portList->createIterator();
        for (iter->first(); !iter->isDone(); iter->next()) {
            port = (GV_Port*)iter->currentObject();
            port->setVisible(true);
            node = port->connectNode();
            if (node) node->setVisible(true);
        }
        delete iter;
    } else {
        mPort0->setVisible(false);
        mPort1->setVisible(false);
        node = mPort0->connectNode();
        if (node) node->setVisible(false);
        node = mPort1->connectNode();
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
QColor GV_Line::getForegroundColor() const {
    QPen pen = this->pen();
    return pen.color();
}

/**
 * Set foreground color
 * @param color
 */
void GV_Line::setForegroundColor(const QColor& color) {
    QPen pen = this->pen();
    pen.setColor(color);
    this->setPen(pen);
}

/**
 * TODO: change considering addClonePortX() and creating Port0 and Port1 at construction
 * and adding ports for text and other
 *
 * Add additional port to graphicsview line item. The line item has standard
 * already two ports, the start and end point of the line.
 * @param port port data of model, not of scene!
 */
GV_Port* GV_Line::addPort(const GV_Port& port) {
    RB_DEBUG->print("GV_Line::addPort()");

    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");

    if (!portList) {
        RB_DEBUG->printObject(this);
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Line::addPort() ERROR");
        return NULL;
    }

    int x = (int)port.location().x;
    int y = (int)port.location().y;

    GV_Port* newPort = (GV_Port*)getFactory()->newObject("", portList);
    portList->addObject(newPort);

    newPort->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
    newPort->setPos(x, y);                  // scene location
    newPort->setPen(mDetailPen);
    dynamic_cast<QGraphicsItem*>(newPort)->setFlag(QGraphicsItem::ItemIsMovable, true);
    dynamic_cast<QGraphicsItem*>(newPort)->setFlag(QGraphicsItem::ItemIsSelectable, true);
    newPort->setParentItem(this);

    // set standard start and end point of default flow done with createStartEndPort()
    return newPort;
}

/**
 * Add a copy of Port0 to the port list. A line can only have custom ports
 * at the start or end of the line for connecting with other lines.
 * @return added port (cloned Port0)
 */
GV_Port* GV_Line::addCopyPort0() {
    RB_DEBUG->print("GV_Line::addCopyPort0()");

    this->port0()->synchronizeObjectData();

    int x = (int)this->port0()->location().x;
    int y = (int)this->port0()->location().y;

    GV_Port* newPort = dynamic_cast<GV_Port*>(this->port0()->copy());
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
    portList->addObject(newPort);
    newPort->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
    newPort->setPos(x, y);                  // scene location
    newPort->setPen(mDetailPen);
    dynamic_cast<QGraphicsItem*>(newPort)->setFlag(QGraphicsItem::ItemIsMovable, true);
    dynamic_cast<QGraphicsItem*>(newPort)->setFlag(QGraphicsItem::ItemIsSelectable, true);
    newPort->setParentItem(this);

    newPort->synchronizeSceneData();

    // set standard start and end point of default flow done with createStartEndPort()
    return newPort;
}

/**
 * Add a copy of Port0 to the port list. A line can only have custom ports
 * at the start or end of the line for connecting with other lines.
 * @return added port (cloned Port0)
 */
GV_Port* GV_Line::addCopyPort1() {
    RB_DEBUG->print("GV_Line::addCopyPort1()");

    this->port1()->synchronizeObjectData();

    int x = (int)this->port1()->location().x;
    int y = (int)this->port1()->location().y;

    GV_Port* newPort = dynamic_cast<GV_Port*>(this->port1()->copy());
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
    portList->addObject(newPort);
    newPort->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
    newPort->setPos(x, y);                  // scene location
    newPort->setPen(mDetailPen);
    dynamic_cast<QGraphicsItem*>(newPort)->setFlag(QGraphicsItem::ItemIsMovable, true);
    dynamic_cast<QGraphicsItem*>(newPort)->setFlag(QGraphicsItem::ItemIsSelectable, true);
    newPort->setParentItem(this);

    newPort->synchronizeSceneData();

    // set standard start and end point of default flow done with createStartEndPort()
    return newPort;
}

/**
 * Set port object, the port is already created and part of the GV_PortList. The
 * port will be set to mPort0 or mPort1 (GV_Line only) and drawn as a rectangle.
 * mPort0 has always to be done first (and mPort1 second). This function is
 * used by GV_Drawing
 * TODO: change to custom ports only
 */
void GV_Line::setPort(GV_Port* portItem) {
    portItem->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
    portItem->setPen(mDetailPen);
    dynamic_cast<QGraphicsItem*>(portItem)->setFlag(QGraphicsItem::ItemIsMovable, true);
    dynamic_cast<QGraphicsItem*>(portItem)->setFlag(QGraphicsItem::ItemIsSelectable, true);

    // set standard start and end point of default flow
    if (!mPort0) {
        mPort0 = portItem;
        portItem->setParentItem(this);
    } else if (!mPort1) {
        mPort1 = portItem;
        portItem->setParentItem(this);
    }
}

void GV_Line::dragEnterEvent (QGraphicsSceneDragDropEvent* e) {
//    QGraphicsLineItem::dragEnterEvent(e);
    e->accept();
    setBeingHovered(true);
    drawNodePorts();
}

void GV_Line::dragLeaveEvent (QGraphicsSceneDragDropEvent* e) {
//    QGraphicsLineItem::dragLeaveEvent(e);
    e->accept();
    setBeingHovered(false);
    drawNodePorts();
}

/**
 * Show ports and node when mouse enters item
 * @param e graphics scene hover event
 */
void GV_Line::hoverEnterEvent(QGraphicsSceneHoverEvent* e) {
    QGraphicsLineItem::hoverEnterEvent(e); // TODO: no e->accept()? to prevent underlaying object
    setBeingHovered(true);
    drawNodePorts();
}

/**
 * Show ports and node when mouse enters item
 * @param e graphics scene hover event
 */
//void GV_Line::hoverMoveEvent(QGraphicsSceneHoverEvent* e) {
//  QGraphicsLineItem::hoverMoveEvent(e);
//    setBeingHovered(true);
//    drawNodePorts();
//}

/**
 * Hide ports and node when mouse leaves item,
 * unless item is selected.
 * @param e graphics scene hover event
 */
void GV_Line::hoverLeaveEvent(QGraphicsSceneHoverEvent* e) {
    QGraphicsLineItem::hoverLeaveEvent(e); // TODO: no e->accept()? to prevent underlaying object
    setBeingHovered(false);
    drawNodePorts();
}

/**
 * Mouse move event, moves line without snapping to grid. Updates Port0
 * and Port1 position but also the custom ports
 * @param event mouse event containing the last- and current position
 */
void GV_Line::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsLineItem::mouseMoveEvent(event);
    updatePosition();
}

void GV_Line::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    RB_DEBUG->print("GV_Line::mousePressEvent()");
    setBeingHovered(false);
    QGraphicsLineItem::mousePressEvent(event); // TODO: no e->accept()? to prevent underlaying object
}

///**
// * Mouse release event to make a connection between to items, by checking first
// * whether this item free port is over another item free port.
// */
//void GV_Line::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
//	RB_DEBUG->print("GV_Symbol::mouseReleaseEvent()");
//
//	setBeingHovered(false);
//    bool success = false;
//
//    if (!mPort1->connectNode()) {
//        // Port1 does not have a node and is therefore not connected to anything
//        success = connectFreePort(mPort1);
//    }
//
//	if (!success) {
//        if (this->origineNode()->freeConnectors() > 0) {
//            success = connectFreePort(mPort0);
//        }
//	}
//
//    // Note: connect free port is not applicable for custom ports
//	QGraphicsLineItem::mouseReleaseEvent(event);
//}

/**
 * Overrides QGraphicsItem::itemChange().
 * Item change is called by QGraphicsItem to notify custom items or settings
 * of a change to this item. It is used here to notify ports and node that
 * the selection has changed and that the visibility of the ports and node
 * need to change also.
 */
QVariant GV_Line::itemChange(GraphicsItemChange change, const QVariant& value) {
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
void GV_Line::focusOutEvent(QFocusEvent* e) {
    emit lostFocus(this);
    QGraphicsLineItem::focusOutEvent(e);
}

/**
 * Overrides QGraphicsItem::contextMenuEvent().
 * Clears selection, set this item as selected and show context menu
 */
void GV_Line::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    scene()->clearSelection();
    setSelected(true);
//    contextMenu->exec(e->screenPos());
}

QPainterPath GV_Line::path() const {
    QPainterPath path;
//    path.addPath(path());
    path.moveTo(0.0, 0.0);
    path.lineTo(this->line().length(), 0.0);
    // TODO: set type of line with mPath
    return path;
}

/**
 * Update position of this item and connecting items
 * @param node origine node position change, if NULL move has been done by
 * mouse move event of this item
 */
//void GV_Line::updatePosition(GV_Node* movedNode) {
//	// RB_DEBUG->print("GV_Line::updatePosition()");
//
//    if(!movedNode) return;
//
//	// to update boundary area
//	prepareGeometryChange();
//
//    // called by node
//    if (mPort0 == movedNode->connectPort()
//            || mPort0 == movedNode->parentPort()) {
//        updatePortPosition(mPort0, movedNode->scenePos());
//    } else if (mPort1 == movedNode->connectPort()
//            || mPort1 == movedNode->parentPort()) {
//        updatePortPosition(mPort1, movedNode->scenePos());
//    } else {
//        // custom port is connected to node
//        RB_ObjectContainer* portList = getContainer("GV_PortList");
//        RB_ObjectIterator* iter = portList->createIterator();
//        GV_Port* nodePort = NULL;
//
//        for (iter->first(); !iter->isDone() && !nodePort; iter->next()) {
//            GV_Port* portItem = (GV_Port*)iter->currentObject();
//
//            if (portItem == movedNode->connectPort()
//                    || portItem == movedNode->parentPort()) {
//                nodePort = portItem;
//            }
//        }
//
//        delete iter;
//
//        if (nodePort) {
//            if (nodePort->getValue("basePort").toString() == "Port0") {
//                // port is copy of Port0
//                updatePortPosition(mPort0, movedNode->scenePos());
//            } else if (nodePort->getValue("basePort").toString() == "Port1") {
//                // port is copy of Port1
//                updatePortPosition(mPort1, movedNode->scenePos());
//            }
//        }
//    }
//
//}

/**
 * Update port position based on mouse move event on an individual port.
 * @param port port to be updated
 * @param posAtParent position at parent where symbol port location is to be located
 */
void GV_Line::updatePortPosition(GV_Port* port, const QPointF& posAtParent) {
    // RB_DEBUG->print("GV_Line::updatePortPosition()");

    if (!port) return;

    // to update boundary area
    prepareGeometryChange();

    if (port == mPort0 || port->getValue("basePort").toString() == "Port0") {
        // port1 is rotation point and remains at position
        QPointF sceneP2 = mapToParent(line().p2()); // store local position mPort1
        QPointF delta = mapToParent(line().p1()) - posAtParent;
        moveBy(-delta.x(), -delta.y());
        QLineF tmpLine(QPointF(0.0, 0.0), mapFromParent(sceneP2));
        QGraphicsLineItem::setLine(tmpLine);

        mPort0->setPos(QPointF(0.0, 0.0));
        mPort1->setPos(line().p2());
        updatePortRotation();
        updatePosition();
    } else if (port == mPort1 || port->getValue("basePort").toString() == "Port1") {
        // port0 is rotation point and remains at position
        QLineF tmpLine(QPointF(0.0, 0.0), mapFromParent(posAtParent));
        QGraphicsLineItem::setLine(tmpLine);

        mPort0->setPos(QPointF(0.0, 0.0));
        mPort1->setPos(line().p2());
        updatePortRotation();
        updatePosition();
    }

    // for each port if not mPort0 and not mPort1 update position as per basePort
    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
        if (port != mPort0 && port != mPort1
            && port->getValue("basePort").toString() == "Port0") {
            port->setPos(QPointF(0.0, 0.0)); // TODO: required? line is moved for Port0
        } else if (port != mPort0 && port != mPort1
                   && port->getValue("basePort").toString() == "Port1") {
            port->setPos(line().p2());
        }
    }

    delete iter;
}

/**
 * Update port rotation. Port does not automaticaly rotate as with GV_Symbol
 * @param port
 */
void GV_Line::updatePortRotation() {
    QLineF tmpL = line();
    double angleDeg = 0.0;

    if (tmpL.length() > 0.0) {
        angleDeg = acos(tmpL.dx() / tmpL.length()) * 180.0/M_PI;

        if (tmpL.y1() > tmpL.y2()) {
            angleDeg *= -1.0;
        }
    }

    RB_ObjectContainer* portList = getContainer(mContextPrefix + "_PortList");
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
        port->setRotation(angleDeg);
    }

    delete iter;
}

/**
 * @return scene position of start port (normally port0) which can be port1
 * in case of reversed order
 */
QPointF GV_Line::point0() {
    if (isReverseOrder()) {
        return line().p2();
    }

    return line().p1();
}

/**
 * @return scene position of start port (normally port0) which can be port1
 * in case of reversed order
 */
QPointF GV_Line::point1() {
    if (isReverseOrder()) {
        return line().p1();
    }

    return line().p2();
}

