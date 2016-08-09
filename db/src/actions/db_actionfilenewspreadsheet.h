/*****************************************************************
 * $Id: db_actionfilenewspreadsheet.h 2016-08-09 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONFILENEWSPREADSHEET_H
#define DB_ACTIONFILENEWSPREADSHEET_H

#include "rb_action.h"


/**
 * This action creates a new spreadsheet MDI window document.
 */
class DB_EXPORT DB_ActionFileNewSpreadSheet : public RB_Action {

    Q_OBJECT

public:
    DB_ActionFileNewSpreadSheet();
    virtual ~DB_ActionFileNewSpreadSheet() {}

    static RB_String getName() {
        return "File New spreadsheet document";
    }

    virtual RB_String name() {
        return DB_ActionFileNewSpreadSheet::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif // DB_ACTIONFILENEWSPREADSHEET_H
