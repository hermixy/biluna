/*****************************************************************
 * $Id: gv_node.cpp 1452 2011-09-10 08:19:50Z rutger $
 * Created: Apr 3, 2008 3:07:13 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_node.h"

#include <QGraphicsScene>
#include "gv_component.h"
#include "gv_port.h"
#include "rb_debug.h"


GV_Node::GV_Node(const RB_String& id, RB_ObjectBase* p,
        const RB_String& n, RB_ObjectFactory* f,
        QGraphicsItem* parent)
        : QGraphicsEllipseItem(parent), RB_Node(id, p, n, f) {

    mParentComponent = NULL;
    mParentPort = NULL;
    mConnectComponent = NULL;
    mConnectPort = NULL;

    // Standard flags, node can not be selected or moved by mouse events
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, false);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, false);
    setZValue(-1000.0);

    mNodeSize = 3;
    isBeingUpdated = false;

    drawItem();
}

GV_Node::GV_Node(GV_Node* node)
        : QGraphicsEllipseItem(node->parentItem()),  RB_Node(node) {

    mParentComponent = node->parentComponent();
    mParentPort = node->parentPort();
    mConnectComponent = node->connectComponent();
    mConnectPort = node->connectPort();

    // Standard flags
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsMovable, true);
    QGraphicsItem::setFlag(QGraphicsItem::ItemIsSelectable, true);
    setZValue(-1000.0);

    mNodeSize = 3;
    isBeingUpdated = false;

    drawItem();
}

GV_Node::~GV_Node() {
    RB_DEBUG->print("GV_Node::~GV_Node()");

    this->disconnect();

    RB_DEBUG->print("GV_Node::~GV_Node() OK");
}

/**
 * Draw node item, red circle
 */
void GV_Node::drawItem() {
    // x, y, width, height
    setRect(QRectF(-mNodeSize, -mNodeSize, 2 * mNodeSize, 2 * mNodeSize));
    // Pen
    mPen.setStyle(Qt::SolidLine);
    mPen.setWidth(0.0);
    mPen.setBrush(Qt::red);
    mPen.setCapStyle(Qt::RoundCap);
    mPen.setJoinStyle(Qt::RoundJoin);
    setPen(mPen);
    drawState();
}

/**
 * Draw node state, white brush is connecting only, yellow means connection to
 * connecting and parent component is OK
 * TODO: check on ID also?
 */
void GV_Node::drawState() {
    if (/*mParentComponent && */mParentPort
                /*&&  mConnectComponent*/ && mConnectPort) {
        setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    } else {
        setBrush(QBrush(Qt::white, Qt::SolidPattern));
    }
}

/**
 * Set parent component
 */
//void GV_Node::setParentComponent(GV_Component* parentComponent) {
//	RB_DEBUG->print("GV_Node::setParentComponent()");
//	mParentComponent = parentComponent;
//
//	if (parentComponent) {
//		setParentComponentId(parentComponent->getId());
//	} else {
//		setParentComponentId("");
//	}
//}

/**
 * Set connecting component, this node is the positioning node for the
 * connecting component.
 * @param connectComponent component connected to this node
 */
//void GV_Node::setConnectComponent(GV_Component* connectComponent) {
//	RB_DEBUG->print("GV_Node::setConnectComponent()");
//	mConnectComponent = connectComponent;
//
//	if (connectComponent) {
//		setConnectComponentId(connectComponent->getId());
//	} else {
//		setConnectComponentId("");
//	}
//}

/**
 * Set parent port and component to which this node is connecting to
 * @param parentPort parent port
 */
void GV_Node::setParentPort(GV_Port* parentPort) {
    RB_DEBUG->print("GV_Node::setParentPort()");
    mParentPort = parentPort;

    if (parentPort) {
        setParentPortId(parentPort->getId());
        mParentComponent = parentPort->getParentComponent();
        setParentComponentId(mParentComponent->getId());
    } else {
        setParentPortId("");
        mParentComponent = NULL;
        setParentComponentId("");
    }
    // only here because each node always has a connectPort
    drawState();
}

/**
 * Set connect port and component to which this node is connected to
 * @param connectPort connecting port
 */
void GV_Node::setConnectPort(GV_Port* connectPort) {
    mConnectPort = connectPort;

    if (connectPort) {
        setConnectPortId(connectPort->getId());
        mConnectComponent = connectPort->getParentComponent();
        setConnectComponentId(mConnectComponent->getId());
    } else {
        setConnectPortId("");
        mConnectComponent = NULL;
        setConnectComponentId("");
    }
}

/**
 * Set port, first port is the origine port of a component. The parentport
 * can be contain the origine port of component. If this is an interconnecting
 * node than both ports are not origine ports
 * @param port
 */
