#include <QSet>

#include "RDocument.h"
#include "RPasteOperation.h"
#include "RExporter.h"
#include "RBlockReferenceEntity.h"

RPasteOperation::RPasteOperation(RDocument& sourceDocument)
    : sourceDocument(sourceDocument),
    scale(1.0),
    rotation(0.0),
    flipHorizontal(false),
    flipVertical(false),
    toCurrentLayer(false),
    overwriteLayers(false),
    overwriteBlocks(false) {
}



RTransaction RPasteOperation::apply(RDocument& document) const {
    return previewOrApply(document);
    /*
    RTransaction transaction(document.getStorage(), "Paste clipboard");
    copy(
        sourceDocument, document,
        offset, scale, rotation, flipHorizontal, flipVertical,
        toCurrentLayer, / *toCurrentBlock=* / true,
        overwriteLayers, overwriteBlocks,
        blockName,
        transaction,
        false, false
    );

    transaction.end();

    return transaction;
    */
}



void RPasteOperation::preview(RDocument& document, RExporter& exporter) const {
    //return;
    // TODO refactor into:
    previewOrApply(document, &exporter);

    /*
    if (!blockName.isNull() && document.hasBlock(blockName) && !overwriteBlocks) {
		RBlockReferenceEntity ref(&document, RBlockReferenceData(
				document.getBlockId(blockName), RVector(0, 0, 0), RVector(1, 1,
						1), 0));

        // apply operation to entity:
        if (flipHorizontal) {
        	ref.flipHorizontal();
        }
        if (flipVertical) {
        	ref.flipVertical();
        }
        ref.scale(scale);
        ref.rotate(rotation);
        ref.move(offset);

		exporter.exportEntity(ref);
		return;
	}

    QSet<REntity::Id> entityIds = sourceDocument.queryAllEntities();
    QSet<REntity::Id>::iterator it;
    for (it=entityIds.begin(); it!=entityIds.end(); it++) {
        QSharedPointer<REntity> entity =
           sourceDocument.queryEntity(*it);
        if (entity.isNull()) {
            continue;
        }

        // apply operation to entity:
        if (flipHorizontal) {
            entity->flipHorizontal();
        }
        if (flipVertical) {
            entity->flipVertical();
        }
        entity->scale(scale);
        entity->rotate(rotation);
        entity->move(offset);
        if (toCurrentLayer) {
            entity->setDocument(&document);
            entity->setLayerId(document.getCurrentLayerId());
        }
        if (!overwriteLayers) {
            QString layerName =
                sourceDocument.getLayerName(entity->getLayerId());
            if (document.hasLayer(layerName)) {
                QSharedPointer<RLayer> layer = document.queryLayer(layerName);
                if (!layer.isNull()) {
                    if (entity->getColor()==RColor(RColor::ByLayer)) {
                        entity->setColor(layer->getColor());
                    }
                    if (entity->getLineweight()==RLineweight::WeightByLayer) {
                        entity->setLineweight(layer->getLineweight());
                    }
                    if (entity->getLinetypeId()==RLineweight::WeightByLayer) {
                        QString ltName =
                            sourceDocument.getLinetypeName(
                                entity->getLinetypeId()
                            );
                        QSharedPointer<RLinetype> lt =
                            document.queryLinetype(ltName);
                        if (!lt.isNull()) {
                            entity->setLinetypeId(lt->getId());
                        }
                    }
                }
            }
        }

        // add entity to preview:
        exporter.exportEntity(*entity);
    }
    */
}


RTransaction RPasteOperation::previewOrApply(
    RDocument& document, RExporter* exporter) const {

    if (exporter!=NULL) {
        // TODO: setLayerSource / blockSource if appropriate
        // if overwrite layers is true or layer does not exist in target
    }

    RTransaction transaction(document.getStorage(), "Inserting object(s)", undoable);
    transaction.setPreviewExporter(exporter);

    copy(
        sourceDocument, document,
        offset, scale, rotation, flipHorizontal, flipVertical,
        toCurrentLayer, /*toCurrentBlock=*/ true,
        overwriteLayers, overwriteBlocks,
        blockName,
        transaction,
        false, false
    );

    transaction.end();
    return transaction;
}
