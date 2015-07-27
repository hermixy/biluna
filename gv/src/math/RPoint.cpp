#include <cmath>

#include "RBox.h"
#include "RPoint.h"
#include "RLine.h"
#include "RDebug.h"

/**
 * Creates a point object with an invalid position
 */
RPoint::RPoint() {
}

RPoint::RPoint(const RPoint& other) {
	this->position = other.position;
}

/**
 * Creates a point object with the given position.
 *
 * \param position the position
 *
 */
RPoint::RPoint(const RVector& position) :
	position(position) {
}

RPoint::~RPoint() {
}

RBox RPoint::getBoundingBox() const {
	return RBox(position, position);
}

QList<RVector> RPoint::getEndPoints() const {
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

QList<RVector> RPoint::getMiddlePoints() const {
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

QList<RVector> RPoint::getCenterPoints() const {
    QList<RVector> ret;
    ret.append(position);
    return ret;
}

QList<RVector> RPoint::getPointsWithDistanceToEnd(double /*distance*/) const {
    QList<RVector> ret;
    return ret;
}

RVector RPoint::getVectorTo(const RVector& point, bool /*limited*/) const {
	return point - position;
}

/*
RVector RPoint::getVectorTo(const RLine& line, bool limited) const {
	return -line.getVectorTo(position, limited);
}
*/

bool RPoint::move(const RVector& offset) {
    if (!offset.isValid() || offset.getMagnitude() < RS::PointTolerance) {
        return false;
    }
    position += offset;
    return true;
}

bool RPoint::rotate(double rotation, const RVector& center){
	if (fabs(rotation) < RS::AngleTolerance) {
		return false;
	}
	position.rotate(rotation, center);
	return true;
}

bool RPoint::scale(const RVector& scaleFactors, const RVector& center) {
	position.scale(scaleFactors, center);
    return true;
}

bool RPoint::mirror(const RLine& axis) {
    position.mirror(axis);
    return true;
}

bool RPoint::flipHorizontal() {
	position.flipHorizontal();
    return true;
}

bool RPoint::flipVertical() {
	position.flipVertical();
    return true;
}
