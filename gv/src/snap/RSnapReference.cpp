#include "RSnapReference.h"

QList<RVector> RSnapReference::snapEntity(QSharedPointer<REntity> entity, const RVector& /*point*/) {
    return entity->getReferencePoints();
}
