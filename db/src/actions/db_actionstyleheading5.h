/*****************************************************************
 * $Id: db_actionstyleheading5.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSTYLEHEADING5_H
#define DB_ACTIONSTYLEHEADING5_H

#include "rb_action.h"

/**
 * This action class can handle user events for text to <h5> formating
 */
class DB_EXPORT DB_ActionStyleHeading5 : public RB_Action{

    Q_OBJECT

public:
    DB_ActionStyleHeading5();
    virtual ~DB_ActionStyleHeading5() {}

    static RB_String getName() {
        return "Style Heading 5";
    }

    virtual RB_String name() {
        return DB_ActionStyleHeading5::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
