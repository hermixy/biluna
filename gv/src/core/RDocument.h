#ifndef RDOCUMENT_H
#define RDOCUMENT_H

#include <QString>
#include <QSharedPointer>

#include "RDocument.h"
#include "REntity.h"
#include "RNonCopyable.h"
#include "RRequireHeap.h"
#include "RSpatialIndex.h"
#include "RTransactionStack.h"
#include "RUcs.h"
#include "RLayer.h"
#include "RLinetype.h"
#include "RView.h"

class RVector;
class RStorage;



/**
 * A graphics document contains and owns entities, layers, user coordinate
 * systems, variables, block definitions, etc.
 *
 * Every document maintains a spatial index for fast entity lookups.
 * Documents also handle transactions through a \ref RTransactionStack.
 * A document is backed by an \ref RStorage implementation to
 * store the document for the duration of the document's lifetime.
 *
 * \ingroup core
 * \scriptable
 */
class RDocument : public RRequireHeap, RNonCopyable {
public:
    RDocument(RStorage& storage, RSpatialIndex& spatialIndex);
    ~RDocument();

    void init();

    RStorage& getStorage();
    RSpatialIndex& getSpatialIndex();
    RTransactionStack& getTransactionStack();

    void clear();

    REntity::Id queryClosestXY(
        const RVector& wcsPosition,
        double range
    );
    REntity::Id queryClosestXY(
        QSet<REntity::Id>& candidates,
        const RVector& wcsPosition,
        double range
    );

    /*
    REntity::Id queryClosest(
        const RVector& wcsPosition,
        double range
    );
    REntity::Id queryClosest(
        const RVector& wcsPosition,
        double range,
        double zMin,
        double zMax
    );
    REntity::Id queryClosest(
        QSet<REntity::Id>& candidates,
        const RVector& wcsPosition,
        double range
    );

    REntity::Id queryClosestToLine(
        const RLine& wcsRay,
        double range
    );
    REntity::Id queryClosestToLine(
        QSet<REntity::Id>& candidates,
        const RLine& wcsRay,
        double range
    );
    */

    QSet<RObject::Id> queryAllObjects() const;
    QSet<RObject::Id> queryAllObjectsFromContainer() const;
    QSet<REntity::Id> queryAllEntities(bool undone = false, bool allBlocks =
			false) const;
    QSet<RUcs::Id> queryAllUcs() const;
    QSet<RLayer::Id> queryAllLayers() const;
    QSet<RBlock::Id> queryAllBlocks() const;
    QSet<RView::Id> queryAllViews() const;
    QSet<RLinetype::Id> queryAllLinetypes() const;

    QSet<REntity::Id> queryLayerEntities(RLayer::Id layerId) const;
    QSet<REntity::Id> queryBlockEntities(RBlock::Id blockId) const;
    QSet<REntity::Id> queryBlockReferences(RBlock::Id blockId) const;

    /*
    QSet<REntity::Id> queryIntersectedEntities(
        const RBox& box,
        bool checkBoundingBoxOnly=false
    );
    */
    QSet<REntity::Id> queryContainedEntities(
        const RBox& box
    );
    QSet<REntity::Id> queryIntersectedEntitiesXY(
        const RBox& box,
        bool checkBoundingBoxOnly=false
    );
    QSet<REntity::Id> queryContainedEntitiesXY(
        const RBox& box
    );
    /*
    QSet<REntity::Id> queryEntitiesContainedXYIntersectedZ(
        const RBox& box
    );
    */
    QSet<REntity::Id> querySelectedEntities();

    QSharedPointer<RObject> queryObject(RObject::Id objectId) const;
    QSharedPointer<RObject> queryObjectFromContainer(RObject::Id objectId) const;
    QSharedPointer<REntity> queryEntity(REntity::Id entityId) const;
    QSharedPointer<REntity> queryEntityDirect(REntity::Id entityId) const;
    QSharedPointer<RUcs> queryUcs(RUcs::Id ucsId) const;
    QSharedPointer<RUcs> queryUcs(const QString& ucsName) const;
    QSharedPointer<RLayer> queryLayer(RLayer::Id layerId) const;
    QSharedPointer<RLayer> queryLayer(const QString& layerName) const;
    QSharedPointer<RBlock> queryBlock(RBlock::Id blockId) const;
    QSharedPointer<RBlock> queryBlock(const QString& blockName) const;
    QSharedPointer<RView> queryView(RView::Id viewId) const;
    QSharedPointer<RView> queryView(const QString& viewName) const;
    QSharedPointer<RLinetype> queryLinetype(RLinetype::Id linetypeId) const;
    QSharedPointer<RLinetype> queryLinetype(const QString& linetypeName) const;

