/*****************************************************************
 * $Id: db_actionfilenewhtmleditor.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Oct 23, 2010 4:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILENEWHTMLEDITOR_H
#define DB_ACTIONFILENEWHTMLEDITOR_H

#include "rb_action.h"


/**
 * This action class can handle user events to create a new HTML editor.
 */
class DB_EXPORT DB_ActionFileNewHtmlEditor : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileNewHtmlEditor();
    virtual ~DB_ActionFileNewHtmlEditor() {}

    static RB_String getName() {
        return "File New HTML Editor";
    }

    virtual RB_String name() {
        return DB_ActionFileNewHtmlEditor::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
