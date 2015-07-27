#include "RMemoryStorage.h"

RMemoryStorage::RMemoryStorage() :
    counter(0), 
    maxDrawOrder(0), 
    maxLineweight(RLineweight::Weight000), 
    inTransaction(false), 
    boundingBoxChanged(true) {

	setLastTransactionId(-1);
}

RMemoryStorage::~RMemoryStorage(){

}

void RMemoryStorage::clear() {
    counter = 0;
    maxDrawOrder = 0;
    maxLineweight = RLineweight::Weight000;
    inTransaction = false;
    boundingBoxChanged = true;
    boundingBox = RBox();
    objectMap.clear();
    entityMap.clear();
    transactionMap.clear();
    variables.clear();
    setLastTransactionId(-1);

    qDebug() << "clear:";
    qDebug() << *this;
}

void RMemoryStorage::setCurrentBlock(RBlock::Id blockId) {
    RStorage::setCurrentBlock(blockId);
    boundingBoxChanged = true;
}

QList<REntity::Id> RMemoryStorage::orderBackToFront(const QSet<REntity::Id>& entityIds) {
    QMap<int, REntity::Id> res;
    QSet<REntity::Id>::const_iterator it;
    maxDrawOrder = 0;
    for (it = entityIds.begin(); it != entityIds.end(); ++it) {
        QSharedPointer<REntity> e = queryEntityDirect(*it);
        if (!e.isNull()) {
            res.insertMulti(e->getDrawOrder(), *it);
            maxDrawOrder = qMax(e->getDrawOrder(), maxDrawOrder);
        }
    }
    return res.values();
}

bool RMemoryStorage::isSelected(REntity::Id entityId) {
    QSharedPointer<REntity> e = queryEntityDirect(entityId);
    return (!e.isNull() && e->isSelected());
}

void RMemoryStorage::beginTransaction() {
    inTransaction = true;

	// delete transactions that are lost for good due to this transaction:
	deleteTransactionsFrom(getLastTransactionId() + 1);
}

void RMemoryStorage::commitTransaction() {
    // copy transaction objects to memory storage:
    /*
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = transactionObjectMap.begin(); it!=transactionObjectMap.end(); ++it) {
        if (it.value().isNull()) {
            objectMap.remove(it.key());
        }
        else {
            objectMap[it.key()] = it.value();
        }
    }

    transactionObjectMap.clear();
    */
    inTransaction = false;
    boundingBoxChanged = true;
}

void RMemoryStorage::rollbackTransaction() {
    // drop transaction objects:
    //transactionObjectMap.clear();
    Q_ASSERT(false);
}

QSet<RObject::Id> RMemoryStorage::queryAllObjects() {
    //result = QSet<RObject::Id>::fromList(objectMap.keys());
    QSet<RObject::Id> result;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        if (!(*it).isNull() && !(*it)->isUndone()) {
            result.insert((*it)->getId());
        }
    }
    return result;
}

QSet<REntity::Id> RMemoryStorage::queryAllEntities(bool undone, bool allBlocks) {
    QSet<REntity::Id> result;
    result.reserve(entityMap.count());
    RBlock::Id currentBlock = getCurrentBlockId();
    QHash<REntity::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && (undone || !e->isUndone()) && (allBlocks
				|| e->getBlockId() == currentBlock)) {
			result.insert(e->getId());
		}
    }
    return result;
}

QSet<RUcs::Id> RMemoryStorage::queryAllUcs() {
    QSet<RUcs::Id> result;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RUcs> u = it->dynamicCast<RUcs>();
        if (!u.isNull() && !u->isUndone()) {
            result.insert(u->getId());
        }
    }
    return result;
}

QSet<RLayer::Id> RMemoryStorage::queryAllLayers(bool undone) {
    QSet<RLayer::Id> result;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RLayer> l = it->dynamicCast<RLayer>();
        if (!l.isNull() && (undone || !l->isUndone())) {
            result.insert(l->getId());
        }
    }
    return result;
}

QSet<RBlock::Id> RMemoryStorage::queryAllBlocks(bool undone) {
    QSet<RBlock::Id> result;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RBlock> l = it->dynamicCast<RBlock>();
        if (!l.isNull() && (undone || !l->isUndone())) {
            result.insert(l->getId());
        }
    }
    return result;
}

QSet<RView::Id> RMemoryStorage::queryAllViews(bool undone) {
    QSet<RView::Id> result;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RView> v = it->dynamicCast<RView>();
        if (!v.isNull() && (undone || !v->isUndone())) {
            result.insert(v->getId());
        }
    }
    return result;
}

