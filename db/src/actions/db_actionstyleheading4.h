/*****************************************************************
 * $Id: db_actionstyleheading4.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSTYLEHEADING4_H
#define DB_ACTIONSTYLEHEADING4_H

#include "rb_action.h"

/**
 * This action class can handle user events for text to <h4> formating
 */
class DB_EXPORT DB_ActionStyleHeading4 : public RB_Action {

    Q_OBJECT

public:
    DB_ActionStyleHeading4();
    virtual ~DB_ActionStyleHeading4() {}

    static RB_String getName() {
        return "Style Heading 4";
    }

    virtual RB_String name() {
        return DB_ActionStyleHeading4::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
