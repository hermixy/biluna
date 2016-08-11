/****************************************************************************
** $Id: gv_math.cpp 10328 2008-04-05 22:00:01Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you. Updated for Biluna 2016-08-11 Updated for Biluna 2016-08-11
**
** Updated for Biluna 2016-08-11
**********************************************************************/

#include "gv_math.h"

#include "rb_debug.h"

#ifndef exp10
#define exp10(x) pow(10,x)
#endif


/**
 * Save pow function
 */
double GV_Math::pow(double x, double y) {
    errno = 0;
    double ret = ::pow(x, y);
    if (errno==EDOM) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
            "GV_Math::pow: EDOM in pow");
        ret = 0.0;
    }
    else if (errno==ERANGE) {
        RB_DEBUG->print(RB_Debug::D_WARNING,
            "GV_Math::pow: ERANGE in pow");
        ret = 0.0;
    }
    return ret;
}



/**
 * Converts radians to degrees.
 */
double GV_Math::rad2deg(double a) {
    return (a/(2.0*M_PI)*360.0);
}



/**
 * Converts degrees to radians.
 */
double GV_Math::deg2rad(double a) {
    return ((a/360.0)*(2.0*M_PI));
}



/**
 * Converts radians to gradians.
 */
double GV_Math::rad2gra(double a) {
    return (a/(2.0*M_PI)*400.0);
}



/**
 * Finds greatest common divider using Euclid's algorithm.
 */
int GV_Math::findGCD(int a, int b) {
    int rem;

    while (b!=0) {
        rem = a % b;
        a = b;
        b = rem;
    }

    return a;
}



/**
 * Tests if angle a is between a1 and a2. a, a1 and a2 must be in the 
 * range between 0 and 2*PI.
 * All angles in rad.
 *
 * @param reversed true for clockwise testing. false for ccw testing.
 * @return true if the angle a is between a1 and a2.
 */
bool GV_Math::isAngleBetween(double a,
                             double a1, double a2,
                             bool reversed) {

    bool ret = false;

    if (reversed) {
        double tmp = a1;
        a1 = a2;
        a2 = tmp;
    }

    if(a1>=a2-1.0e-12) {
        if(a>=a1-1.0e-12 || a<=a2+1.0e-12) {
            ret = true;
        }
    } else {
        if(a>=a1-1.0e-12 && a<=a2+1.0e-12) {
            ret = true;
        }
    }
    //RB_DEBUG->print("angle %f is %sbetween %f and %f",
    //                a, ret ? "" : "not ", a1, a2);
    return ret;
}



/**
 * Corrects the given angle to the range of 0-2*Pi.
 */
double GV_Math::correctAngle(double a) {
    // prevent very long execution time due to faulty input values:
    if (fabs(a)>1e4) {
        return 0.0;
    }

    while (a>2*M_PI)
        a-=2.0*M_PI;
    while (a<0.0)
        a+=2.0*M_PI;
    return a;
}



/**
 * @return The angle that needs to be added to a1 to reach a2.
 *         Always positive and less than 2*pi.
 */
double GV_Math::getAngleDifference(double a1, double a2) {
    double ret;

    if (a1>=a2) {
        a2+=2*M_PI;
    }
    ret = a2-a1;

    if (ret>=2*M_PI) {
        ret=0.0;
    }

    return ret;
}



/**
 * @return The angle that needs to be added to a1 to reach a2.
 *         Always between -PI and PI.
 */
double GV_Math::getAngleDifference180(double a1, double a2) {
    double ret;

    ret = a2 - a1;
    if (ret>M_PI) {
        ret = -(2*M_PI-ret);
    }
    if (ret<-M_PI) {
        ret = 2*M_PI+ret;
    }

    return ret;
}


/**
* Makes a text constructed with the given angle readable. Used
* for dimension texts and for mirroring texts.
*
* @param readable true: make angle readable, false: unreadable
* @param corrected Will point to true if the given angle was
*   corrected, false otherwise.
* 
 * @return The given angle or the given angle+PI, depending which on
 * is readable from the bottom or right.
 */
double GV_Math::makeAngleReadable(double angle, bool readable,
                                  bool* corrected) {

    double ret;

    bool cor = isAngleReadable(angle) ^ readable;

    // quadrant 1 & 4
    if (!cor) {
        ret = angle;
    }
    // quadrant 2 & 3
    else {
        ret = angle+M_PI;
    }

    if (corrected!=NULL) {
        *corrected = cor;
    }

    return ret;
}


/**
 * @return true: if the given angle is in a range that is readable 
 * for texts created with that angle.
 */
bool GV_Math::isAngleReadable(double angle) {
    if (angle>M_PI/2.0*3.0+0.001 ||
            angle<M_PI/2.0+0.001) {
        return true;
    } else {
        return false;
    }
}



/**
 * @param tol Tolerance in rad.
 * @retval true The two angles point in the same direction.
 */
bool GV_Math::isSameDirection(double dir1, double dir2, double tol) {
    double diff = fabs(dir1-dir2);
    if (diff<tol || diff>2*M_PI-tol) {
        //std::cout << "GV_Math::isSameDirection: " << dir1 << " and " << dir2
        //    << " point in the same direction" << "\n";
        return true;
    }
    else {
        //std::cout << "GV_Math::isSameDirection: " << dir1 << " and " << dir2
        //    << " don't point in the same direction" << "\n";
        return false;
    }
}


