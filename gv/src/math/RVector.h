#ifndef RVECTOR_H
#define RVECTOR_H

#include <QDebug>
#include <QMetaType>
#include <QList>

#include "RGlobal.h"

class RMatrix;
class RBox;
class RLine;

/**
 * Represents a 3d vector (x/y/z).
 *
 * \ingroup math
 * \scriptable
 * \copyable
 */
class RVector {
public:
    /**
     * Constructor for a valid vector (0/0/0).
     */
    inline RVector() :
            x(0.0), y(0.0), z(0.0), valid(true) {
    }

    /**
     * Constructor for a point with given valid flag.
     *
     * \param v true: a valid vector with default coordinates is created.
     *              false: an invalid vector is created
     */
    //	inline explicit RVector(bool v) :
    //		x(0.0), y(0.0), z(0.0), valid(v) {
    //	}

    /**
     * Constructor for a point with given coordinates.
     */
    RVector(double vx, double vy, double vz = 0.0, bool valid_in = true);

    /**
     * Sets new values for the vector and makes the vector valid.
     */
    inline void set(double vx, double vy, double vz = 0.0) {
            x = vx;
            y = vy;
            z = vz;
            valid = true;
    }
    void setPolar(double radius, double angle);

    bool isValid() const;

    bool isInside(const RBox& b) const;

    double getDistanceTo(const RVector& v) const;
    double getDistanceTo2d(const RVector& v) const;
    double getAngle() const;
    double getAngleToPlaneXY() const;
    double getAngleTo(const RVector& v) const;
    double getMagnitude() const;
    double getMagnitude2d() const;
    RVector getLerp(const RVector& v, double t) const;
    RVector getUnitVector() const;
    void setX(double x);
    double getX();
    void setY(double y);
    double getY();
    void setZ(double z);
    double getZ();

    bool isInWindow(const RVector& firstCorner, const RVector& secondCorner);

    RVector move(const RVector& offset);
    RVector rotate(double rotation);
    RVector rotate(double rotation, const RVector& center);
    RVector scale(double factor, const RVector& center=nullVector);
    RVector scale(const RVector& factors, const RVector& center=nullVector);
    RVector mirror(const RLine& axis);
    RVector mirror(const RVector& axis1, const RVector& axis2);
    RVector flipHorizontal();
    RVector flipVertical();
    RVector isoProject(RS::IsoProjectionType type);
    RVector transform(const RMatrix& m);
    RVector transform2d(const RMatrix& m);

    RVector getTransformed(const RMatrix& m) const;
    RVector getTransformed2d(const RMatrix& m) const;

    RVector getDividedComponents(const RVector& v) const;
    RVector getMultipliedComponents(const RVector& v) const;

    RVector getClosest(const QList<RVector>& v) const;

    RVector operator +(const RVector& v) const;
    RVector operator -(const RVector& v) const;
    RVector operator *(double s) const;
    RVector operator /(double s) const;
    RVector operator -() const;
    RVector negate() const;

    void operator +=(const RVector& v);
    void operator -=(const RVector& v);
    void operator *=(double s);
    void operator /=(double s);

    bool operator ==(const RVector& v) const;
    bool operator !=(const RVector& v) const {
            return !operator==(v);
    }

    RVector getFloor() const;
    RVector getCeil() const;

    static RVector getMinimum(const QList<RVector>& vectors);
    static RVector getMaximum(const QList<RVector>& vectors);

    static RVector getMinimum(const RVector& v1, const RVector& v2);
    static RVector getMaximum(const RVector& v1, const RVector& v2);

    static RVector getCrossProduct(const RVector& v1, const RVector& v2);
    static double getDotProduct(const RVector& v1, const RVector& v2);
    static RVector createPolar(double radius, double angle) {
            RVector ret;
            ret.setPolar(radius, angle);
            return ret;
    }

public:
    /**
     * \getter{getX}
     * \setter{setX}
     */
    double x;

    /**
     * \getter{getY}
     * \setter{setY}
     */
    double y;

    /**
     * \getter{getZ}
     * \setter{setZ}
     */
    double z;

    /**
     * \getter{isValid}
     */
    bool valid;

    /**
     * invalid vector
     */
    static const RVector invalid;

    /**
     * null vector
     */
    static const RVector nullVector;
};

RVector operator*(double s, const RVector& v);

QDebug operator<<(QDebug dbg, const RVector& v);

Q_DECLARE_METATYPE(QList<RVector>)
Q_DECLARE_METATYPE(QList<RVector>*)
typedef QMultiMap<int, RVector> _RMapIntVector;
Q_DECLARE_METATYPE(_RMapIntVector*)
Q_DECLARE_METATYPE(RVector)
Q_DECLARE_METATYPE(RVector*)
Q_DECLARE_METATYPE(const RVector*)

#endif
