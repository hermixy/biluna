/*****************************************************************
 * $Id: sail_trackoperation.h 1734 2012-08-16 13:12:19Z rutger $
 * Created: Aug 13, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Sailing (SAIL) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_TRACKOPERATION_H
#define SAIL_TRACKOPERATION_H

#include "rb_modelfactory.h"


/**
 * Operation class for track report
 */
class SAIL_TrackOperation {

public:
    SAIL_TrackOperation();
    virtual ~SAIL_TrackOperation();

    virtual bool execute(RB_ObjectContainer *trackList = NULL);
    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output);

    virtual void setWindFromDirection(double windFromDirection) {
        mWindFromDirection = windFromDirection;
    }

protected:
    double speedInKnots(double bearing);

    RB_ObjectBase* mObject;
    double mWindFromDirection;
};

#endif // SAIL_TRACKOPERATION_H
