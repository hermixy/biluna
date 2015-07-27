/*****************************************************************
 * $Id: gv_line.h 1380 2011-03-07 20:47:51Z rutger $
 * Created: Apr 10, 2008 10:07:28 AM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_LINE_H
#define GV_LINE_H

#include <QGraphicsLineItem>
#include <QObject>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include "gv_component.h"
#include "gv_node.h"
#include "gv_port.h"


/**
 * Object class for pipe line, instrument line, pneumatic lines, etc. on
 * flowsheets 
 */
class GV_Line : public QObject, public QGraphicsLineItem, public GV_Component {

    Q_OBJECT

public:
    GV_Line(const RB_String& id, RB_ObjectBase* p,
            const RB_String& n, RB_ObjectFactory* f = NULL,
			QGraphicsItem* parent = NULL);
	GV_Line(GV_Line* line);
	virtual ~GV_Line();
	
	virtual void port(const RB_String& portId, GV_Port& p);
	virtual void port(int portNumber, GV_Port& p);


	// required to distinguish between GV_Path (symbols) and node, port, lever
	enum { Type = UserType + 2 };

    int type() const {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    virtual void init();
    virtual void createPorts();

    virtual GV_Port* addPort(const GV_Port& port);
    virtual GV_Port* addCopyPort0();
    virtual GV_Port* addCopyPort1();
    virtual void setPort(GV_Port* portItem);
    
    virtual void updatePortPosition(GV_Port* port, const QPointF& mousePoint);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    
    virtual QPointF point0();
    virtual QPointF point1();

    // QGraphicsLineItem does not have a path for line decoration
    virtual void setPath(const QPainterPath& path) { mPath = path; }
    virtual QPainterPath path() const;

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

    virtual void dragEnterEvent (QGraphicsSceneDragDropEvent* e);
    virtual void dragLeaveEvent (QGraphicsSceneDragDropEvent* e);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* e);
    // virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* e);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    virtual void mousePressEvent (QGraphicsSceneMouseEvent * e);
    // virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void focusOutEvent(QFocusEvent* e);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *e);

private:
    void updatePortRotation();

    //! Decoration of the line
    QPainterPath mPath;
    //! Context prefix such as GV and RPT
    RB_String mContextPrefix;

};

#endif /*GV_LINE_H*/
