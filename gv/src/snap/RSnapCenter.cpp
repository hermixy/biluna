#include "RSnapCenter.h"

QList<RVector> RSnapCenter::snapEntity(QSharedPointer<REntity> entity,
    const RVector& /*point*/) {

    return entity->getCenterPoints();
}
