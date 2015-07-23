/*****************************************************************
 * $Id: db_actionsystemtoolsexportmodel.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 6, 2009 4:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSEXPORTMODEL_H
#define DB_ACTIONSYSTEMTOOLSEXPORTMODEL_H

#include "rb_action.h"


/**
 * Tools action class for the export of the model to the text window.
 */
class DB_EXPORT DB_ActionSystemToolsExportModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsExportModel();
    virtual ~DB_ActionSystemToolsExportModel() {}

    static RB_String getName() {
        return "Export Model";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsExportModel::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
