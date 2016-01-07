/*****************************************************************
 * $Id: db_actionsystemtoolsdevelop.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 17, 2011 1:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSDEVELOP_H
#define DB_ACTIONSYSTEMTOOLSDEVELOP_H

#include "rb_action.h"


/**
 * Tools action class for development, debugging and testing only
 */
class DB_EXPORT DB_ActionSystemToolsDevelop : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsDevelop();
    virtual ~DB_ActionSystemToolsDevelop() {}

    static RB_String getName() {
        return "Develop, debug and test only";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsDevelop::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

private:
    void testEncryption();
    void testIdxSplit();
    void updateDatabaseIdxFields();
    void testDbReadWhere();
};

#endif
