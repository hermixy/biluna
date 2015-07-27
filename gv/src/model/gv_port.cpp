/*****************************************************************
 * $Id: gv_port.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Apr 4, 2008 2:10:11 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_port.h"

#include "gv_drawing.h"
#include "gv_line.h"
#include "gv_node.h"
#include "gv_symbol.h"
#include "gv_text.h"

GV_Port::GV_Port(const RB_String& id, RB_ObjectBase* p,
        const RB_String& n, RB_ObjectFactory* f,
        QGraphicsItem* parent)
        : QGraphicsRectItem(parent), RB_Port(id, p, n, f) {

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setZValue(1000.0);
    mConnectNode = NULL;

    createMembers();

    if (n.contains("_")) {
        // For example GV_Symbol or RPT_Symbol
        RB_StringList strL = n.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
        mContextPrefix = strL.at(0);
    }
}

GV_Port::GV_Port(GV_Port* port)
        : QGraphicsRectItem(port->parentItem()), RB_Port(port)  {

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setZValue(1000.0);
    mConnectNode = NULL;

    // For example GV_Symbol or RPT_Symbol
    RB_String n = port->getName();
    RB_StringList strL = n.split("_", QString::SkipEmptyParts, Qt::CaseInsensitive);
    mContextPrefix = strL.at(0);
}

GV_Port::~GV_Port() {
    setParentItem(NULL);
}

/**
 * Set connecting node to this port. If NULL no node or item is connected
 * to this port
 * @param node connecting node
 */
void GV_Port::setConnectNode(GV_Node* node) {
    mConnectNode = node;
}

/**
 * Mouse move event, ignored here
 * @param e QGraphicsSceneMouseEvent
 */
void GV_Port::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    // QGraphicsRectItem::mouseMoveEvent(e);
    e->ignore();
}

/**
 * Mouse presse event will select an item if applicable and set the
 * relevant pipe line or equipment
 * @param e mouse scene event
 */
void GV_Port::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    RB_DEBUG->print("GV_Port::mousePressEvent()");

    if (parentItem()) {
        scene()->clearSelection(); // Hack: to deselect other items
        parentItem()->setSelected(true);
        e->accept();
    } else {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                  "GV_Port::mousePressEvent() no parentItem ERROR");
        e->ignore();
    }
}

/**
 * Mouse release event to make a connection between to items, by checking first
 * whether this item free port is over another item free port.
 */
void GV_Port::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    GV_Component* comp = getParentComponent();
    comp->setBeingHovered(false);
    if (comp->isConnectable()) {
        connect();
    }
    e->accept();
}

/**
 * Hoverenter event, implementation ignores event to be passed to parentitem
 */
void GV_Port::hoverEnterEvent(QGraphicsSceneHoverEvent* e) {
    GV_Component* comp = this->getParentComponent();
    comp->setBeingHovered(true);
    comp->drawNodePorts();
    QGraphicsRectItem::hoverEnterEvent(e);
}

/**
 * Hoverleave event, implementation ignores event to be passed to parentitem
 */
void GV_Port::hoverLeaveEvent(QGraphicsSceneHoverEvent* e) {
    GV_Component* comp = this->getParentComponent();
    comp->setBeingHovered(false);
    comp->drawNodePorts();
    QGraphicsRectItem::hoverLeaveEvent(e);
}


/**
 * Connect this port to the first free colliding port
 * in case of valid connection
 */
void GV_Port::connect() {
    if (!this->isFree()) return;
    QList<QGraphicsItem*> itemList = collidingItems(Qt::IntersectsItemShape);
    if (itemList.size() <= 0)  return;
    GV_Port* freePort = NULL;

    for (int i = 0; i < itemList.size() && !freePort; ++i) {
        GV_Port* colPort = dynamic_cast<GV_Port*>(itemList.at(i));

        if (colPort && colPort->parentItem() != this->parentItem()
                && colPort->isFree() && this->isValidConnection(colPort)) {
           freePort = colPort;
        }
    }

    if (!freePort) return;

    if (!freePort->connectNode() && !this->connectNode()) {
        // Create interconnecting node
        GV_Drawing* drawing = this->getParentComponent()->getDrawing();
        RB_ObjectContainer* nodeList = drawing->getContainer(mContextPrefix + "_NodeList");
        RB_ObjectFactory* objFactory = this->getFactory();
        GV_Node* interNode = dynamic_cast<GV_Node*>(objFactory->newObject("", nodeList, "", true));
        drawing->addItem(interNode);
        interNode->connect(this, freePort);
        interNode->setInterconnecting(true);
    } else if (freePort->connectNode() && !this->connectNode()) {
        // Connect to freePort node
        GV_Node* node = freePort->connectNode();
        node->connect(freePort, this);
    } else if (!freePort->connectNode() && this->connectNode()) {
        // Connect to this node
        GV_Node* node = this->connectNode();
        node->connect(this, freePort);
    } else if (freePort->connectNode() && this->connectNode()) {
        // Remove this (origine) node and connect to freePort node
        GV_Component* comp = this->getParentComponent();
        GV_Drawing* drawing = comp->getDrawing();
        GV_Node* node = this->connectNode();
        node->disconnect(this);
        drawing->removeItem(node);
        if (node->getFlags() & RB2::FlagFromDatabase) {
            dynamic_cast<RB_Node*>(node)->setFlag(RB2::FlagIsDeleted);
        } else {
            RB_ObjectBase* objC = node->getParent();
            objC->remove(node, true);
        }
        // Connect to freePort node
        GV_Node* freeNode = freePort->connectNode();
        freeNode->connect(freePort, this);
        // Set as origine node also from this parentComponent
        comp->setOrigineNode(freeNode);
    }
}