QSet<RLinetype::Id> RMemoryStorage::queryAllLinetypes(){
    QSet<RLinetype::Id> result;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RLinetype> l = it->dynamicCast<RLinetype>();
        if (!l.isNull() && !l->isUndone()) {
            result.insert(l->getId());
        }
    }
    return result;
}

QSet<REntity::Id> RMemoryStorage::querySelectedEntities() {
    RBlock::Id currentBlock = getCurrentBlockId();
    QSet<REntity::Id> result;
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && !e->isUndone() && e->isSelected() && e->getBlockId()
				== currentBlock) {
			result.insert(e->getId());
		}
    }
    return result;
}

QSet<REntity::Id> RMemoryStorage::queryLayerEntities(RLayer::Id layerId){
    RBlock::Id currentBlock = getCurrentBlockId();
    QSet<REntity::Id> result;
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && e->getLayerId() == layerId && !e->isUndone()
				&& e->getBlockId() == currentBlock) {
			result.insert(e->getId());
		}
    }
    return result;
}

QSet<REntity::Id> RMemoryStorage::queryBlockEntities(RBlock::Id blockId){
    QSet<REntity::Id> result;
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && e->getBlockId() == blockId && !e->isUndone()) {
			result.insert(e->getId());
		}
    }
    return result;
}

QSet<REntity::Id> RMemoryStorage::queryBlockReferences(RBlock::Id blockId){
    QSet<REntity::Id> result;
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<RBlockReferenceEntity> e = it->dynamicCast<RBlockReferenceEntity>();
        if (!e.isNull() && e->getReferencedBlockId() == blockId && !e->isUndone()) {
			result.insert(e->getId());
		}
    }
    return result;
}

/*
QSet<REntity::Id> RMemoryStorage::queryViewEntities(RView::Id viewId){
    QSet<REntity::Id> result;
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && e->getViewId() == viewId && !e->isUndone()) {
			result.insert(e->getId());
		}
    }
    return result;
}
*/

QSharedPointer<RObject> RMemoryStorage::queryObject(RObject::Id objectId) {
	if (!objectMap.contains(objectId)) {
		return QSharedPointer<RObject> ();
	}
    if (!objectMap[objectId].isNull()) {
        return QSharedPointer<RObject>(objectMap[objectId]->clone());
    }
    return QSharedPointer<RObject>();
}

QSharedPointer<RObject> RMemoryStorage::queryObjectDirect(RObject::Id objectId) {
    return objectMap[objectId];
}

QSharedPointer<REntity> RMemoryStorage::queryEntity(REntity::Id objectId) {
	if (!entityMap.contains(objectId)) {
		return QSharedPointer<REntity> ();
	}
    if (!entityMap[objectId].isNull()) {
        return QSharedPointer<RObject>(entityMap[objectId]->clone()).dynamicCast<REntity>();
    }
    return QSharedPointer<REntity>();
}

QSharedPointer<REntity> RMemoryStorage::queryEntityDirect(REntity::Id objectId) {
    return entityMap[objectId];
}

QSharedPointer<RLayer> RMemoryStorage::queryLayerDirect(RLayer::Id layerId) {
    return objectMap[layerId].dynamicCast<RLayer>();
}

QSharedPointer<RLayer> RMemoryStorage::queryLayer(RLayer::Id layerId) {
	if (!objectMap.contains(layerId)) {
		return QSharedPointer<RLayer> ();
	}
	if (objectMap[layerId].isNull()) {
		return QSharedPointer<RLayer> ();
	}
    if (!objectMap[layerId].dynamicCast<RLayer>().isNull()) {
        return QSharedPointer<RLayer>((RLayer*)objectMap[layerId]->clone());
    }
    return QSharedPointer<RLayer>();
}

QSharedPointer<RLayer> RMemoryStorage::queryLayer(const QString& layerName) {
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RLayer> l = it->dynamicCast<RLayer>();
        if (!l.isNull() && l->getName() == layerName && !l->isUndone()) {
			return QSharedPointer<RLayer> (l->clone());
		}
    }
    return QSharedPointer<RLayer>();
}

