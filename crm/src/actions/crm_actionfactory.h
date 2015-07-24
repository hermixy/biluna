/*****************************************************************
 * $Id: crm_actionfactory.h 1545 2012-02-11 17:34:25Z rutger $
 * Created: Feb 12, 2012 14:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONFACTORY_H
#define CRM_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define CRM_ACTIONFACTORY CRM_ActionFactory::getInstance()


class CRM_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~CRM_ActionFactory();
    static CRM_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getMenu(QMenu* menu, QToolBar* tb);

private:
    CRM_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static CRM_ActionFactory* mActiveFactory;

};

#endif // CRM_ACTIONFACTORY_H
