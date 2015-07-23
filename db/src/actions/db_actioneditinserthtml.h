/*****************************************************************
 * $Id: db_actioneditinserthtml.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Apr 24, 2012 4:25:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONEDITINSERTHTML_H
#define DB_ACTIONEDITINSERTHTML_H

#include "rb_action.h"

/**
 * This action class can handle user events for inserting HTML
 */
class DB_EXPORT DB_ActionEditInsertHtml : public RB_Action {

    Q_OBJECT

public:
    DB_ActionEditInsertHtml();
    virtual ~DB_ActionEditInsertHtml() {}

    static RB_String getName() {
        return "Edit Insert HTML";
    }

    virtual RB_String name() {
        return DB_ActionEditInsertHtml::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONEDITINSERTHTML_H
