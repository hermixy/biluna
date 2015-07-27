#include "RSnapOnEntity.h"

QList<RVector> RSnapOnEntity::snapEntity(QSharedPointer<REntity> entity, const RVector& point) {
    QList<RVector> ret;
    ret.append(entity->getClosestPointOnEntity(point));
    return ret;
}
