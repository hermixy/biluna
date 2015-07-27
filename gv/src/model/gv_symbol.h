/*****************************************************************
 * $Id: gv_symbol.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 24, 2008 9:38:33 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_SYMBOL_H
#define GV_SYMBOL_H

#include <QGraphicsPathItem>
#include <QObject>
#include <QPen>
#include <list>
#include "gv_component.h"
#include "gv_resizehandle.h"
#include "gv_rotationlever.h"
#include "rb_string.h"

class GV_Node;
class GV_Port;

class QPixmap;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;



/**
 * Class of all object symbols based on QGraphicsPathItem for representation
 * on the QGraphicsView and the RB_Component from the GL project containing the
 * data of the object, the ports to connect to and the position (location and
 * orientation) of the object. The representation is the SVG data which will
 * be rendered by the QGraphicsPathItem as well as handling the move and rotate
 * actions. The RB_Component part will contain the boundary box dimensions,
 * standard port and custom port position (location and rotation).
 */
class GV_Symbol : public QObject, public QGraphicsPathItem, public GV_Component {

    Q_OBJECT

public:
    GV_Symbol(const RB_String& id, RB_ObjectBase* p,
            const RB_String& n, RB_ObjectFactory* f = NULL,
            QGraphicsItem* parent = NULL);
    GV_Symbol(GV_Symbol* symbol);
    virtual ~GV_Symbol();

    virtual void port(const RB_String& portId, GV_Port& p);
    virtual void port(int portNumber, GV_Port& p);

    // required to distinguish between GV_Symbol (symbols) and node, port, lever
    enum { Type = UserType + 1 };
    int type() const { return Type; }

//    virtual GV2::CompType type() { return GV2::CompSymbol; }

    virtual void init();
    virtual void createPorts();

    virtual GV_Port* addPort(const GV_Port& port);
    virtual void setPort(GV_Port* portItem);

    virtual void setPath(const QPainterPath& path);
//    virtual void updatePosition(GV_Node* origineNode = NULL);
    virtual void updatePortPosition(GV_Port* port, const QPointF& mousePoint);

    virtual QRectF boundingRect() const;
    virtual void setPos(const QPointF& p);
    virtual void scale(GV_ResizeHandle* resHandle, const QPointF& delta);
    virtual void setDistanceToCorner(GV_ResizeHandle* resHandle,
                                     QGraphicsSceneMouseEvent* e);
    virtual QPointF getRelativeCornerPos(QGraphicsSceneMouseEvent* e);
    virtual void updateCornerPosition(GV_ResizeHandle* resHandle,
                                      const QPointF& scenePos);

//    virtual void setTransform(const QTransform& m);
//    virtual void rotate(double angle);

    virtual QPointF point0();
    virtual QPointF point1();

    virtual void synchronizeObjectData();
    virtual void synchronizeSceneData();

    virtual void drawNodePorts();
    virtual QColor getForegroundColor() const;
    virtual void setForegroundColor(const QColor& color);

signals:
    void lostFocus(QGraphicsItem* item);
    void selectedChange(QGraphicsItem* item);

protected:
    virtual void createMembers();

    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mousePressEvent (QGraphicsSceneMouseEvent* e);
//	void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* e);
    void hoverMoveEvent(QGraphicsSceneHoverEvent* e);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* e);
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event);

    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void focusOutEvent(QFocusEvent* e);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);

private:
//    void resetPortPosition();
    void createHandles();
    void setHandlePosition();

    //! Rotation handle/lever
    GV_RotationLever* mRotLever;
    //! Resize handle left top
    GV_ResizeHandle* mResHandleLT;
    //! Resize handle left bottom
    GV_ResizeHandle* mResHandleLB;
    //! Resize handle right bottom
    GV_ResizeHandle* mResHandleRB;
    //! Resize handle right top
    GV_ResizeHandle* mResHandleRT;
    //! Current distance of mousepoint on resizehandle to corner
    QPointF mDistance;

};

#endif /*GV_SYMBOL_H*/
