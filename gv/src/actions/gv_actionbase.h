/*****************************************************************
 * $Id: gv_actionbase.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Oct 4, 2008 6:42:21 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna gv project.
 * Based on rs_actionbase.h 7382
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONBASE_H
#define GV_ACTIONBASE_H

#include "gv_actioncomponent.h"
#include "gv_commandevent.h"
#include "gv_dialogfactory.h"
#include "gv_drawing.h"
#include "gv_graphicsview.h"
#include "gv_modelfactory.h"
#include "rb_action.h"
#include "rb_mdiwindow.h"


/**
 * Base class for all actions in module GV
 * Initializes the snap mode to auto snap and disables
 * snap restrictions and provides basic access methods to
 * the drawing model. Allows single selection.
 */
class GV_ActionBase : public RB_Action, public GV_ActionComponent {

    Q_OBJECT

public:
    GV_ActionBase(GV_Drawing& dwg, GV_GraphicsView& gv);
    virtual ~GV_ActionBase();

    virtual void init();
    virtual void init(int status);

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent* e);
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent* e);
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent* e);
    virtual void dropEvent(QGraphicsSceneDragDropEvent * e);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

    virtual void coordinateEvent(GV_CoordinateEvent* /*, QGraphicsSceneMouseEvent* = NULL */);
    virtual void commandEvent(GV_CommandEvent*);
    virtual RB_StringList getAvailableCommands(const RB_String& start);
    virtual RB_StringList getAvailableCommands();

    virtual void trigger();
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();

    virtual void setPredecessor(GV_ActionBase* pre);
    virtual void suspend();
    virtual void resume();

    virtual RB_String command(const RB_String& cmd) {
        return cmd;
    }
    virtual RB_String msgAvailableCommands() {
        return QObject::tr("Available commands:");
    }
    virtual bool checkCommand(const RB_String& cmd, const RB_String& str) {
        return cmd.toLower()==str.toLower();
    }

    virtual RVector snapPoint(QGraphicsSceneMouseEvent* e);
    virtual RVector snapPoint(QGraphicsSceneDragDropEvent* e);
    virtual RVector snapPoint(double x, double y);

};

#endif
