/*****************************************************************
 * $Id: rb_dockwidget.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 29, 2010 12:34:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_DOCKWIDGET_H
#define RB_DOCKWIDGET_H

#include <QDockWidget>
#include <QModelIndex>
#include <QObject>
#include "db_global.h"
#include "rb.h"
#include "db_commandevent.h"
#include "rb_widget.h"

class RB_Action;
class RB_MainWindow;


/**
 * Base dockwidget class
 */
class DB_EXPORT RB_DockWidget : public QDockWidget {

    Q_OBJECT

public:
    RB_DockWidget(RB_MainWindow* mw, RB_Widget* mwgt);
    virtual ~RB_DockWidget();

    virtual RB_String getId() const;
    virtual RB_String getName() const;
    virtual int getWidgetType() const;
    virtual void setWidgetType(int type);
    virtual int getParentWidgetType() const;
    virtual void setParentWidgetType(int type);
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual RB_Widget* getWidget();
//    virtual void setWidget(RB_Widget* wdgt);

    // to be overriden by drawing type windows
    virtual void commandEvent(DB_CommandEvent* /* e */) { }
    virtual RB_Action* getCurrentAction() { return NULL; }

signals:
    void refreshToggledActions(RB2::PerspectiveType type); // TODO: still required?

public slots:
    virtual void show(); // QMdiSubWindow::show() is not virtual
    virtual void slotWindowStateHasChanged(Qt::WindowStates oldState,
                                           Qt::WindowStates newState);
//    virtual void slotChangedModification(bool changed);

protected:
    void closeEvent(QCloseEvent* e);

private:
    void updateWindowTitle(bool changed);

    //! Parent mainwindow
    RB_MainWindow* mMainWindow;
    //! Central widget
    RB_Widget* mCentralWidget;
    //! Sequence number for untitled documents
    // static int mUntitledNumber;
};

#endif // RB_DOCKWIDGET_H
