#include <cmath>

#include "RCircle.h"
#include "RBox.h"
#include "RTriangle.h"

/**
 * Creates a line object with invalid start and end points.
 */
RCircle::RCircle() {
}

RCircle::RCircle(const RVector& center, const double radius) :
	center(center), radius(radius) {
}

RCircle::~RCircle() {
}

RVector RCircle::getCenter() const{
	return center;
}

void RCircle::setCenter(const RVector& vector){
	center = vector;
}

double RCircle::getRadius() const{
	return radius;
}

void RCircle::setRadius(double r){
	radius = r;
}

RBox RCircle::getBoundingBox() const {
	return RBox(center - RVector(radius, radius), center + RVector(radius,
			radius));
}

QList<RVector> RCircle::getEndPoints() const {
    QList<RVector> ret;
    return ret;
}

QList<RVector> RCircle::getMiddlePoints() const {
    QList<RVector> ret;
    return ret;
}

QList<RVector> RCircle::getCenterPoints() const {
    QList<RVector> ret;
    ret.append(center);
    return ret;
}

QList<RVector> RCircle::getPointsWithDistanceToEnd(double /*distance*/) const {
    QList<RVector> ret;
    return ret;
}

RVector RCircle::getVectorTo(const RVector& point, bool /*limited*/) const {
	RVector v = point - center;
	return RVector::createPolar(v.getMagnitude() - radius, v.getAngle());
}

/**
 * \todo implement limited param
 */
/*
RVector RCircle::getVectorTo(const RLine& line, bool limited) const {
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
//bool RCircle::intersectsWith(const RShape& other, bool limited) const {
//	// TODO
////	const RTriangle* triangle = dynamic_cast<const RTriangle*> (&other);
////	if (triangle != NULL) {
////		return triangle->intersectsWith(*this, limited);
////	}
//	return false;
//}

//QList<RVector> RCircle::getIntersectionPoints(const RShape& other,
//    bool limited) const {
//	// TODO
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

bool RCircle::move(const RVector& offset) {
    if (!offset.isValid() || offset.getMagnitude() < RS::PointTolerance) {
        return false;
    }
    center += offset;
    return true;
}

bool RCircle::rotate(double rotation, const RVector& c){
	if (fabs(rotation) < RS::AngleTolerance) {
		return false;
	}
	center.rotate(rotation, c);
	return true;
}

bool RCircle::scale(const RVector& scaleFactors, const RVector& c) {
	center.scale(scaleFactors, c);
    radius *= scaleFactors.x;
    if (radius < 0.0) {
        radius *= -1.0;
    }
    return true;
}

bool RCircle::mirror(const RLine& axis) {
    center.mirror(axis);
    return true;
}

bool RCircle::flipHorizontal() {
	center.flipHorizontal();
    return true;
}

bool RCircle::flipVertical() {
	center.flipVertical();
    return true;
}
