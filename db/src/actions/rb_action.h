/*****************************************************************
 * $Id: rb_action.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 2, 2009 5:34:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ACTIONINTERFACE_H
#define RB_ACTIONINTERFACE_H

#include <QKeyEvent>
#include <QObject>
#include <QtPrintSupport/QPrinter>
#include "db_global.h"
#include "rb_guiaction.h"
#include "rb_string.h"
#include "rb_stringlist.h"

class RB_ObjectBase;
class DB_CommandEvent;
class RB_Widget;

/**
 * This is the interface that must be implemented for all
 * action classes. Action classes handle actions such
 * as drawing lines, moving entities or zooming in.
 *
 * Inherited from QObject for Qt translation features.
 */
class DB_EXPORT RB_Action : public QObject {

    Q_OBJECT

public:
    RB_Action();
    virtual ~RB_Action();

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();

    virtual RB_String name() {
        return "Action Interface";
    }

    /**
     * Reimplement to make the action exclusive (i.e. terminate
     * all other actions when this one is triggered).
     */
    virtual bool isExclusive() {
        return false;
    }

    /**
     * Reimplement to make the action persistent (i.e. never terminate
     * it unless the user terminates it).
     */
    virtual bool isPersistent() {
        return false;
    }

    virtual void init();
    virtual void init(int status);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);
    virtual void dragEnterEvent(QDragEnterEvent* e);
    virtual void dragMoveEvent(QDragMoveEvent* e);
    virtual void dropEvent(QDropEvent* e);
    //    virtual void coordinateEvent(RB_CoordinateEvent*);
    virtual void commandEvent(DB_CommandEvent*);
    virtual RB_StringList getAvailableCommands(const RB_String& start);
    virtual RB_StringList getAvailableCommands();
    virtual void setStatus(int status);
    virtual int getStatus();
    virtual void trigger();
    virtual void updateMouseButtonHints();
    virtual void updateMouseCursor();
    virtual void updateToolBar();
    virtual bool isFinished();
    virtual void setFinished();
    virtual void finish();
    virtual void setPredecessor(RB_Action* pre);
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

    void setWidget(RB_Widget* wdgt);

protected:
    //! This flag is set when the action has terminated and can be deleted.
    bool mFinished;
    //! Predecessor of this action or NULL.
    RB_Action* mPredecessor;
    //! Widget to be used in this action, set from outside by RB_Action::setWidget()
    RB_Widget* mWidget;
    //! Sequence number for untitled documents
    static int mUntitledNumber;

private:
    /**
     * Current status of the action. After an action has
     * been created the action status is set to 0. Actions
     * that are terminated have a status of -1. Other status
     * numbers can be used to describe the stage this action
     * is in. E.g. a window zoom consists of selecting the
     * first corner (status 0), and selecting the second
     * corner (status 1).
     */
    int mStatus;

};

#endif // RB_ACTION_H