QSharedPointer<RBlock> RMemoryStorage::queryBlock(RBlock::Id blockId) {
	if (!objectMap.contains(blockId)) {
		return QSharedPointer<RBlock> ();
	}
	if (objectMap[blockId].isNull()) {
		return QSharedPointer<RBlock> ();
	}
    if (!objectMap[blockId].dynamicCast<RBlock>().isNull()) {
        return QSharedPointer<RBlock>((RBlock*)objectMap[blockId]->clone());
    }
    return QSharedPointer<RBlock>();
}

QSharedPointer<RBlock> RMemoryStorage::queryBlock(const QString& blockName) {
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RBlock> l = it->dynamicCast<RBlock>();
        if (!l.isNull() && l->getName() == blockName && !l->isUndone()) {
			return QSharedPointer<RBlock> (l->clone());
		}
    }
    return QSharedPointer<RBlock>();
}

QString RMemoryStorage::getBlockName(RBlock::Id blockId){
    QSharedPointer<RBlock> l = queryBlock(blockId);
    if (l.isNull()) {
        return QString();
    }
    return l->getName();
}

QSet<QString> RMemoryStorage::getBlockNames() {
	QSet<QString> ret;
	QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
	for (it = objectMap.begin(); it != objectMap.end(); ++it) {
		QSharedPointer<RBlock> l = it->dynamicCast<RBlock> ();
		if (!l.isNull() && !l->isUndone()) {
			ret.insert(l->getName());
		}
	}
	return ret;
}

QString RMemoryStorage::getViewName(RView::Id viewId){
    QSharedPointer<RView> v = queryView(viewId);
    if (v.isNull()) {
        return QString();
    }
    return v->getName();
}

QSet<QString> RMemoryStorage::getViewNames() {
	QSet<QString> ret;
	QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
	for (it = objectMap.begin(); it != objectMap.end(); ++it) {
		QSharedPointer<RView> v = it->dynamicCast<RView> ();
		if (!v.isNull() && !v->isUndone()) {
			ret.insert(v->getName());
		}
	}
	return ret;
}

QSharedPointer<RView> RMemoryStorage::queryView(RView::Id viewId) {
	if (!objectMap.contains(viewId)) {
		return QSharedPointer<RView> ();
	}
	if (objectMap[viewId].isNull()) {
		return QSharedPointer<RView> ();
	}
    if (!objectMap[viewId].dynamicCast<RView>().isNull()) {
        return QSharedPointer<RView>((RView*)objectMap[viewId]->clone());
    }
    return QSharedPointer<RView>();
}

QSharedPointer<RView> RMemoryStorage::queryView(const QString& viewName) {
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RView> l = it->dynamicCast<RView>();
        if (!l.isNull() && l->getName() == viewName && !l->isUndone()) {
			return QSharedPointer<RView> (l->clone());
		}
    }
    return QSharedPointer<RView>();
}

QSharedPointer<RUcs> RMemoryStorage::queryUcsDirect(RUcs::Id ucsId) {
    return objectMap[ucsId].dynamicCast<RUcs>();
}

QSharedPointer<RUcs> RMemoryStorage::queryUcs(RUcs::Id ucsId) {
	if (!objectMap.contains(ucsId)) {
		return QSharedPointer<RUcs> ();
	}
    if (!objectMap[ucsId].dynamicCast<RUcs>().isNull()) {
        return QSharedPointer<RObject>(objectMap[ucsId]->clone()).dynamicCast<RUcs>();
    }
    return QSharedPointer<RUcs>();
}

QSharedPointer<RUcs> RMemoryStorage::queryUcs(const QString& ucsName) {
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RUcs> u = it->dynamicCast<RUcs>();
        if (!u.isNull() && u->name==ucsName) {
            return u;
        }
    }

    return QSharedPointer<RUcs>();
}

QSharedPointer<RLinetype> RMemoryStorage::queryLinetype(RLinetype::Id linetypeId){
    return objectMap[linetypeId].dynamicCast<RLinetype>();
}

QSharedPointer<RLinetype> RMemoryStorage::queryLinetype(const QString& linetypeName){
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RLinetype> l = it->dynamicCast<RLinetype>();
        if (!l.isNull() && !l->getName().compare(linetypeName, Qt::CaseInsensitive)) {
            return l;
        }
    }

    return QSharedPointer<RLinetype>();
}

void RMemoryStorage::clearEntitySelection(QSet<REntity::Id>* affectedEntities) {
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && e->isSelected()) {
            if (affectedEntities!=NULL) {
                affectedEntities->insert(e->getId());
            }
            e->setSelected(false);
        }
    }
}

