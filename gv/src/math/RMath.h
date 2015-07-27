#ifndef RMATH_H
#define RMATH_H

#include <QMetaType>

#include <cstdio>
#include <cmath>
#include <cerrno>
#include <limits>

#include "RVector.h"

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2        1.57079632679489661923
#endif

#ifndef exp10
#define exp10(x) pow(10,x)
#endif

#define ARAD 57.29577951308232


// Somewhere in the guts of Visual C++ a macro 'max' is defined which
// breaks std::numeric_limits<double>::max(). This fix is not correct
// but good enough for now.
#ifdef _MSC_VER
#define RMAXDOUBLE 1e10
#define RMINDOUBLE -1e10
#else
#define RMAXDOUBLE std::numeric_limits<double>::max()
#define RMINDOUBLE -std::numeric_limits<double>::max()
#endif

#ifndef RNANDOUBLE
#define RNANDOUBLE std::numeric_limits<double>::quiet_NaN()
#endif

/**
 * Math functions.
 *
 * \ingroup math
 * \scriptable
 */
class RMath {
public:
    /**
     * Rounds the given double to the next int.
     */
    static inline int mround(double v) {
#if defined(__GCC2x__) || defined(_MSC_VER)
        return (v-floor(v)<0.5 ? (int)floor(v) : (int)ceil(v));
#else
        return (int) round(v);
#endif
    }

    static double pow(double x, double y);
    static /*inline*/ bool isNormal(double v);

    static double eval(const QString& expression, bool* ok = NULL);
    static QString getError();
    static bool hasError();
    //static double stringToAngle(const QString& str);
    static QString angleToString(double a);

    static double rad2deg(double a);
    static double deg2rad(double a);
    static double rad2gra(double a);
    static double gra2deg(double a);

    static int getGcd(int a, int b);
    static bool isAngleBetween(double a, double a1, double a2, bool reversed);
    static double getNormalizedAngle(double a);
    static double getAngleDifference(double a1, double a2);
    static double getAngleDifference180(double a1, double a2);
    static double makeAngleReadable(double angle, bool readable = true,
                                    bool* corrected = NULL);
    static bool isAngleReadable(double angle, double tolerance = 1.0e-3);
    static bool isSameDirection(double dir1, double dir2, double tol = 1.0e-3);
    static int absmod(int a, int b);

    static void toFraction(double v, int maxDenominator, int& number, int& numerator, int& denominator);
    static void simplify(int numerator, int denominator, int& numeratorRes, int& denominatorRes);

    static bool fuzzyCompare(double v1, double v2, double tolerance=1.0e-12);

//	static RVector getLineTriangleIntersection(const RVector& lineP1,
//			const RVector& lineP2, const RVector& triangleP1,
//			const RVector& triangleP2, const RVector& triangleP3);
private:
    static QString lastError;
};

Q_DECLARE_METATYPE(RMath*)

#endif
