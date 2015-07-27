#include "RExporter.h"
#include "RStorage.h"
#include "RTransaction.h"
#include "RDocument.h"

RTransaction::RTransaction()
    : storage(NULL),
      previewExporter(NULL),
      transactionId(-1),
      undoable(true),
      failed(false) {
}


/**
 * Constructs an empty, invalid transaction.
 */
RTransaction::RTransaction(RStorage& storage)
    : storage(&storage),
      previewExporter(NULL),
      transactionId(-1),
      undoable(true),
      failed(false) {
}



/**
 * Constructor for existing transactions. This is used by storage
 * implementations to instantiate transactions from a storage (e.g. DB).
 */
RTransaction::RTransaction(
    RStorage& storage,
    int transactionId,
    const QString& text,
    const QSet<RObject::Id>& affectedObjects,
    //const QSet<RObject::Id>& affectedEntities,
    const QMultiMap<RObject::Id, RPropertyChange>& propertyChanges,
    RTransaction* parent)
    : storage(&storage),
      previewExporter(NULL),
      transactionId(transactionId),
      text(text),
      affectedObjects(affectedObjects),
      //affectedEntities(affectedEntities),
      propertyChanges(propertyChanges),
      undoable(true),
      failed(false) {

    if (parent!=NULL) {
        parent->appendChild(*this);
    }

}



/**
 * Constructor for new transactions.
 */
RTransaction::RTransaction(
    RStorage& storage,
    const QString& text,
    bool undoable,
    RTransaction* parent)
    : storage(&storage),
      previewExporter(NULL),
      transactionId(-1),
      text(text),
      undoable(undoable),
      failed(false) {

    if (parent!=NULL) {
        parent->appendChild(*this);
    }

    this->storage->beginTransaction();
}



RTransaction::~RTransaction() {}


void RTransaction::setPreviewExporter(RExporter* e) {
    previewExporter = e;
}

RExporter* RTransaction::getPreviewExporter() {
    return previewExporter;
}

/*
QSet<REntity::Id> RTransaction::getAffectedEntites(){
    return affectedEntities;
}
*/


/**
 * (Re-)applies this transaction to the document.
 */
void RTransaction::redo(RDocument* objectContainer) {
    // iterate through all objects that were affected by this transaction:
    QSet<RObject::Id>::iterator it;
    for (it=affectedObjects.begin(); it!=affectedObjects.end(); ++it) {
        // no properties have changed for this object,
        // i.e. object was created or deleted:
        if (propertyChanges.count(*it)==0) {
            // toggle undo status of affected object:
            storage->toggleUndoStatus(*it);
        } else {
            // redo property changes:
        	QSharedPointer<RObject> object = storage->queryObject(*it);
            if (object.isNull()) {
                RDebug::error("RTransaction::redo: object '%d' not found in storage", *it);
                continue;
            }
            if (objectContainer != NULL) {
				objectContainer->removeFromSpatialIndex(object);
			}

            QList<RPropertyChange> objectChanges = propertyChanges.values(*it);
            for (int i=0; i<objectChanges.size(); ++i) {
                RPropertyTypeId propertyTypeId = objectChanges.at(i).propertyTypeId;
                object->setProperty(propertyTypeId, objectChanges.at(i).newValue);
            }

            storage->saveObject(object);
        }
    }
}



/**
 * Undoes this transaction.
 */
void RTransaction::undo(RDocument* objectContainer) {
    // iterate through all objects that were affected by this transaction:
    QSet<RObject::Id>::iterator it;
    for (it=affectedObjects.begin(); it!=affectedObjects.end(); ++it) {
        // no properties have changed for this object,
        // i.e. object was created or deleted:
        if (propertyChanges.count(*it)==0) {
            // toggle undo status of affected object:
            storage->toggleUndoStatus(*it);
        }
        else {
            // undo property changes:
        	QSharedPointer<RObject> object = storage->queryObject(*it);
            if (object.isNull()) {
                RDebug::error("RTransaction::undo: "
                    "object '%d' not found in storage", *it);
                continue;
            }
            if (objectContainer != NULL) {
				objectContainer->removeFromSpatialIndex(object);
			}

            QList<RPropertyChange> objectChanges = propertyChanges.values(*it);
            for (int i=0; i<objectChanges.size(); ++i) {
                RPropertyTypeId propertyTypeId = objectChanges.at(i).propertyTypeId;
                object->setProperty(propertyTypeId, objectChanges.at(i).oldValue);
            }

            storage->saveObject(object);
        }
    }
    //qDebug() << "RTransaction::undo() storage:" << *storage;
}



/**
 * Saves this command to the storage of the document.
 */
