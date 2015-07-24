/*****************************************************************
 * $Id: sail_routeoperation.cpp 1746 2012-08-23 21:01:10Z rutger $
 * Created: Aug 23, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Sailing (SAIL) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "sail_routeoperation.h"

#include <cmath>
#include "sail_coordcalc.h"
#include "sail_modelfactory.h"
#include "sail_sqlcommonfunctions.h"


/**
 * Constructor
 */
SAIL_RouteOperation::SAIL_RouteOperation() {
    mObject = NULL;
    mWindFromDirection = 0.0;

    mDistance = 0.0;
    mDuration = 0.0;
}

/**
 * Destructor
 */
SAIL_RouteOperation::~SAIL_RouteOperation() {

}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool SAIL_RouteOperation::execute(RB_ObjectContainer* /*objC*/) {
    SAIL_SqlCommonFunctions sqlFunc;

    QSqlQuery query(SAIL_MODELFACTORY->getDatabase());
    sqlFunc.selectRouteDistanceData(query, mCoordinateIdList);

     /*
     * SQL:
     * 0 - seqno
     * 1 - mname
     * 2 - latitude
     * 3 - longitude
     */

    SAIL_CoordCalc coordCalc;
    double distance = 0.0;
    double duration = 0.0;
    double bearingStart = 0.0;
    double speed = 0.0;
    double lat1 = 0.0;
    double lon1 = 0.0;
    double lat2 = 0.0;
    double lon2 = 0.0;
    bool isFirstRecord = true;

    while (query.next()) {
        lat2 = query.value(2).toDouble();
        lon2 = query.value(3).toDouble();

        if (!isFirstRecord) {
            distance = coordCalc.distanceInNauticalMiles(lat1, lon1, lat2, lon2);
            bearingStart = coordCalc.bearingStart(lat1, lon1, lat2, lon2);
            speed = speedInKnots(bearingStart);
            duration = distance / speed;

            mDistance += distance;
            mDuration += duration;
        }

        isFirstRecord = false;

        lat1 = lat2;
        lon1 = lon2;
    }

    return true;
}

/**
 * Execute operation
 * @param input input object, if applicable
 * @param output result/output object, if applicable
 * @return true on success, default false
 */
bool SAIL_RouteOperation::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "SAIL_RouteOperation::execute() "
                    "not implemented ERROR");
    return false;
}

/**
 * TODO: to utility SAIL_TrackOperation has same function
 * @param bearing bearing
 * @returns speed in knots or nautical miles
 */
double SAIL_RouteOperation::speedInKnots(double bearing) {
    if (bearing < 0.0 || bearing > 360.0 || mWindFromDirection < 0.0
            || mWindFromDirection > 360.0) {
        return 0.0;
    }

    // Small angle of track direction to wind direction in degrees
    double trackAngleToWind = std::fabs(mWindFromDirection - bearing);
    double distanceFactor = 1.0;
    double speed30 = 2.0;
    double speed60 = 5.5;
    double speed140 = 7.0;
    double speed180 = 6.0;
    double speed = 0.0;

    if (trackAngleToWind > 180.0) {
        if (mWindFromDirection < 180.0) {
            trackAngleToWind = std::fabs((mWindFromDirection + 360) - bearing);
        } else if (bearing < 180.0) {
            trackAngleToWind = std::fabs(mWindFromDirection - (bearing + 360));
        }
    }

    // Calculate average speed over track
    // TODO: implement calculation based on user data
    if (trackAngleToWind < 45.0) {
        // upwind- or cross leg, just adds a leg to the distance
        // based on the angle which has a distance ratio reduces
        // speed at 45 degree
        distanceFactor = std::cos((45 - trackAngleToWind) * M_PI / 180.0)
                + std::sin((45 - trackAngleToWind) * M_PI / 180.0);

        // speed at 45 degree
        double speed45 = (speed60 - speed30) * (45.0 - 30.0) / (60.0 - 30.0) + speed30;
        speed = speed45 / distanceFactor;
    } else if (trackAngleToWind < 60.0) {
        speed = (speed60 - speed30) * (trackAngleToWind - 30.0) / (60.0 - 30.0) + speed30;
    } else if (trackAngleToWind < 140.0) {
        speed = (speed140 - speed60) * (trackAngleToWind - 60.0) / (140.0 - 60.0) + speed60;
    } else if (trackAngleToWind <= 180.0) {
        speed = (speed180 - speed140) * (trackAngleToWind - 140.0) / (180.0 - 140.0) + speed140;
    }

    return speed;
}
