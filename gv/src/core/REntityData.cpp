#include "RDocument.h"
#include "REntity.h"
#include "REntityData.h"
#include "RBlockReferenceEntity.h"

REntityData::REntityData(RDocument* document) :
    document(document),
    selectionStatus(false), 
    drawOrder(0), 
    layerId(RLayer::INVALID_ID), 
    blockId(RBlock::INVALID_ID), 
    linetypeId(RLinetype::INVALID_ID),
    lineweight(RLineweight::WeightInvalid),
    color() {

}

/**
 * \return Color of this entity.
 *
 * \param resolve Resolve color if ByLayer or ByBlock.
 */
RColor REntityData::getColor(bool resolve,
    RLayer* layer, RBlockReferenceEntity* blockRef) const {

    if (!resolve) {
        return getColor();
    }

    if (color.isByLayer() && layer!=NULL) {
        return layer->getColor();
    }

    if (color.isByBlock() && blockRef!=NULL && document!=NULL) {
        QSharedPointer<RLayer> layer =
            document->queryLayer(blockRef->getLayerId());
        RColor ret = blockRef->getColor(resolve, layer.data(), blockRef);
        return ret;
    }

    return getColor();
}

// TODO: fix
double REntityData::getLineweightInUnits(RLayer* layer, RBlockReferenceEntity* blockRef) const {
    RLineweight::Lineweight lw = getLineweight(true, layer, blockRef);
    // TODO: unit conversion:
    return lw / 100.0;
}

RLineweight::Lineweight REntityData::getLineweight(bool resolve,
    RLayer* layer, RBlockReferenceEntity* blockRef) const {

    if (!resolve) {
        return getLineweight();
    }

    RLineweight::Lineweight lw = lineweight;

    if (lineweight==RLineweight::WeightByLayer && layer!=NULL) {
        lw = layer->getLineweight();
    }

    if (lineweight==RLineweight::WeightByBlock) {
        if (blockRef!=NULL) {
            lw = blockRef->getLineweight(resolve, layer, blockRef);
        }
        else {
            lw = RLineweight::Weight000;
        }
    }

    if (lw==RLineweight::WeightByLwDefault) {
        // TODO: return default line weight:
        lw = RLineweight::Weight000;
    }

    if (lw==RLineweight::WeightInvalid) {
        // TODO: return default line weight:
        lw = RLineweight::Weight000;
    }

    return lw;
}

RLinetype::Id REntityData::getLinetypeId(bool resolve,
    RLayer* layer, RBlockReferenceEntity* blockRef) const {

    if (!resolve) {
        return getLinetypeId();
    }

    if (document!=NULL && document->isByLayer(linetypeId) && layer!=NULL) {
        return layer->getLinetypeId();
    }

    if (document!=NULL && document->isByBlock(linetypeId) && blockRef!=NULL) {
        return blockRef->getLinetypeId(resolve, layer, blockRef);
    }

    return getLinetypeId();
}


/**
 * \return The shortest distance from this entity to the given point.
 */
double REntityData::getDistanceTo(const RVector& point, bool limited) const {
	RVector v = getVectorTo(point, limited);
	if (v.isValid()) {
		return v.getMagnitude();
	}
	return RNANDOUBLE;
}


void REntityData::setLinetype(RLinetype linetype) {
    if (document!=NULL) {
        this->linetypeId = document->getLinetypeId(linetype.getName());
    }
}

/**
 * \return The bounding box that contains this entity.
 */
RBox REntityData::getBoundingBox() const {
    RBox bb;
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        if (!bb.isValid()) {
            bb = shapes.at(i)->getBoundingBox();
        }
        else {
            bb.growToInclude(shapes.at(i)->getBoundingBox());
        }
    }
    return bb;
}

/**
 * \return Vector of end points of this entity. Used for snap to end points.
 */
QList<RVector> REntityData::getEndPoints() const {
    QList<RVector> ret;
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret.append(shapes.at(i)->getEndPoints());
    }
    return ret;
}

/**
 * \return Vector of middle points of this entity. Used for snap to
 *        middle points.
 */
QList<RVector> REntityData::getMiddlePoints() const {
    QList<RVector> ret;
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret.append(shapes.at(i)->getMiddlePoints());
    }
    return ret;
}

/**
 * \return Vector of center points of this entity. Used for snap to
 *        center points.
 */
QList<RVector> REntityData::getCenterPoints() const {
    QList<RVector> ret;
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret.append(shapes.at(i)->getCenterPoints());
    }
    return ret;
}

/**
 * \return Points on entity with given \c distance to either endpoint.
 * Used for snap to points with distance on entity.
 */
QList<RVector> REntityData::getPointsWithDistanceToEnd(double distance) const {
    QList<RVector> ret;
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret.append(shapes.at(i)->getPointsWithDistanceToEnd(distance));
    }
    return ret;
}

/**
 * \return Closest point to \c point on this entity. Used for snap to
 *        points on entity.
 */
