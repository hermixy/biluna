#include <cmath>

#include "RS.h"
#include "RShape.h"
#include "RArc.h"
#include "RLine.h"
#include "RDebug.h"
#include "RMath.h"

/**
 * \return Point on this shape that is closest to p. Based on getVectorTo.
 */
RVector RShape::getClosestPointOnShape(const RVector& p, bool limited) const {
    RVector dv = getVectorTo(p, limited);
    if (!dv.isValid()) {
        return RVector::invalid;
    }
    return p - dv;
}

/**
 * \return True if this shape intersects with the given shape.
 */
bool RShape::intersectsWith(const RShape& other, bool limited) const {
    return !getIntersectionPoints(other, limited).isEmpty();
}

/**
 * \return The intersection point(s) between this shape and the given
 *      other shape.
 */
QList<RVector> RShape::getIntersectionPoints(const RShape& other,
        bool limited) const {
    return getIntersectionPoints(*this, other, limited);
}


QList<RVector> RShape::getIntersectionPoints(const RShape& shape1,
		const RShape& shape2, bool limited) {

	const RLine* line1 = dynamic_cast<const RLine*> (&shape1);
	if (line1 != NULL) {
		const RLine* line2 = dynamic_cast<const RLine*> (&shape2);
		if (line2 != NULL) {
			return getIntersectionPoints(*line1, *line2, limited);
		}
		const RArc* arc2 = dynamic_cast<const RArc*> (&shape2);
		if (arc2 != NULL) {
			return getIntersectionPoints(*line1, *arc2, limited);
		}
	}

	const RArc* arc1 = dynamic_cast<const RArc*> (&shape1);
	if (arc1 != NULL) {
		const RLine* line2 = dynamic_cast<const RLine*> (&shape2);
		if (line2 != NULL) {
            return getIntersectionPoints(*line2, *arc1, limited);
		}
		const RArc* arc2 = dynamic_cast<const RArc*> (&shape2);
		if (arc2 != NULL) {
			return getIntersectionPoints(*arc1, *arc2, limited);
		}
	}

	RDebug::warning(
			"RShape::getIntersectionPoints(): intersection calculation not implemented for given shapes");
	return QList<RVector>();
}

QList<RVector> RShape::getIntersectionPoints(const RArc& arc1,
		const RArc& arc2, bool limited) {
    QList<RVector> res;

    RVector c1 = arc1.getCenter();
    RVector c2 = arc2.getCenter();

    double r1 = arc1.getRadius();
    double r2 = arc2.getRadius();

    RVector u = c2 - c1;

    // concentric
    if (u.getMagnitude() < RS::PointTolerance) {
		return res;
	}

    RVector v = RVector(u.y, -u.x);

    double s, t1, t2, term;

    s = 1.0/2.0 * ((r1*r1 - r2*r2)/(RMath::pow(u.getMagnitude(), 2.0)) + 1.0);

    term = (r1*r1)/(RMath::pow(u.getMagnitude(), 2.0)) - s*s;

    // no intersection:
	if (term < 0.0) {
		return res;
	}

    // one or two intersections:
	t1 = sqrt(term);
	t2 = -sqrt(term);
	bool tangent = false;

	RVector sol1 = c1 + u*s + v*t1;
	RVector sol2 = c1 + u*s + v*t2;

	if (sol1.getDistanceTo(sol2) < 1.0e-4) {
		if (!limited || (arc1.isOnShape(sol1) && arc2.isOnShape(sol1))) {
			res.append(sol1);
		}
		tangent = true;
	} else {
		if (!limited || (arc1.isOnShape(sol1) && arc2.isOnShape(sol1))) {
			res.append(sol1);
		}
		if (!limited || (arc1.isOnShape(sol2) && arc2.isOnShape(sol2))) {
			res.append(sol2);
		}
	}

//	ret.setTangent(tangent);

    return res;
}


QList<RVector> RShape::getIntersectionPoints(const RLine& line1,
		const RArc& arc2, bool limited) {
    QList<RVector> res;

    RVector vLineCenter = line1.getVectorTo(arc2.getCenter(), false);
	double dist = vLineCenter.getMagnitude();

    // special case: arc touches line (tangent):
	if (fabs(dist - arc2.getRadius()) < 1.0e-4) {
		res.append(arc2.getCenter() - vLineCenter);
		// ret.setTangent(true);
		return res;
	}

    RVector p = line1.getStartPoint();
	RVector d = line1.getEndPoint() - line1.getStartPoint();
    if (d.getMagnitude() < 1.0e-6) {
		return res;
	}

    RVector delta = p - arc2.getCenter();

    // root term:
    double term = RMath::pow(RVector::getDotProduct(d, delta), 2.0)
                  - RMath::pow(d.getMagnitude(), 2.0)
                  * (RMath::pow(delta.getMagnitude(), 2.0) - RMath::pow(arc2.getRadius(), 2.0));

    // no intersection:
    if (term<0.0) {
        return res;
    }

    // one or two intersections:
	double t1 = (- RVector::getDotProduct(d, delta) + sqrt(term))
				/ RMath::pow(d.getMagnitude(), 2.0);
	double t2;
	bool tangent = false;

	// only one intersection:
	if (fabs(term) < RS::PointTolerance) {
		t2 = t1;
		tangent = true;
	}

	// two intersections
	else {
		t2 = (-RVector::getDotProduct(d, delta) - sqrt(term))
			 / RMath::pow(d.getMagnitude(), 2.0);
	}

	RVector sol1;
	RVector sol2 = RVector::invalid;

	sol1 = p + d * t1;

	if (!tangent) {
		sol2 = p + d * t2;
	}

	if (!limited || (line1.isOnShape(sol1) && arc2.isOnShape(sol1))) {
		res.append(sol1);
	}
    if (sol2.isValid()) {
        if (!limited || (line1.isOnShape(sol2) && arc2.isOnShape(sol2))) {
            res.append(sol2);
        }
	}
	// ret.setTangent(tangent);

	return res;
}

QList<RVector> RShape::getIntersectionPoints(const RLine& line1,
		const RLine& line2, bool limited) {
	QList<RVector> res;
	double a1 = line1.endPoint.y - line1.startPoint.y;
	double b1 = line1.startPoint.x - line1.endPoint.x;
	double c1 = a1 * line1.startPoint.x + b1 * line1.startPoint.y;

	double a2 = line2.endPoint.y - line2.startPoint.y;
	double b2 = line2.startPoint.x - line2.endPoint.x;
	double c2 = a2 * line2.startPoint.x + b2 * line2.startPoint.y;

	double det = a1 * b2 - a2 * b1;
	if (fabs(det) < 1.0e-6) {
		return res;
	} else {
		RVector v((b2 * c1 - b1 * c2) / det, (a1 * c2 - a2 * c1) / det);

		if (!limited || (line1.isOnShape(v) && line2.isOnShape(v))) {
			res.append(v);
			return res;
		}
	}
	return res;
}

bool RShape::flipHorizontal() {
    return mirror(RLine(RVector(0,0), RVector(0,1)));
}

bool RShape::flipVertical() {
    return mirror(RLine(RVector(0,0), RVector(1,0)));
}
