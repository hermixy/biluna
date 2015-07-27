#ifndef RMOVESELECTIONOPERATION_H
#define RMOVESELECTIONOPERATION_H

#include "RDocument.h"
#include "REntity.h"
#include "RExporter.h"
#include "ROperation.h"
#include "RVector.h"


/**
 * Implementation of an operation that moves a selection
 * by a given distance.
 * 
 * \ingroup operations
 * \scriptable
 */
class RMoveSelectionOperation : public ROperation {
public:
    RMoveSelectionOperation(
        const RVector& referencePoint, const RVector& targetPoint
    );
    virtual ~RMoveSelectionOperation() {}
    
    virtual RTransaction apply(RDocument& document) const;
    virtual void preview(RDocument& document, RExporter& exporter) const;

private:
    RVector referencePoint;
    RVector targetPoint;
};

Q_DECLARE_METATYPE(RMoveSelectionOperation*)

#endif
