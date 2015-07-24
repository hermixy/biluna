/*****************************************************************
 * $Id: acc_actionfactory.h 1352 2011-01-26 22:01:25Z rutger $
 * Created: Dec 21, 2009 11:34:42 AM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONFACTORY_H
#define ACC_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define ACC_ACTIONFACTORY ACC_ActionFactory::getInstance()


class ACC_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~ACC_ActionFactory();
    static ACC_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getAccountingMenu(QMenu* menu, QToolBar* tb);

private:
    ACC_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static ACC_ActionFactory* mActiveFactory;

};

#endif // ACC_ACTIONFACTORY_H
