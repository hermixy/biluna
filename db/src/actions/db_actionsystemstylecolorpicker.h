/*****************************************************************
 * $Id: db_actionsystemstylecolorpicker.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Oct 29, 2014 8:14:42 PM - rutger
 *
 * Copyright (C) 2014 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMSTYLECOLORPICKER_H
#define DB_ACTIONSYSTEMSTYLECOLORPICKER_H

#include "rb_action.h"


/**
 * Action for editing permission dialog
 */
class DB_EXPORT DB_ActionSystemStyleColorPicker : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemStyleColorPicker();
    virtual ~DB_ActionSystemStyleColorPicker() {}

    static RB_String getName() {
        return "Palette color picker";
    }

    virtual RB_String name() {
        return DB_ActionSystemStyleColorPicker::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // DB_ACTIONSYSTEMSTYLECOLORPICKER_H
