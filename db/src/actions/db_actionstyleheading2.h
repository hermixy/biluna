/*****************************************************************
 * $Id: db_actionstyleheading2.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSTYLEHEADING2_H
#define DB_ACTIONSTYLEHEADING2_H

#include "rb_action.h"

/**
 * This action class can handle user events for text to <h2> formating
 */
class DB_EXPORT DB_ActionStyleHeading2 : public RB_Action {

    Q_OBJECT

public:
    DB_ActionStyleHeading2();
    virtual ~DB_ActionStyleHeading2() {}

    static RB_String getName() {
        return "Style Heading 2";
    }

    virtual RB_String name() {
        return DB_ActionStyleHeading2::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
