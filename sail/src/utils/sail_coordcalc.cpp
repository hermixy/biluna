/*****************************************************************
 * $Id: sail_coordcalc.cpp 1973 2013-08-19 15:00:11Z rutger $
 * Created: Jul 30, 2012 17:24:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_coordcalc.h"

#include <cmath>
#include <math.h>


/* WARNING:  These values are very important, as used under the "default" case. */
#define INT_PART 3
#define DEC_PART 2

/**
 * Constructor
 */
SAIL_CoordCalc::SAIL_CoordCalc() {
    // nothing
}

/**
 * Destructor
 */
SAIL_CoordCalc::~SAIL_CoordCalc() {
    // nothing
}


double SAIL_CoordCalc::strToLatLong(char* coord) {
    int sign = +1;
    double val;
    bool isPreviousSeparator = false;

    int i = 0;  /* an index into coord, the text-input string, indicating the character currently being parsed */

    int p[9] = {0,0,1,  /* degrees */
                0,0,1,  /* minutes */
                0,0,1   /* seconds */
               };
    int* ptr = p;   /* p starts at Degrees.
                       It will advance to the Decimal part when a decimal-point is encountered,
                       and advance to minutes & seconds when a separator is encountered */
    int  flag = INT_PART; /* Flips back and forth from INT_PART and DEC_PART */

    while(1)
    {
        switch (coord[i])
        {
            /* Any digit contributes to either degrees,minutes, or seconds */
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                *ptr = 10 * (*ptr) + (coord[i] - '0');
                if (flag == DEC_PART) {  /* it'd be nice if I could find a clever way to avoid this test */
                    ptr[1] *= 10;
                }
                isPreviousSeparator = false;
                break;

            case '.':     /* A decimal point implies ptr is on an integer-part; advance to decimal part */
                flag = DEC_PART; /* after encountering a decimal point, we are now processing the Decimal Part */
                ptr++;  /* ptr[0] is now the Decimal piece; ptr[1] is the Denominator piece (powers of 10) */
                isPreviousSeparator = false;
                break;

            case '\0':    /* A Null terminator triggers return (no break necessary) */
                val = p[0]*3600 + p[3]*60 + p[6];             /* All Integer math */
                if (p[1]) val += ((double)p[1]/p[2]) * 3600;  /* Floating-point operations only if needed */
                if (p[4]) val += ((double)p[4]/p[5]) *   60;  /* (ditto) */
                if (p[7]) val += ((double)p[7]/p[8]);         /* (ditto) */
                return sign * val / 3600.0;                   /* Only one floating-point division! */

            case 'W':
            case 'w':
            case 'S':
            case 's':
                sign = -1;
                isPreviousSeparator = false;
                break;

            /* Any other symbol is a separator, and moves ptr from degrees to minutes, or minutes to seconds */
            default:
                if (!isPreviousSeparator) {
                    /* Note, by setting DEC_PART=2 and INT_PART=3, I avoid an if-test. (testing and branching is slow) */
                    ptr += flag;
                    flag = INT_PART; /* reset to Integer part, we're now starting a new "piece" (degrees, min, or sec). */
                    isPreviousSeparator = true;
                }
        }
        i++;
    }

    return -1.0;  /* Should never reach here! */
}

/**
 * Convert decimal latitude and longitude to string like 00°00'00.00"N
 * @param decDeg decimal degrees
 * @param isLatitude is true in case of latitude conversion
 * @return latitude or longitude string
 */
RB_String SAIL_CoordCalc::latLongToStrSec(double decDeg, bool isLatitude) {
    RB_String str;
    RB_String postscript;
    double posDecDeg = decDeg;

    if (decDeg >= 0.0) { // N or E
        if (isLatitude) { // 00°00'00.00"N or 000°00'00.00"E
            postscript = "N";
        } else {
            postscript = "E";
        }
    } else {
        if (isLatitude) { // 00°00'00.00"S or 000°00'00.00"W
            postscript = "S";
        } else {
            postscript = "W";
        }

        // only positive part required
        posDecDeg *= -1;
    }

    double degrees = std::floor(posDecDeg);
    double minutes = (posDecDeg - std::floor(posDecDeg)) * 60.0;
    double seconds = (minutes - std::floor(minutes)) * 60.0;
    double hundreds = (seconds - std::floor(seconds)) * 100.0;
    // get rid of fractional part
    minutes = std::floor(minutes);
    seconds = std::floor(seconds);
    hundreds = round(hundreds);

    str = RB_String::number((int)degrees) + "°";
    str += RB_String::number((int)minutes) + "\'";
    str += RB_String::number((int)seconds) + ".";
    str += RB_String::number((int)hundreds) + "\"";
    str += postscript;

    return str;
}

/**
 * Convert decimal latitude and longitude to string like 00°00.0000N
 * @param decDeg decimal degrees
 * @param isLatitude is true in case of latitude conversion
 * @return latitude or longitude string
 */
