/*****************************************************************
 * $Id: scan_actioneditproject.h 1832 2012-12-16 12:01:50Z rutger $
 * Created: Dec 14, 2012 9:57:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ACTIONEDITPROJECT_H
#define SCAN_ACTIONEDITPROJECT_H

#include "rb_action.h"

/**
 * Edit projects
 */
class SCAN_ActionEditProject : public RB_Action {

    Q_OBJECT

public:
    SCAN_ActionEditProject();
    virtual ~SCAN_ActionEditProject() { }

    static RB_String getName() {
        return "Edit project";
    }

    virtual RB_String name() {
        return SCAN_ActionEditProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SCAN_ACTIONEDITPROJECT_H
