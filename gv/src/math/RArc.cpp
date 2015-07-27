#include <cmath>

#include "../core/RS.h"
#include "RArc.h"
#include "RBox.h"
#include "RMath.h"

/**
 * Creates an arc shape with invalid start and end points.
 */
RArc::RArc() :
    center(RVector::invalid),
    radius(0.0),
    startAngle(0.0),
    endAngle(0.0),
    reversed(false){
}

RArc::RArc(const RVector& center, double radius, double startAngle,
		double endAngle, bool reversed) :
    center(center),
    radius(radius),
    startAngle(startAngle),
    endAngle(endAngle),
    reversed(reversed) {
}

RArc::~RArc() {
}

/**
 * Creates an arc from its startpoint, endpoint and bulge (= tan(angle/4)).
 */
bool RArc::createFrom2PBulge(const RVector& startPoint,
        const RVector& endPoint, double bulge) {
    reversed = (bulge < 0.0);
    double alpha = atan(bulge) * 4.0;

    RVector middle = (startPoint + endPoint) / 2.0;
    double dist = startPoint.getDistanceTo(endPoint) / 2.0;

    // alpha can't be 0.0 at this point
    radius = fabs(dist / sin(alpha / 2.0));

    double wu = fabs(RMath::pow(radius, 2.0) - RMath::pow(dist, 2.0));
    double h = sqrt(wu);
    double angle = startPoint.getAngleTo(endPoint);

    if (bulge > 0.0) {
        angle += M_PI / 2.0;
    } else {
        angle -= M_PI / 2.0;
    }

    if (fabs(alpha) > M_PI) {
        h *= -1.0;
    }

    center.setPolar(h, angle);
    center += middle;
    startAngle = center.getAngleTo(startPoint);
    endAngle = center.getAngleTo(endPoint);

    return true;
}


void RArc::moveStartpoint(const RVector& pos) {
	double bulge = getBulge();
	createFrom2PBulge(pos, getEndPoint(), bulge);
}

void RArc::moveEndpoint(const RVector& pos) {
	double bulge = getBulge();
	createFrom2PBulge(getStartPoint(), pos, bulge);
}

double RArc::getBulge() const {
	double bulge = tan(fabs(getSweep()) / 4.0);
	if (isReversed()) {
		bulge *= -1;
	}
	return bulge;
}

double RArc::getLength() const {
	return getSweep() * radius;
}

/**
 * \return Arc sweep in rad. The sweep is the angle covered by this arc.
 * Positive for ccw, negative for cw.
 */
double RArc::getSweep() const {
	double ret = 0.0;

	if (isReversed()) {
		if (startAngle < endAngle) {
			ret = -(startAngle + 2 * M_PI - endAngle);
		} else {
			ret = -(startAngle - endAngle);
		}
	} else {
		if (endAngle < startAngle) {
			ret = endAngle + 2 * M_PI - startAngle;
		} else {
			ret = endAngle - startAngle;
		}
	}

	// full circle:
//	if (!allowForZeroLength && fabs(ret) < 1.0e-6) {
//		ret = 2 * M_PI;
//	}

	return ret;
}

RVector RArc::getCenter() const{
	return center;
}

void RArc::setCenter(const RVector& vector){
	center = vector;
}

double RArc::getRadius() const{
	return radius;
}

void RArc::setRadius(double r){
	radius = r;
}

double RArc::getStartAngle() const {
	return startAngle;
}

void RArc::setStartAngle(double a) {
	startAngle = a;
}

double RArc::getEndAngle() const {
	return endAngle;
}

void RArc::setEndAngle(double a) {
	endAngle = a;
}

RVector RArc::getMiddlePoint() const {
    return center;
}

RVector RArc::getStartPoint() const {
	return RVector(center.x + cos(startAngle) * radius, center.y + sin(
			startAngle) * radius);
}

