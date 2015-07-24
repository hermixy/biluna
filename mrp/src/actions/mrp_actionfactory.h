/*****************************************************************
 * $Id: sail_actionfactory.h 1723 2012-07-27 14:42:24Z rutger $
 * Created: Oct 24, 2014 21:17:25 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna MRP project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef MRP_ACTIONFACTORY_H
#define MRP_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define MRP_ACTIONFACTORY MRP_ActionFactory::getInstance()


class MRP_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~MRP_ActionFactory();
    static MRP_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getMenu(QMenu* menu, QToolBar* tb);

private:
    MRP_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static MRP_ActionFactory* mActiveFactory;

};

#endif // MRP_ACTIONFACTORY_H
