#ifndef RSHAPE_H
#define RSHAPE_H

#include <QList>

#include "RVector.h"
#include "RMath.h"

class RBox;
class RArc;
class RLine;

/**
 * Interface for geometrical shape classes.
 *
 * \ingroup math
 * \scriptable
 */
class RShape {
public:
	RShape() {
	}

	virtual ~RShape() {
	}

	virtual RVector getClosestPointOnShape(const RVector& p, bool limited =
            true) const;

	/**
	 * \return The bounding box of this shape.
	 */
	virtual RBox getBoundingBox() const = 0;

	/**
	 * \return Shortest vector from this shape to the given point.
	 *
	 * \param point the given point
	 *
	 * \param limited If false, the shape is treaded as an unlimited
	 *      shape that extends in a logical way. For example an
	 *      unlimited line for a line, full circle for an arc or
	 *      a plane for a triangle.
	 */
	virtual RVector getVectorTo(const RVector& point,
			bool limited = true) const = 0;

	/**
	 * \return Shortest distance from this shape to the given point.
	 *      Based on \ref getVectorTo.
	 */
	virtual double getDistanceTo(const RVector& point, bool limited = true) const {
		RVector v = getVectorTo(point, limited);
		if (v.isValid()) {
			return v.getMagnitude();
		}
		return RNANDOUBLE;
	}

	/**
	 * \return true if the given point is on this shape.
	 */
	virtual bool isOnShape(const RVector& point, bool limited = true) const {
		double d = getDistanceTo(point, limited);
                if (std::isnan(d)) {
			return false;
		}
		return d < RS::PointTolerance;
	}

	/**
	 * \return Shortest vector from any end point of this shape
	 *      to the given point.
	 */
	virtual RVector getVectorFromEndpointTo(const RVector& point) const {
        QList<RVector> endPoints = getEndPoints();
        RVector closest = point.getClosest(endPoints);
        return point - closest;
    }

    /**
     * \return The end point(s) of this shape.
     */
    virtual QList<RVector> getEndPoints() const = 0;

    /**
     * \return The middle point(s) of this shape.
     */
    virtual QList<RVector> getMiddlePoints() const = 0;

    /**
     * \return The center point(s) of this shape.
     */
    virtual QList<RVector> getCenterPoints() const = 0;

    /**
     * \return All points on this shape with the given distance to an endpoint.
     */
    virtual QList<RVector> getPointsWithDistanceToEnd(
        double distance) const = 0;

	virtual bool intersectsWith(const RShape& other, 
        bool limited = true) const;

    QList<RVector> getIntersectionPoints(const RShape& other,
            bool limited = true) const;

	/**
	 * \return The intersection point(s) between this shape and the given
	 *      other shape.
	 */
	static QList<RVector> getIntersectionPoints(const RShape& shape1,
			const RShape& shape2, bool limited = true);
	static QList<RVector> getIntersectionPoints(const RArc& arc1,
			const RArc& arc2, bool limited = true);
	static QList<RVector> getIntersectionPoints(const RLine& line1,
			const RArc& arc2, bool limited = true);
	static QList<RVector> getIntersectionPoints(const RLine& line1,
			const RLine& line2, bool limited = true);

    virtual bool move(const RVector& offset)=0;
	virtual bool rotate(double rotation, const RVector& center = RVector())=0;
	virtual bool scale(const RVector& scaleFactors, const RVector& center =
			RVector())=0;
	virtual bool mirror(const RLine& axis)=0;
    virtual bool flipHorizontal();
    virtual bool flipVertical();
};

Q_DECLARE_METATYPE(RShape*)
Q_DECLARE_METATYPE(const RShape*)
Q_DECLARE_METATYPE(QList<RShape*>)
Q_DECLARE_METATYPE(QList<const RShape*>)

#endif
