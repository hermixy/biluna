/*****************************************************************
 * $Id: gv_graphicsscene.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Mar 12, 2008 8:14:47 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_GRAPHICSSCENE_H
#define GV_GRAPHICSSCENE_H

#include <QGraphicsScene>
#include "gv_line.h"
#include "gv_node.h"
#include "gv_symbol.h"
#include "gv_text.h"
#include "gv.h"
#include "RVector.h"

class GV_ActionBase;
class GV_ActionComponent;
class GV_EventHandler;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;

class GV_GraphicsScene : public QGraphicsScene {

    Q_OBJECT

public:
    // enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    GV_GraphicsScene();
    virtual ~GV_GraphicsScene();

    void setContextMenu(QMenu* menu);
//    QFont font() const
//        { return myFont; }
//    QColor textColor() const
//        { return myTextColor; }
//    QColor itemColor() const
//        { return myItemColor; }
//    QColor lineColor() const
//        { return myLineColor; }
//    void setLineColor(const QColor& color);
//    void setTextColor(const QColor& color);
//    void setItemColor(const QColor& color);
//    void setFont(const QFont& font);

//    GV2::ActionType actionType() { return mActionType; }

    virtual void setDefaultAction(GV_ActionBase* action);
    virtual GV_ActionBase*  getDefaultAction();
    virtual void setCurrentAction(GV_ActionBase* action);
    virtual GV_ActionBase* getCurrentAction();

    virtual void killAllActions();

    virtual void back();
    virtual void enter();

    virtual void emulateMouseMoveEvent();
    virtual void emulateMousePressEvent();
    virtual void emulateMouseReleaseEvent();

    virtual void sceneMousePressEvent(QGraphicsSceneMouseEvent* e);
    virtual void sceneMouseMoveEvent(QGraphicsSceneMouseEvent* e);
    virtual void sceneMouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    virtual void sceneKeyPressEvent(QKeyEvent* e);
    virtual void sceneKeyReleaseEvent(QKeyEvent* e);
    virtual void sceneDragEnterEvent(QGraphicsSceneDragDropEvent* e);
    virtual void sceneDragMoveEvent(QGraphicsSceneDragDropEvent* e);
    virtual void sceneDragLeaveEvent(QGraphicsSceneDragDropEvent* e);
    virtual void sceneDropEvent(QGraphicsSceneDragDropEvent* e);

public slots:
//    void editorLostFocus(GV_Text* item);
    void clear();
    void erase();

signals:
//    void itemInserted(QGraphicsItem* item);
//    void textInserted(GV_Text* item);
//    void itemSelected(QGraphicsItem* item); // from mousePressEvent text

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* e);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* e);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* e);
    virtual void dropEvent(QGraphicsSceneDragDropEvent* e);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

    GV_EventHandler* eventHandler;

private:
//    bool isItemChange(int type);
//	GV_Component* mSelectedComponent;



    // actions
//	void actionAddLine(QGraphicsSceneMouseEvent* mouseEvent,
//			QGraphicsItem* itemSelected);
//	void actionAddSymbol(QGraphicsSceneMouseEvent* mouseEvent,
//			QGraphicsItem* itemSelected);

    QMenu* contextMenu;
    GV2::ActionType mActionType;

    /** current action */
    GV_ActionComponent* mAction;

    bool leftButtonDown;

    QPointF startPoint;
    GV_Line* mLine;

    QFont myFont;
    GV_Text* textItem;
    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;




};

#endif // GV_GRAPHICSSCENE_H