void RMemoryStorage::selectEntity(REntity::Id entityId, bool add, 
    QSet<REntity::Id>* affectedEntities) {

    if (!add) {
        QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
        for (it = entityMap.begin(); it != entityMap.end(); ++it) {
            QSharedPointer<REntity> e = *it;
            if (!e.isNull() && e->isSelected() && e->getId()!=entityId) {
                if (affectedEntities!=NULL) {
                    affectedEntities->insert(e->getId());
                }
                e->setSelected(false);
            }
        }
    }
    
    QSharedPointer<REntity> e = queryEntityDirect(entityId);
    if (!e.isNull() && !e->isSelected()) {
        if (affectedEntities!=NULL) {
            affectedEntities->insert(e->getId());
        }
        e->setSelected(true);
    }
}

void RMemoryStorage::deselectEntity(REntity::Id entityId,
    QSet<REntity::Id>* affectedEntities) {

    QSharedPointer<REntity> e = queryEntityDirect(entityId);
    if (!e.isNull() && e->isSelected()) {
        if (affectedEntities!=NULL) {
            affectedEntities->insert(e->getId());
        }
        e->setSelected(false);
    }
}

void RMemoryStorage::selectEntities(QSet<REntity::Id>& entityIds, bool add,
        QSet<REntity::Id>* affectedEntities) {

    if (!add) {
        QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
        for (it = entityMap.begin(); it != entityMap.end(); ++it) {
            QSharedPointer<REntity> e = *it;
            if (!e.isNull() && e->isSelected() && !entityIds.contains(e->getId())) {
                if (affectedEntities!=NULL) {
                    affectedEntities->insert(e->getId());
                }
                e->setSelected(false);
            }
        }
    }
    
    QSet<REntity::Id>::iterator it;
    for (it = entityIds.begin(); it != entityIds.end(); ++it) {
        QSharedPointer<REntity> e = queryEntityDirect(*it);
        if (!e.isNull() && !e->isSelected()) {
            if (affectedEntities!=NULL) {
                affectedEntities->insert(e->getId());
            }
            e->setSelected(true);
        }
    }
}

bool RMemoryStorage::hasSelection() const {
    RBlock::Id currentBlock = getCurrentBlockId();
    QHash<RObject::Id, QSharedPointer<REntity> >::const_iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && !e->isUndone() && e->isSelected() && e->getBlockId()
                == currentBlock) {
            return true;
        }
    }

    return false;
}

RBox RMemoryStorage::getBoundingBox() {
    if (!boundingBoxChanged) {
        return boundingBox;
    }

    RBlock::Id currentBlockId = getCurrentBlockId();
    boundingBox = RBox();
    maxLineweight = RLineweight::Weight000;
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && !e->isUndone()) {
            if (e->getBlockId() == currentBlockId) {
                if (!boundingBox.isValid()) {
                    boundingBox = e->getBoundingBox();
                } else {
                    boundingBox.growToInclude(e->getBoundingBox());
                }
            }
            if (!boundingBox.isValid()) {
                maxLineweight = e->getLineweight();
            } else {
                maxLineweight = qMax(e->getLineweight(), maxLineweight);
            }
        }
    }

    boundingBoxChanged = false;
    return boundingBox;
}

RBox RMemoryStorage::getSelectionBox() {
    RBlock::Id currentBlock = getCurrentBlockId();
    RBox ret;
    QHash<RObject::Id, QSharedPointer<REntity> >::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        QSharedPointer<REntity> e = *it;
        if (!e.isNull() && !e->isUndone() && e->isSelected() && e->getBlockId()
				== currentBlock) {
			if (!ret.isValid()) {
				ret = e->getBoundingBox();
			} else {
				ret.growToInclude(e->getBoundingBox());
			}
		}
    }

    return ret;
}

