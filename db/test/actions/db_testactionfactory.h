/*****************************************************************
 * $Id: db_testactionfactory.h 1273 2010-08-06 15:27:09Z rutger $
 * Created: Nov 22, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_TESTACTIONFACTORY_H
#define DB_TESTACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"

#define DB_TESTACTIONFACTORY DB_TestActionFactory::getInstance()



class DB_TestActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~DB_TestActionFactory();
    static DB_TestActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getTestMenu(QMenu* menu, QToolBar* toolBar);
    virtual void getSystemMenu(QMenu* menu, QToolBar* tb);

private:
    DB_TestActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static DB_TestActionFactory* mActiveFactory;
};

#endif // DB_TESTACTIONFACTORY_H
