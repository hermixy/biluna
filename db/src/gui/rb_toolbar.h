/*****************************************************************
 * $Id: rb_toolbar.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jul 22, 2013 9:34:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_TOOLBAR_H
#define RB_TOOLBAR_H

#include <QToolBar>
#include <QObject>
#include "rb.h"
#include "db_commandevent.h"
#include "db_global.h"
#include "rb_widget.h"

class RB_Action;
class RB_MainWindow;


/**
 * Base toolbar class
 */
class DB_EXPORT RB_ToolBar : public QToolBar {

    Q_OBJECT

public:
    RB_ToolBar(RB_MainWindow* mw, RB_Widget* mwgt);
    virtual ~RB_ToolBar();

    virtual RB_String getId() const;
    virtual RB_String getName() const;
    virtual int getWidgetType() const;
    virtual void setWidgetType(int type);
    virtual int getParentWidgetType() const;
    virtual void setParentWidgetType(int type);
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual RB_Widget* getWidget();
    virtual void setWidget(RB_Widget* wdgt);

    // to be overriden by drawing type windows
    virtual void commandEvent(DB_CommandEvent* /* e */) { }
    virtual RB_Action* getCurrentAction() { return NULL; }

signals:
    void refreshToggledActions(RB2::PerspectiveType type); // TODO: still required?

public slots:
    virtual void show(); // QMdiSubWindow::show() is not virtual
    virtual void slotWindowStateHasChanged(Qt::WindowStates oldState,
                                           Qt::WindowStates newState);

protected:
    void closeEvent(QCloseEvent* e);

private:
    void updateWindowTitle(bool changed);

    //! Parent mainwindow
    RB_MainWindow* mMainWindow;
    //! Central widget
    RB_Widget* mCentralWidget;
};

#endif // RB_TOOLBAR_H
