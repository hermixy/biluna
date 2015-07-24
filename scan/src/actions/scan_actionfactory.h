/*****************************************************************
 * $Id: scan_actionfactory.h 1401 2011-04-27 20:58:49Z rutger $
 * Created: Apr 26, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ACTIONFACTORY_H
#define SCAN_ACTIONFACTORY_H

#include <QMenu>
#include <QToolBar>
#include "rb_actionfactory.h"


#define SCAN_ACTIONFACTORY SCAN_ActionFactory::getInstance()


class SCAN_ActionFactory : public RB_ActionFactory {

    Q_OBJECT

public:
    virtual ~SCAN_ActionFactory();
    static SCAN_ActionFactory* getInstance(RB_MainWindow* mw = 0);

    virtual void getScanMenu(QMenu* menu, QToolBar* tb);

public slots:
    virtual void slotDbIdValid(int valid);

signals:
    //! Only emitted when DB ID is invalid
    void dbIdValid(int valid);

protected:
    void connectSignalSlot(RB_GuiAction* ga);

private:
    SCAN_ActionFactory(RB_MainWindow* mw);

    //! Unique factory instance
    static SCAN_ActionFactory* mActiveFactory;

};

#endif // SCAN_ACTIONFACTORY_H
