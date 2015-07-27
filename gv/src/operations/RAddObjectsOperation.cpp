#include "RDocument.h"
#include "RAddObjectsOperation.h"

RAddObjectsOperation::RAddObjectsOperation(bool undoable) :
	ROperation(undoable), list() {
}

RAddObjectsOperation::RAddObjectsOperation(
        QList<QSharedPointer<RObject> >& list,
        bool useCurrentAttributes, bool undoable) : ROperation(undoable) {

    for (int i=0; i<list.count(); ++i) {
        addObject(list[i], useCurrentAttributes);
    }
}

void RAddObjectsOperation::addObject(const QSharedPointer<RObject>& object,
                                     bool useCurrentAttributes) {
    if (object.isNull()) {
        return;
    }

    list.append(QPair<QSharedPointer<RObject>, bool>(object, useCurrentAttributes));

    if (object->getDocument()==NULL) {
        return;
    }

    QSharedPointer<REntity> entity = object.dynamicCast<REntity>();
    if (entity.isNull()) {
        return;
    }

    // place entity on current layer / block, set current pen attributes:
    /*
    if (useCurrentAttributes || entity->getLayerId()==RLayer::INVALID_ID) {
        entity->setLayerId(object->getDocument()->getCurrentLayerId());
    }
    if (useCurrentAttributes || entity->getBlockId()==RBlock::INVALID_ID) {
        entity->setBlockId(object->getDocument()->getCurrentBlockId());
    }
    if (useCurrentAttributes || !entity->getColor().isValid()) {
        entity->setColor(object->getDocument()->getCurrentColor());
    }
    if (useCurrentAttributes || entity->getLineweight()!=RLineweight::WeightInvalid) {
        entity->setLineweight(object->getDocument()->getCurrentLineweight());
    }
    if (useCurrentAttributes || entity->getLinetypeId()!=RLinetype::INVALID_ID) {
        entity->setLinetype(object->getDocument()->getCurrentLinetype());
    }
    */
}

RTransaction RAddObjectsOperation::apply(RDocument& document) const {
    return previewOrApply(document);
    /*
    RTransaction transaction(document.getStorage(), "Adding object(s)", undoable);
	for (int i = 0; i < list.size(); ++i) {
		if (list[i].isNull()) {
			RDebug::error(QString(
					"RAddObjectsOperation::apply: list contains NULL object"));
			continue;
		}

        transaction.addObject(list[i].first, list[i].second);
	}
	transaction.end();
	return transaction;
    */
}

void RAddObjectsOperation::preview(RDocument& document, RExporter& exporter) const {
    previewOrApply(document, &exporter);
    /*
	for (int i = 0; i < list.size(); ++i) {
        QSharedPointer<REntity> entity = list[i].first.dynamicCast<REntity> ();
		if (!entity.isNull()) {
            / *
            entity->setDocument(&document);
            if (entity->getLayerId()==RLayer::INVALID_ID) {
                entity->setLayerId(document.getCurrentLayerId());
            }
            if (!entity->getColor().isValid()) {
                entity->setColor(document.getCurrentLayerId());
            }
            * /
            //entity->exportEntity(exporter);
            exporter.exportEntity(*entity);
		}
	}
    */
}

RTransaction RAddObjectsOperation::previewOrApply(RDocument& document,
                                                  RExporter* exporter) const {

    RTransaction transaction(document.getStorage(), "Adding object(s)", undoable);
    if (exporter!=NULL) {
        transaction.setPreviewExporter(exporter);
    }

    for (int i = 0; i < list.size(); ++i) {
        if (list[i].first.isNull()) {
            RDebug::error(QString(
                    "RAddObjectsOperation::apply: list contains NULL object"));
            continue;
        }

        transaction.addObject(list[i].first, list[i].second);
    }
    transaction.end();
    return transaction;
}
