#include "RBlockReferenceData.h"

RBlockReferenceData::RBlockReferenceData() :
    referencedBlockId(RBlock::INVALID_ID), rotation(0) {
}

RBlockReferenceData::RBlockReferenceData(RDocument* document,
    const RBlockReferenceData& data) {
    *this = data;
    this->document = document;
}

RBlockReferenceData::RBlockReferenceData(
		RBlock::Id referencedBlockId, const RVector& position,
		const RVector& scaleFactors, double rotation) :
        referencedBlockId(referencedBlockId),
        position(position), scaleFactors(scaleFactors), rotation(rotation) {
}

RVector RBlockReferenceData::getVectorTo(const RVector& point, bool limited) const {
	if (document == NULL) {
		return RVector::invalid;
	}
	QSet<REntity::Id> ids =
        document->queryBlockEntities(referencedBlockId);
	QSet<REntity::Id>::iterator it;
	double minDist = RMAXDOUBLE;
	RVector res(RVector::invalid);
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<REntity> entity = queryEntity(*it);
		if (entity.isNull()) {
			continue;
		}
		RVector v = entity->getVectorTo(point, limited);
		double dist = v.getMagnitude();
		if (dist < minDist) {
			minDist = dist;
			res = v;
		}
	}
	return res;
}

/*
RVector RBlockReferenceData::getVectorTo(const RLine& line, bool limited) const {
	if (document == NULL) {
		return RVector::invalid;
	}
	QSet<REntity::Id> ids =
        document->queryBlockEntities(referencedBlockId);
	QSet<REntity::Id>::iterator it;
	double minDist = RMAXDOUBLE;
	RVector res(RVector::invalid);
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<REntity> entity = queryEntity(*it);
		if (entity.isNull()) {
			continue;
		}
		RVector v = entity->getVectorTo(line, limited);
		double dist = v.getMagnitude();
		if (dist < minDist) {
			minDist = dist;
			res = v;
		}
	}
	return res;
}
*/

RBox RBlockReferenceData::getBoundingBox() const {
	if (document == NULL) {
		return RBox();
	}

    static int recursionDepth=0;
    recursionDepth++;
    /*
    if (recursionDepth>10) {
        recursionDepth--;
        RDebug::error("RBlockReferenceData::exportEntity: "
            "maximum recursion depth reached");
        return RBox();
    }
    */

	QSet<REntity::Id> ids =
        document->queryBlockEntities(referencedBlockId);
	QSet<REntity::Id>::iterator it;
	RBox box;
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<REntity> entity = queryEntity(*it);
		if (entity.isNull()) {
			continue;
		}
        RBox b = entity->getBoundingBox();
        if (!box.isValid()) {
            box = b;
			continue;
		}
        if (b.isValid()) {
            box.growToInclude(b);
        }
	}

    recursionDepth--;

	return box;
}

QList<RBox> RBlockReferenceData::getBoundingBoxes() const {
	if (document == NULL) {
		return QList<RBox>();
	}

    QList<RBox> ret;
	QSet<REntity::Id> ids =
        document->queryBlockEntities(referencedBlockId);
	QSet<REntity::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<REntity> entity = queryEntity(*it);
		if (entity.isNull()) {
			continue;
		}
        ret.append(entity->getBoundingBox());
	}
	return ret;
}

QSharedPointer<REntity> RBlockReferenceData::queryEntity(REntity::Id entityId) const {
	if (document == NULL) {
        RDebug::error("RBlockReferenceData::queryEntity: document is NULL");
		return QSharedPointer<REntity> ();
	}
	QSharedPointer<REntity> entity = document->queryEntity(entityId);
	if (entity.isNull()) {
        RDebug::error("RBlockReferenceData::queryEntity: "
            "entity %d is NULL", entityId);
        return QSharedPointer<REntity> ();
	}
	entity->scale(scaleFactors);
	entity->rotate(rotation);
	entity->move(position);
	return entity;
}

QList<RVector> RBlockReferenceData::getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/) const {
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

QList<RVector> RBlockReferenceData::getEndPoints() const {
    QList<RVector> ret;
    return ret;
}

QList<RVector> RBlockReferenceData::getMiddlePoints() const {
    QList<RVector> ret;
    return ret;
}

QList<RVector> RBlockReferenceData::getCenterPoints() const {
    QList<RVector> ret;
    return ret;
}

RVector RBlockReferenceData::getClosestPointOnEntity(const RVector& point) const {
    return RVector();
}

QList<RVector> RBlockReferenceData::getIntersectionPoints(
    const REntityData& other, bool limited) const {

    QList<RVector> ret;
    return ret;
}

QList<RVector> RBlockReferenceData::getPointsWithDistanceToEnd(double distance) const {
    QList<RVector> ret;
    return ret;
}

bool RBlockReferenceData::intersectsWith(const RTriangle& triangle) const {
	if (document == NULL) {
		return false;
	}
	QSet<REntity::Id> ids =
        document->queryBlockEntities(referencedBlockId);
	QSet<REntity::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
		QSharedPointer<REntity> entity = queryEntity(*it);
		if (entity.isNull()) {
			continue;
		}
		if (entity->intersectsWith(triangle)) {
			return true;
		}
	}
	return false;
}

bool RBlockReferenceData::moveReferencePoint(const RVector& referencePoint,
        const RVector& targetPoint) {
    return false;
}

bool RBlockReferenceData::move(const RVector& offset) {
    if (!offset.isValid() || offset.getMagnitude() < RS::PointTolerance) {
        return false;
    }
    position += offset;
    return true;
}

bool RBlockReferenceData::rotate(double rotation, const RVector& center) {
	if (fabs(rotation) < RS::AngleTolerance) {
		return false;
	}
	position.rotate(rotation, center);
    this->rotation += rotation;
	return true;
}

bool RBlockReferenceData::mirror(const RLine& axis) {
    position.mirror(axis);

    RVector vec = RVector::createPolar(1.0, rotation);
    vec.mirror(RVector(0.0,0.0), axis.endPoint-axis.startPoint);
    rotation = vec.getAngle();

    scaleFactors.y*=-1;
    return true;
}

bool RBlockReferenceData::scale(const RVector& scaleFactors, const RVector& center) {
    position.scale(scaleFactors, center);
    this->scaleFactors.scale(scaleFactors);
    return true;
}

bool RBlockReferenceData::flipHorizontal() {
    return mirror(RLine(RVector(0,0), RVector(0,1)));
}

bool RBlockReferenceData::flipVertical() {
    return mirror(RLine(RVector(0,0), RVector(1,0)));
}
