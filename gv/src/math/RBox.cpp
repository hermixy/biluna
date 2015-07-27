#include "RBox.h"



/**
 * Constructor for an invalid box with invalid corner points.
 */
RBox::RBox() :
	c1(RVector::invalid), c2(RVector::invalid) {
}


/**
 * Constructor for a box with two given diagonally opposite corners.
 */
RBox::RBox(const RVector& c1, const RVector& c2)
    : c1(c1), c2(c2) {}


bool RBox::isValid() const {
    return (c1.isValid() && c2.isValid());
}

RBox& RBox::grow(double offset) {
	RVector min = getMinimum();
	RVector max = getMaximum();
	min -= RVector(offset, offset, offset);
	max += RVector(offset, offset, offset);
	c1 = min;
	c2 = max;
	return *this;
}

RBox& RBox::growXY(double offset) {
	RVector min = getMinimum();
	RVector max = getMaximum();
	min -= RVector(offset, offset);
	max += RVector(offset, offset);
	c1 = min;
	c2 = max;
	return *this;
}

RVector RBox::getSize() const {
    return c2 - c1;
}



RVector RBox::getCenter() const {
    return (c1 + c2) / 2.0;
}



RVector RBox::getMinimum() const {
    return RVector::getMinimum(c1, c2);
}



RVector RBox::getMaximum() const {
    return RVector::getMaximum(c1, c2);
}



RVector RBox::getCorner1() {
    return c1;
}

void RBox::setCorner1(const RVector& v){
	c1 = v;
}

RVector RBox::getCorner2() {
    return c2;
}

void RBox::setCorner2(const RVector& v){
	c2 = v;
}

/**
 * \return True if this box is completely inside the given box.
 */
//bool RBox::isInside(const RBox& other) const {
//    return other.contains(*this);
//}



/**
 * \return True if the given box is completely outside this box.
 */
bool RBox::isOutside(const RBox& other) const {
    RVector maximum = getMaximum();
    RVector minimum = getMinimum();
    RVector otherMaximum = other.getMaximum();
    RVector otherMinimum = other.getMinimum();

    return (
        minimum.x > otherMaximum.x ||
        minimum.y > otherMaximum.y ||
        minimum.z > otherMaximum.z ||
        maximum.x < otherMinimum.x ||
        maximum.y < otherMinimum.y ||
        maximum.z < otherMinimum.z
    );
}



/**
 * \return True if the given box is completely inside this box.
 */
bool RBox::contains(const RBox& other) const {
    return other.c1.isInside(*this) && other.c2.isInside(*this);
}



bool RBox::intersects(const RBox& other) const {
    RVector maximum = getMaximum();
    RVector minimum = getMinimum();
    RVector otherMaximum = other.getMaximum();
    RVector otherMinimum = other.getMinimum();

    if (minimum.x>otherMaximum.x || 
        minimum.y>otherMaximum.y || 
        minimum.z>otherMaximum.z) {
        return false;
    }
    if (maximum.x<otherMinimum.x || 
        maximum.y<otherMinimum.y || 
        maximum.z<otherMinimum.z) {
        return false;
    }

    return true;
}



/**
 * Grows this box to include the given box.
 */
void RBox::growToInclude(const RBox& other) {
    c1 = RVector::getMinimum(getMinimum(), other.getMinimum());
    c2 = RVector::getMaximum(getMaximum(), other.getMaximum());
}



/**
 * \return 8 vectors for the eight corners of the box.
 *
 * The first box corner is the first defining corner \c c1.
 * The next three corners are the corners on the same Z level
 * as \c c1, counter-clockwise.
 * The next four corners are on the same Z-level as \c c2
 * starting with the corner at the same X/Y position as \c c1,
 * continueing counter-clockwise.
 */
QList<RVector> RBox::getCorners() const {
    QList<RVector> ret;

    /*
    ret.append(RVector(c1.x, c1.y, c1.z);
    ret.append(RVector(c1.x, c1.y, c2.z);
    ret.append(RVector(c1.x, c2.y, c1.z);
    ret.append(RVector(c1.x, c2.y, c2.z);
    ret.append(RVector(c2.x, c1.y, c1.z);
    ret.append(RVector(c2.x, c1.y, c2.z);
    ret.append(RVector(c2.x, c2.y, c1.z);
    ret.append(RVector(c2.x, c2.y, c2.z);
    */
    
    ret.append(RVector(c1.x, c1.y, c1.z));
    ret.append(RVector(c2.x, c1.y, c1.z));
    ret.append(RVector(c2.x, c2.y, c1.z));
    ret.append(RVector(c1.x, c2.y, c1.z));
    ret.append(RVector(c1.x, c1.y, c2.z));
    ret.append(RVector(c2.x, c1.y, c2.z));
    ret.append(RVector(c2.x, c2.y, c2.z));
    ret.append(RVector(c1.x, c2.y, c2.z));

    return ret;
}



/**
 * \return 12 triangles, two for each side of the box.
 */
QList<RTriangle> RBox::getTriangles() const {
    QList<RTriangle> ret;
    QList<RVector> corners = getCorners();

    // front:
    ret.append(RTriangle(corners[0], corners[1], corners[5]));
    ret.append(RTriangle(corners[0], corners[5], corners[4]));

    // right:
    ret.append(RTriangle(corners[1], corners[2], corners[6]));
    ret.append(RTriangle(corners[1], corners[6], corners[5]));

    // back:
    ret.append(RTriangle(corners[2], corners[3], corners[7]));
    ret.append(RTriangle(corners[2], corners[7], corners[6]));

    // left
    ret.append(RTriangle(corners[3], corners[0], corners[4]));
    ret.append(RTriangle(corners[3], corners[4], corners[7]));

    // bottom:
    ret.append(RTriangle(corners[0], corners[2], corners[1]));
    ret.append(RTriangle(corners[0], corners[3], corners[2]));

    // top:
    ret.append(RTriangle(corners[4], corners[5], corners[7]));
    ret.append(RTriangle(corners[5], corners[6], corners[7]));

    return ret;
}


QRectF RBox::toQRectF() const {
	return QRectF(c1.x, c1.y, c2.x - c1.x, c2.y - c1.y);
}


/**
 * == operator
 */
bool RBox::operator ==(const RBox& other) const {
    return c1==other.c1 && c2==other.c2;
}

