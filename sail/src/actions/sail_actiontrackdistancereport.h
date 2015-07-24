/*****************************************************************
 * $Id: sail_actiontrackdistancereport.h 1732 2012-08-11 16:46:12Z rutger $
 * Created: Aug 11, 2012 8:58:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ACTIONTRACKDISTANCEREPORT_H
#define SAIL_ACTIONTRACKDISTANCEREPORT_H

#include "rb_action.h"


/**
 * Action opens track distance report window
 */
class SAIL_ActionTrackDistanceReport : public RB_Action {

    Q_OBJECT

public:
    SAIL_ActionTrackDistanceReport();
    virtual ~SAIL_ActionTrackDistanceReport() {}

    static RB_String getName() {
        return "Track Distance Report";
    }

    virtual RB_String name() {
        return SAIL_ActionTrackDistanceReport::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SAIL_ACTIONTRACKDISTANCEREPORT_H