/**
 * Compares two double values with a tolerance.
 */
bool GV_Math::cmpDouble(double v1, double v2, double tol) {
    return (fabs(v2-v1)<tol);
}



/**
 * Evaluates a mathematical expression and returns the result.
 * If an error occured, the given default value 'def' will be returned.
 */
double GV_Math::eval(const QString& expr, double def) {

    bool ok;
    double res = GV_Math::eval(expr, &ok);

    if (!ok) {
        //std::cerr << "GV_Math::evaluate: Parse error at col "
        //<< ret << ": " << fp.ErrorMsg() << "\n";
        return def;
    }

    return res;
}



/**
 * Converts a double into a string which is as short as possible
 *
 * @param value The double value
 * @param prec Precision e.g. a precision of 1 would mean that a 
 *     value of 2.12030 will be converted to "2.1". 2.000 is always just "2").
 */
QString GV_Math::doubleToString(double value, double prec,
        bool /*showLeadingZeroes*/, bool /*showTrailingZeroes*/) {

    QString ret;
    QString exaStr;
    int dotPos;
    int num;
    if (prec>1.0e-12) {
        num = GV_Math::mround(value / prec);
    }
    else {
        num = GV_Math::mround(value);
    }

    exaStr = GV_Math::doubleToString(prec, 10);
    dotPos = exaStr.indexOf('.');

    if (dotPos==-1) {
        ret.sprintf("%d", GV_Math::mround(num*prec));
    } else {
        int digits = exaStr.length() - dotPos - 1;
        ret = GV_Math::doubleToString(num*prec, digits);
    }

    return ret;
}




/**
 * Converts a double into a string which is as short as possible.
 *
 * @param value The double value
 * @param prec Precision
 */
QString GV_Math::doubleToString(double value, int prec,
        bool /*showLeadingZeroes*/, bool showTrailingZeroes) {

//    RB_DEBUG->print("GV_Math::doubleToString: %f", value);

    QString ret;

    QString formatString;
    if (showTrailingZeroes) {
        formatString = QString("%.0%1f").arg(prec);
    }
    else {
        formatString = QString("%.%1f").arg(prec);
    }
    
    // avoid banker's rounding, always round 0.5 up and -0.5 down:
    double fuzz = 1.0e-13;
    if (value<0.0) {
        fuzz*=-1;
    }

    ret.sprintf(formatString.toLatin1(), value + fuzz);
    
    if (!showTrailingZeroes) {
        if (ret.contains('.')) {
            // Remove zeros at the end:
            while (ret.at(ret.length()-1)=='0') {
                ret.truncate(ret.length()-1);
            }
     
            if(ret.at(ret.length()-1)=='.') {
                ret.truncate(ret.length()-1);
            }
        }
    }

    return ret;

    /*
    QString valStr;

    value *= exp10(prec);
    value = GV_Math::mround(value);
    value /= exp10(prec);
    
    valStr.setNum(value, 'f', prec);

    if (valStr.contains('.')) {
        // Remove zeros at the end:
        while (valStr.at(valStr.length()-1)=='0') {
            valStr.truncate(valStr.length()-1);
        }
 
        if(valStr.at(valStr.length()-1)=='.') {
            valStr.truncate(valStr.length()-1);
        }
    }
    
    return valStr;
    */
}



/**
 * @returns on which side of the given line (lineStart/lineEnd) the given point is.
 */
GV2::Side GV_Math::getSideOfPoint(const GV_Vector& lineStart, const GV_Vector& lineEnd,
        const GV_Vector& point) {
    
    double entityAngle = lineStart.angleTo(lineEnd);
    double angleToCoord = lineStart.angleTo(point);
    double angleDiff = GV_Math::getAngleDifference(entityAngle, angleToCoord);

    if (angleDiff<M_PI) {
        return GV2::LeftHand;
    }
    else {
        return GV2::RightHand;
    }
}



/**
 * @returns distance from the given line (lineStart/lineEnd) to the given point.
 */
double GV_Math::getDistanceToPoint(const GV_Vector& lineStart, const GV_Vector& lineEnd,
        const GV_Vector& point) {

    double dist = lineStart.distanceTo(point);

    GV_Vector ae = lineEnd-lineStart;
    GV_Vector ea = lineStart-lineEnd;
    GV_Vector ap = point-lineStart;
//    GV_Vector ep = point-lineEnd;

    if (ae.magnitude()<1.0e-6 || ea.magnitude()<1.0e-6) {
        return dist;
    }

    // Orthogonal projection from both sides:
//    GV_Vector ba = ae * GV_Vector::dotP(ae, ap) /
//                   GV_Math::pow(ae.magnitude(), 2);
//    GV_Vector be = ea * GV_Vector::dotP(ea, ep) /
//                   GV_Math::pow(ea.magnitude(), 2);

    GV_Vector cp = GV_Vector::crossP(ap, ae);
    dist = cp.magnitude() / ae.magnitude();

    return dist;

}