RB_String SAIL_CoordCalc::latLongToStrMin(double decDeg, bool isLatitude) {
    RB_String str;
    RB_String postscript;
    double posDecDeg = decDeg;

    if (decDeg >= 0.0) { // N or E
        if (isLatitude) { // 00°00.0000N or 000°00.0000E
            postscript = "N";
        } else {
            postscript = "E";
        }
    } else {
        if (isLatitude) { // 00°00.0000S or 000°00.0000W
            postscript = "S";
        } else {
            postscript = "W";
        }

        // only positive part required
        posDecDeg *= -1;
    }

    double degrees = std::floor(posDecDeg);
    double minutes = (posDecDeg - std::floor(posDecDeg)) * 60.0;
    double thousands = (minutes - std::floor(minutes)) * 1000.0;
    // get rid of fractional part
    minutes = std::floor(minutes);
    thousands = round(thousands);

    str = RB_String::number((int)degrees) + "°";

    if (minutes < 1) {
        str += "00.";
    } else if (minutes < 10) {
        str += "0" + RB_String::number((int)minutes) + ".";
    } else {
        str += RB_String::number((int)minutes) + ".";
    }

    if (thousands < 1) {
        str += "000";
    } else if (thousands < 10) {
        str += "00" + RB_String::number((int)thousands);
    } else if (thousands < 100) {
        str += "0" + RB_String::number((int)thousands);
    } else {
        str += RB_String::number((int)thousands);
    }

    str += postscript;

    return str;
}

/**
 * Distance between two coordinates in nautical miles
 * @param lat1 latitude position 1 in degrees
 * @param lon1 longitude position 1 in degrees
 * @param lat2 latitude position 2 in degrees
 * @param lon2 longitude position 2 in degrees
 * @return distance in nautical miles
 */
double SAIL_CoordCalc::distanceInNauticalMiles(double lat1, double lon1,
                                               double lat2, double lon2) {
    double llat1 = lat1 * M_PI / 180; // radians
    double llon1 = lon1 * M_PI / 180;
    double llat2 = lat2 * M_PI / 180;
    double llon2 = lon2 * M_PI / 180;

    double radius = 6371.0; // earth radius in kilometer
    double distance =  std::acos(std::sin(llat1) * std::sin(llat2) +
                                 std::cos(llat1) * std::cos(llat2) *
                                 std::cos(llon2 - llon1)) * radius;
    return distance / 1.8520; // 1.852 is km per nautical mile exact
}

/**
 * Bearing (or azimuth) start at (lat1, lon1) of a track
 * between two coordinates in degrees
 * @param lat1 latitude position 1 in degrees
 * @param lon1 longitude position 1 in degrees
 * @param lat2 latitude position 2 in degrees
 * @param lon2 longitude position 2 in degrees
 * @return start bearing
 */
double SAIL_CoordCalc::bearingStart(double lat1, double lon1,
                                    double lat2, double lon2) {
    double llat1 = lat1 * M_PI / 180; // radians
    double llon1 = lon1 * M_PI / 180;
    double llat2 = lat2 * M_PI / 180;
    double llon2 = lon2 * M_PI / 180;

    double y = std::sin(llon2 - llon1)* std::cos(llat2);
    double x = std::cos(llat1) * std::sin(llat2)
            - std::sin(llat1) * std::cos(llat2) * std::cos(llon2 -llon1);
    double angle100 = std::atan2(y, x) * 180 / M_PI * 100; // -180 ... +180
    double bearingStart = (((int)angle100 + 36000) % 36000) / 100.0; // example: (-152 + 360) % 360 = 208
    return bearingStart;
}

/**
 * Bearing end at (lat2, lon2) of a track between two coordinates in degrees
 * @param lat1 latitude position 1 in degrees
 * @param lon1 longitude position 1 in degrees
 * @param lat2 latitude position 2 in degrees
 * @param lon2 longitude position 2 in degrees
 * @return end bearing
 */
double SAIL_CoordCalc::bearingEnd(double lat1, double lon1,
                                  double lat2, double lon2) {
    double llat1 = lat2 * M_PI / 180; // radians
    double llon1 = lon2 * M_PI / 180;
    double llat2 = lat1 * M_PI / 180;
    double llon2 = lon1 * M_PI / 180;

    double y = std::sin(llon2 - llon1)* std::cos(llat2);
    double x = std::cos(llat1) * std::sin(llat2)
            - std::sin(llat1) * std::cos(llat2) * std::cos(llon2 -llon1);
    double angle100 = std::atan2(y, x) * 180 / M_PI * 100; // -180 ... +180
    double bearingEnd = (((int)angle100 + 18000) % 36000) / 100.0; // 180 to reverse the direction
    return bearingEnd;
}
