/*****************************************************************
 * $Id: gv_component.h 1965 2013-08-08 16:33:15Z rutger $
 * Created: Apr 2, 2008 5:34:25 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_COMPONENT_H
#define GV_COMPONENT_H

#include <QtWidgets>
#include "gv_coordinateevent.h"
// #include "gv.h"
#include "rb_component.h"

class GV_Drawing;
class GV_Node;
class GV_Port;
class QGraphicsItem;
class QObject;
class RB_ObjectContainer;


/**
 * Class containing basic dimensional data and ports of a graphics item
 */
class GV_Component : public RB_Component {

public:
    GV_Component(const RB_String& id = "", RB_ObjectBase* p = NULL,
                const RB_String& n = "", RB_ObjectFactory* f = NULL);
    GV_Component(GV_Component* component);
    virtual ~GV_Component();

    int type() const { return -1; }

    /**
     * Initialization of object for creation of ports, line type etc.
     * called by for example GV_ObjectFactory::newObject() and
     * GV_Drawing::synchonizeGraphicsNodeData. Does nothing here.
     */
    virtual void init() { }
    virtual void createPorts() { }

    virtual void setOrigineNode(GV_Node* node);
    virtual bool removeNode(GV_Node* node, bool deleteObj = true);
    virtual GV_Node* origineNode() { return mOrigineNode; }

    virtual GV_Port* addPort(const GV_Port& /* port */) { return NULL; }
    virtual void addPort(RB_ObjectBase* p);
    virtual void setPort(GV_Port* /* port */) { }
    virtual bool connectNode(GV_Node* node, GV_CoordinateEvent* e = NULL);

    virtual GV_Port* getFreePort(const QPointF& point);
//	virtual bool connectFreePorts();
//	virtual bool connectFreePort(GV_Port* itemPort);
    virtual void disconnect();
    virtual bool isConnectable();
    virtual void setConnectable(bool connectable);

    virtual double distanceToPort(GV_Port* portItem, const QPointF& point);

//    virtual void updatePosition(GV_Node* movedNode) = 0;
    virtual void updatePosition();
    virtual void updatePortPosition(GV_Port* port, const QPointF& mousePoint) = 0;

    virtual GV_Port* port0() { return mPort0; }
    virtual void setPort0(GV_Port* port) { mPort0 = port; }
    virtual GV_Port* port1() { return mPort1; }
    virtual void setPort1(GV_Port* port) { mPort1 = port; }

    virtual QPointF point0() { return QPointF(); }
    virtual QPointF point1() { return QPointF(); }

    virtual QPointF scenePos() const;
    virtual void setPos(const QPointF& p);
    virtual QTransform transform() const;
    virtual void setTransform(const QTransform& m);
    virtual void rotate(double angle);

    virtual void synchronizeObjectData();
    virtual void synchronizeSceneData();
    virtual void synchronizeScenePosition(GV_Node* node);

    // virtual GV2::CompType type() { return GV2::CompNone; }
    virtual bool acceptCustomPort();

    virtual void setBeingUpdated(bool beingUpdated);
    virtual bool isBeingUpdated();
    virtual void setReverseOrder(bool reverse);
    virtual bool isReverseOrder();
    virtual void setBeingHovered(bool beingHovered);
    virtual bool isBeingHovered();

    virtual GV_Drawing* getDrawing();
    virtual QGraphicsItem* getGraphicsItem() const;
    virtual QObject* getQObject() const;
//    virtual void addItem(GV_Component* comp);

    virtual RB_ObjectContainer* getLocalNodeList();
    virtual RB_ObjectContainer* getGlobalNodeList();

    virtual void drawNodePorts() = 0;

    virtual QColor getForegroundColor() const = 0;
    virtual void setForegroundColor(const QColor& color) = 0;

protected:
    //! Alternative for mouseMoveEvent()
    virtual void mouseCoordinateEvent(GV_CoordinateEvent* /* event */) { }

//	virtual void beforeDestruction();

    //! Type id of this item
    // RB_String mTypeId; // TODO: required? not used yet
    QGraphicsItem* mGraphicsItem;
    QObject* mObject;

    //! Basic pen for drawing this item
    QPen mPen;
    //! Pen drawing the detail controls for rotation and connection points
    QPen mDetailPen;

    //! Positioning node of this item
    GV_Node* mOrigineNode;

    //! Start point of default flow through symbol or start port of line
    GV_Port* mPort0;
    //! End point of default flow through symbol or end port of line
    GV_Port* mPort1;

    //! Context prefix such as GV and RPT
    RB_String mContextPrefix;

    //! Half port size
    double mPortSize;
    //! Text point size to corresponding unit size [mm]
    double mFontSizeFactor;

private:
    /** Flag indicating that component is being re-positioned,
        to prevent endless loop */
    bool mBeingUpdated;
    //! Flag indicating that initial drawing of line is from Port1 to Port0
    bool mReverseOrder;
    //! Flag indicating that the mouse is hovering over the item
    bool mBeingHovered;
    //! Flag indicating whether component ports can connect to other component ports
    bool mConnectable;

};

#endif /*GV_COMPONENT_H*/
