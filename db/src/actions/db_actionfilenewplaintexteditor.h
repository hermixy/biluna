/*****************************************************************
 * $Id: db_actionfilenewplaintexteditor.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Jan 23, 2012 4:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILENEWPLAINTEXTEDITOR_H
#define DB_ACTIONFILENEWPLAINTEXTEDITOR_H

#include "rb_action.h"


/**
 * This action class opens a new plain text editor.
 */
class DB_EXPORT DB_ActionFileNewPlainTextEditor : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileNewPlainTextEditor();
    virtual ~DB_ActionFileNewPlainTextEditor() {}

    static RB_String getName() {
        return "File New plain text document";
    }

    virtual RB_String name() {
        return DB_ActionFileNewPlainTextEditor::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
