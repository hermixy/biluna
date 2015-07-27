#include "RSnapEnd.h"

QList<RVector> RSnapEnd::snapEntity(QSharedPointer<REntity> entity,
    const RVector& /*point*/) {

    return entity->getEndPoints();
}
