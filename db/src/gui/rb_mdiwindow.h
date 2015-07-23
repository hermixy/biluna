/*****************************************************************
 * $Id: rb_mdiwindow.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 4, 2009 12:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_MDIWINDOW_H
#define RB_MDIWINDOW_H

#include <QMdiSubWindow>
#include <QModelIndex>
#include <QObject>
#include "rb.h"
#include "db_commandevent.h"
#include "db_global.h"
#include "rb_widget.h"

class RB_Action;
class RB_MainWindow;


/**
 * Base MDI window class
 */
class DB_EXPORT RB_MdiWindow : public QMdiSubWindow {

    Q_OBJECT

public:
    RB_MdiWindow(RB_MainWindow* mw, RB_Widget* mwgt);
    virtual ~RB_MdiWindow();

    virtual RB_String getId() const;
    virtual RB_String getName() const;
    virtual RB_String getSaveAsFileName() const;
    virtual int getWidgetType() const;
    virtual void setWidgetType(int type);
    virtual int getParentWidgetType() const;
    virtual void setParentWidgetType(int type);
    virtual RB2::PerspectiveType getPerspectiveType() const;
    virtual RB_Widget* getWidget();

    // to be overriden by drawing type windows
    virtual void commandEvent(DB_CommandEvent* /* e */) { }
    virtual RB_Action* getCurrentAction() { return NULL; }

signals:


public slots:
    virtual void show(); // QMdiSubWindow::show() is not virtual
    virtual void slotChangedModification(bool dirty);

protected:
    void closeEvent(QCloseEvent* e);

private:
    void updateWindowTitle(bool dirty);

    //! Parent mainwindow
    RB_MainWindow* mMainWindow;
    //! Central widget
    RB_Widget* mCentralWidget;
};

#endif // RB_MDIWINDOW_H
