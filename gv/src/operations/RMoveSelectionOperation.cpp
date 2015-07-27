#include <QSet>

#include "RDocument.h"
#include "RMoveSelectionOperation.h"
#include "RExporter.h"


RMoveSelectionOperation::RMoveSelectionOperation(
    const RVector& referencePoint, 
    const RVector& targetPoint)
    : referencePoint(referencePoint),
      targetPoint(targetPoint) {
}



RTransaction RMoveSelectionOperation::apply(RDocument& document) const {
    RTransaction transaction(document.getStorage(), "Moving reference point");

    QSet<REntity::Id> selectedEntities = document.querySelectedEntities();
    QSet<REntity::Id>::iterator it;
    for (it=selectedEntities.begin(); it!=selectedEntities.end(); it++) {
    	QSharedPointer<REntity> entity = document.queryEntity(*it);
        if (entity.isNull()) {
            continue;
        }

        // apply operation to entity:
        if (entity->move(targetPoint - referencePoint)) {
            transaction.addObject(entity, false);
        }
    }

    transaction.end();

    return transaction;
}



void RMoveSelectionOperation::preview(RDocument& document, RExporter& exporter) const {
    QSet<REntity::Id> selectedEntities = document.querySelectedEntities();
    QSet<REntity::Id>::iterator it;
    for (it=selectedEntities.begin(); it!=selectedEntities.end(); it++) {
    	QSharedPointer<REntity> entity = document.queryEntity(*it);
        if (entity.isNull()) {
            continue;
        }

        // apply operation to entity:
        entity->move(targetPoint - referencePoint);

        // add entity to preview:
        exporter.exportEntity(*entity);
    }
}

