#include "RDocument.h"
#include "RGraphicsView.h"
#include "RMouseEvent.h"
#include "RSnapIntersection.h"
#include "RSettings.h"

RSnapIntersection::RSnapIntersection() {
}

RVector RSnapIntersection::snap(const RVector& position, RGraphicsView& view) {
    entityIds.clear();
    RDocument* document = view.getDocument();

    if (document==NULL) {
        return position;
    }

    int rangePixels = RSettings::getSnapRange();
    double range = view.mapDistanceFromView(rangePixels);
    RVector rangeV(range, range, range);

    QSet<REntity::Id> ids =
        document->queryIntersectedEntitiesXY(
            RBox(position-rangeV, position+rangeV), false);
    
    REntity::Id entityId1 = REntity::INVALID_ID;
    REntity::Id entityId2 = REntity::INVALID_ID;
    RVector ret = RVector::invalid;
    double minDist = RMAXDOUBLE;
    double dist;

    QSet<REntity::Id>::iterator it1;
    for (it1=ids.begin(); it1!=ids.end(); it1++) {
        QSharedPointer<REntity> e1 = document->queryEntity(*it1);
        if (e1.isNull()) {
            continue;
        }
        QSet<REntity::Id>::iterator it2;
        for (it2=ids.begin(); it2!=ids.end(); it2++) {
            QSharedPointer<REntity> e2 = document->queryEntity(*it2);
            if (e2.isNull()) {
                continue;
            }

            RVector candidate = 
                position.getClosest(
                    e1->getIntersectionPoints(*e2)
                );
            dist = candidate.getDistanceTo(position);
            if (dist<minDist) {
                ret = candidate;
                minDist = dist;
                entityId1 = e1->getId();
                entityId2 = e2->getId();
            }
        }
    }
    
    if (!ret.isValid()) {
        ret = position;
        ret.valid = false;
        return ret;
    }
    else {
        entityIds.insert(entityId1);
        entityIds.insert(entityId2);
        return ret;
    }
}
