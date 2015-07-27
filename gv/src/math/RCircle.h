#ifndef RCIRCLE_H
#define RCIRCLE_H

#include "RShape.h"
#include "RVector.h"

class RBox;

/**
 * Low-level mathematical representation of a circle.
 *
 * \ingroup math
 * \scriptable
 */
class RCircle: public RShape {
public:
	RCircle();
	RCircle(const RVector& center, const double radius);
	virtual ~RCircle();

	virtual RBox getBoundingBox() const;

	virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance) const;

	virtual RVector getVectorTo(const RVector& point,
			bool limited = true) const;
//	virtual RVector getVectorTo(const RLine& line,
//			bool limited = true) const;
//	virtual bool intersectsWith(const RShape& other, bool limited = true) const;
//	virtual QList<RVector> getIntersectionPoints(const RShape& other, bool limited = true) const;

    RVector getCenter() const;
	void setCenter(const RVector& vector);
    double getRadius() const;
	void setRadius(double radius);

    virtual bool move(const RVector& offset);
    virtual bool rotate(double rotation, const RVector& center = RVector());
    virtual bool scale(const RVector& scaleFactors, const RVector& center = RVector());
    virtual bool mirror(const RLine& axis);
    virtual bool flipHorizontal();
    virtual bool flipVertical();

public:
	/**
	 * \getter{getCenter}
	 * \setter{setCenter}
	 */
	RVector center;
	/**
	 * \getter{getRadius}
	 * \setter{setRadius}
	 */
	double radius;
};

Q_DECLARE_METATYPE(const RCircle*)
Q_DECLARE_METATYPE(RCircle*)
Q_DECLARE_METATYPE(RCircle)

#endif