void RTransaction::commit() {
    if (failed) {
            RDebug::debug(QString(
                            "RTransaction::commit: transaction is in state 'failed'"));
            return;
    }
    if (affectedObjects.size()>0) {
        storage->saveTransaction(*this);
    }
    storage->commitTransaction();

    //qDebug() << "RTransaction::commit() storage:" << *storage;
}


void RTransaction::rollback() {
    storage->rollbackTransaction();
}

void RTransaction::end() {
    if (failed) {
        rollback();
    } else {
        commit();
    }
}

/**
 * Adds the given object to the transaction. Adding an object to a
 * transaction means that this transaction modifies or adds the object.
 * Entities that are added with an invalid layer or block ID are placed
 * on the current layer / block.
 *
 * @param modifiedPropertyTypeId: Property ID that has changed if known 
 *      by caller, NULL otherwise.
 * \todo remove useCurrentAttributes
 */
bool RTransaction::addObject(QSharedPointer<RObject> object,
    bool useCurrentAttributes,
    const QSet<RPropertyTypeId>& modifiedPropertyTypeIds) {

    if(object.isNull()){
        RDebug::error(QString("RTransaction::addObject: object is NULL"));
        failed=true;
        return false;
    }
    if (storage == NULL) {
        RDebug::error(QString("RTransaction::addObject: storage is NULL"));
        failed=true;
        return false;
    }
    if (transactionId != -1) {
        RDebug::error("RTransaction::addObject: "
                "Trying to add object to an existing transaction. "
                "Transactions are immutable once committed.");
        failed=true;
        Q_ASSERT(false);
    }
    if (object->getDocument()==NULL) {
        RDebug::error("RTransaction::addObject: "
            "Object is not linked to a document.");
        failed=true;
        Q_ASSERT(false);
    }

    QSharedPointer<REntity> entity = object.dynamicCast<REntity>();
    if (!entity.isNull()) {
        // place entity on current layer / block, set current pen attributes:
        if (useCurrentAttributes || entity->getLayerId()==RLayer::INVALID_ID) {
            entity->setLayerId(object->getDocument()->getCurrentLayerId());
        }
        if (useCurrentAttributes || entity->getBlockId()==RBlock::INVALID_ID) {
            entity->setBlockId(object->getDocument()->getCurrentBlockId());
        }
        if (useCurrentAttributes || !entity->getColor().isValid()) {
            entity->setColor(object->getDocument()->getCurrentColor());
        }
        if (useCurrentAttributes || entity->getLineweight()==RLineweight::WeightInvalid) {
            entity->setLineweight(object->getDocument()->getCurrentLineweight());
        }
        if (useCurrentAttributes || entity->getLinetypeId()==RLinetype::INVALID_ID) {
            entity->setLinetype(object->getDocument()->getCurrentLinetype());
        }
    }

    bool objectHasChanged = false;

    // this is an existing object that might have changed:
    if (object->getId() != RObject::INVALID_ID) {
        // store diff between previous object and this object
        // as part of this transaction:
        QSharedPointer<RObject> oldObject =
            storage->queryObjectDirect(object->getId());
        if (oldObject.isNull()) {
            RDebug::error("RTransaction::addObject: "
                "original object not found in storage");
            failed=true;
            return false;
        }

        if (typeid(*oldObject) != typeid(*object)) {
                RDebug::error("RTransaction::addObject: "
                        "type of original object and "
                        "type of modified object don't match");
                failed=true;
                return false;
        }

        if (oldObject->getId() != object->getId()) {
                RDebug::error("RTransaction::addObject: "
                        "id of original object and "
                        "id of modified object don't match");
                failed=true;
                return false;
        }

        // iterate through all properties of the original object
        // and store the property changes (if any):
        QSet<RPropertyTypeId> propertyTypeIds;
        if (modifiedPropertyTypeIds.isEmpty()) {
            propertyTypeIds = object->getPropertyTypeIds();
        }
        else {
            propertyTypeIds = modifiedPropertyTypeIds;
        }

        QSet<RPropertyTypeId>::iterator it;
        for (it=propertyTypeIds.begin(); it!=propertyTypeIds.end(); ++it) {
            QPair<QVariant, RPropertyAttributes> newProperty =
                object->getProperty(*it);
            QPair<QVariant, RPropertyAttributes> oldProperty =
                oldObject->getProperty(*it);

            objectHasChanged |= addPropertyChange(
                object->getId(),
                RPropertyChange(
                    *it, 
                    oldProperty.first,
                    newProperty.first
                )
            );
        }

        if (objectHasChanged) {
            if (object->getDocument()!=NULL) {
                object->getDocument()->removeFromSpatialIndex(oldObject);
            }
        }
    }

    bool ret = true;
    // this is a new object or an existing object that has changed:
    if (object->getId()==RObject::INVALID_ID || objectHasChanged) {
        // preview transaction only through exporter:
        if (previewExporter!=NULL) {
            QSharedPointer<REntity> e = object.dynamicCast<REntity>();
            if (!e.isNull()) {
                previewExporter->exportEntity(*e.data());
            }
        }
        else {
            ret = storage->saveObject(object);
            addAffectedObject(object->getId());
        }
    }
    else {
        RDebug::warning(QString("RTransaction::addObject: "
            "object not new and not changed"));
    }
    if (!ret) {
    	RDebug::error(QString("RTransaction::addObject: saveObject() failed"));
    	failed=true;
    }
    return ret;
}



