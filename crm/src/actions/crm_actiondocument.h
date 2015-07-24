/*****************************************************************
 * $Id: crm_actiondocument.h 2076 2014-01-27 16:02:14Z rutger $
 * Created: Mar 6, 2012 11:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef CRM_ACTIONDOCUMENT_H
#define CRM_ACTIONDOCUMENT_H

#include "rb_action.h"


/**
 * Action opens the document edit window
 */
class CRM_ActionDocument : public RB_Action {

    Q_OBJECT

public:
    CRM_ActionDocument();
    virtual ~CRM_ActionDocument() {}

    static RB_String getName() {
        return "Document";
    }

    virtual RB_String name() {
        return CRM_ActionDocument::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // CRM_ACTIONDOCUMENT_H
