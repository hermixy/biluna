/*****************************************************************
 * $Id: srm_actionfactory.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONFACTORY_H
#define SRM_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define SRM_ACTIONFACTORY SRM_ActionFactory::getInstance()


class SRM_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~SRM_ActionFactory();
    static SRM_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getMenu(QMenu* menu, QToolBar* tb);

private:
    SRM_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static SRM_ActionFactory* mActiveFactory;

};

#endif // SRM_ACTIONFACTORY_H
