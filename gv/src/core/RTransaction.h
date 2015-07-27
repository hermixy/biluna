#ifndef RTRANSACTION_H
#define RTRANSACTION_H

#include <QList>
#include <QMap>
#include <QSet>
#include <QSharedPointer>

#include "RObject.h"
#include "REntity.h"
#include "RPropertyTypeId.h"
#include "RPropertyChange.h"

class RStorage;
class RExporter;
class RDocument;

/**
 * Transaction implementation.
 * A transaction encapsulates a change of state of a document.
 * All editing in the application is done by creating instances of
 * transactions. Transaction objects can be applied to a document and are
 * handled by an \ref RTransactionStack. Transactions are stored in a
 * storage.
 *
 * Transactions can be undone / redone.
 *
 * \ingroup core
 * \scriptable
 */
class RTransaction {
public:
	RTransaction();

	RTransaction(RStorage& storage);

	RTransaction(RStorage& storage, int transactionId, const QString& text,
			const QSet<RObject::Id>& affectedObjects,
            //const QSet<RObject::Id>& affectedEntities,
            const QMultiMap<RObject::Id, RPropertyChange>& propertyChanges,
			RTransaction* parent = NULL);

	RTransaction(RStorage& storage, const QString& text, bool undoable = true,
			RTransaction* parent = NULL);

	virtual ~RTransaction();

    void setPreviewExporter(RExporter* e);
    RExporter* getPreviewExporter();
    bool isPreview() {
        return previewExporter!=NULL;
    }

	/**
	 * \return True if this transaction should store undo / redo information
	 *      for later undo / redo functionality.
	 */
	bool isUndoable() {
		return undoable;
	}

	bool isFailed() {
		return failed;
	}

	/**
	 * Applies this command to the document.
	 */
    virtual void redo(RDocument* objectContainer = NULL);

	/**
	 * Reverts a change to the document. After undo() is called, the state of
	 * the document should be the same as before redo() was called.
	 */
    virtual void undo(RDocument* objectContainer = NULL);

	/**
	 * Previews this transaction (as it would be applied) by exporting the result to
	 * the given exporter. The exporter typically exports into the preview of a
	 * graphics scene.
	 */
	//virtual void preview(RExporter& /*exporter*/) const {}

	void end();

	/**
	 * Sets the ID of this transaction. Typically called by a storage which manages
	 * transaction IDs.
	 */
	void setId(int id) {
		transactionId = id;
	}

	/**
	 * \return Unique ID of this transaction.
	 */
	int getId() {
		return transactionId;
	}

	/**
	 * \return Logged text for this transaction.
	 */
	QString getText() {
		return text;
	}

	//	void addUnmodifiedObject(QSharedPointer<RObject> object);
    bool addObject(QSharedPointer<RObject> object,
        bool useCurrentAttributes = true,
        const QSet<RPropertyTypeId>& modifiedPropertyTypeIds =
            QSet<RPropertyTypeId>());

	void addAffectedObject(RObject::Id objectId);

    void deleteObject(RObject::Id objectId, RDocument* container);

	/**
	 * \return Set of object IDs of objects that are affected by this command.
	 */
	QSet<RObject::Id> getAffectedObjects() {
		return affectedObjects;
	}

	QSet<REntity::Id> getAffectedEntites();

	QMultiMap<RObject::Id, RPropertyChange> getPropertyChanges() {
		return propertyChanges;
	}

protected:
    bool addPropertyChange(RObject::Id objectId, const RPropertyChange& propertyChange);
	void appendChild(RTransaction& t);

	void commit();
	void rollback();

protected:
	/**
	 * A transaction always belongs to the storage of a document.
	 */
	RStorage* storage;

    /**
     * A transaction can export results to an expoter instead of
     * changing the storage.
     */
    RExporter* previewExporter;

	/**
	 * Unique ID of this transaction.
	 */
	int transactionId;

	/**
	 * Log entry for this transaction
	 */
	QString text;

    /**
     * Set of IDs of all objects that are affected by this transaction.
     */
	QSet<RObject::Id> affectedObjects;

    /**
     * Set of IDs of all entities that are affected by this transaction.
     */
    //QSet<RObject::Id> affectedEntities;

	/**
	 * Map of properties that are changed by this transaction. For every
	 * undoable, a whole list of properties might have changed.
	 */
	QMultiMap<RObject::Id, RPropertyChange> propertyChanges;

	/**
	 * Child transactions of this transaction.
	 */
	QList<RTransaction> childTransactions;

	/**
	 * True if this transaction can be undone.
	 */
	bool undoable;

    /**
     * True if this transaction failed.
     */
	bool failed;
};

QDebug operator<<(QDebug dbg, RTransaction& t);

Q_DECLARE_METATYPE(RTransaction)
Q_DECLARE_METATYPE(RTransaction*)

#endif
