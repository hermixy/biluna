/*****************************************************************
 * $Id: peng_actioneditproject.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Dec 14, 2012 9:57:42 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PENG_ACTIONEDITPROJECT_H
#define PENG_ACTIONEDITPROJECT_H

#include "rb_action.h"

/**
 * Edit projects
 */
class PENG_ActionEditProject : public RB_Action {

    Q_OBJECT

public:
    PENG_ActionEditProject();
    virtual ~PENG_ActionEditProject() { }

    static RB_String getName() {
        return "Edit project";
    }

    virtual RB_String name() {
        return PENG_ActionEditProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PENG_ACTIONEDITPROJECT_H
