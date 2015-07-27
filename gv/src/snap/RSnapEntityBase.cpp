#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsView.h"
#include "RMouseEvent.h"
#include "RSettings.h"
#include "RSnapEntityBase.h"



RVector RSnapEntityBase::snap(const RVector& position, RGraphicsView& view) {
    entityIds.clear();
    RDocument* document = view.getDocument();

    RVector ret = RVector::invalid;

    if (document==NULL) {
        ret = position;
        ret.valid = false;
        return ret;
    }

    int rangePixels = RSettings::getSnapRange();
    double range = view.mapDistanceFromView(rangePixels);
    RVector rangeV(range, range, range);

    QSet<REntity::Id> ids =
    document->queryIntersectedEntitiesXY(
        RBox(position-rangeV, position+rangeV), false);
    
    REntity::Id entityId;
    double minDist = RMAXDOUBLE;
    double dist;

    QSet<REntity::Id>::iterator it;
    for (it=ids.begin(); it!=ids.end(); it++) {
        QSharedPointer<REntity> e = document->queryEntity(*it);
        if (e.isNull()) {
            continue;
        }
        
        RVector candidate = 
            position.getClosest(
                snapEntity(e, position)
            );

        dist = candidate.getDistanceTo(position);
        if (dist<minDist) {
            ret = candidate;
            minDist = dist;
            entityId = e->getId();
        }
    }
    
    if (!ret.isValid()) {
        ret = position;
        ret.valid = false;
        return ret;
    }
    else {
        entityIds.insert(entityId);
        return ret;
    }
}
