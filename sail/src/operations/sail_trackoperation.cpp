/*****************************************************************
 * $Id: sail_trackoperation.cpp 1983 2013-09-02 18:10:02Z rutger $
 * Created: Aug 13, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Sailing (SAIL) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "sail_trackoperation.h"

#include <cmath>
#include "sail_coordcalc.h"
#include "sail_modelfactory.h"
#include "sail_sqlcommonfunctions.h"


/**
 * Constructor
 */
SAIL_TrackOperation::SAIL_TrackOperation() {
    mObject = NULL;
    mWindFromDirection = 0.0;

}

/**
 * Destructor
 */
SAIL_TrackOperation::~SAIL_TrackOperation() {

}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool SAIL_TrackOperation::execute(RB_ObjectContainer* trackList) {
    SAIL_SqlCommonFunctions sqlFunc;

    QSqlQuery query(SAIL_MODELFACTORY->getDatabase());
    sqlFunc.selectTrackDistanceData(query);

     /*
     * SQL:
     * 0 - mname
     * 1 - from_name
     * 2 - to_name
     * 3 - weightfactor
     * 4 - from_lon
     * 5 - from_lat
     * 6 - to_lon
     * 7 - to_lat
     * 8 - comment
     */

    SAIL_CoordCalc coordCalc;
    double weight = 0.0;
    double distance = 0.0;
    double speed = 0.0;
    double efficiency = 0.0;
    double bearingStart = 0.0;
    double bearingEnd = 0.0;
    double lat1 = 0.0;
    double lon1 = 0.0;
    double lat2 = 0.0;
    double lon2 = 0.0;

    while (query.next()) {
        weight = query.value(3).toDouble();
        lat1 = query.value(4).toDouble();
        lon1 = query.value(5).toDouble();
        lat2 = query.value(6).toDouble();
        lon2 = query.value(7).toDouble();

        distance = coordCalc.distanceInNauticalMiles(lat1, lon1, lat2, lon2);
        bearingStart = coordCalc.bearingStart(lat1, lon1, lat2, lon2);
        bearingEnd = coordCalc.bearingEnd(lat1, lon1, lat2, lon2);
        speed = speedInKnots(bearingStart);
        efficiency = (distance / weight) * speed;

        RB_ObjectBase* obj = new RB_ObjectAtomic("", trackList, "SAIL_TrackDistance");
        trackList->addObject(obj);
        obj->addMember(obj, "mname", "-", query.value(0), RB2::MemberChar125);
        obj->addMember(obj, "from_name", "-", query.value(1), RB2::MemberChar125);
        obj->addMember(obj, "to_name", "-", query.value(2), RB2::MemberChar125);
        obj->addMember(obj, "weightfactor", "-", weight, RB2::MemberDouble);
        obj->addMember(obj, "distance", "nmile", distance, RB2::MemberDouble);
        obj->addMember(obj, "speed", "knots", speed, RB2::MemberDouble);
        obj->addMember(obj, "efficiency", "-", efficiency, RB2::MemberDouble);
        obj->addMember(obj, "bearingstart", "degree", bearingStart, RB2::MemberDouble);
        obj->addMember(obj, "bearingend", "degree", bearingEnd, RB2::MemberDouble);
        obj->addMember(obj, "comment", "-", query.value(8), RB2::MemberDouble);
    }

    return true;
}

/**
 * Execute operation
 * @param input input object, if applicable
 * @param output result/output object, if applicable
 * @return true on success, default false
 */
bool SAIL_TrackOperation::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "SAIL_TrackOperation::execute() "
                    "not implemented ERROR");
    return false;
}

/**
 * @param bearing bearing
 * @returns speed in knots or nautical miles
 */
double SAIL_TrackOperation::speedInKnots(double bearing) {
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
