/*****************************************************************
 * $Id: gv_component.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Apr 2, 2008 5:34:42 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_component.h"

#include <QtWidgets>
#include "gv_drawing.h"
#include "gv_node.h"
#include "gv_objectfactory.h"
#include "gv_port.h"
#include "rb_uuid.h"

/**
 * Base class for all components such as line, symbol (path) and text
 * TODO: move all mPort1 actions to GV_Line if they are only applicable
 * to a line
 */
GV_Component::GV_Component(const RB_String& id, RB_ObjectBase* p,
            const RB_String& n, RB_ObjectFactory* f)
            : RB_Component(id, p, n, f) {

     /* for derived classes such as GV_Line and GV_Symbol
     setStandardPortCount(x);
     createMembers();
     */
    mGraphicsItem = NULL;

    // Basic pen
    mPen.setStyle(Qt::SolidLine);
    mPen.setWidth(0.5);
    mPen.setBrush(Qt::black);
    mPen.setCapStyle(Qt::RoundCap);
    mPen.setJoinStyle(Qt::RoundJoin);

    // Detail pen of ports and rotation lever
    mDetailPen.setStyle(Qt::SolidLine);
    mDetailPen.setWidth(0.0);
    mDetailPen.setBrush(Qt::blue);
    mDetailPen.setCapStyle(Qt::RoundCap);
    mDetailPen.setJoinStyle(Qt::RoundJoin);

    mPortSize = 2;
    mFontSizeFactor = 0.3528; // 1 / 72 * 25.4 mm (1 inch) = 1 pt (point)

    mOrigineNode = NULL;
    mPort0 = NULL;
    mPort1 = NULL;

    mContextPrefix = "";
    mPortSize = 0.0;
    mFontSizeFactor = 0.0;

    setBeingUpdated(false);
    setReverseOrder(false);
    setBeingHovered(false);
    setConnectable(true);
}


GV_Component::GV_Component(GV_Component* component) : RB_Component(component) {
    /* for derived classes such as EQL_Cylinder and EQL_Elbow
    setStandardPortCount(x);
    createMembers();
    *this = *component;
    */
    // mGraphicsItem = component->getGraphicsItem(); set outside

    // Pen of symbol
    mPen.setStyle(Qt::SolidLine);
    mPen.setWidth(0.5);
    mPen.setBrush(Qt::black);
    mPen.setCapStyle(Qt::RoundCap);
    mPen.setJoinStyle(Qt::RoundJoin);

    // Pen of ports and rotation lever
    mDetailPen.setStyle(Qt::SolidLine);
    mDetailPen.setWidth(0.0);
    mDetailPen.setBrush(Qt::blue);
    mDetailPen.setCapStyle(Qt::RoundCap);
    mDetailPen.setJoinStyle(Qt::RoundJoin);

    mOrigineNode = NULL;
    mPort0 = NULL;
    mPort1 = NULL;

    mContextPrefix = "";
    mPortSize = 2.0;
    mFontSizeFactor = 0.0;

    setBeingUpdated(false);
    setReverseOrder(false);
    setBeingHovered(false);
}

GV_Component::~GV_Component() {
    RB_DEBUG->print("GV_Component::~GV_Component()");
    // clean up of children done in RB_ObjectBase and RB_ObjectContainer
    // removing from scene done by implementation of GV_Line and GV_Symbol

    RB_DEBUG->print("GV_Component::~GV_Component() OK");
}

/**
 * Set origine node of component
 * @param node origine node
 */
void GV_Component::setOrigineNode(GV_Node* node) {
    if (mOrigineNode && node) {
        RB_DEBUG->print(RB_Debug::D_INFORMATIONAL,
                "GV_Component::setOrigineNode() "
                "mOrigineNode and node not NULL INFO");
    }

    mOrigineNode = node;
}

/**
 * Connect node to this symbol. The node is added for quick update of connecting
 * component during mouse move events. In GL project the nodes do not have
 * a direct pointer with the parent and connecting component but are only
 * listed in the nodeList. Here in GV it is both in the nodeList and at
 * the components
 * @param node the new origine node from an added component, this component
 * will be the parent component
 * @param ce coordinate event to calculate the closest free port or to
 * create custom port for vessels, etc.
 */
