#include <QSet>

#include "RClipboardOperation.h"
#include "RStorage.h"
#include "RBlockReferenceEntity.h"


RClipboardOperation::RClipboardOperation() {
}

void RClipboardOperation::copy(
        RDocument& src, RDocument& dest,
        const RVector& offset,
        double scale,
        double rotation,
        bool flipHorizontal,
        bool flipVertical,
        bool toCurrentLayer,
        bool toCurrentBlock,
        bool overwriteLayers,
        bool overwriteBlocks,
        const QString& blockName,
        RTransaction& transaction,
        bool selectionOnly, bool clear) const {

    if (clear) {
        dest.clear();
    }

    QSet<REntity::Id> entityIds;
    if (selectionOnly) {
        entityIds = src.querySelectedEntities();
    }
    else {
        entityIds = src.queryAllEntities();
    }
    RVector off = offset;

    bool hasBlock = false;

    // this part is used to insert ('paste') blocks from the part library
    // as new blocks:
    if (!blockName.isNull()) {
        // copy into dest as a new block:
        QSharedPointer<RBlock> block;
        hasBlock = dest.hasBlock(blockName);
        if (!hasBlock || overwriteBlocks) {
            // block does not exist in dest - or -
            // block exists in dest and must be overwritten:
            QSet<REntity::Id> refs;
			if (hasBlock) {
                // temorarily 'ground' all existing references to the existing block:
				refs = dest.queryBlockReferences(dest.getBlockId(blockName));
				QSet<REntity::Id>::iterator it;
				for (it = refs.begin(); it != refs.end(); ++it) {
					QSharedPointer<RBlockReferenceEntity> e =
							dest.queryEntity(*it).dynamicCast<RBlockReferenceEntity> ();
					if (!e.isNull() && !e->isUndone()) {
						e->setReferencedBlockId(REntity::INVALID_ID);
                        transaction.addObject(e, false);
					}
				}
                // delete existing block in dest:
				transaction.deleteObject(dest.getBlockId(blockName), &dest);
			}

            // add new block to dest:
			block = QSharedPointer<RBlock> (new RBlock(&dest, blockName,
					RVector(0, 0, 0)));
			transaction.addObject(block);

			if (hasBlock) {
                // point previously grounded block references to new block:
				QSet<REntity::Id>::iterator it;
				for (it = refs.begin(); it != refs.end(); ++it) {
					QSharedPointer<RBlockReferenceEntity> e =
							dest.queryEntity(*it).dynamicCast<RBlockReferenceEntity> ();
					if (!e.isNull() && !e->isUndone()) {
						e->setReferencedBlockId(block->getId());
                        transaction.addObject(e, false);
					}
				}
			}
		} else {
            // block exists and must not be overwritten:
            block = dest.queryBlock(blockName);
        }

        // insert new block reference along with new block:
        RBlockReferenceEntity* ref = new RBlockReferenceEntity(&dest,
                RBlockReferenceData(block->getId(), RVector(0,0,0), RVector(1.0,
                        1.0, 1.0), 0.0));
        ref->setBlockId(dest.getCurrentBlockId());
        QSharedPointer<RBlockReferenceEntity> refp(ref);
        transaction.addObject(refp, true);
        off = RVector(0, 0, 0);
        if (flipHorizontal) {
            ref->flipHorizontal();
        }
        if (flipVertical) {
            ref->flipVertical();
        }
        ref->scale(scale);
        ref->rotate(rotation);
        ref->move(offset);
        scale = 1.0;
        rotation = 0.0;
        flipHorizontal = false;
        flipVertical = false;
        toCurrentLayer = false;

        // if the block existed already in dest and was not overwritten,
        // there's nothing left to do here:
        if (hasBlock && !overwriteBlocks) {
            return;
        }
	}

    QSet<REntity::Id>::iterator it;
    for (it=entityIds.begin(); it!=entityIds.end(); ++it) {
        QSharedPointer<REntity> entity = src.queryEntityDirect(*it);
        if (entity.isNull() || entity->isUndone()) {
            continue;
        }
        copyEntity(
            *entity.data(), 
            src, dest,
            off, scale, rotation,
            flipHorizontal, flipVertical,
            toCurrentLayer, toCurrentBlock,
            overwriteLayers, overwriteBlocks,
            transaction.isPreview() && !hasBlock ? QString() : blockName,
            transaction
        );
    }
}



/**
 * Adds the given entity (and its layer(s) and block reference(s)) to the
 * given document.
 *
 * \param blockName Name of an existing block in dest
 */