void GV_Node::setPort(GV_Port* port) {
    if (!mConnectPort && port) {
        mConnectPort = port;
        setConnectPortId(port->getId());
        mConnectComponent = port->getParentComponent();
        setConnectComponentId(mConnectComponent->getId());
        port->setConnectNode(this);

        if (!mConnectComponent->origineNode()) {
            mConnectComponent->setOrigineNode(this);
            setConnectNodeType(GV2::NodeOrigine);
        }
    } else if (!mParentPort && port) {
        mParentPort = port;
        setParentPortId(port->getId());
        mParentComponent = port->getParentComponent();
        setParentComponentId(mParentComponent->getId());
        port->setConnectNode(this);

        if (!mParentComponent->origineNode()) {
            mParentComponent->setOrigineNode(this);
            setConnectNodeType(GV2::NodeOrigine);
        }
        // only here because each node always has a connectPort
        drawState();
    }
}

/**
 * Update node position according port position
 * @param port
 */
void GV_Node::updatePosition(GV_Port* port) {
    if (!isBeingUpdated) {
        isBeingUpdated = true;

        // port parent and node have same parentItem
        QPointF posAtParent = port->parentItem()->mapToParent(port->pos());
        setPos(posAtParent);
        GV_CoordinateEvent ce(RVector(posAtParent.x(), posAtParent.y()));
        if (mConnectPort && port == mParentPort) {
            mConnectPort->updatePosition(&ce);
        } else if (mParentPort && port == mConnectPort) {
            mParentPort->updatePosition(&ce);
        }

        isBeingUpdated = false;
    }
}

/**
 * Mouse move event, ignored here
 * @param e QGraphicsSceneMouseEvent
 */
void GV_Node::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    e->accept();
}

/**
 * @return number of free connectors on this node 0, 1 or 2. 2 is an error.
 */
int GV_Node::freeConnectors() {
    int counter = 0;

    if (!mConnectComponent) {
        ++counter;
    }

    if (!mParentComponent) {
        ++counter;
    }

    return counter;
}

/**
 * Give the other of two ports then the parameter port
 * @param port connect or parent port
 * @return other port than parameter port
 */
GV_Port* GV_Node::otherPort(GV_Port* port) {
    if (port == mConnectPort) {
        return mParentPort;
    } else if (port == mParentPort) {
        return mConnectPort;
    }
    return NULL;
}

/**
 * Update position of this node
 * @param callingComp calling component asking for update
 */
//void GV_Node::updatePosition(GV_Component* callingComp) {
//	// RB_DEBUG->print("GV_Node::updatePosition()");
//
//    if (callingComp == mConnectComponent && !isBeingUpdated) {
//		isBeingUpdated = true;
//		// set QGraphicsScene data
//		setPos(mConnectPort->scenePos());
//
//        if (mParentComponent) {
//			// Can be NULL because this is not the origine node. The request
//			// for update comes from a move event
//			mParentComponent->updatePosition(this);
//		}
//
//        // need to be here, otherwise with next call will be set true
//        isBeingUpdated = false;
//    } else if (callingComp == mParentComponent && !isBeingUpdated) {
//		isBeingUpdated = true;
//		setPos(mParentPort->scenePos());
//
//		if (mConnectComponent) {
//			// should never NULL because it is the origine node.
//			mConnectComponent->updatePosition(this);
//		}
//
//        // need to be here, otherwise with next call will be set true
//        isBeingUpdated = false;
//	}
//
//
//}

/**
 * Connect ports and relevant objects to this node
 * @param connectPort origine port of connecting component, can not be NULL
 * @param parentPort standard- or custom port of parent component, can be NULL
 */
void GV_Node::connect(GV_Port* connectPort, GV_Port* parentPort) {
    if (!connectPort) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                "GV_Node::connect() connectPort NULL ERROR");
        return;
    }

    mConnectPort = connectPort;
    setConnectPortId(connectPort->getId());
    GV_Component* conComp = (GV_Component*)connectPort->getParentComponent();
    mConnectComponent = conComp;
    setConnectComponentId(conComp->getId());
    connectPort->setConnectNode(this);

    if (parentPort) {
        mParentPort = parentPort;
        setParentPortId(parentPort->getId());
        GV_Component* parComp = (GV_Component*)parentPort->getParentComponent();
        mParentComponent = parComp;
        setParentComponentId(parComp->getId());
        parentPort->setConnectNode(this);
    }

    drawState();
}

/**
 * Disconnect node from parent- and connect component and related ports
 * @param port port to disconnect from, if NULL diconnect from both ports
 */
