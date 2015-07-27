/*****************************************************************
 * $Id: gv_graphicsscene.cpp 1999 2013-09-26 16:36:13Z rutger $
 * Created: Mar 12, 2008 8:01:18 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "gv_graphicsscene.h"

#include <QtWidgets>
#include <iostream>
#include "gv_actioncomponent.h"
#include "gv_actionbase.h"
#include "gv_coordinateevent.h"
#include "gv_eventhandler.h"
#include "gv_mainwindow.h"
#include "gv_port.h"


GV_GraphicsScene::GV_GraphicsScene() : QGraphicsScene() {
    RB_DEBUG->print("GV_GraphicsScene::GV_GraphicsScene()");

    contextMenu = NULL;

    mAction = NULL; // new GV_ActionComponent(mDrawing, mdiWindow->getGraphicsView());
    mActionType = GV2::ActionSelectItem;

    mLine = NULL;
    textItem = NULL;

    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

    eventHandler = NULL;
}

GV_GraphicsScene::~GV_GraphicsScene() {

    // remove all items from scene, do not delete
    foreach (QGraphicsItem* item, items()) {
        if (item->scene() == this) {
            removeItem(item);
        }
    }

//	if (eventHandler) {
//		delete eventHandler;
//		eventHandler = NULL;
//	}

    RB_DEBUG->print("GV_GraphicsScene::~GV_GraphicsScene() OK");
}


/**
 * Set pointer to context menu for this scene
 * @param menu context menu
 */
void GV_GraphicsScene::setContextMenu(QMenu* menu) {
    contextMenu = menu;
}

/**
 * Clear the items from the scene without deleting
 */
void GV_GraphicsScene::clear() {
    QList<QGraphicsItem*> topLevel;
    QList<QGraphicsItem*> itemList = items();

    int count = itemList.size();

    for (int i = 0; i < count; i++) {
        QGraphicsItem *item = itemList.at(i);
        if (item->parentItem() == NULL) {
            topLevel.append(item);
        }
    }

    count = topLevel.size();

    for (int i = 0; i < count; i++) {
        removeItem(topLevel.at(i));
    }
}

/**
 * Erase is clear and delete the items from the scene
 */
void GV_GraphicsScene::erase() {
    QGraphicsScene::clear();
}

/**
 * @return Current action or NULL.
 */
GV_ActionBase* GV_GraphicsScene::getDefaultAction() {
    if (eventHandler!=NULL) {
        return eventHandler->getDefaultAction();
    } else {
        return NULL;
    }
}

/**
 * Sets the default action of the event handler.
 */
void GV_GraphicsScene::setDefaultAction(GV_ActionBase* action) {
    if (eventHandler!=NULL) {
        eventHandler->setDefaultAction(action);
    }
}

/**
 * @return Current action or NULL.
 */
GV_ActionBase* GV_GraphicsScene::getCurrentAction() {
    if (eventHandler!=NULL) {
        return eventHandler->getCurrentAction();
    } else {
        return NULL;
    }
}

/**
 * Sets the current action of the event handler.
 */
void GV_GraphicsScene::setCurrentAction(GV_ActionBase* action) {
    RB_DEBUG->print("GV_GraphicsScene::setCurrentAction");

    if (eventHandler!=NULL) {
        eventHandler->setCurrentAction(action);
    }

    RB_DEBUG->print("GV_GraphicsScene::setCurrentAction: OK");
}

/**
 * Kills all running actions.
 */
void GV_GraphicsScene::killAllActions() {
    if (eventHandler!=NULL) {
        eventHandler->killAllActions();
    }
}

/**
 * Go back in menu or current action.
 */
void GV_GraphicsScene::back() {
    if (eventHandler!=NULL && eventHandler->hasAction()) {
        eventHandler->back();
    } else {
        if (GV_DIALOGFACTORY!=NULL) {
//            GV_DIALOGFACTORY->requestPreviousMenu();
        }
    }
}

/**
 * Go forward with the current action.
 */
void GV_GraphicsScene::enter() {
    if (eventHandler!=NULL && eventHandler->hasAction()) {
        eventHandler->enter();
    }
}


//void GV_GraphicsScene::setItemType(GV_Polygon::DiagramType type) {
//    myItemType = type;
//}

