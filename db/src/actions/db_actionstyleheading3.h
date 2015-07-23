/*****************************************************************
 * $Id: db_actionstyleheading3.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSTYLEHEADING3_H
#define DB_ACTIONSTYLEHEADING3_H

#include "rb_action.h"

/**
 * This action class can handle user events for text to <h3> style
 */
class DB_EXPORT DB_ActionStyleHeading3 : public RB_Action {

    Q_OBJECT

public:
    DB_ActionStyleHeading3();
    virtual ~DB_ActionStyleHeading3() {}

    static RB_String getName() {
        return "Style Heading 3";
    }

    virtual RB_String name() {
        return DB_ActionStyleHeading3::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
