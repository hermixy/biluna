#include "RPointData.h"
#include "RPointEntity.h"

RPointData::RPointData() {
}

RPointData::RPointData(RDocument* document, const RPointData& data)
    : REntityData(document) {
    *this = data;
    this->document = document;
    if (document!=NULL) {
        linetypeId = document->getLinetypeId("BYLAYER");
    }
}

RPointData::RPointData(const RVector& point) :
	RPoint(point) {
}

/*
RVector RPointData::getPosition() const {
    return RPoint::position;
}

RBox RPointData::getBoundingBox() const {
    return RPoint::getBoundingBox();
}
*/

QList<RVector> RPointData::getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/) const {
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

/*
QList<RVector> RPointData::getEndPoints() const {
    return RPoint::getEndPoints();
}

QList<RVector> RPointData::getMiddlePoints() const {
    QList<RVector> ret;
    ret.append(getPosition());
    return ret;
}

QList<RVector> RPointData::getCenterPoints() const {
    return getMiddlePoints();
}

RVector RPointData::getClosestPointOnEntity(const RVector& point) const {
    return getClosestPointOnShape(point);
}

QList<RVector> RPointData::getIntersectionPoints(
    const REntityData& other, bool limited) const {

//    const RPointData* otherLine = dynamic_cast<const RPointData*>(&other);
//    if (otherLine!=NULL) {
//        return RPoint::getIntersectionPoints(*otherLine, limited);
//    }
    // TODO other entity types

    return QList<RVector>();
}

RVector RPointData::getVectorTo(const RVector& point, bool limited) const {
    return RPoint::getVectorTo(point, limited);
}

RVector RPointData::getVectorTo(const RLine& line, bool limited) const {
	return RPoint::getVectorTo(line, limited);
}

QList<RVector> RPointData::getPointsWithDistanceToEnd(double distance) const {
    return QList<RVector>();
}

bool RPointData::intersectsWith(const RTriangle& triangle) const {
    return RPoint::intersectsWith(triangle);
}
*/

bool RPointData::moveReferencePoint(const RVector& referencePoint,
		const RVector& targetPoint) {
	bool ret = false;
	if (referencePoint.getDistanceTo(position) < RS::PointTolerance) {
		position = targetPoint;
		ret = true;
	}
	return ret;
}
