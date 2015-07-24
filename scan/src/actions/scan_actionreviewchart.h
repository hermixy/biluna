/*****************************************************************
 * $Id: scan_actionreviewchart.h 1468 2011-10-12 23:23:42Z rutger $
 * Created: Oct 10, 2011 8:14:42 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_ACTIONREVIEWCHART_H
#define SCAN_ACTIONREVIEWCHART_H

#include "rb_action.h"


/**
 * Action opens review chart window
 */
class SCAN_ActionReviewChart : public RB_Action {

    Q_OBJECT

public:
    SCAN_ActionReviewChart();
    virtual ~SCAN_ActionReviewChart() {}

    static RB_String getName() {
        return "Review Chart Types";
    }

    virtual RB_String name() {
        return SCAN_ActionReviewChart::getName();
    }

    static RB_GuiAction* createGuiAction();
    static RB_Action* factory();
    virtual void trigger();

};

#endif // SCAN_ACTIONREVIEWCHART_H
