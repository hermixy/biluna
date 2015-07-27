/*****************************************************************
 * $Id: gv_node.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 3, 2008 3:07:22 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_NODE_H
#define GV_NODE_H

#include <QtWidgets>
#include "rb_node.h"
#include <QGraphicsEllipseItem>

class GV_Component;
class GV_Port;
class QGraphicsRectItem;
class QGraphicsScene;


/**
 * Node class which positions and connects components. The node defines the
 * position and the orientation of the connecting component
 */
class GV_Node : public QGraphicsEllipseItem, public RB_Node  {

public:
    GV_Node(const RB_String& id = "", RB_ObjectBase* p = NULL,
            const RB_String& n = "", RB_ObjectFactory* f = NULL,
            QGraphicsItem* parent = NULL);
    GV_Node(GV_Node* node);
    virtual ~GV_Node();

    // required to distinguish between GV_Symbol (symbols) and node, port, lever
    enum { Type = UserType + 4 };

    int type() const {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    virtual GV_Component* parentComponent() { return mParentComponent; }
    virtual GV_Component* connectComponent() { return mConnectComponent; }
    virtual GV_Port* parentPort() { return mParentPort; }
    virtual GV_Port* connectPort() { return mConnectPort; }
//	virtual void setParentComponent(GV_Component* parentComponent);
//	virtual void setConnectComponent(GV_Component* connectComponent);
    virtual void setParentPort(GV_Port* parentPort);
    virtual void setConnectPort(GV_Port* connectPort);

    // new
    virtual void setPort(GV_Port* port);
    virtual void updatePosition(GV_Port* port);


    virtual int freeConnectors();
    virtual GV_Port* otherPort(GV_Port* port);
//    virtual void updatePosition(GV_Component* callingComp);

    virtual void connect(GV_Port* connectPort, GV_Port* parentPort);
    virtual void disconnect(GV_Port* port = NULL);

    virtual bool isInterconnecting();
    virtual void setInterconnecting(bool interconnect);

    virtual void synchronizeObjectData();
    virtual void synchronizeSceneData();
    virtual double getRotation();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);

    //! Half node rectangle size
    double mNodeSize;


private:
    void drawItem();
    void drawState();

    /** parent component to which this node is connecting to */
    GV_Component* mParentComponent;
    /** Connecting component positioned by this node */
    GV_Component* mConnectComponent;
    /** Pen drawing this item */
    QPen mPen;
    /** Port of parent component to which this node is connected */
    GV_Port* mParentPort;
    /** Port of connecting component to which this node is connected */
    GV_Port* mConnectPort;

    /** Flag indicating that this node position is being updated */
    bool isBeingUpdated;
};

#endif /*GV_NODE_H_*/
