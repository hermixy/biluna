#include "RCircleData.h"
#include "RCircleEntity.h"

RCircleData::RCircleData() {
}

RCircleData::RCircleData(RDocument* document, const RCircleData& data) {
    *this = data;
    this->document = document;
}

RCircleData::RCircleData(const RVector& center, double radius) :
	RCircle(center, radius) {
}

/*
RVector RCircleData::getCenter() const {
    return RCircle::center;
}

double RCircleData::getRadius() const {
    return RCircle::radius;
}

RBox RCircleData::getBoundingBox() const {
    return RCircle::getBoundingBox();
}
*/

QList<RVector> RCircleData::getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/) const {
    QList<RVector> ret;
    ret.append(center);
    ret.append(center + RVector(radius, 0));
	ret.append(center + RVector(0, radius));
	ret.append(center - RVector(radius, 0));
	ret.append(center - RVector(0, radius));
    return ret;
}

/*
QList<RVector> RCircleData::getEndPoints() const {
    return RCircle::getEndPoints();
}

QList<RVector> RCircleData::getMiddlePoints() const {
    QList<RVector> ret;
    ret.append(getCenter());
    return ret;
}

QList<RVector> RCircleData::getCenterPoints() const {
    return getMiddlePoints();
}

RVector RCircleData::getClosestPointOnEntity(const RVector& point) const {
    return getClosestPointOnShape(point);
}

QList<RVector> RCircleData::getIntersectionPoints(
    const REntityData& other, bool limited) const {

//    const RPointData* otherLine = dynamic_cast<const RPointData*>(&other);
//    if (otherLine!=NULL) {
//        return RPoint::getIntersectionPoints(*otherLine, limited);
//    }
    // TODO other entity types

    return QList<RVector>();
}

RVector RCircleData::getVectorTo(const RVector& point, bool limited) const {
    return RCircle::getVectorTo(point, limited);
}

RVector RCircleData::getVectorTo(const RLine& line, bool limited) const {
	return RCircle::getVectorTo(line, limited);
}

QList<RVector> RCircleData::getPointsWithDistanceToEnd(double distance) const {
    return QList<RVector>();
}

bool RCircleData::intersectsWith(const RTriangle& triangle) const {
    return RCircle::intersectsWith(triangle);
}
*/

bool RCircleData::moveReferencePoint(const RVector& referencePoint,
		const RVector& targetPoint) {
	bool ret = false;
	if (referencePoint.getDistanceTo(center) < RS::PointTolerance) {
		center = targetPoint;
		ret = true;
	}
	return ret;
}