void GV_Node::disconnect(GV_Port* port) {
    if (port && port == mParentPort && !isInterconnecting()) {
        mParentPort->setConnectNode(NULL);

        // should not be necessary
        if (mParentComponent && mParentComponent->origineNode() == this)
            mParentComponent->setOrigineNode(NULL);

        mParentComponent = NULL;
        mParentPort = NULL;
        setParentComponentId("");
        setParentPortId("");
    } else if (port && port == mConnectPort && !isInterconnecting()) {
        mConnectPort->setConnectNode(NULL);
        mConnectComponent->setOrigineNode(NULL);
        mConnectComponent = NULL;
        mConnectPort = NULL;
        setConnectComponentId("");
        setConnectPortId("");
    } else {
        if (mParentPort)
            mParentPort->setConnectNode(NULL);

        if (mConnectPort)
            mConnectPort->setConnectNode(NULL);

        if (mConnectComponent && mConnectComponent->origineNode() == this)
            mConnectComponent->setOrigineNode(NULL);

        // should not be necessary
        if (mParentComponent && mParentComponent->origineNode() == this)
            mParentComponent->setOrigineNode(NULL);

        mParentComponent = NULL;
        mConnectComponent = NULL;
        mParentPort = NULL;
        mConnectPort = NULL;
        setParentComponentId("");
        setParentPortId("");
        setConnectComponentId("");
        setConnectPortId("");
    }

    // redraw
    drawItem();
}

/**
 * This node is connecting two components but is not a origine node of
 * either component, for example connecting the end of a line to a nozzle
 */
bool GV_Node::isInterconnecting() {
    if (connectNodeType() == GV2::NodeInterconnecting
            && parentNodeType() == GV2::NodeInterconnecting) return true;
    return false;
}

/**
 * Set this node as an interconnecting node, if true.
 */
void GV_Node::setInterconnecting(bool interconnect) {
    if (interconnect) {
        setConnectNodeType(GV2::NodeInterconnecting);
        setParentNodeType(GV2::NodeInterconnecting);
    } else {
        setConnectNodeType(GV2::NodeNone);
        setParentNodeType(GV2::NodeNone);
    }
}

/**
 * During the editing of the Qt items of the scene and graphics view the data is
 * not transfered to the Biluna objects. Before saving the data is synchronized
 * with this method.
 */
void GV_Node::synchronizeObjectData() {
    RB_DEBUG->print("GV_Node::synchronizeObjectData()");
    if ((dynamic_cast<RB_Node*>(this))->getFlags() & RB2::FlagIsDeleted) return;

    if (!mConnectPort || !mConnectComponent) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "GV_Node::synchronizeObjectData() port or "
                        "component, orphan? ERROR");
        return;
    }

    // location: copy data only of one Port0
    setValue("locX", this->pos().x());
    setValue("locY", this->pos().y());
    // get position (dx, dy) alternative to above
    // double dx = mGraphicsItem->sceneTransform().dx();
    // double dy = mGraphicsItem->sceneTransform().dy();

    // direction: get matrix values (m11, m12, m21, m22)
    double m11 = mConnectComponent->getGraphicsItem()->transform().m11();
    double m12 = mConnectComponent->getGraphicsItem()->transform().m12();
    // double m21 = mConnectComponent->getGraphicsItem()->sceneTransform().m21();
    // double m22 = mConnectComponent->getGraphicsItem()->sceneTransform().m22();

    // direction: calculate absolute angle in scene of parent component,
    // refer to GV_RotationLever
    double angleRad = 0.0;
    if (m11 > 0.0 && m12 > 0.0) {
        angleRad = atan(m12 / m11);
    } else if (m11 < 0.0 && m12 > 0.0) {
        angleRad = -M_PI + atan(m12 / m11);
    } else if (m11 > 0.0 && m12 < 0.0) {
        angleRad = atan(m12 / m11);
    } else if (m11 < 0.0 && m12 < 0.0) {
        angleRad = -M_PI + atan(m12 / m11);
    } else if (m11 == 0.0  && m12 < 0.0) {
        angleRad = -M_PI / 2;
    } else if (m11 == 0.0  && m12 > 0.0) {
        angleRad = M_PI / 2;
    }

    // direction
    setValue("dirX", cos(angleRad));
    setValue("dirY", sin(angleRad));

}

void GV_Node::synchronizeSceneData() {
    setPos(getValue("locX").toDouble(),
           getValue("locY").toDouble());
}

/**
 * Get rotation in xy plane
 * @return rotation in degree
 */
double GV_Node::getRotation() {
    RB_DEBUG->print("GV_Node::getRotation()");

    this->synchronizeObjectData();
    return this->direction().getAngle() * 180 / M_PI;
}
