/*****************************************************************
 * $Id: gv_port.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Apr 4, 2008 2:05:54 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_PORT_H
#define GV_PORT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include "gv_coordinateevent.h"
#include "rb_port.h"

class GV_Component;
class GV_Node;


/**
 * Class defining the port at a symbol or line. The port information
 * consists position which is location, direction, orientation (not in 2D) and
 * the link (pointer) to a possible connecting item. The pointer is implemented
 * here for quick update of connecting item positions and is not part of GL.
 * The parent of a port is a QGraphicsItem such as GV_Symbol or GV_Line 
 */
class GV_Port : public QGraphicsRectItem, public RB_Port {
	
public:
    GV_Port(const RB_String& id = "", RB_ObjectBase* p = NULL,
            const RB_String& n = "", RB_ObjectFactory* f = NULL,
            QGraphicsItem* parent = NULL);
    GV_Port(GV_Port* port);
    virtual ~GV_Port();

    // required to distinguish between GV_Symbol (symbols) and node, port, lever
    enum { Type = UserType + 3 };

    int type() const {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    virtual GV_Node* connectNode() { return mConnectNode; }
    virtual void setConnectNode(GV_Node* connectNode);

    virtual void connect();
    virtual void disconnect();
    virtual bool isFree();
	
    virtual void updatePosition(GV_CoordinateEvent* e);
    virtual void setPosition(GV_Component* comp, double x, double y);
    virtual GV_Component* getParentComponent();

    virtual void synchronizeObjectData();
    virtual void synchronizeSceneData();

	
protected:
    void createMembers();
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* e);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* e);


    bool isValidConnection(GV_Port* port);

private:
    /** Node connecting to this port */
    GV_Node* mConnectNode;
    //! Context prefix such as GV and RPT
    RB_String mContextPrefix;

};

#endif /*GV_PORT_H*/
