/*****************************************************************
 * $Id: peng_actioneditproject.h 1831 2012-12-15 15:07:57Z rutger $
 * Created: Apr 29, 2015 4:34:42 PM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef PCALC_ACTIONEDITPROJECT_H
#define PCALC_ACTIONEDITPROJECT_H

#include "rb_action.h"

/**
 * Edit projects
 */
class PCALC_ActionEditProject : public RB_Action {

    Q_OBJECT

public:
    PCALC_ActionEditProject();
    virtual ~PCALC_ActionEditProject() { }

    static RB_String getName() {
        return "Edit project";
    }

    virtual RB_String name() {
        return PCALC_ActionEditProject::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // PCALC_ACTIONEDITPROJECT_H