//void GV_GraphicsScene::editorLostFocus(GV_Text* item) {
//    QTextCursor cursor = item->textCursor();
//    cursor.clearSelection();
//    item->setTextCursor(cursor);
//
//    if (item->toPlainText().isEmpty()) {
//        removeItem(item);
//        dynamic_cast<GV_Component*>(item)->deleteLater();
//    }
//}

/**
 * Emulate a mouse move event with the last known mouse position.
 * Called from GV_ActionBase when status is > 0. TODO: not used
 */
void GV_GraphicsScene::emulateMouseMoveEvent() {
    /* not possible
    QMouseEvent e(QEvent::MouseMove, QPoint(mx, my),
                    Qt::NoButton, Qt::NoButton, Qt::NoModifier);
    */
    QGraphicsSceneMouseEvent e(QEvent::User); // if (e->type() == QEvent::User) etc.
    mouseMoveEvent(&e);
}

// TODO: not used
void GV_GraphicsScene::emulateMousePressEvent() {
    /*
    QMouseEvent e(QEvent::MouseButtonPress, QPoint(mx, my),
                    Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    */
    QGraphicsSceneMouseEvent e(QEvent::User);
    mousePressEvent(&e);
}

// TODO: not used
void GV_GraphicsScene::emulateMouseReleaseEvent() {
    /*
    QMouseEvent e(QEvent::MouseButtonRelease, QPoint(mx, my),
                    Qt::LeftButton, Qt::NoButton, Qt::NoModifier);
    */
    QGraphicsSceneMouseEvent e(QEvent::User);
    mouseReleaseEvent(&e);
}

/**
 * Executes the protected QGraphicsScene::mousePressEvent
 */
void GV_GraphicsScene::sceneMousePressEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsScene::mousePressEvent(e);
}

/**
 * Executes the protected QGraphicsScene::mouseMoveEvent
 */
void GV_GraphicsScene::sceneMouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsScene::mouseMoveEvent(e);
}

/**
 * Executes the protected QGraphicsScene::mouseReleaseEvent
 */
void GV_GraphicsScene::sceneMouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsScene::mouseReleaseEvent(e);
}

void GV_GraphicsScene::sceneKeyPressEvent(QKeyEvent* e) {
    QGraphicsScene::keyPressEvent(e);
}

void GV_GraphicsScene::sceneKeyReleaseEvent(QKeyEvent* e) {
    QGraphicsScene::keyReleaseEvent(e);
}

void GV_GraphicsScene::sceneDragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    QGraphicsScene::dragEnterEvent(e);
}

void GV_GraphicsScene::sceneDragLeaveEvent(QGraphicsSceneDragDropEvent* e) {
    QGraphicsScene::dragLeaveEvent(e);
}

void GV_GraphicsScene::sceneDragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    QGraphicsScene::dragMoveEvent(e);
}

void GV_GraphicsScene::sceneDropEvent(QGraphicsSceneDragDropEvent* e) {
    QGraphicsScene::dropEvent(e);
}

//bool GV_GraphicsScene::isItemChange(int type){
//    foreach (QGraphicsItem *item, selectedItems()) {
//        if (item->type() == type)
//            return true;
//    }
//
//    return false;
//}

void GV_GraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());

    if (!item && contextMenu) {
        clearSelection();
        contextMenu->exec(event->screenPos());
    } else {
        QGraphicsScene::contextMenuEvent(event);
    }
}

void GV_GraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->dragEnterEvent(e);
    }
}

void GV_GraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->dragLeaveEvent(e);
    }
}

void GV_GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->dragMoveEvent(e);
    }
}

void GV_GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->dropEvent(e);
    }
}

void GV_GraphicsScene::keyPressEvent(QKeyEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->keyPressEvent(e);
    }
}

void GV_GraphicsScene::keyReleaseEvent(QKeyEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->keyReleaseEvent(e);
    }
}

void GV_GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->mouseMoveEvent(e);
    }
}

void GV_GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    if (eventHandler!=NULL) {
        eventHandler->mousePressEvent(e);
    }
}

void GV_GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (eventHandler!=NULL) {
        if (e->button() != Qt::RightButton ||
            eventHandler->hasAction()) {
            eventHandler->mouseReleaseEvent(e);
        } else /* if (e->button() == Qt::RightButton) */ {
            back();
            e->accept();
        }
    }
}

