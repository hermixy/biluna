/*****************************************************************
 * $Id: gv_text.h 1450 2011-08-29 17:40:53Z rutger $
 * Created: Mar 7, 2008 5:37:57 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_TEXT_H
#define GV_TEXT_H

#include <QGraphicsTextItem>
#include <QPen>

#include "gv_component.h"
#include "gv_resizehandle.h"
#include "gv_rotationlever.h"
// #include "gv.h"
#include "rb_string.h"

class GV_Node;
class GV_Port;

class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;

class GV_Text : public QGraphicsTextItem, public GV_Component {

    Q_OBJECT

public:
//    GV_Text(QMenu* textMenu, QGraphicsItem* parent = 0, QGraphicsScene* scene = 0);
    GV_Text(const RB_String& id, RB_ObjectBase* p,
            const RB_String& n, RB_ObjectFactory* f = NULL,
            QGraphicsItem* parent = NULL);
    GV_Text(GV_Text* text);
    virtual ~GV_Text();

    enum { Type = UserType + 5 };
    int type() const { return Type; }

    virtual void init();
    virtual void createPorts();

    virtual GV_Port* addPort(const GV_Port& port);
    virtual void setPort(GV_Port* portItem);

    virtual void setHtml(const QString& str);
    virtual void updatePortPosition(GV_Port* port, const QPointF& mousePoint);

    virtual QRectF boundingRect() const;
    virtual void setPos(const QPointF& p);
    virtual void scale(GV_ResizeHandle* resHandle, const QPointF& delta);
    virtual void setDistanceToCorner(GV_ResizeHandle* resHandle,
                                     QGraphicsSceneMouseEvent* e);
    virtual QPointF getRelativeCornerPos(QGraphicsSceneMouseEvent* e);
    virtual void updateCornerPosition(GV_ResizeHandle* resHandle,
                                      const QPointF& scenePos);

    virtual void synchronizeObjectData();
    virtual void synchronizeSceneData();

    virtual void drawNodePorts();
    virtual QColor getForegroundColor() const;
    virtual void setForegroundColor(const QColor& color);

signals:
    void lostFocus(QGraphicsItem* item);
    void selectedChange(QGraphicsItem* item);

protected:
    void createMembers();

    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* e);
    void hoverMoveEvent(QGraphicsSceneHoverEvent* e);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    void focusOutEvent(QFocusEvent* e);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);

    /** The graphicsscene unit corresponds with 1 mm,
        this correction factor is to correct the font size accordingly
        0.3528 ~ 25.4 [mm/inch] / 72 [dpi] */
    static const double FontScale;

private:
    QMenu* contextMenu;

private:
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
    //! Context prefix such as GV and RPT
    RB_String mContextPrefix;
};

#endif