/**
 * Disconnect port from connecting node and node will be deleted if
 * it is an interconnecting node.
 */
void GV_Port::disconnect() {
    RB_DEBUG->print("GV_Port::disconnect()");

    GV_Node* node = connectNode();

    if (node) {
        // if interconnecting node also other port will be disconnected
        node->disconnect(this);
    }

    if (node && node->isInterconnecting()) {
        GV_Component* comp = this->getParentComponent();
        GV_Drawing* drawing = comp->getDrawing();
        drawing->removeItem(node);
        if (node->getFlags() & RB2::FlagFromDatabase) {
            dynamic_cast<RB_Node*>(node)->setFlag(RB2::FlagIsDeleted);
        } else {
            RB_ObjectBase* objC = node->getParent();
            objC->remove(node, true);
        }
    }
}

/**
 * @returns true in case port has no connecting node or connecting node has
 * a free port position
 */
bool GV_Port::isFree() {
    if (!mConnectNode) return true;
    if (mConnectNode->freeConnectors() > 0) return true;
    return false;
}

/**
 * Is this a valid connection with colliding port. To be overriden by actual
 * ports depending the parent component type, for example an instrumentation
 * line can not be connected with a pipe line
 * @param colPort colliding port
 * @return true on valid connection, default returns true
 */
bool GV_Port::isValidConnection(GV_Port* /* colPort */) {
    return true;
}

/**
 * Update position of port, called by move action, port can snap to grid
 * @param ce coordinate event, coordinates at port parentItem's parent
 */
void GV_Port::updatePosition(GV_CoordinateEvent* ce) {
    RB_DEBUG->print("GV_Port::updatePosition()");

    if (!parentItem()) return;
    QPointF posAtParent = ce->pos();

    // TODO: can this be more generic without
    if (parentItem()->type() == UserType + 2) {
        // parent item is GV_Line
        GV_Line* line = dynamic_cast<GV_Line*>(parentItem());
        line->updatePortPosition(this, posAtParent);


//        if (this->getValue("basePort").toString() == "") {
//            line->updatePortPosition(this, posAtParent);
//        } else if (this->getValue("basePort").toString() == "Port0") {
//            // port is copy of Port0
//            line->updatePortPosition(line->port0(), posAtParent);
//        } else if (this->getValue("basePort").toString() == "Port1") {
//            // port is copy of Port1
//            line->updatePortPosition(line->port1(), posAtParent);
//        }



    // TODO: Single casting to GV_Component not possible yet
    } else if (parentItem()->type() == UserType + 1) {
        // parent item is GV_Symbol
        GV_Symbol* symbol = (GV_Symbol*)parentItem();
        symbol->updatePortPosition(this, posAtParent);
    } else if (parentItem()->type() == UserType + 5) {
        // parent item is GV_Symbol
        GV_Text* text = dynamic_cast<GV_Text*>(parentItem());
        text->updatePortPosition(this, posAtParent);
    }
}

/**
 * Reimplemented set position to update connecting nodes. This function is
 * called by GV_Symbol::scale().
 * @param comp Component being scaled TODO: not used
 * @param x X-coordinate on parent
 * @param y Y-coordinate on parent
 */
void GV_Port::setPosition(GV_Component* /*comp*/, double x, double y) {
    QGraphicsRectItem::setPos(x, y);
    GV_Node* node = connectNode();
    if (node) {
        node->updatePosition(this);
    }
}

/**
 * Get parent component. This method is used to bypass the GV_PortList which
 * is not available for Port0 and sometimes Port1 (for lines). For example
 * in GV_Node this function is used to quickly access the parent component.
 */
GV_Component* GV_Port::getParentComponent() {
    return dynamic_cast<GV_Component*>(parentItem());
}

/**
 * During the editing of the Qt items of the scene and graphics view the data is
 * not transfered to the Biluna objects. Before saving the data is synchronized
 * with this method.
 */
void GV_Port::synchronizeObjectData() {
    RB_DEBUG->print("GV_Port::synchronizeObjectData()");
    if ((dynamic_cast<RB_Port*>(this))->getFlags() & RB2::FlagIsDeleted) return;

    this->setValue("locX", this->pos().x());
    this->setValue("locY",this->pos().y());
}

/**
 * Set the port position on the scene to the port model data
 */
void GV_Port::synchronizeSceneData() {
    RB_DEBUG->print("GV_Component::synchronizeSceneData()");
    this->setPos(getValue("locX").toDouble(),
                 getValue("locY").toDouble());
}

/**
 * Create the members of this class
 */
void GV_Port::createMembers() {
    // line can have more ports at start and end port, these ports remain
    // at the location of their parentPort when the line is moved or changed
    // this field has Port0 or Port1 as possibilities
    addMember(this, "basePort", "-", "", RB2::MemberChar40);	// 6
}
