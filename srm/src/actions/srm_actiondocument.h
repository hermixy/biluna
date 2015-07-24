/*****************************************************************
 * $Id: srm_actiondocument.h 1893 2013-02-21 21:41:33Z rutger $
 * Created: Mar 6, 2012 11:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna CRM project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SRM_ACTIONDOCUMENT_H
#define SRM_ACTIONDOCUMENT_H

#include "rb_action.h"


/**
 * Action opens document edit window
 */
class SRM_ActionDocument : public RB_Action {

    Q_OBJECT

public:
    SRM_ActionDocument();
    virtual ~SRM_ActionDocument() {}

    static RB_String getName() {
        return "Document";
    }

    virtual RB_String name() {
        return SRM_ActionDocument::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SRM_ACTIONDOCUMENT_H
