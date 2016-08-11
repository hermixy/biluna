#include "RArcData.h"
#include "RArcEntity.h"

RArcData::RArcData() {
}

RArcData::RArcData(RDocument* document, const RArcData& data) {
    *this = data;
    this->document = document;
}

RArcData::RArcData(const RVector& center, double radius, double startAngle,
		double endAngle, bool reversed) :
	RArc(center, radius, startAngle, endAngle, reversed) {
}

QList<RVector> RArcData::getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/) const {
    QList<RVector> ret;
    ret.append(center);
    ret.append(getStartPoint());
    ret.append(getEndPoint());
    return ret;
}

bool RArcData::moveReferencePoint(const RVector& referencePoint,
		const RVector& targetPoint) {
	bool ret = false;
	if (referencePoint.getDistanceTo(center) < RS::PointTolerance) {
		center = targetPoint;
		ret = true;
	} else if (referencePoint.getDistanceTo(getStartPoint())
			< RS::PointTolerance) {
		moveStartpoint(targetPoint);
		ret = true;
	} else if (referencePoint.getDistanceTo(getEndPoint()) < RS::PointTolerance) {
		moveEndpoint(targetPoint);
		ret = true;
	}
	return ret;
}