bool GV_Component::connectNode(GV_Node* node, GV_CoordinateEvent* ce) {
    RB_DEBUG->print("GV_Component::addNode()");

    if (!mOrigineNode) {
        // origine node, always added unless connection of this component is
        // to the origine node of another component.
        // NOTE: portItem == mPort0 if GV_Line

        GV_Port* portItem = mPort0;
        portItem->setConnectNode(node);

        if (!node->connectComponent()) {
            // normal origine node of added component
            node->setConnectPort(portItem);
            node->setConnectNodeType(GV2::NodeOrigine);
        } else {
            // node is already a origine node of another component
            node->setParentPort(portItem);
            node->setConnectNodeType(GV2::NodeOrigine);
        }

        setOrigineNode(node);
    } else {
        // add existing node to closest free port at clicked position
        GV_Port* closestFreePort = getFreePort(
                QPointF(ce->getCoordinate().x, ce->getCoordinate().y));

        if (!closestFreePort && mOrigineNode->parentComponent()) {

            RB_DEBUG->print("GV_Component::addNode() line [%i] passed", __LINE__);

            // no free port found and parent component also occupied
            node->setParentNodeType(GV2::NodeNone);
            return false;
        } else if (!closestFreePort && !mOrigineNode->parentComponent()) {
            // no closest free port, but parent port on origine node is free
            // should not occur since getFreePort() checks for free connection on node
            GV_Component* comp = node->connectComponent();
            // remove node from scene first
            getDrawing()->removeItem(node);
            // than remove node and delete node
            comp->removeNode(node);
            // add node, also set..Component set..Port and port->setConnectNode
            comp->connectNode(mOrigineNode);
        } else if (closestFreePort && closestFreePort->connectNode()
                && closestFreePort->connectNode()->freeConnectors() > 0) {
            // exceptional: closest port has already a node and node has a free slot
            // possible orphan node or origine node of this component
            GV_Node* orphanNode = closestFreePort->connectNode();
            // remove closest free port origine node from scene first
            getDrawing()->removeItem(orphanNode);
            // than remove node and delete node
            removeNode(orphanNode);

            // Set node as connecting node in port
            // and set port as parent port to node
            closestFreePort->setConnectNode(node);
            node->setParentPort(closestFreePort);

            // reset origine node and the parentNodeType for this component
            // this happens when the origine node of this component is replaced
            // by an origine node of another component and this component
            // becomes the parent component on the node instead of the connect
            // node
            if (!origineNode()) {
                setOrigineNode(node);
                node->setParentNodeType(GV2::NodeOrigine);
            } else {
                node->setParentNodeType(GV2::NodePort);
            }

            // update node position and connecting Component
        } else {
            // Set node as connecting node in port
            // and set port as parent port to node
            closestFreePort->setConnectNode(node);
            node->setParentPort(closestFreePort);
            node->setParentNodeType(GV2::NodePort);
        }
    }

    return true;
}

/**
 * Remove node from node list
 * @param node node to be removed
 */
bool GV_Component::removeNode(GV_Node* node, bool /* deleteObj */) {
    RB_DEBUG->print("GV_Component::removeNode()");

    if (!node) return false;

    RB_ObjectContainer* parentList = dynamic_cast<RB_ObjectContainer*>(node->getParent());
    GV_Drawing* drawing = getDrawing();

    if (drawing && drawing == (GV_Drawing*)node->scene()) {
        node->disconnect();
        parentList->remove(node, false);
        node->scene()->removeItem(node);
    } else if (drawing) {
        node->disconnect();
        parentList->remove(node, true);
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_Component::removeNode() mDrawing not set ERROR");
        return false;
    }

    return true;
}

/**
 * Add new port to component
 * @param p port
 */
void GV_Component::addPort(RB_ObjectBase* p) {
    RB_String str = getName();
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);

    p->setParent(portList);
    portList->addObject(p);

    if (!mPort0) {
        mPort0 = dynamic_cast<GV_Port*>(p);
    } else if (!mPort1) {
        mPort1 = dynamic_cast<GV_Port*>(p);
    }
}

/**
 * @param point event mouse point
 * @return get closest free port at mouse position
 */
