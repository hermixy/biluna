#ifndef RLINEDATA_H
#define RLINEDATA_H

#include "RBox.h"
#include "RDocument.h"
#include "REntity.h"
#include "RLine.h"
#include "RVector.h"

/**
 * \scriptable
 * \ingroup entity
 */
class RLineData: public REntityData, protected RLine {

    friend class RLineEntity;

protected:
    RLineData(RDocument* document, const RLineData& data);

public:
    RLineData();
    RLineData(const RVector& startPoint, const RVector& endPoint);

    RVector getStartPoint() const {
        return RLine::getStartPoint();
    }
    RVector getEndPoint() const {
        return RLine::getEndPoint();
    }

    virtual QList<RVector> getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/ = RS::Top) const;

    virtual bool moveReferencePoint(const RVector& referencePoint, 
        const RVector& targetPoint);

    QList<const RShape*> getShapes() const {
        return QList<const RShape*>() << this;
	}
};

Q_DECLARE_METATYPE(RLineData*)

#endif
