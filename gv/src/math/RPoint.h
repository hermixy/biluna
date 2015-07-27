#ifndef RPOINT_H
#define RPOINT_H

#include "RShape.h"
#include "RVector.h"
#include "RBox.h"

/**
 * Low-level mathematical representation of a point.
 *
 * \ingroup math
 * \scriptable
 * \copyable
 */
class RPoint: public RShape {
public:
	RPoint();
	RPoint(const RVector& position);
	RPoint(const RPoint& other);
	virtual ~RPoint();

    RVector getPosition() const {
       return position;
    }

	virtual RBox getBoundingBox() const;

	virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance) const;

    virtual RVector getVectorTo(const RVector& point,
			bool limited = true) const;
//	virtual RVector getVectorTo(const RLine& line,
//			bool limited = true) const;
//	virtual bool intersectsWith(const RShape& /*other*/, bool /*limited*/ = true) const {
//		return false;
//	}
//	virtual QList<RVector> getIntersectionPoints(const RShape& /*other*/,
//        bool /*limited*/ = true) const {
//        return QList<RVector>();
//	}

    virtual bool move(const RVector& offset);
    virtual bool rotate(double rotation, const RVector& center = RVector());
    virtual bool scale(const RVector& scaleFactors, const RVector& center = RVector());
    virtual bool mirror(const RLine& axis);
    virtual bool flipHorizontal();
    virtual bool flipVertical();

public:
	RVector position;
};

Q_DECLARE_METATYPE(RPoint)
Q_DECLARE_METATYPE(RPoint*)

#endif
