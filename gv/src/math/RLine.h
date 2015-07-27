#ifndef RLINE_H
#define RLINE_H

#include "RShape.h"
#include "RVector.h"

class RBox;

/**
 * Low-level mathematical representation of a line.
 *
 * \ingroup math
 * \scriptable
 */
class RLine: public RShape {
public:
	RLine();
	RLine(const RVector& startPoint, const RVector& endPoint);
	virtual ~RLine();

	virtual RBox getBoundingBox() const;

	virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance) const;

    virtual RVector getVectorTo(const RVector& point,
			bool limited = true) const;
    //virtual RVector getVectorTo(const RLine& line,
    //		bool limited = true) const;
//	virtual bool intersectsWith(const RShape& other, bool limited = true) const;
//	virtual QList<RVector> getIntersectionPoints(const RShape& other, bool limited = true) const;

    RVector getStartPoint() const;
	void setStartPoint(const RVector& vector);
    RVector getEndPoint() const;
	void setEndPoint(const RVector& vector);

    RVector getMiddlePoint() const;
    
    double getLength() const;
    double getAngle() const;

    void clipToXY(const RBox& box);

    virtual bool move(const RVector& offset);
    virtual bool rotate(double rotation, const RVector& center = RVector());
    virtual bool scale(const RVector& scaleFactors, const RVector& center = RVector());
    virtual bool mirror(const RLine& axis);
    virtual bool flipHorizontal();
    virtual bool flipVertical();

public:
	/**
	 * \getter{getStartPoint}
	 * \setter{setStartPoint}
	 */
	RVector startPoint;
	/**
	 * \getter{getEndPoint}
	 * \setter{setEndPoint}
	 */
	RVector endPoint;
};

Q_DECLARE_METATYPE(const RLine*)
Q_DECLARE_METATYPE(RLine*)
Q_DECLARE_METATYPE(RLine)

#endif
