#include "RSnapDistance.h"

RSnapDistance::RSnapDistance() : distance(1.0) {
}

QList<RVector> RSnapDistance::snapEntity(QSharedPointer<REntity> entity,
    const RVector& /*point*/) {

    return entity->getPointsWithDistanceToEnd(distance);
}

void RSnapDistance::showUiOptions() {
}

void RSnapDistance::hideUiOptions() {
}

void RSnapDistance::setDistance(double d) {
    distance = d;
}
