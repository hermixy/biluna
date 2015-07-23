/*****************************************************************
 * $Id: rb_actiondebugexportmodel.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Nov 16, 2009 12:14:42 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_ACTIONDEBUGEXPORTMODEL_H
#define RB_ACTIONDEBUGEXPORTMODEL_H

#include "rb_action.h"


/**
 * Debug model by exporting model to text window
 */
class DB_EXPORT RB_ActionDebugExportModel : public RB_Action {

    Q_OBJECT

public:
    RB_ActionDebugExportModel();
    virtual ~RB_ActionDebugExportModel() {}

    static RB_String getName() {
        return "Debug signal slots";
    }

    virtual RB_String name() {
        return RB_ActionDebugExportModel::getName();
    }

    static RB_GuiAction* createGuiAction();
};

#endif // RB_ACTIONDEBUGEXPORTMODEL_H
