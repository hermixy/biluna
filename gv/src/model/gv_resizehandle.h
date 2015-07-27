/*****************************************************************
 * $Id: gv_resizehandle.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: May 3, 2010 9:01:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_RESIZEHANDLE_H
#define GV_RESIZEHANDLE_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

class GV_Component;
class GV_CoordinateEvent;
class GV_Symbol;
class GV_Text;

/**
 * Lever to rotate a graphics item
 */
class GV_ResizeHandle : public QGraphicsPolygonItem {

public:
    GV_ResizeHandle(QGraphicsItem* parent);
    GV_ResizeHandle(GV_ResizeHandle* handle);
    virtual ~GV_ResizeHandle();

    void draw();

    QPointF getMousePoint() { return mMousePoint; }
    QPointF getDeltaPos() { return mDeltaPos;}

    virtual void updatePosition(GV_CoordinateEvent* e);
    virtual GV_Symbol* getParentSymbol();
    virtual GV_Text* getParentText();

    virtual void setDistanceToCorner(QGraphicsSceneMouseEvent* e);
    virtual QPointF getRelativeCornerPos(QGraphicsSceneMouseEvent* e);

protected:
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

private:
    /** Mouse position in item coordinates */
    QPointF mMousePoint;
    /** Delta position in item coordinates */
    QPointF mDeltaPos;

};

#endif // GV_RESIZEHANDLE_H
