/*****************************************************************
 * $Id: peng_actionfactory.h 1436 2011-07-16 16:49:02Z rutger $
 * Created: Feb 16, 2011 11:34:42 AM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna PENG project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONFACTORY_H
#define PENG_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define PENG_ACTIONFACTORY PENG_ActionFactory::getInstance()


class PENG_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~PENG_ActionFactory();
    static PENG_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getPlantEngineeringMenu(QMenu* menu, QToolBar* tb);

private:
    PENG_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static PENG_ActionFactory* mActiveFactory;

};

#endif // PENG_ACTIONFACTORY_H
