#ifndef RARCDATA_H
#define RARCDATA_H

#include "RBox.h"
#include "RDocument.h"
#include "REntity.h"
#include "RPoint.h"
#include "RVector.h"
#include "RArc.h"

/**
 * \scriptable
 * \ingroup entity
 */
class RArcData: public REntityData, protected RArc {

    friend class RArcEntity;

protected:
    RArcData(RDocument* document, const RArcData& data);

public:
    RArcData();
    RArcData(const RVector& center, double radius, double startAngle,
			double endAngle, bool reversed = false);

    RVector getCenter() const {
        return RArc::getCenter();
    }
    double getRadius() const {
        return RArc::getRadius();
    }
    double getStartAngle() const {
        return RArc::getStartAngle();
    }
    double getEndAngle() const {
        return RArc::getEndAngle();
    }

    virtual QList<RVector> getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/ = RS::Top) const;

    virtual bool moveReferencePoint(const RVector& referencePoint, 
        const RVector& targetPoint);

    QList<const RShape*> getShapes() const {
        return QList<const RShape*>() << this;
    }
};

Q_DECLARE_METATYPE(RArcData*)

#endif
