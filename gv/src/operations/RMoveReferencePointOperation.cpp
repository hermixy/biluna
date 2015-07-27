#include <QSet>
#include <QSharedPointer>

#include "RDocument.h"
#include "RMoveReferencePointOperation.h"
#include "RExporter.h"


RMoveReferencePointOperation::RMoveReferencePointOperation(
    const RVector& referencePoint, 
    const RVector& targetPoint)
    : referencePoint(referencePoint),
      targetPoint(targetPoint) {
}



RTransaction RMoveReferencePointOperation::apply(RDocument& document) const {
    RTransaction transaction(document.getStorage(), "Moving reference point");

    QSet<REntity::Id> selectedEntities =
        document.querySelectedEntities();
    QSet<REntity::Id>::iterator it;
    for (it=selectedEntities.begin(); it!=selectedEntities.end(); it++) {
        QSharedPointer<REntity> entity = document.queryEntity(*it);
        if (entity.isNull()) {
            continue;
        }
        
        // apply operation to entity:
        bool modified = entity->moveReferencePoint(referencePoint, targetPoint);
        
        if (modified) {
            transaction.addObject(entity, false);
        }
    }
        
    transaction.end();

    return transaction;
}



void RMoveReferencePointOperation::preview(RDocument& document,
		RExporter& exporter) const {
	QSet<REntity::Id> selectedEntities =
        document.querySelectedEntities();
	QSet<REntity::Id>::iterator it;
	for (it = selectedEntities.begin(); it != selectedEntities.end(); it++) {
		QSharedPointer<REntity> entity = document.queryEntity(*it);
		if (entity.isNull()) {
			continue;
		}

		// apply operation to entity:
		entity->moveReferencePoint(referencePoint, targetPoint);

		// add entity to preview:
//		entity->exportEntity(exporter);
		exporter.exportEntity(*entity);
	}
}

