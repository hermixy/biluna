/*****************************************************************
 * $Id: db_actiontoolsexportmodel.h 1092 2010-02-10 09:52:33Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSEXPORTMODEL_H
#define DB_ACTIONTOOLSEXPORTMODEL_H

#include "rb_action.h"


/**
 * Tools action class for the export of the model to the text window.
 */
class DB_ActionToolsExportModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsExportModel();
    virtual ~DB_ActionToolsExportModel() {}

    static RB_String getName() {
        return "Export Model";
    }

    virtual RB_String name() {
        return DB_ActionToolsExportModel::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif
