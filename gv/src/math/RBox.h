#ifndef RBOX_H
#define RBOX_H

#include <QRectF>

#include "RVector.h"
#include "RTriangle.h"

/**
 * Represents a box e.g. a bounding box.
 * The surfaces of the box are orthogonal to the Cartesian coordinate system.
 *
 * \ingroup math
 * \copyable
 * \scriptable
 */
class RBox {
public:
	RBox();
	RBox(const RVector& c1, const RVector& c2);

    bool isValid() const;

	RVector getSize() const;
	RVector getCenter() const;
	RVector getMinimum() const;
	RVector getMaximum() const;
	bool isOutside(const RBox& other) const;
    bool contains(const RBox& other) const;
    bool intersects(const RBox& other) const;

    void growToInclude(const RBox& other);

	RVector getCorner1();
    void setCorner1(const RVector& v);
	RVector getCorner2();
	void setCorner2(const RVector& v);

	QList<RVector> getCorners() const;
	QList<RTriangle> getTriangles() const;

	RBox& grow(double offset);
	RBox& growXY(double offset);

	QRectF toQRectF() const;

	bool operator ==(const RBox& other) const;
	bool operator !=(const RBox& other) const {
		return !operator==(other);
	}

public:
	/**
	 * \getter{getCorner1}
	 * \setter{setCorner1}
	 */
	RVector c1;
	/**
	 * \getter{getCorner2}
	 * \setter{setCorner2}
	 */
	RVector c2;
};


Q_DECLARE_METATYPE(RBox)
Q_DECLARE_METATYPE(RBox*)
Q_DECLARE_METATYPE(QList<RBox>)

#endif