RVector REntityData::getClosestPointOnEntity(const RVector& point) const {
    RVector ret = RVector::invalid;
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        RVector r = shapes.at(i)->getClosestPointOnShape(point);
        if (!ret.isValid() || r.getDistanceTo(point)<ret.getDistanceTo(point)) {
            ret = r;
        }
    }
    return ret;
}


/**
 * \return The distance vector from this entity to the given point.
 *
 * \param point the point to which the distance was measured
 *
 * \param limited: If true, an invalid vector is returned if the
 *   closest point on the entity is outside of the entity (e.g. in
 *   the extension line of a line or outside the start / end angle
 *   of an arc.
 */
RVector REntityData::getVectorTo(const RVector& point, bool limited) const {
    RVector ret = RVector::invalid;
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        RVector r = shapes.at(i)->getVectorTo(point, limited);
        if (!ret.isValid() || r.getMagnitude()<ret.getMagnitude()) {
            ret = r;
        }
    }
    return ret;
}

/**
 * \return True if this entity intersects with the given shape,
 *      false otherwise.
 * \todo better to pass another entity data object?
 */
bool REntityData::intersectsWith(const RShape& shape) const {
    QList<const RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        if (shapes.at(i)->intersectsWith(shape)) {
            return true;
        }
    }
    return false;
}

/**
 * \return Points on entity which are intersection points with the
 *     given other entity.
 */
QList<RVector> REntityData::getIntersectionPoints(const REntityData& other,
    bool limited) const {

    QList<RVector> ret;
    QList<const RShape*> shapes1 = getShapes();
    for (int i=0; i<shapes1.size(); i++) {
        QList<const RShape*> shapes2 = other.getShapes();
        for (int k=0; k<shapes2.size(); k++) {
            ret.append(
                shapes1.at(i)->getIntersectionPoints(*shapes2.at(k), limited)
            );
        }
    }
    return ret;
}

/**
 * \return Points on entity which are intersection points in the given document.
 */
/*
QList<RVector> REntityData::getIntersectionPoints(RDocument& document) const {
    QList<RVector> ret;

    QSet<REntity::Id> candidates;
    document.queryIntersectedEntities(candidates, getBoundingBox(), true);

    QSet<REntity::Id>::iterator it;
    for (it=candidates.begin(); it!=candidates.end(); it++) {
        QSharedPointer<REntity> e = document.queryEntity(*it);
        if (e.isNull()) {
            continue;
        }

        QList<RVector> intersections = getIntersectionPoints(e->getData(), true);
        ret += intersections;
    }

    return ret;
}
*/

/**
 * \return True if this entity intersects with the given box or is completely
 *      inside it, false otherwise.
 */
/*
bool REntityData::intersectsWith(const RBox& box) const {
    // if the bounding box of this entity is completely outside of
    // the given box, there is no intersection:
    if (getBoundingBox().isOutside(box)) {
        return false;
    }

    // if the bounding box of this entity is completely inside of
    // the given box, there is an intersection:
    if (box.contains(getBoundingBox())) {
        return true;
    }

    // if the bounding box of this entity is neither completely inside
    // nor outside of the given box, we have to do the more expensive
    // intersection test with the walls of the box:
    QList<RTriangle> triangles = box.getTriangles();
    QList<RTriangle>::iterator it;
    for (it=triangles.begin(); it!=triangles.end(); ++it) {
        if (intersectsWith(*it)) {
            return true;
        }
    }

    return false;
}
*/

/**
 * Moves this entity by the given offset.
 */
bool REntityData::move(const RVector& offset) {
    bool ret = false;
    QList<RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret |= shapes.at(i)->move(offset);
    }
    return ret;
}

bool REntityData::rotate(double rotation, const RVector& center) {
    bool ret = false;
    QList<RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret |= shapes.at(i)->rotate(rotation, center);
    }
    return ret;
}

bool REntityData::scale(const RVector& scaleFactors, const RVector& center) {
    bool ret = false;
    QList<RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret |= shapes.at(i)->scale(scaleFactors, center);
    }
    return ret;
}

bool REntityData::scale(double scaleFactor, const RVector& center) {
    return scale(RVector(scaleFactor, scaleFactor, scaleFactor), center);
}

bool REntityData::mirror(const RLine& axis) {
    bool ret = false;
    QList<RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret |= shapes.at(i)->mirror(axis);
    }
    return ret;
}

bool REntityData::mirror(const RVector& axis1, const RVector& axis2) {
    return mirror(RLine(axis1, axis2));
}

bool REntityData::flipHorizontal() {
    bool ret = false;
    QList<RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret |= shapes.at(i)->flipHorizontal();
    }
    return ret;
}

bool REntityData::flipVertical() {
    bool ret = false;
    QList<RShape*> shapes = getShapes();
    for (int i=0; i<shapes.size(); i++) {
        ret |= shapes.at(i)->flipVertical();
    }
    return ret;
}
