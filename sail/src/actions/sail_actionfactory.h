/*****************************************************************
 * $Id: sail_actionfactory.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Jul 26, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ACTIONFACTORY_H
#define SAIL_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define SAIL_ACTIONFACTORY SAIL_ActionFactory::getInstance()


class SAIL_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~SAIL_ActionFactory();
    static SAIL_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getMenu(QMenu* menu, QToolBar* tb);

private:
    SAIL_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static SAIL_ActionFactory* mActiveFactory;

};

#endif // SAIL_ACTIONFACTORY_H
