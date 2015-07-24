/*****************************************************************
 * $Id: sail_coordcalc.h 1973 2013-08-19 15:00:11Z rutger $
 * Created: Jul 30, 2012 17:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SAIL_COORDCALC_H
#define SAIL_COORDCALC_H

#include "rb_string.h"


/**
 * Utility class for geocoordinate calculations
 */
class SAIL_CoordCalc {

public:
    SAIL_CoordCalc();
    virtual ~SAIL_CoordCalc();

    double strToLatLong(char* coord);
    RB_String latLongToStrSec(double decDeg, bool isLatitude);
    RB_String latLongToStrMin(double decDeg, bool isLatitude);
    double distanceInNauticalMiles(double lat1, double lon1,
                                   double lat2, double lon2);
    double bearingStart(double lat1, double lon1,
                        double lat2, double lon2);
    double bearingEnd(double lat1, double lon1,
                      double lat2, double lon2);



};

#endif // SAIL_COORDCALC_H