RVector RArc::getEndPoint() const {
	return RVector(center.x + cos(endAngle) * radius, center.y + sin(
			endAngle) * radius);
}

bool RArc::isReversed() const {
	return reversed;
}

void RArc::setReversed(bool r) {
	reversed = r;
}

RBox RArc::getBoundingBox() const {
	RVector minV;
	RVector maxV;
    double minX = qMin(getStartPoint().x, getEndPoint().x);
    double minY = qMin(getStartPoint().y, getEndPoint().y);
    double maxX = qMax(getStartPoint().x, getEndPoint().x);
    double maxY = qMax(getStartPoint().y, getEndPoint().y);

    if (getStartPoint().getDistanceTo(getEndPoint()) < 1.0e-6 && getRadius()
			> 1.0e5) {
		minV = RVector(minX, minY);
		maxV = RVector(maxX, maxY);
		return RBox(minV, maxV);
	}

    double a1 = !isReversed() ? startAngle : endAngle;
    double a2 = !isReversed() ? endAngle : startAngle;

    // check for left limit:
    if ((a1<M_PI && a2>M_PI) ||
            (a1>a2-1.0e-12 && a2>M_PI) ||
            (a1>a2-1.0e-12 && a1<M_PI) ) {

        minX = qMin(center.x - radius, minX);
    }

    // check for right limit:
    if (a1 > a2-1.0e-12) {
        maxX = qMax(center.x + radius, maxX);
    }

    // check for bottom limit:
    if ((a1<(M_PI_2*3) && a2>(M_PI_2*3)) ||
            (a1>a2-1.0e-12    && a2>(M_PI_2*3)) ||
            (a1>a2-1.0e-12    && a1<(M_PI_2*3)) ) {

        minY = qMin(center.y - radius, minY);
    }

    // check for top limit:
    if ((a1<M_PI_2 && a2>M_PI_2) ||
            (a1>a2-1.0e-12   && a2>M_PI_2) ||
            (a1>a2-1.0e-12   && a1<M_PI_2) ) {

        maxY = qMax(center.y + radius, maxY);
    }

    minV = RVector(minX, minY);
    maxV = RVector(maxX, maxY);

	return RBox(minV, maxV);
}

QList<RVector> RArc::getEndPoints() const {
    QList<RVector> ret;
    ret.append(getStartPoint());
    ret.append(getEndPoint());
    return ret;
}

QList<RVector> RArc::getMiddlePoints() const {
    QList<RVector> ret;

    double a;
    if (reversed) {
        a = startAngle - getLength()/2.0;
    } else {
        a = startAngle + getLength()/2.0;
    }
    RVector v = RVector::createPolar(radius, a);
    v+=center;
    ret.append(v);

    return ret;
}

QList<RVector> RArc::getCenterPoints() const {
    QList<RVector> ret;
    ret.append(getCenter());
    return ret;
}

QList<RVector> RArc::getPointsWithDistanceToEnd(double distance) const {
    QList<RVector> ret;

    if (radius<RS::PointTolerance) {
        return ret;
    }

    double a1;
    double a2;
    RVector p;
    double aDist = distance / radius;

    if (isReversed()) {
        a1 = getStartAngle() - aDist;
        a2 = getEndAngle() + aDist;
    } else {
        a1 = getStartAngle() + aDist;
        a2 = getEndAngle() - aDist;
    }
    p.setPolar(radius, a1);
    p += center;
    ret.append(p);

    p.setPolar(radius, a2);
    p += center;
    ret.append(p);

    return ret;
}

RVector RArc::getVectorTo(const RVector& point, bool limited) const {
	double angle = center.getAngleTo(point);
	if (limited
			&& !RMath::isAngleBetween(angle, startAngle, endAngle, reversed)) {
		return RVector::invalid;
	}

	RVector v = point - center;
	return RVector::createPolar(v.getMagnitude() - radius, v.getAngle());
}

/**
 * \todo implement limited param
 */
