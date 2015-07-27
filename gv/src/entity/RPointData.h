#ifndef RPOINTDATA_H
#define RPOINTDATA_H

#include "RBox.h"
#include "RDocument.h"
#include "REntity.h"
#include "RPoint.h"
#include "RVector.h"
#include "RLine.h"

/**
 * \scriptable
 * \ingroup entity
 */
class RPointData: public REntityData, protected RPoint {

    friend class RPointEntity;

protected:
    RPointData(RDocument* document, const RPointData& data);

public:
    RPointData();
    RPointData(const RVector& point);

    RVector getPosition() const {
       return RPoint::getPosition();
    }

    /*
    virtual RBox getBoundingBox() const;
    */

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

    QList<const RShape*> getShapes() const {
        return QList<const RShape*>() << this;
    }

};

Q_DECLARE_METATYPE(RPointData*)

#endif
