#ifndef RCIRCLEDATA_H
#define RCIRCLEDATA_H

#include "RBox.h"
#include "RDocument.h"
#include "REntity.h"
#include "RPoint.h"
#include "RVector.h"
#include "RCircle.h"

/**
 * \scriptable
 * \ingroup entity
 */
class RCircleData: public REntityData, protected RCircle {

    friend class RCircleEntity;

protected:
    RCircleData(RDocument* document, const RCircleData& data);

public:
    RCircleData();
    RCircleData(const RVector& center, double radius);

    RVector getCenter() const {
        return RCircle::getCenter();
    }
    double getRadius() const {
        return RCircle::getRadius();
    }

    //virtual RBox getBoundingBox() const;

    virtual QList<RVector> getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/ = RS::Top) const;
    /*
    virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual RVector getClosestPointOnEntity(const RVector& point) const;
    virtual QList<RVector> getIntersectionPoints(const REntityData& other, 
        bool limited = true) const;
    virtual RVector getVectorTo(const RVector& point,
        bool limited = true) const;
    virtual RVector getVectorTo(const RLine& line,
        bool limited = true) const;
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance) const;
    RVector getPosition() const;

    virtual bool intersectsWith(const RTriangle& triangle) const;

    */
    virtual bool moveReferencePoint(const RVector& referencePoint, 
        const RVector& targetPoint);
    /*
    RVector getCenter() const;
    double getRadius() const;
    */

    QList<const RShape*> getShapes() const {
        return QList<const RShape*>() << this;
    }

};

Q_DECLARE_METATYPE(RCircleData*)

#endif