GV_Port* GV_Component::getFreePort(const QPointF& point) {
    RB_DEBUG->print("GV_Component::getFreePort()");

    GV_Port* closestFreePort = NULL;
    double testDistance = 1.0;
    double distance = -1.0;

    // standard and custom ports
    RB_String str = getName();
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* portItem = (GV_Port*)iter->currentObject();
        testDistance = distanceToPort(portItem, point);

        if (!closestFreePort || testDistance < distance || distance < 0.0) {
            distance = testDistance;

            if (portItem->connectNode() == NULL
                    || portItem->connectNode()->freeConnectors() > 0) {
                // free port because no connecting item to this port yet
                closestFreePort = portItem;
            }
        }
    }

    delete iter;
    return closestFreePort;
}

/**
 * Disconnect component ports of nodes and delete interconnecting nodes
 */
void GV_Component::disconnect() {
    RB_String str = getName(); // For example GV_Symbol
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);

    if (!portList) {
        RB_DEBUG->print(RB_Debug::D_ERROR, "GV_Component::disconnect()"
                        " no portList ERROR");
    }
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = (GV_Port*)iter->currentObject();
        port->disconnect();
        // Hack: disconnect from parent item otherwise will be deleted with parent
        port->setParentItem(NULL);
    }

    delete iter;
    setOrigineNode(NULL);
}

/**
 * @returns whether ports of this component will connect to other component ports
 */
bool GV_Component::isConnectable() {
    return mConnectable;
}

/**
 * Set whether ports should connect to other component ports
 * @param connectable true if ports should connect to other component ports
 */
void GV_Component::setConnectable(bool connectable) {
    mConnectable = connectable;
}

/**
 * Distance of point to port
 * @param portItem port to which the distance is calculated
 * @param point point from whicht the distance to port is calculated
 * @return distance
 */
double GV_Component::distanceToPort(GV_Port* portItem, const QPointF& point) {
    RVector portLoc;
    portLoc.z = 0.0;
    RVector mouseLoc;
    QPointF localPoint;

    // GV_Port* portItem = (GV_Port*)iter->currentObject();

    if (mGraphicsItem->type() == mGraphicsItem->UserType + 2) {
        // HACK: 'line' mGraphicsItem->mapFromScene(point) does not work properly
        portLoc.x = portItem->scenePos().x();
        portLoc.y = portItem->scenePos().y();

        localPoint = point;
    } else {
        // port location vector
        portLoc.x = portItem->pos().x();
        portLoc.y = portItem->pos().y();

        // mouse click location vector transfered from scene coordinates
        // to item coordinates because this event has been passed
        // from the scene
        localPoint = mGraphicsItem->mapFromScene(point);
    }

    mouseLoc.x = localPoint.x();
    mouseLoc.y = localPoint.y();

    return portLoc.getDistanceTo(mouseLoc);
}

/**
 * Update all connected node positions
 */
void GV_Component::updatePosition() {
    RB_String str = getName(); // For example GV_Symbol
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());

        if (port) {
            GV_Node* node = port->connectNode();

            if (node) {
                node->updatePosition(port);
            }
        }
    }

    delete iter;
}

/**
 * Wrapper for QGraphicsItem::setPos(), also set position of symbol or text
 * and the origine node
 * @param p position of origine
 */
void GV_Component::setPos(const QPointF& p) {
    mGraphicsItem->setPos(p);
    port0()->connectNode()->updatePosition(port0());
}

/**
 * Wrapper for QGraphicsItem::scenePos(), called from rotationlever.
 */
QPointF GV_Component::scenePos() const {
    return mGraphicsItem->scenePos();
}

/**
 * Wrapper for QGraphicsItem::transform(), called from rotationlever.
 */
QTransform GV_Component::transform() const {
    return mGraphicsItem->transform();
}

/**
 * Wrapper for QGraphicsItem::setTransform() and also handles the rotation
 * for GV_Symbol and GV_Text, called from rotationlever.
 */
void GV_Component::setTransform(const QTransform& m) {
    mGraphicsItem->setTransform(m);

    // Update port positions
    RB_String str = getName();
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);
    RB_ObjectIterator* iter = portList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
        GV_Node* node = port->connectNode();
        if (node) {
//            node->updatePosition(this);
            node->updatePosition(port);
        }
    }
    delete iter;
}

/**
 * Wrapper for QGraphicsItem::rotate() and also handles the roation for
 * GV_Symbol and GV_Text, called from rotationlever.
 */
