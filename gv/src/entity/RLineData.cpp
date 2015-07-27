#include "RLineData.h"
#include "RLineEntity.h"

RLineData::RLineData() {
}

RLineData::RLineData(RDocument* document, const RLineData& data)
    : REntityData(document) {
    *this = data;
    this->document = document;
    if (document!=NULL) {
        linetypeId = document->getLinetypeId("BYLAYER");
    }
}

RLineData::RLineData(const RVector& startPoint, const RVector& endPoint) :
    RLine(startPoint, endPoint) {
}

/*
RVector RLineData::getStartPoint() const {
    return RLine::startPoint;
}

RVector RLineData::getEndPoint() const {
    return RLine::endPoint;
}
*/

QList<RVector> RLineData::getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/) const {
    QList<RVector> ret;
    ret.append(startPoint);
    ret.append(endPoint);
    return ret;
}

/*
QList<RVector> RLineData::getIntersectionPoints(
    const REntityData& other, bool limited) const {

    const RLineData* otherLine = dynamic_cast<const RLineData*>(&other);
    if (otherLine!=NULL) {
//        return RLine::getIntersectionPoints(*otherLine, limited);
    	return RShape::getIntersectionPoints(*this, *otherLine, limited);
    }
    // TODO other entity types

    return QList<RVector>();
}

RVector RLineData::getVectorTo(const RVector& point, bool limited) const {
    return RLine::getVectorTo(point, limited);
}

RVector RLineData::getVectorTo(const RLine& line, bool limited) const {
    return RLine::getVectorTo(line, limited);
}

QList<RVector> RLineData::getPointsWithDistanceToEnd(double distance) const {
    return RLine::getPointsWithDistanceToEnd(distance);
}
*/

/*
bool RLineData::intersectsWith(const RTriangle& triangle) const {
    return RLine::intersectsWith(triangle);
}
*/

bool RLineData::moveReferencePoint(const RVector& referencePoint,
        const RVector& targetPoint) {
    bool ret = false;
    if (referencePoint.getDistanceTo(startPoint) < RS::PointTolerance) {
        startPoint = targetPoint;
        ret = true;
    }
    if (referencePoint.getDistanceTo(endPoint) < RS::PointTolerance) {
        endPoint = targetPoint;
        ret = true;
    }
    return ret;
}
