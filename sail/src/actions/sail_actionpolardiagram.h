/*****************************************************************
 * $Id: sail_actionpolardiagram.h 1979 2013-08-22 19:56:38Z rutger $
 * Created: Aug 22, 2013 9:07:42 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ACTIONPOLARDIAGRAM_H
#define SAIL_ACTIONPOLARDIAGRAM_H

#include "rb_action.h"

/**
 * Boat polar speed diagram
 */
class SAIL_ActionPolarDiagram : public RB_Action {

    Q_OBJECT

public:
    SAIL_ActionPolarDiagram();
    virtual ~SAIL_ActionPolarDiagram() { }

    static RB_String getName() {
        return "Polar Diagram";
    }

    virtual RB_String name() {
        return SAIL_ActionPolarDiagram::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SAIL_ACTIONPOLARDIAGRAM_H
