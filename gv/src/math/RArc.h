#ifndef RARC_H
#define RARC_H

#include "RShape.h"
#include "RVector.h"
#include "../core/RS.h"

class RBox;

/**
 * Low-level mathematical representation of an arc.
 *
 * \ingroup math
 * \scriptable
 */
class RArc: public RShape {
public:
	RArc();
	RArc(const RVector& center, double radius, double startAngle,
			double endAngle, bool reversed = false);
	virtual ~RArc();

    bool createFrom2PBulge(const RVector& startPoint, const RVector& endPoint,
            double bulge);

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
    double getStartAngle() const;
	void setStartAngle(double startAngle);
    double getEndAngle() const;
	void setEndAngle(double endAngle);
	double getSweep() const;
	double getLength() const;

    RVector getStartPoint() const;
    RVector getEndPoint() const;
    RVector getMiddlePoint() const;

    bool isReversed() const;
    void setReversed(bool reversed);

    void moveStartpoint(const RVector& pos);
	void moveEndpoint(const RVector& pos);
	double getBulge() const;

    virtual bool move(const RVector& offset);
	virtual bool rotate(double rotation, const RVector& center = RVector());
	virtual bool scale(const RVector& scaleFactors, const RVector& center =
			RVector());
	virtual bool mirror(const RLine& axis);
	bool reverse();

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
	/**
	 * \getter{getStartAngle}
	 * \setter{setStartAngle}
	 */
	double startAngle;
	/**
	 * \getter{getEndAngle}
	 * \setter{setEndAngle}
	 */
	double endAngle;
	/**
	 * \getter{isReversed}
	 * \setter{setReversed}
	 */
	bool reversed;
};

Q_DECLARE_METATYPE(const RArc*)
Q_DECLARE_METATYPE(RArc*)
Q_DECLARE_METATYPE(RArc)

#endif