void RClipboardOperation::copyEntity(
        REntity& entity,
        RDocument& src, RDocument& dest,
        const RVector& offset,
        double scale,
        double rotation,
        bool flipHorizontal,
        bool flipVertical,
        bool toCurrentLayer,
        bool toCurrentBlock,
        bool overwriteLayers,
        bool overwriteBlocks,
        const QString& blockName,
        RTransaction& transaction) const {

    // add layer the entity is on, if the layer exists it is overwritten
    // if overwriteLayers is true:
    QSharedPointer<RLayer> srcLayer =
        src.queryLayer(entity.getLayerId());
    if (srcLayer.isNull()) {
        RDebug::error("RClipboardOperation::copyToDocument: "
            "layer of entity is NULL.");
        return;
    }
    QSharedPointer<RLayer> destLayer;
    if (!dest.hasLayer(srcLayer->getName()) || overwriteLayers) {
        destLayer = QSharedPointer<RLayer>(srcLayer->clone());
        dest.getStorage().setObjectId(*destLayer.data(), RObject::INVALID_ID);
        destLayer->setDocument(&dest);
        transaction.addObject(destLayer);
    }
    else {
        destLayer = dest.queryLayer(srcLayer->getName());
    }

    // add block the entity belongs to, if the block exists it is overwritten
    // if overwriteBlocks is true:
    QSharedPointer<RBlock> srcBlock =
        src.queryBlock(entity.getBlockId());
    if (srcBlock.isNull()) {
        RDebug::error("RClipboardOperation::copyToDocument: "
            "block of entity is NULL.");
        return;
    }
    QSharedPointer<RBlock> destBlock;
    QString destBlockName;
    if (!blockName.isNull()) {
        destBlockName = blockName;
    }
    else {
        if (toCurrentBlock) {
            destBlockName = dest.getBlockName(dest.getCurrentBlockId());
        }
        else {
            destBlockName = srcBlock->getName();
        }
    }
    if (!dest.hasBlock(destBlockName) || overwriteBlocks) {
        destBlock = QSharedPointer<RBlock> (srcBlock->clone());
        dest.getStorage().setObjectId(*destBlock.data(),
                RObject::INVALID_ID);
        destBlock->setDocument(&dest);
        transaction.addObject(destBlock);
    } else {
        destBlock = dest.queryBlock(destBlockName);
    }

    // entity is a block reference:
    // add entities of the block the block reference refers to (the block
    // definition is then added automatically):
    RBlockReferenceEntity* blockRef =
        dynamic_cast<RBlockReferenceEntity*>(&entity);
    if (blockRef!=NULL) {
        QSharedPointer<RBlock> refBlock =
            src.queryBlock(blockRef->getReferencedBlockId());
        if (refBlock.isNull()) {
            RDebug::error("RClipboardOperation::copyToDocument: "
                "entity references a NULL block.");
            return;
        }

        // if block exists in dest, it has already been copied:
        QSharedPointer<RBlock> refBlockDest = dest.queryBlock(refBlock->getName());
        if (refBlockDest.isNull()) {
            QSet<REntity::Id> ids = src.queryBlockEntities(refBlock->getId());
            QSet<REntity::Id>::iterator it;
            for (it=ids.begin(); it!=ids.end(); ++it) {
                QSharedPointer<REntity> e = src.queryEntityDirect(*it);
                if (e.isNull()) {
                    continue;
                }
                copyEntity(
                    *e.data(),
                    src, dest,
                    RVector::nullVector,
                    1.0, 0.0, false, false,
                    false, false,
                    overwriteLayers, overwriteBlocks,
                    QString(),
                    transaction
                );
            }
        }
    }

    // add entity self:
    QSharedPointer<REntity> destEntity = QSharedPointer<REntity>(entity.clone());
    dest.getStorage().setObjectId(*destEntity.data(), RObject::INVALID_ID);
    destEntity->setSelected(false);

    // apply transformations:
    if (flipHorizontal) {
        destEntity->flipHorizontal();
    }
    if (flipVertical) {
        destEntity->flipVertical();
    }
    destEntity->scale(scale);
    destEntity->rotate(rotation);
    destEntity->move(offset);

    if (!transaction.isPreview()) {
        destEntity->setDocument(&dest);
        if (toCurrentLayer) {
            // paste to current layer:
            destEntity->setLayerId(dest.getCurrentLayerId());
        }
        else {
            // paste to original layer:
            destEntity->setLayerId(destLayer->getId());
        }
        destEntity->setBlockId(destBlock->getId());

        // correct referenced block id of pasted block reference:
        QSharedPointer<RBlockReferenceEntity> destBlockRef =
            destEntity.dynamicCast<RBlockReferenceEntity>();
        if (!destBlockRef.isNull() && blockRef!=NULL) {
            QString bn = src.getBlockName(blockRef->getReferencedBlockId());
            RBlock::Id blockId = dest.getBlockId(bn);
            destBlockRef->setReferencedBlockId(blockId);
        }
    }
    else  {
        if (toCurrentLayer) {
            // paste to current layer:
            destEntity->setLayerId(dest.getCurrentLayerId());
            transaction.getPreviewExporter()->setLayerSource(&dest);
        }
        else {
            QString layerName = src.getLayerName(destEntity->getLayerId());
            if (dest.hasLayer(layerName) && !overwriteLayers) {
                // layer exists and must not be overwritten:
                destEntity->setLayerId(dest.getLayerId(layerName));
                transaction.getPreviewExporter()->setLayerSource(&dest);
            }
        }
    }

    transaction.addObject(destEntity, false);

    if (transaction.isPreview()) {
        transaction.getPreviewExporter()->setLayerSource(NULL);
    }
}
