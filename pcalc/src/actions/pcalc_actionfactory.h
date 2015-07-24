/*****************************************************************
 * $Id: peng_actionfactory.h 1436 2011-07-16 16:49:02Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONFACTORY_H
#define PCALC_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define PCALC_ACTIONFACTORY PCALC_ActionFactory::getInstance()


class PCALC_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~PCALC_ActionFactory();
    static PCALC_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getPlantCalculationMenu(QMenu* menu, QToolBar* tb);

private:
    PCALC_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static PCALC_ActionFactory* mActiveFactory;

};

#endif // PCALC_ACTIONFACTORY_H