bool RMemoryStorage::saveObject(QSharedPointer<RObject> object) {
    if (object.isNull()) {
        return false;
    }

    // never allow two layers with identical names, update layer instead:
    QSharedPointer<RLayer> layer = object.dynamicCast<RLayer>();
    if (!layer.isNull()) {
        RLayer::Id id = getLayerId(layer->getName());
        if (id != RLayer::INVALID_ID) {
            setObjectId(*layer, id);
        }
    }

    // never allow two blocks with identical names, update block instead:
	QSharedPointer<RBlock> block = object.dynamicCast<RBlock> ();
	if (!block.isNull()) {
		RBlock::Id id = getBlockId(block->getName());
		if (id != RBlock::INVALID_ID) {
			setObjectId(*block, id);
		}
	}

    // avoid block recursions:
	QSharedPointer<RBlockReferenceEntity> blockRef = object.dynamicCast<
			RBlockReferenceEntity> ();
	if (!blockRef.isNull()) {
        RBlock::Id id = blockRef->getBlockId();
        RBlock::Id refId = blockRef->getReferencedBlockId();
        // check if block with 'id' may contain a block reference which refers to
        // block with 'refid':
        if(checkRecursion(id, refId)){
			RDebug::error(QString("RMemoryStorage::saveObject: recursion found"));
            return false;
		}
	}

    // assign new object ID to new objects:
    if (object->getId() == RObject::INVALID_ID) {
		setObjectId(*object, ++counter);
        // assign draw order to new entities:
        QSharedPointer<REntity> entity = object.dynamicCast<REntity> ();
        if (!entity.isNull()) {
            entity->setDrawOrder(maxDrawOrder++);
        }
	}

    // TODO: save original object for rollback:
    if (inTransaction) {
        //transactionObjectMap[object->getId()] = object;
    }

    objectMap[object->getId()] = object;

	QSharedPointer<REntity> entity = object.dynamicCast<REntity> ();
	if (!entity.isNull()) {
        entityMap[object->getId()] = entity;
        maxDrawOrder = qMax(entity->getDrawOrder(), maxDrawOrder);
    }

    return true;
}

/**
 * Checks recursively, if the given block is allowed to contain
 * references to the potential child block.
 *
 * \return true if a recusrion has been found.
 */
bool RMemoryStorage::checkRecursion(
    RBlock::Id blockId, RBlock::Id potentialChildBlockId) {

    RDebug::debug(QString("RMemoryStorage::checkRecursion: id: %1").arg(blockId));
    RDebug::debug(QString("RMemoryStorage::checkRecursion: id: %1").arg(potentialChildBlockId));

    if (blockId==potentialChildBlockId) {
        return true;
    }

    // iterate through all entities inside potential child block and check
    // if anything refers back to the given block:
    QSet<REntity::Id> ids = queryBlockEntities(potentialChildBlockId);
	QSet<REntity::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); ++it) {
		QSharedPointer<REntity> e = queryEntity(*it);
        QSharedPointer<RBlockReferenceEntity> blockRef = e.dynamicCast<
				RBlockReferenceEntity> ();
        if (blockRef.isNull()) {
            continue;
        }

        RDebug::debug(
                QString(
                        "RMemoryStorage::checkRecursion: blockRef2->getReferencedBlockId(): %1").arg(
                        blockRef->getReferencedBlockId()));
        if (blockRef->getReferencedBlockId() == blockId) {
            return true;
        }
        if (checkRecursion(blockId, blockRef->getReferencedBlockId())) {
            return true;
        }
	}
	return false;
}

void RMemoryStorage::deleteObject(RObject::Id objectId) {
    // TODO: save original object for rollback:
    if (inTransaction) {
        //transactionObjectMap.insert(objectId, QSharedPointer<RObject>());
    }

    objectMap.remove(objectId);
    if (entityMap.contains(objectId)) {
        entityMap.remove(objectId);
    }
}

void RMemoryStorage::saveTransaction(RTransaction& transaction) {
	// if the given transaction is not undoable, we don't need to
	// store anything here:
	if (!transaction.isUndoable()) {
		return;
	}

	// assign new unique ID for this transaction:
	transaction.setId(getLastTransactionId() + 1);

	// delete transactions that are lost for good due to this transaction:
	//deleteTransactionsFrom(transaction.getId());

    transactionMap[transaction.getId()] = transaction;

	setLastTransactionId(transaction.getId());
}

void RMemoryStorage::deleteTransactionsFrom(int transactionId) {
    QSet<int> keysToRemove;

    {
        QHash<int, RTransaction>::iterator it;
        for (it = transactionMap.begin(); it!=transactionMap.end(); ++it) {
            if (it.key()>=transactionId) {
                // delete orphaned objects:
                QSet<RObject::Id> affectedObjects =
                    it.value().getAffectedObjects();

                QSet<RObject::Id>::iterator it2;
                for (it2=affectedObjects.begin(); it2!=affectedObjects.end(); ++it2) {
                    QSharedPointer<RObject> obj = queryObjectDirect(*it2);
                    if (!obj.isNull() && obj->isUndone()) {
                        deleteObject(*it2);
                    }
                }

                // delete transaction:
                keysToRemove.insert(it.key());
            }
        }
    }

    {
        QSet<int>::iterator it;
        for (it=keysToRemove.begin(); it!=keysToRemove.end(); ++it) {
            transactionMap.remove(*it);
        }
    }
}