    void clearSelection(
        QSet<REntity::Id>* affectedEntities=NULL
    );
    void selectEntity(
        REntity::Id entityId,
        bool add=false,
        QSet<REntity::Id>* affectedEntities=NULL
    );
    void deselectEntity(
        REntity::Id entityId,
        QSet<REntity::Id>* affectedEntities=NULL
    );
    void selectEntities(
        QSet<REntity::Id> entityId,
        bool add=false,
        QSet<REntity::Id>* affectedEntities=NULL
    );
    bool isSelected(REntity::Id entityId);

    bool hasSelection() const;

    RBox getBoundingBox();
    RBox getSelectionBox();

    //void updateSpatialIndex(RObject& object, bool isNew = false);
	void updateSpatialIndex(REntity& entity, bool isNew = false);
	virtual void rebuildSpatialIndex();
	virtual void removeFromSpatialIndex(QSharedPointer<RObject> object);

    RTransaction undo();
    RTransaction redo();
    bool isUndoAvailable() const;
    bool isRedoAvailable() const;
    void resetTransactionStack();

    void setFileName(const QString& fn);
    QString getFileName();

    QSharedPointer<RLayer> queryCurrentLayer();
    void setCurrentLayer(RLayer::Id layerId);
    void setCurrentLayer(const QString& layerName);
    RLayer::Id getCurrentLayerId();

    void setCurrentColor(const RColor& color);
    RColor getCurrentColor();

    void setCurrentLineweight(RLineweight::Lineweight lw);
    RLineweight::Lineweight getCurrentLineweight();

    void setCurrentLinetype(RLinetype lt);
    RLinetype getCurrentLinetype();

    QSharedPointer<RBlock> queryCurrentBlock();
    void setCurrentBlock(RBlock::Id blockId);
    void setCurrentBlock(const QString& blockName);
    RBlock::Id getCurrentBlockId();

    QSharedPointer<RView> queryCurrentView();
    void setCurrentView(RView::Id viewId);
    void setCurrentView(const QString& viewName);
    RView::Id getCurrentViewId();

    QString getBlockName(RBlock::Id blockId);
    QSet<QString> getBlockNames();
    QString getLayerName(RLayer::Id layerId);
    QSet<QString> getLayerNames();
    QSet<QString> getViewNames();

    bool hasLayer(const QString& layerName);
    bool hasBlock(const QString& blockName);
    bool hasView(const QString& viewName);

    RLayer::Id getLayerId(const QString& layerName);
    RBlock::Id getBlockId(const QString& blockName);

    RLinetype::Id getLinetypeId(const QString& linetypeName);
    QString getLinetypeName(RLinetype::Id linetypeId);
    QSet<QString> getLinetypeNames();
    bool isByLayer(RLinetype::Id linetypeId);
    bool isByBlock(RLinetype::Id linetypeId);

    RLineweight::Lineweight getMaxLineweight();

    void setVariable(const QString& key, const QVariant & value);
    QVariant getVariable(const QString& key);

    void setUnit(RS::Unit unit);
    RS::Unit getUnit() const;

    /*
    void copyToDocument(const RVector& reference, RDocument& other,
        bool selectionOnly, bool clear, RTransaction& transaction);
    void copyToDocument(REntity& entity, const RVector& reference,
        RDocument& other, RTransaction& transaction);
    */

    static RDocument& getClipboard();

    /**
     * \nonscriptable
     */
    friend QDebug operator<<(QDebug dbg, RDocument& d);

    void dump();

protected:
    static RDocument* clipboard;

private:
    QString fileName;
    RStorage& storage;
    RSpatialIndex& spatialIndex;
    RTransactionStack transactionStack;
    RS::Unit unit;
    RLinetype::Id linetypeByLayerId;
    RLinetype::Id linetypeByBlockId;
};

Q_DECLARE_METATYPE(RDocument*)

#endif