/*
RVector RArc::getVectorTo(const RLine& line, bool limited) const {
	RTriangle t(center, RVector(center.x + radius, center.y, center.z),
			RVector(center.x, center.y + radius, center.z));
//	QList<RVector> l = t.getIntersectionPoints(line, false);
	QList<RVector> l = RShape::getIntersectionPoints(t, line, false);
	if (!l.isEmpty()) {
		return getVectorTo(l.at(0), limited);
	}
	return RVector::invalid;
}
*/

/**
 * \copydoc RShape::intersectsWith
 *
 * \todo implement for all other shapes
 */
//bool RArc::intersectsWith(const RShape& other, bool limited) const {
//	const RTriangle* triangle = dynamic_cast<const RTriangle*> (&other);
//	if (triangle != NULL) {
//		return triangle->intersectsWith(*this, limited);
//	}
//	return false;
//}

//QList<RVector> RArc::getIntersectionPoints(const RShape& other,
//    bool limited) const {
//
////	const RCircle* line = dynamic_cast<const RCircle*> (&other);
////	if (line != NULL) {
////        QList<RVector> res;
////        double a1 = endPoint.y - startPoint.y;
////        double b1 = startPoint.x - endPoint.x;
////        double c1 = a1*startPoint.x + b1*startPoint.y;
////
////        double a2 = line->endPoint.y - line->startPoint.y;
////        double b2 = line->startPoint.x - line->endPoint.x;
////        double c2 = a2*line->startPoint.x + b2*line->startPoint.y;
////
////        double det = a1*b2 - a2*b1;
////        if (fabs(det) < 1.0e-6) {
////            return res;
////        }else{
////            RVector v(
////                (b2*c1 - b1*c2)/det,
////                (a1*c2 - a2*c1)/det
////            );
////
////            if (!limited || (isOnShape(v) && line->isOnShape(v))) {
////                res.append(v);
////                return res;
////            }
////        }
////        return res;
////	}
////
////	const RTriangle* triangle = dynamic_cast<const RTriangle*> (&other);
////	if (triangle != NULL) {
////		return triangle->getIntersectionPoints(*this, limited);
////	}
//
//    return QList<RVector>();
//}

bool RArc::move(const RVector& offset) {
    if (!offset.isValid() || offset.getMagnitude() < RS::PointTolerance) {
        return false;
    }
    center += offset;
    return true;
}

bool RArc::rotate(double rotation, const RVector& c) {
	if (fabs(rotation) < RS::AngleTolerance) {
		return false;
	}

	center.rotate(rotation, c);
	startAngle = RMath::getNormalizedAngle(startAngle + rotation);
	endAngle = RMath::getNormalizedAngle(endAngle + rotation);

	return true;
}

bool RArc::scale(const RVector& scaleFactors, const RVector& c) {
	// negative scaling: mirroring
	if (scaleFactors.x < 0.0) {
		mirror(RLine(center, center + RVector(0.0, 1.0)));
	}
	if (scaleFactors.y < 0.0) {
		mirror(RLine(center, center + RVector(1.0, 0.0)));
	}

	center.scale(scaleFactors, c);
	radius *= scaleFactors.x;
	if (radius < 0.0) {
		radius *= -1.0;
	}

	return true;
}

bool RArc::mirror(const RLine& axis) {
    center.mirror(axis);
    reversed = (!reversed);

    RVector v;
	v.setPolar(1.0, startAngle);
	v.mirror(RVector(0.0, 0.0), axis.endPoint - axis.startPoint);
	startAngle = v.getAngle();

	v.setPolar(1.0, endAngle);
	v.mirror(RVector(0.0, 0.0), axis.endPoint - axis.startPoint);
	endAngle = v.getAngle();

	return true;
}

bool RArc::reverse() {
	double a = startAngle;
	startAngle = endAngle;
	endAngle = a;
	reversed = !reversed;
	return true;
}