RTransaction RMemoryStorage::getTransaction(int transactionId) {
    if (transactionMap.contains(transactionId)) {
        return transactionMap[transactionId];
    }
    else {
        RDebug::error("RMemoryStorage::getTransaction: transaction not found: %d", transactionId);
        return RTransaction(*this);
    }
}

void RMemoryStorage::resetTransactionStack() {
    transactionMap.clear();
	setLastTransactionId(-1);
}

void RMemoryStorage::toggleUndoStatus(QSet<RObject::Id>& objects) {
    QSet<RObject::Id>::iterator it;
    for (it=objects.begin(); it!=objects.end(); ++it) {
        toggleUndoStatus(*it);
    }
}

void RMemoryStorage::toggleUndoStatus(RObject::Id objectId) {
    QSharedPointer<RObject> obj = queryObjectDirect(objectId);
    if (!obj.isNull()) {
        obj->setUndone(!obj->isUndone());
    }
}

void RMemoryStorage::setUndoStatus(RObject::Id objectId, bool status){
    QSharedPointer<RObject> obj = queryObjectDirect(objectId);
	if (obj.isNull()) {
		RDebug::error(QString("RMemoryStorage::setUndoStatus: object is NULL"));
	}
	obj->setUndone(status);
}

int RMemoryStorage::getMaxTransactionId() {
    int ret = -1;

    QHash<int, RTransaction>::iterator it;
    for (it = transactionMap.begin(); it!=transactionMap.end(); ++it) {
        if (it.key()>ret) {
            ret = it.key();
        }
    }

    return ret;
}

void RMemoryStorage::setVariable(const QString& key, const QVariant & value) {
    variables[key] = value;
}

QVariant RMemoryStorage::getVariable(const QString& key) const {
    if (!variables.contains(key)) {
        return QVariant();
    }
    return variables[key];
}

QString RMemoryStorage::getLayerName(RLayer::Id layerId){
    QSharedPointer<RLayer> l = queryLayer(layerId);
    if (l.isNull()) {
        return QString();
    }
    return l->getName();
}

QSet<QString> RMemoryStorage::getLayerNames() {
    QSet<QString> ret;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RLayer> l = it->dynamicCast<RLayer>();
        if (!l.isNull() && !l->isUndone()) {
            ret.insert(l->getName());
        }
    }
    return ret;
}

RLayer::Id RMemoryStorage::getLayerId(const QString& layerName) {
    QSharedPointer<RLayer> l = queryLayer(layerName);
    if (l.isNull()) {
        return RLayer::INVALID_ID;
    }
    return l->getId();
}

RBlock::Id RMemoryStorage::getBlockId(const QString& blockName) {
    QSharedPointer<RBlock> b = queryBlock(blockName);
    if (b.isNull()) {
        return RBlock::INVALID_ID;
    }
    return b->getId();
}

RView::Id RMemoryStorage::getViewId(const QString& viewName) {
    QSharedPointer<RView> v = queryView(viewName);
    if (v.isNull()) {
        return RView::INVALID_ID;
    }
    return v->getId();
}

QString RMemoryStorage::getLinetypeName(RLinetype::Id linetypeId) {
    QSharedPointer<RLinetype> l = queryLinetype(linetypeId);
    if (l.isNull()) {
        return QString();
    }
    return l->getName();
}

QSet<QString> RMemoryStorage::getLinetypeNames(){
    QSet<QString> ret;
    QHash<RObject::Id, QSharedPointer<RObject> >::iterator it;
    for (it = objectMap.begin(); it != objectMap.end(); ++it) {
        QSharedPointer<RLinetype> l = it->dynamicCast<RLinetype>();
        if (!l.isNull() && !l->isUndone()) {
            ret.insert(l->getName());
        }
    }
    return ret;
}

RLinetype::Id RMemoryStorage::getLinetypeId(const QString& linetypeName){
    QSharedPointer<RLinetype> l = queryLinetype(linetypeName);
    if (l.isNull()) {
        return RLinetype::INVALID_ID;
    }
    return l->getId();
}

void RMemoryStorage::setLastTransactionId(int transactionId) {
    RStorage::setLastTransactionId(transactionId);
    boundingBoxChanged = true;
}

RLineweight::Lineweight RMemoryStorage::getMaxLineweight() {
    return maxLineweight;
}
