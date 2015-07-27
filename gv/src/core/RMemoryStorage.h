#ifndef RMEMORYSTOARGE_H_
#define RMEMORYSTOARGE_H_

#include <QSharedPointer>

#include "RStorage.h"
#include "RBlockReferenceEntity.h"

/**
 * Basic memory based storage.
 *
 * \ingroup core
 * \scriptable
 */
class RMemoryStorage: public RStorage {
public:
	RMemoryStorage();
	~RMemoryStorage();

    virtual void clear();

    virtual void setCurrentBlock(RBlock::Id blockId);

	virtual void beginTransaction();
	virtual void commitTransaction();
	virtual void rollbackTransaction();

    virtual QList<REntity::Id> orderBackToFront(const QSet<REntity::Id>& entityIds);

	virtual QSet<RObject::Id> queryAllObjects();
	virtual QSet<REntity::Id> queryAllEntities(bool undone = false, bool allBlocks = false);
	virtual QSet<RUcs::Id> queryAllUcs();
	virtual QSet<RLayer::Id> queryAllLayers(bool undone = false);
	virtual QSet<RBlock::Id> queryAllBlocks(bool undone = false);
	virtual QSet<RView::Id> queryAllViews(bool undone = false);
	virtual QSet<RLinetype::Id> queryAllLinetypes();
	virtual QSet<REntity::Id> querySelectedEntities();

	virtual QSet<REntity::Id> queryLayerEntities(RLayer::Id layerId);
	virtual QSet<REntity::Id> queryBlockEntities(RBlock::Id blockId);
	virtual QSet<REntity::Id> queryBlockReferences(RBlock::Id blockId);
    //virtual QSet<REntity::Id> queryViewEntities(RView::Id viewId);

	virtual QSharedPointer<RObject> queryObject(RObject::Id objectId);
	virtual QSharedPointer<REntity> queryEntity(REntity::Id objectId);
	virtual QSharedPointer<RLayer> queryLayer(RLayer::Id layerId);
	virtual QSharedPointer<RLayer> queryLayer(const QString& layerName);
	virtual QSharedPointer<RBlock> queryBlock(RBlock::Id blockId);
	virtual QSharedPointer<RBlock> queryBlock(const QString& blockName);
	virtual QSharedPointer<RView> queryView(RView::Id viewId);
	virtual QSharedPointer<RView> queryView(const QString& viewName);
	virtual QSharedPointer<RUcs> queryUcs(RUcs::Id ucsId);
	virtual QSharedPointer<RUcs> queryUcs(const QString& ucsName);

	virtual void clearEntitySelection(QSet<REntity::Id>* affectedEntities = NULL);
	virtual void selectEntity(REntity::Id entityId, bool add = false, 
        QSet<REntity::Id>* affectedEntities = NULL);
    virtual void deselectEntity(REntity::Id entityId,
        QSet<REntity::Id>* affectedEntities=NULL);
    virtual void selectEntities(QSet<REntity::Id>& entityIds, bool add = false,
			QSet<REntity::Id>* affectedEntities = NULL);
	virtual bool isSelected(REntity::Id entityId);

    virtual bool hasSelection() const;

	virtual RBox getBoundingBox();
	virtual RBox getSelectionBox();

	virtual bool saveObject(QSharedPointer<RObject> object);
    bool checkRecursion(
        RBlock::Id blockId, RBlock::Id potentialChildBlockId
    );
	virtual void deleteObject(RObject::Id objectId);
	virtual void saveTransaction(RTransaction& transaction);
	virtual void deleteTransactionsFrom(int transactionId);
	virtual RTransaction getTransaction(int transactionId);

	virtual void resetTransactionStack();

	virtual void toggleUndoStatus(QSet<RObject::Id>& objects);
	virtual void toggleUndoStatus(RObject::Id object);
	virtual void setUndoStatus(RObject::Id objectId, bool status);
	virtual int getMaxTransactionId();

    void setVariable(const QString& key, const QVariant & value);
    QVariant getVariable(const QString& key) const;

    virtual QSharedPointer<RLinetype> queryLinetype(RLinetype::Id linetypeId);
    virtual QSharedPointer<RLinetype> queryLinetype(const QString& linetypeName);

	virtual QString getLayerName(RLayer::Id layerId);
	virtual QSet<QString> getLayerNames();
	virtual RLayer::Id getLayerId(const QString& layerName);

	virtual QString getBlockName(RBlock::Id blockId);
	virtual QSet<QString> getBlockNames();
	virtual RBlock::Id getBlockId(const QString& blockName);

    virtual QString getViewName(RView::Id viewId);
    virtual QSet<QString> getViewNames();
    virtual RView::Id getViewId(const QString& viewName);

    virtual QString getLinetypeName(RLinetype::Id linetypeId);
    virtual QSet<QString> getLinetypeNames();
    virtual RLinetype::Id getLinetypeId(const QString& linetypeName);

	virtual QSharedPointer<RObject> queryObjectDirect(RObject::Id objectId);
	virtual QSharedPointer<REntity> queryEntityDirect(REntity::Id objectId);

    virtual void setLastTransactionId(int transactionId);

    virtual RLineweight::Lineweight getMaxLineweight();

protected:
	virtual QSharedPointer<RUcs> queryUcsDirect(RUcs::Id ucsId);
	virtual QSharedPointer<RLayer> queryLayerDirect(RLayer::Id layerId);

private:
	RObject::Id counter;
    int maxDrawOrder;
    RLineweight::Lineweight maxLineweight;
    bool inTransaction;
    bool boundingBoxChanged;
    RBox boundingBox;
	QHash<RObject::Id, QSharedPointer<RObject> > objectMap;
	QHash<REntity::Id, QSharedPointer<REntity> > entityMap;
    QHash<int, RTransaction> transactionMap;
    QHash<QString, QVariant> variables;
};

Q_DECLARE_METATYPE(RMemoryStorage*)

#endif /* RMEMORYSTOARGE_H_ */
