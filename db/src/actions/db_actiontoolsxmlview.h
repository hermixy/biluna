/*****************************************************************
 * $Id: db_actionfilenewplaintexteditor.h 1538 2012-01-25 23:37:38Z rutger $
 * Created: Nov 11, 2013 9:08:25 AM - Rutger Botermans
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_ACTIONTOOLSXMLVIEW_H
#define DB_ACTIONTOOLSXMLVIEW_H

#include "rb_action.h"

/**
 * This action class opens a new XML viewer/validator.
 */
class DB_EXPORT DB_ActionToolsXmlView : public RB_Action {

    Q_OBJECT

public:
    DB_ActionToolsXmlView();
    virtual ~DB_ActionToolsXmlView() {}

    static RB_String getName() {
        return "XML viewer/validator tool";
    }

    virtual RB_String name() {
        return DB_ActionToolsXmlView::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();
};

#endif // DB_ACTIONTOOLSXMLVIEW_H
