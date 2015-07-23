/*****************************************************************
 * $Id: rb_actionfactory.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ACTIONFACTORY_H
#define RB_ACTIONFACTORY_H

#include <map>
#include <QObject>
#include "db_global.h"
#include "rb_guiaction.h"
#include "rb_mainwindow.h"


/**
 * Action factory class, for the creation and deletion
 * of all actions on menus, toolbars and global dockwindows
 */
class DB_EXPORT RB_ActionFactory : public QObject {

    Q_OBJECT

public:
    RB_ActionFactory(RB_MainWindow* mw);
    virtual ~RB_ActionFactory();

    // Example menus and toolbars
    // virtual void getFileMenu(QMenu* menu, QToolbar* toolBar) = 0;

    // Example
    // virtual void getWidget();

    virtual int getPerspective();
    virtual void setPerspective(int perspectiveType);

public slots:
    virtual void slotUpdateAction();

protected:
    //! Main window
    RB_MainWindow* mMainWindow;
    //! Action factory perspective
    int mPerspective;

};

#endif // RB_ACTIONFACTORY_H
