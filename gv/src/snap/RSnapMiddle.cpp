#include "RSnapMiddle.h"

QList<RVector> RSnapMiddle::snapEntity(QSharedPointer<REntity> entity, const RVector& /*point*/) {
    return entity->getMiddlePoints();
}
