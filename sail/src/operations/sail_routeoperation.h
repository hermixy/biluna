/*****************************************************************
 * $Id: sail_routeoperation.h 1746 2012-08-23 21:01:10Z rutger $
 * Created: Aug 23, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Sailing (SAIL) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_ROUTEOPERATION_H
#define SAIL_ROUTEOPERATION_H

#include "rb_modelfactory.h"


/**
 * Operation class for route calculations
 */
class SAIL_RouteOperation {

public:
    SAIL_RouteOperation();
    virtual ~SAIL_RouteOperation();

    virtual bool execute(RB_ObjectContainer* objC = NULL);
    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output);

    virtual double calculatedDistance() {
        return mDistance;
    }

    virtual double calculatedDuration() {
        return mDuration;
    }

    virtual void setWindFromDirection(double windFromDirection) {
        mWindFromDirection = windFromDirection;
    }

    virtual void setCoordinateIdList(RB_StringList& idList) {
        mCoordinateIdList = idList;
    }

protected:
    double speedInKnots(double bearing);

    RB_ObjectBase* mObject;
    double mWindFromDirection;

    //! Calculated distance in nautical miles
    double mDistance;
    //! Calculated duration in hours
    double mDuration;
    //! List coordinate ID's
    RB_StringList mCoordinateIdList;
};

#endif // SAIL_ROUTEOPERATION_H