void GV_Component::rotate(double angle) {
    mGraphicsItem->setRotation(angle);

    // Update port positions
    RB_String str = getName();
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);
    RB_ObjectIterator* iter = portList->createIterator();
    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
        GV_Node* node = port->connectNode();
        if (node) {
            node->updatePosition(port);
        }
    }
    delete iter;
}

/**
 * During the editing of the Qt items of the scene and graphics view the data is
 * not transfered to the Biluna objects. Before saving the data is synchronized
 * with this method, including the standard- and custom port data.
 */
void GV_Component::synchronizeObjectData() {
    RB_DEBUG->print("GV_Component::synchronizeObjectData()");

    if (!mPort0 || !mOrigineNode) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
        "GV_Component::synchronizeObjectData() origineNode or port ERROR");
        return;
    }

    // set custom port location related to parentItem, direction is not changed
    RB_String str = getName();
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = (GV_Port*)iter->currentObject();
        port->setValue("locX", port->pos().x());
        port->setValue("locY", port->pos().y());
    }

    delete iter;
}

/**
 * Set the port position on the scene to the port model data. This function
 * works only one time at initiation of the component. The ports are only edited
 * or updated by for example adding a connection or changing the position
 * after initiation of the component.
 */
void GV_Component::synchronizeSceneData() {
    RB_DEBUG->print("GV_Component::synchronizeSceneData()");

    // set custom port location related to parentItem, direction is not changed
    int portCounter = 0;
    RB_String str = getName();
    RB_StringList strL = str.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    str = strL.at(0) + RB_String("_PortList");
    RB_ObjectContainer* portList = getContainer(str);
    RB_ObjectIterator* iter = portList->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        GV_Port* port = dynamic_cast<GV_Port*>(iter->currentObject());
        if (port->parentItem()) return; // apparently ports have already been initiated

        // The standard ports have not yet been set if item has been read
        // from database or file, TODO: make sure the correct ports from list are used
        if (portCounter == 0) {
            mPort0 = port;
        } else if(portCounter == 1) {
            mPort1 = port;
        }
        // Set parent also adds port to scene
        port->setParentItem(getGraphicsItem());

        // set GV model data in parent coordinates
        port->setPos(port->getValue("locX").toDouble(),
                port->getValue("locY").toDouble());

        // draw port rectangle
        port->setRect(-mPortSize, -mPortSize, 2 * mPortSize, 2 * mPortSize);
        port->setPen(mDetailPen);
        port->setVisible(false);
        ++portCounter;
    }

    delete iter;
}

/**
 * Set absolute scene position and rotation of graphicsitem, used at opening
 * of existing drawing. Refer also to GV_RotationLever::mouseMoveEvent()
 * @param node positioning node with location and rotation
 */
void GV_Component::synchronizeScenePosition(GV_Node* node) {
    RB_DEBUG->print("GV_Component::synchronizeScenePosition()");

    if (!node) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                  "GV_Component::synchronizeScenePosition() node NULL ERROR");
    }

    // location
    double locX = node->getValue("locX").toDouble();
    double locY = node->getValue("locY").toDouble();

    // rotation
    RVector nodeDir(node->getValue("dirX").toDouble(),
            node->getValue("dirY").toDouble());
    double angle = nodeDir.getAngle() * 180.0/M_PI;

    // scaling TODO: calculate the scaling factors
    double xScale = 1.0;
    double yScale = 1.0;

    /* Do not use because the translate changes the origine position on the item
    QMatrix matrix;
    matrix.translate(locX, locY);
    matrix.rotate(angle);
    matrix.scale(xScale, yScale);
    mGraphicsItem->setTransform(QTransform(matrix));
    */

    // Identity matrix update for translate, rotation and scaling
    QMatrix matrix;
    matrix.translate(locX, locY);
    matrix.rotate(angle);
    matrix.scale(xScale, yScale);
    mGraphicsItem->setTransform(QTransform(matrix));

    // set position, rotation and scaling
//    mGraphicsItem->setPos(locX, locY);
//    mGraphicsItem->setRotation(angle);
//    mGraphicsItem->setScale(xScale, yScale);
}

/**
 * @return true if component type accepts custom ports,
 * to be overriden by implementation
 */
bool GV_Component::acceptCustomPort() {
    return false;
}

/**
 * To prevent endless loop during update
 * @param beingUpdated component is being updated or re-positioned
 */