/**
 * Adds the given property change for the given object to this transaction.
 */
bool RTransaction::addPropertyChange(RObject::Id objectId, const RPropertyChange& propertyChange) {
    if (!RS::compare(propertyChange.oldValue, propertyChange.newValue)) {
        propertyChanges.insert(objectId, propertyChange);
        return true;
    }
    return false;
}


/**
 * Adds the given object to the list of objects that are
 * affected by this transaction.
 */
void RTransaction::addAffectedObject(RObject::Id objectId) {
	if (storage == NULL) {
		return;
	}

	affectedObjects.insert(objectId);
    /*
	if (storage->isEntity(objectId)) {
		affectedEntities.insert(objectId);
	}
    */
}


void RTransaction::deleteObject(RObject::Id objectId, RDocument* container) {
	if (storage == NULL || container == NULL) {
		return;
	}

	QSharedPointer<RObject> object = storage->queryObject(objectId);
	if (object.isNull()) {
		RDebug::error("RTransaction::deleteObject: "
			"original object not found in storage");
		failed = true;
		return;
	}

    // if a layer is deleted, delete all entities on the layer:
	QSharedPointer<RLayer> layer = object.dynamicCast<RLayer>();
	if (!layer.isNull()) {
		if (layer->getName() == "0") {
			RDebug::warning(
					QString("RTransaction::deleteObject: trying to delete the default layer \"0\""));
			return;
		}
		QSet<REntity::Id> ids =
            storage->queryLayerEntities(objectId);
		QSetIterator<REntity::Id> i(ids);
		while (i.hasNext()) {
			deleteObject(i.next(), container);
		}
		if (objectId == storage->getCurrentLayerId()) {
			storage->setCurrentLayer("0");
		}
	}

    // if a block is deleted, delete all entities on the block:
	QSharedPointer<RBlock> block = object.dynamicCast<RBlock> ();
	if (!block.isNull()) {
		if (block->getName() == "*Model_Space") {
			RDebug::warning(QString("RTransaction::deleteObject: trying to delete the model space block"));
			return;
		}

        // delete all entities of this block definition:
		QSet<REntity::Id> ids = storage->queryBlockEntities(objectId);
		QSetIterator<REntity::Id> i(ids);
		while (i.hasNext()) {
			deleteObject(i.next(), container);
		}
		if (objectId == storage->getCurrentBlockId()) {
			storage->setCurrentBlock("*Model_Space");
		}

        // delete all block references to this block:
        ids = storage->queryBlockReferences(objectId);
		i = QSetIterator<REntity::Id>(ids);
		while (i.hasNext()) {
			deleteObject(i.next(), container);
		}
	}

    // if a view is deleted, reset current view:
    QSharedPointer<RView> view = object.dynamicCast<RView>();
    if (!view.isNull()) {
        if (objectId == storage->getCurrentViewId()) {
            storage->setCurrentView(QString());
        }
    }

   	container->removeFromSpatialIndex(object);

	addAffectedObject(objectId);

	if (!undoable) {
		storage->deleteObject(objectId);
	} else {
		storage->setUndoStatus(objectId, true);
	}
}

/**
 * Appends the given child to the list of child transactions of
 * this transaction. This transaction takes o
 */
void RTransaction::appendChild(RTransaction& t) {
    childTransactions.push_back(t);
}

/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, RTransaction& t) {
	dbg.nospace() << "RTransaction(";

	dbg.nospace() << "id: " << t.getId();
	dbg.nospace() << ", text: " << t.getText();

    dbg.nospace() << "\n, affectedObjects: ";
	QSet<REntity::Id> objs = t.getAffectedObjects();
	QSetIterator<RObject::Id> i(objs);
	while (i.hasNext()) {
		dbg.nospace() << i.next() << ", ";
	}

    dbg.nospace() << "\n, propertyChanges: (";
    QMultiMap<RObject::Id, RPropertyChange> propertyChanges =
        t.getPropertyChanges();
    QMultiMap<RObject::Id, RPropertyChange>::iterator it;
    for (it = propertyChanges.begin(); it != propertyChanges.end(); ++it) {
         dbg.nospace() << "objectId: " << it.key() << ", " << it.value() << "\n";
    }
    dbg.nospace() << "\n)\n";

	dbg.nospace() << ")";
	return dbg.space();
}

