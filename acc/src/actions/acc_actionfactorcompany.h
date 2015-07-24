/*****************************************************************
 * $Id: acc_actionfactorcompany.h 1846 2012-12-23 14:39:36Z rutger $
 * Created: Mar 18, 2010 11:30:42 AM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_ACTIONFACTORCOMPANY_H
#define ACC_ACTIONFACTORCOMPANY_H

#include "rb_action.h"


/**
 * Action opens factor companies edit dialog
 */
class ACC_ActionFactorCompany : public RB_Action {

    Q_OBJECT

public:
    ACC_ActionFactorCompany();
    virtual ~ACC_ActionFactorCompany() {}

    static RB_String getName() {
        return "Factor Companies";
    }

    virtual RB_String name() {
        return ACC_ActionFactorCompany::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // ACC_ACTIONFACTORCOMPANY_H
