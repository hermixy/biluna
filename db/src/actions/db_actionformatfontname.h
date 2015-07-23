/*****************************************************************
 * $Id: db_actionformatfontname.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 11, 2010 11:25:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFORMATFONTNAME_H
#define DB_ACTIONFORMATFONTNAME_H

#include "rb_action.h"

/**
 * This action class can handle user events for changing the font(name)
 */
class DB_EXPORT DB_ActionFormatFontName : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFormatFontName();
    virtual ~DB_ActionFormatFontName() {}

    static RB_String getName() {
        return "Format Font Name";
    }

    virtual RB_String name() {
        return DB_ActionFormatFontName::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif
