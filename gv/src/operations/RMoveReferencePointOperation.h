#ifndef RMOVEREFERENCEPOINTOPERATION_H
#define RMOVEREFERENCEPOINTOPERATION_H

#include "RDocument.h"
#include "RExporter.h"
#include "ROperation.h"
#include "RVector.h"

class REntity;

/**
 * Implementation of an operation that moves a reference point
 * by a given distance.
 * 
 * \ingroup operations
 * \scriptable
 */
class RMoveReferencePointOperation : public ROperation {
public:
    RMoveReferencePointOperation(
        const RVector& referencePoint, const RVector& targetPoint
    );
    virtual ~RMoveReferencePointOperation() {}
    
    virtual RTransaction apply(RDocument& document) const;
    virtual void preview(RDocument& document, RExporter& exporter) const;

private:
    RVector referencePoint;
    RVector targetPoint;
};

Q_DECLARE_METATYPE(RMoveReferencePointOperation*)

#endif
