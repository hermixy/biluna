/*****************************************************************
 * $Id: gv_actionfactory.h 1452 2011-09-10 08:19:50Z rutger $
 * Created: Apr 19, 2010 11:34:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna GV project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef GV_ACTIONFACTORY_H
#define GV_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"

#define GV_ACTIONFACTORY GV_ActionFactory::getInstance()

class GV_ActionBase;

/**
 * Action factory
 */
class GV_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~GV_ActionFactory();
    static GV_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual GV_ActionBase* getDefaultAction();

    virtual void getFileNewMenu(QMenu* menu, QToolBar* tb = NULL);
    virtual void getDrawingMenu(QMenu* menu, QToolBar* tb = NULL);
    virtual void getSystemMenu(QMenu* menu, QToolBar* tb = NULL);
    virtual void getTestMenu(QMenu* menu, QToolBar* tb = NULL);

private:
    GV_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static GV_ActionFactory* mActiveFactory;
};

#endif // GV_ACTIONFACTORY_H