void GV_Component::setBeingUpdated(bool beingUpdated) {
    mBeingUpdated = beingUpdated;
}

/**
 * @return true if component is in the process of being re-positioned
 */
bool GV_Component::isBeingUpdated() {
    return mBeingUpdated;
}

/**
 * Reverse order set to true indicates that the initial drawing of the line
 * by the user will be done from port1 to port0 instead of normally port0 to
 * port1. The port0 will always contain and contect to the positioning/origine
 * node of the component. For example when in exceptional case a line will be
 * started from the origine node/port of a component the starting point of the
 * line will otherwise also contain a node. In this case the line will be drawn
 * from port1 (without node) to port0 (with node).
 * The reverse order flag will also be used to indicate the flow direction.
 * @param reverse reverse order if true
 */
void GV_Component::setReverseOrder(bool reverse) {
    mReverseOrder = reverse;
}

/**
 * @return true if drawing direction or flow direction of component is reversed
 */
bool GV_Component::isReverseOrder() {
    return mReverseOrder;
}

/**
 * Set whether the item is being hovered by the mouse. This allows for
 * example that the ports are being painted without the item being selected
 */
void GV_Component::setBeingHovered(bool beingHovered) {
    mBeingHovered = beingHovered;
}

/**
 * Indicates whether the item is being hovered by the mouse. This allows for
 * example that the ports are being painted without the item being selected
 */
bool GV_Component::isBeingHovered() {
    return mBeingHovered;
}

/**
 * Get drawing (scene) of this component
 */
GV_Drawing* GV_Component::getDrawing() {
    GV_Drawing* drawing = NULL;
    int count = 0;
    RB_ObjectBase* obj = getParent();

    if (obj) {
        while (!drawing && obj && count < 20) {
            obj = obj->getParent();
            if (obj && obj->getName().endsWith("_Drawing", Qt::CaseInsensitive)) {
                drawing = dynamic_cast<GV_Drawing*>(obj);
            }
            ++count;
        }
    }

    return drawing;
}

/**
 * @return Local (first) nodelist of this component
 */
RB_ObjectContainer* GV_Component::getLocalNodeList() {
    RB_ObjectContainer* nodeList = NULL;
    int count = 0;
    RB_ObjectBase* obj = getParent();

    if (obj) {
        while (!nodeList && obj && count < 20) { // first
            obj = obj->getParent();
            if (obj && (obj->getName() == mContextPrefix + "_Symbol"
                        || obj->getName() == mContextPrefix + "_Drawing")) {
                nodeList = obj->getContainer(mContextPrefix + "_NodeList");
            }
            ++count;
        }
    }

    return nodeList;
}

/**
 * @return Global (upper) nodelist of this component
 */
RB_ObjectContainer* GV_Component::getGlobalNodeList() {
    RB_ObjectContainer* nodeList = NULL;
    int count = 0;
    RB_ObjectBase* obj = getParent();

    if (obj) {
        while (obj && count < 20) { // last
            obj = obj->getParent();
            if (obj && (obj->getName() == mContextPrefix + "_Symbol"
                        || obj->getName() == mContextPrefix + "_Drawing")) {
                nodeList = obj->getContainer(mContextPrefix + "_NodeList");
            }
            ++count;
        }
    }

    return nodeList;
}

/**
 * Get QGraphicsitem, hack to bypass multiple inheritance problem
 */
QGraphicsItem* GV_Component::getGraphicsItem() const {
    return mGraphicsItem;
}

/**
 * Get QObject, hack to bypass multiple inheritance problem, still used?
 */
QObject* GV_Component::getQObject() const {
    return mObject;
}

///**
// * Add graphics item to scene or line/symbol/text
// */
//void GV_Component::addItem(GV_Component* comp) {
//    QGraphicsItem* item = comp->getGraphicsItem();
////    switch (type()) {
////    case QGraphicsItem::UserType+2 :
////        (dynamic_cast<QGraphicsLineItem*>(mGraphicsItem))->addItem(item);
////        break;
////    case QGraphicsItem::UserType+1 :
////        (dynamic_cast<QGraphicsPathItem*>(mGraphicsItem))->addItem(item);
////        break;
////    case QGraphicsItem::UserType+5 :
////        (dynamic_cast<QGraphicsTextItem*>(mGraphicsItem))->addItem(item);
////        break;
////    }
//    item->setParentItem()
//}
