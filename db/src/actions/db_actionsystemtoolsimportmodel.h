/*****************************************************************
 * $Id: db_actionsystemtoolsimportmodel.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 29, 2011 4:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONSYSTEMTOOLSIMPORTMODEL_H
#define DB_ACTIONSYSTEMTOOLSIMPORTMODEL_H

#include "rb_action.h"


/**
 * Tools action class for the import of model or default settings.
 */
class DB_EXPORT DB_ActionSystemToolsImportModel : public RB_Action {

    Q_OBJECT

public:
    DB_ActionSystemToolsImportModel();
    virtual ~DB_ActionSystemToolsImportModel() {}

    static RB_String getName() {
        return "Import model";
    }

    virtual RB_String name() {
        return DB_ActionSystemToolsImportModel::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif
