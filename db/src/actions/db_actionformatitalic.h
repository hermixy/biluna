/*****************************************************************
 * $Id: db_actionformatitalic.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 9, 2010 11:48:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFORMATITALIC_H
#define DB_ACTIONFORMATITALIC_H

#include "rb_action.h"

/**
 * This action class can handle user events for selecting all data
 */
class DB_EXPORT DB_ActionFormatItalic : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFormatItalic();
    virtual ~DB_ActionFormatItalic() {}

    static RB_String getName() {
        return "Format Italic";
    }

    virtual RB_String name() {
        return DB_ActionFormatItalic::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};
#endif
