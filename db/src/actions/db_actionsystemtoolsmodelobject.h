/*****************************************************************
 * $Id: db_actiontoolsmodelobject.h 2248 2015-12-28 14:13:00Z rutger $
 * Created: Dec 28, 2015 14:38:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSMODELOBJECT_H
#define DB_ACTIONSYSTEMTOOLSMODELOBJECT_H

#include "rb_action.h"

/**
 * Create model object
 */
class DB_EXPORT DB_ActionSystemToolsModelObject : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsModelObject();
    virtual ~DB_ActionSystemToolsModelObject() { }

    static RB_String getName() {
        return "Model object dialog";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsModelObject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
//    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMTOOLSMODELOBJECT_H
