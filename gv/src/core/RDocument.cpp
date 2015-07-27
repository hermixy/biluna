#include "RBox.h"
#include "RDebug.h"
#include "RDocument.h"
#include "RMath.h"
#include "RMemoryStorage.h"
#include "RSpatialIndexSimple.h"
#include "RStorage.h"
#include "RUcs.h"


RDocument* RDocument::clipboard = NULL;


/**
 * Creates a new document with the given storage as back-end.
 * The document takes ownership of the storage and spatial index.
 * A new document contains one layer ("0") and a number of default
 * line types. The default unit is Millimeter.
 */
RDocument::RDocument(
    RStorage& storage,
    RSpatialIndex& spatialIndex)
    : storage(storage),
      spatialIndex(spatialIndex),
      transactionStack(*this) {

    init();
}

void RDocument::init() {
    RTransaction t(storage, "", false);

    // add default line types:
    QSharedPointer<RObject> ltByLayer(new RLinetype(this, "BYLAYER"));
    t.addObject(ltByLayer);
    QSharedPointer<RObject> ltByBlock(new RLinetype(this, "BYBLOCK"));
    t.addObject(ltByBlock);
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "CONTINUOUS")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "BORDER")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "BORDER2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "BORDERX2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "CENTER")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "CENTER2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "CENTERX2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DASHDOT")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DASHDOT2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DASHDOTX2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DASHED")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DASHED2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DASHEDX2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DIVIDE")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DIVIDE2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DIVIDEX2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DOT")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DOT2")));
    t.addObject(QSharedPointer<RObject>(new RLinetype(this, "DOTX2")));

    // add default layer:
    QSharedPointer<RLayer> layer(
        new RLayer(
            this, "0", false, false,
            RColor(Qt::black), getLinetypeId("CONTINUOUS"),
            RLineweight::Weight025
        )
    );
    t.addObject(layer);

    // add default block:
    QSharedPointer<RBlock> modelSpace(
        new RBlock(
            this, "*Model_Space", RVector()
        )
    );
    t.addObject(modelSpace);

    t.end();

    setCurrentBlock("*Model_Space");

    linetypeByLayerId = ltByLayer->getId();
    linetypeByBlockId = ltByBlock->getId();
    storage.setCurrentLayer(layer->getId());

	setUnit(RS::Millimeter);
}


/**
 * Resets this document to its initial, empty state.
 */
void RDocument::clear() {
    fileName = "";
    storage.clear();
    spatialIndex.clear();
    transactionStack.reset();
    init();

    qDebug() << "RDocument::clear:";
    qDebug() << *this;
}


RDocument::~RDocument() {
	storage.doDelete();
    spatialIndex.doDelete();
}

void RDocument::setUnit(RS::Unit unit) {
	this->unit = unit;
}

RS::Unit RDocument::getUnit() const {
	return unit;
}

void RDocument::setVariable(const QString& key, const QVariant & value) {
	storage.setVariable(key, value);
}

QVariant RDocument::getVariable(const QString& key) {
	return storage.getVariable(key);
}

QSharedPointer<RLayer> RDocument::queryCurrentLayer() {
	return storage.queryCurrentLayer();
}

RLayer::Id RDocument::getCurrentLayerId(){
    return storage.getCurrentLayerId();
}

void RDocument::setCurrentColor(const RColor& color) {
    storage.setCurrentColor(color);
}

RColor RDocument::getCurrentColor() {
    return storage.getCurrentColor();
}

void RDocument::setCurrentLineweight(RLineweight::Lineweight lw) {
    storage.setCurrentLineweight(lw);
}

RLineweight::Lineweight RDocument::getCurrentLineweight() {
    return storage.getCurrentLineweight();
}

void RDocument::setCurrentLinetype(RLinetype lt) {
    storage.setCurrentLinetype(lt);
}

RLinetype RDocument::getCurrentLinetype() {
    return storage.getCurrentLinetype();
}

void RDocument::setCurrentLayer(RLayer::Id layerId) {
	storage.setCurrentLayer(layerId);
}

void RDocument::setCurrentLayer(const QString& layerName) {
	storage.setCurrentLayer(layerName);
}

QSharedPointer<RBlock> RDocument::queryCurrentBlock() {
	return storage.queryCurrentBlock();
}

RBlock::Id RDocument::getCurrentBlockId(){
    return storage.getCurrentBlockId();
}

void RDocument::setCurrentBlock(RBlock::Id blockId) {
	storage.setCurrentBlock(blockId);
}

void RDocument::setCurrentBlock(const QString& blockName) {
	storage.setCurrentBlock(blockName);
}

QString RDocument::getBlockName(RBlock::Id blockId) {
	return storage.getBlockName(blockId);
}

QSet<QString> RDocument::getBlockNames(){
	return storage.getBlockNames();
}

void RDocument::setCurrentView(RView::Id viewId) {
	storage.setCurrentView(viewId);
}

void RDocument::setCurrentView(const QString& viewName) {
	storage.setCurrentView(viewName);
}

RView::Id RDocument::getCurrentViewId(){
    return storage.getCurrentViewId();
}

QSharedPointer<RView> RDocument::queryCurrentView() {
	return storage.queryCurrentView();
}

bool RDocument::hasView(const QString& viewName) {
	return getViewNames().contains(viewName);
}

QString RDocument::getLayerName(RLayer::Id layerId) {
	return storage.getLayerName(layerId);
}

QSet<QString> RDocument::getLayerNames(){
	return storage.getLayerNames();
}

QSet<QString> RDocument::getViewNames(){
	return storage.getViewNames();
}

bool RDocument::hasLayer(const QString& layerName) {
	return getLayerNames().contains(layerName);
}

RLayer::Id RDocument::getLayerId(const QString& layerName){
	return storage.getLayerId(layerName);
}

bool RDocument::hasBlock(const QString& blockName) {
    return getBlockNames().contains(blockName);
}

RBlock::Id RDocument::getBlockId(const QString& blockName){
	return storage.getBlockId(blockName);
}

QString RDocument::getLinetypeName(RLinetype::Id linetypeId) {
	return storage.getLinetypeName(linetypeId);
}

RLinetype::Id RDocument::getLinetypeId(const QString& linetypeName) {
	return storage.getLinetypeId(linetypeName);
}

QSet<QString> RDocument::getLinetypeNames() {
	return storage.getLinetypeNames();
}

bool RDocument::isByLayer(RLinetype::Id linetypeId) {
    return linetypeId == linetypeByLayerId;
}

bool RDocument::isByBlock(RLinetype::Id linetypeId) {
    return linetypeId == linetypeByBlockId;
}

RLineweight::Lineweight RDocument::getMaxLineweight() {
	return storage.getMaxLineweight();
}

void RDocument::setFileName(const QString& fn) {
	fileName = fn;
}

QString RDocument::getFileName() {
	return fileName;
}

void RDocument::resetTransactionStack() {
	transactionStack.reset();
}

bool RDocument::isUndoAvailable() const {
	return transactionStack.isUndoAvailable();
}

bool RDocument::isRedoAvailable() const {
	return transactionStack.isRedoAvailable();
}


/**
 * \return Reference to storage that backs the document.
 */
RStorage& RDocument::getStorage() {
    return storage;
}



/**
 * \return Reference to the spatial index.
 */
RSpatialIndex& RDocument::getSpatialIndex() {
    return spatialIndex;
}



/**
 * \return Reference to the transaction stack for undo/redo handling.
 */
RTransactionStack& RDocument::getTransactionStack() {
    return transactionStack;
}



/**
 * Queries all objects of this document.
 *
 * \return Set of object IDs.
 */
QSet<RObject::Id> RDocument::queryAllObjects() const {
    return storage.queryAllObjects();
}

QSet<RObject::Id> RDocument::queryAllObjectsFromContainer() const {
	return storage.queryAllObjects();
}

/**
 * Queries all entities of this document. This operation can be
 * slow, depending on the total number of entities and the
 * storage type that is used for this document.
 *
 * \return Set of entity IDs.
 */
QSet<REntity::Id> RDocument::queryAllEntities(bool undone, bool allBlocks) const {
    return storage.queryAllEntities(undone, allBlocks);
}



/**
 * Queries all UCSs of this document.
 *
 * \return Set of UCS IDs.
 */
QSet<RUcs::Id> RDocument::queryAllUcs() const {
    return storage.queryAllUcs();
}

/**
 * Queries all layers of this document.
 *
 * \return Set of layer IDs.
 */
QSet<RLayer::Id> RDocument::queryAllLayers() const {
	return storage.queryAllLayers();
}

/*
QList<RLayer::Id> RDocument::queryAllLayers() const {
	QSet<RLayer::Id> result;
    storage.queryAllLayers(result);
    return result.toList();
}
*/

QSet<RBlock::Id> RDocument::queryAllBlocks() const {
	return storage.queryAllBlocks();
}

/*
QList<RBlock::Id> RDocument::queryAllBlocks() const {
	QSet<RBlock::Id> result;
    storage.queryAllBlocks(result);
    return result.toList();
}
*/

QSet<RView::Id> RDocument::queryAllViews() const {
	return storage.queryAllViews();
}

QSet<RLinetype::Id> RDocument::queryAllLinetypes() const{
	return storage.queryAllLinetypes();
}

QSet<REntity::Id> RDocument::queryLayerEntities(RLayer::Id layerId) const {
	return storage.queryLayerEntities(layerId);
}

QSet<REntity::Id> RDocument::queryBlockEntities(RBlock::Id blockId) const {
	return storage.queryBlockEntities(blockId);
}

QSet<REntity::Id> RDocument::queryBlockReferences(RBlock::Id blockId) const {
	return storage.queryBlockReferences(blockId);
}

/*
QSet<REntity::Id> RDocument::queryViewEntities(RView::Id viewId) const {
	return storage.queryViewEntities(viewId);
}
*/

/**
 * Queries the one entity that is closest to the given position and
 * within the given range (2d).
 *
 * \param wcsPosition The position to which the entity has to be close (2d).
 * \param range The range in which to search.
 */
REntity::Id RDocument::queryClosestXY(
    const RVector& wcsPosition,
    double range) {

    RVector rangeV(
        range,
        range
    );

    // find entities that are within given maximum distance
    //   (approximation based on bounding boxes):
    QSet<REntity::Id> candidates =
    queryIntersectedEntitiesXY(
        RBox(
            wcsPosition - rangeV,
            wcsPosition + rangeV
        ),
        true
    );

    return queryClosestXY(candidates, wcsPosition, range);
}

/**
 * Queries the entity that is closest to the given position \c wcsPosition.
 * Only entities in the given set of \c candidates are considered.
 * \c candidates is usually the result of an approximate spatial
 * index query.
 *
 * \param candidates Sets of candidates to consider.
 *
 * \param wcsPosition
 *
 * \param range Maximum distance between position and entity.
 */
REntity::Id RDocument::queryClosestXY(
    QSet<REntity::Id>& candidates,
    const RVector& wcsPosition,
    double range) {

    double minDist = RMAXDOUBLE;
    REntity::Id ret = -1;

    QSet<REntity::Id>::iterator it;
    for (it=candidates.begin(); it!=candidates.end(); it++) {
        QSharedPointer<REntity> e = queryEntity(*it);
        if (e.isNull()) {
            continue;
        }

        double dist = e->getDistanceTo(wcsPosition);
        if (!std::isnan(dist) && dist < minDist && dist < range) {
            minDist = dist;
            ret = *it;
        }
    }

    return ret;
}

/**
 * Queries the one entity that is closest to the given position and
 * within the given range.
 *
 * \param wcsPosition The position to which the entity has to be close (3d).
 * \param range The range in which to search.
 */
/*
REntity::Id RDocument::queryClosest(
    const RVector& wcsPosition,
    double range) {

    RVector rangeV(
        range,
        range,
        range
    );

    // find entities that are within given maximum distance
    //   (approximation based on bounding boxes):
    QSet<REntity::Id> candidates =
    queryIntersectedEntities(
        RBox(
            wcsPosition - rangeV,
            wcsPosition + rangeV
        ),
        true
    );

    return queryClosest(candidates, wcsPosition, range);
}
*/



/**
 * Queries the one entity that is closest to the given position and
 * within the given range and within the given Z values \c zMin
 * and \c zMax.
 *
 * \param wcsPosition The point to which the entity has to be close (3d).
 * \param range The range in which to search.
 * \param zMin
 * \param zMax
 */
/*
REntity::Id RDocument::queryClosest(
    const RVector& wcsPosition,
    double range,
    double zMin,
    double zMax
    ) {

    RVector rangeV(
        range,
        range,
        range
    );

    RBox queryBox(
        wcsPosition - rangeV,
        wcsPosition + rangeV
    );

    // adjust query box to given Z range:
    if (queryBox.c1.z<zMin) {
        queryBox.c1.z = zMin;
    }

    if (queryBox.c2.z>zMax) {
        queryBox.c2.z = zMax;
    }

    // find entities that are within given maximum distance
    //   (approximation based on bounding boxes):
    QSet<REntity::Id> candidates = 
    queryIntersectedEntities(
        queryBox
    );

    return queryClosest(candidates, wcsPosition, range);
}
*/



/**
 * Queries the entity that is closest to the given position \c wcsPosition.
 * Only entities in the given set of \c candidates are considered.
 * \c candidates is usually the result of an approximate spatial
 * index query.
 *
 * \param candidates Sets of candidates to consider.
 *
 * \param wcsPosition
 *
 * \param range Maximum distance between position and entity.
 */
/*
REntity::Id RDocument::queryClosest(
    QSet<REntity::Id>& candidates,
    const RVector& wcsPosition,
    double range) {

    double minDist = RMAXDOUBLE;
    REntity::Id ret = -1;

    QSet<REntity::Id>::iterator it;
    for (it=candidates.begin(); it!=candidates.end(); it++) {
    	QSharedPointer<REntity> e = queryEntity(*it);
        if (e.isNull()) {
            continue;
        }

        double dist = e->getDistanceTo(wcsPosition);
        qDebug() << "RDocument::queryClosest: dist:"<<dist;

        if (dist < minDist && dist < range) {
            minDist = dist;
            ret = *it;
        }
    }

    return ret;
}
*/



/**
 * Queries the one entity that is closest to the given position line and
 * within the given range.
 *
 * \param wcsRay The ray to which the entity has to be close (3d).
 *      Usually an imaginary ray that extends from a position (e.g. mouse cursor)
 *      to a point in the user's viewing direction.
 * \param range The range in which to search.
 */
/*
REntity::Id RDocument::queryClosestToLine(
    const RLine& wcsRay,
    double range) {

    RBox bb = wcsRay.getBoundingBox();
    bb.grow(range);

    // find entities that are within given maximum distance
    //   (approximation based on bounding boxes):
    QSet<REntity::Id> candidates = queryIntersectedEntities(bb, true);

    return queryClosestToLine(candidates, wcsRay, range);
}
*/



/**
 * Queries the entity that is closest to the given ray \c wcsRay.
 * Only entities in the given set of \c candidates are considered.
 * \c candidates is usually the result of an approximate spatial
 * index query.
 *
 * \param candidates Sets of candidates to consider.
 *
 * \param wcsRay
 *
 * \param range Maximum distance between position and entity.
 */
/*
REntity::Id RDocument::queryClosestToLine(
    QSet<REntity::Id>& candidates,
    const RLine& wcsRay,
    double range) {

    double minDist = RMAXDOUBLE;
    REntity::Id ret = -1;

    QSet<REntity::Id>::iterator it;
    for (it=candidates.begin(); it!=candidates.end(); it++) {
    	QSharedPointer<REntity> e = queryEntity(*it);
        if (e.isNull()) {
            continue;
        }

        double dist = e->getDistanceTo(wcsRay, false);

        if (dist < minDist && dist < range) {
            minDist = dist;
            ret = *it;
        }
    }

    return ret;
}
*/



/**
 * Queries all entities which are completely or partly inside the
 * given box.
 *
 * \param box Query box.
 * \param checkBoundingBoxOnly True to do an approximate query based on the
 *      bounding box only.
 *
 * \param result Set of IDs of entities that are completely or partly inside the
 *      given area.
 */
/*
QSet<REntity::Id> RDocument::queryIntersectedEntities(
		const RBox& box, bool checkBoundingBoxOnly) {
	// box is completely inside the bounding box of this document:
    if (box.contains(getBoundingBox())) {
		return queryAllEntities();
	}

    // box is completely outside the bounding box of this document:
    if (box.isOutside(getBoundingBox())) {
        return QSet<REntity::Id>();
    }

    QSet<REntity::Id> candidates = spatialIndex.queryIntersected(box);

    if (checkBoundingBoxOnly) {
        return candidates;
    }

    // filter out entities that don't intersect with the given box:
    QSet<REntity::Id> outsiders;
    QSet<REntity::Id>::iterator it;
    for (it=candidates.begin(); it!=candidates.end(); ++it) {
    	QSharedPointer<REntity> entity = queryEntity(*it);
        if (entity.isNull()) {
            continue;
        }

        if (!entity->intersectsWith(box)) {
            outsiders.insert(*it);
        }
    }

    return candidates - outsiders;
}
*/



/**
 * Queries all entities which are completely inside the given box.
 *
 * \param box Query box.
 *
 * \param result Set of IDs of entities that are completely inside the
 *      given area.
 */
QSet<REntity::Id> RDocument::queryContainedEntities(const RBox& box) {
    return spatialIndex.queryContained(box);
}


QSet<REntity::Id> RDocument::queryIntersectedEntitiesXY(
        const RBox& box, bool checkBoundingBoxOnly) {
    RBox boxExpanded = box;
    boxExpanded.c1.z = RMINDOUBLE;
    boxExpanded.c2.z = RMAXDOUBLE;

    // box is completely inside the bounding box of this document:
    if (boxExpanded.contains(getBoundingBox())) {
        return queryAllEntities();
    }

    // box is completely outside the bounding box of this document:
    if (boxExpanded.isOutside(getBoundingBox())) {
        return QSet<REntity::Id>();
    }

    QSet<REntity::Id> candidates = spatialIndex.queryIntersected(boxExpanded);

    /*
    if (checkBoundingBoxOnly) {
        return candidates;
    }
    */

    RBox boxFlattened = box;
    boxFlattened.c1.z = 0.0;
    boxFlattened.c2.z = 0.0;
    RLine l1(boxFlattened.c1, RVector(boxFlattened.c2.x, boxFlattened.c1.y));
    RLine l2(RVector(boxFlattened.c2.x, boxFlattened.c1.y), boxFlattened.c2);
    RLine l3(boxFlattened.c2, RVector(boxFlattened.c1.x, boxFlattened.c2.y));
    RLine l4(RVector(boxFlattened.c1.x, boxFlattened.c2.y), boxFlattened.c1);

    // filter out entities that don't intersect with the given box
    // or are on not on the current block:
    QSet<REntity::Id> outsiders;
    QSet<REntity::Id>::iterator it;
    for (it=candidates.begin(); it!=candidates.end(); ++it) {
        QSharedPointer<REntity> entity = queryEntity(*it);
        if (entity.isNull()) {
            outsiders.insert(*it);
            continue;
        }
        if (entity->isUndone() || entity->getBlockId() != getCurrentBlockId()) {
            outsiders.insert(*it);
            continue;
        }

        if (boxExpanded.contains(entity->getBoundingBox())) {
            continue;
        }

        if (!checkBoundingBoxOnly &&
            !entity->intersectsWith(l1) &&
            !entity->intersectsWith(l2) &&
            !entity->intersectsWith(l3) &&
            !entity->intersectsWith(l4)) {
            outsiders.insert(*it);
        }
    }

    return candidates - outsiders;
}

QSet<REntity::Id> RDocument::queryContainedEntitiesXY(const RBox& box) {
    RBox boxExpanded = box;
    boxExpanded.c1.z = RMINDOUBLE;
    boxExpanded.c2.z = RMAXDOUBLE;
    return spatialIndex.queryContained(boxExpanded);
}



/* *
 * \todo refactor (3D)
 *
 * Queries all entities which are completely inside the given box in X/Y direction
 * and at least partially inside the given box in Z direction.
 * This is used for selections on the XY plane.
 *
 * \param box Query box.
 *
 * \param result Set of entity IDs.
 */
/*
QSet<REntity::Id> RDocument::queryEntitiesContainedXYIntersectedZ(const RBox& box) {
	QSet<REntity::Id> intersected =
        queryIntersectedEntities(box, true);

    QSet<REntity::Id> contained2d =
        spatialIndex.queryContained(
            box.c1.x, box.c1.y, RMINDOUBLE,
            box.c2.x, box.c2.y, RMAXDOUBLE
        );

    return intersected.intersect(contained2d);
}
*/



/**
 * Queries all selected entities.
 *
 * \return Set of IDs of all selected entities.
 */
QSet<REntity::Id> RDocument::querySelectedEntities() {
    return storage.querySelectedEntities();
}



/**
 * Queries the object with the given ID.
 *
 * \return Shared pointer to the object or a null pointer.
 */
QSharedPointer<RObject> RDocument::queryObject(RObject::Id objectId) const {
    return storage.queryObject(objectId);
}

QSharedPointer<RObject> RDocument::queryObjectFromContainer(
		RObject::Id objectId) const {
	return storage.queryObjectDirect(objectId);
}


/**
 * Queries the entity with the given ID.
 *
 * \return Pointer to the entity or NULL.
 */
QSharedPointer<REntity> RDocument::queryEntity(REntity::Id entityId) const {
    return storage.queryEntity(entityId);
}


/**
 * Queries the entity with the given ID.
 * If the storage has the entity instantiated, the instantiated entity is
 * returned (rather than a clone).
 *
 * \return Pointer to the entity or NULL.
 */
QSharedPointer<REntity> RDocument::queryEntityDirect(REntity::Id entityId) const {
    return storage.queryEntityDirect(entityId);
}



/**
 * Queries the UCS with the given ID.
 *
 * \return Pointer to the UCS or NULL.
 */
QSharedPointer<RUcs> RDocument::queryUcs(RUcs::Id ucsId) const {
    return storage.queryUcs(ucsId);
}



/**
 * Queries the UCS with the given name.
 *
 * \return Pointer to the UCS or NULL.
 */
QSharedPointer<RUcs> RDocument::queryUcs(const QString& ucsName) const {
    return storage.queryUcs(ucsName);
}


/**
 * Queries the layer with the given ID.
 *
 * \return Pointer to the layer or NULL.
 */
QSharedPointer<RLayer> RDocument::queryLayer(RLayer::Id layerId) const {
    return storage.queryLayer(layerId);
}

/**
 * Queries the layer with the given name.
 *
 * \return Pointer to the layer or NULL.
 */
QSharedPointer<RLayer> RDocument::queryLayer(const QString& layerName) const {
    return storage.queryLayer(layerName);
}

QSharedPointer<RBlock> RDocument::queryBlock(RBlock::Id blockId) const {
    return storage.queryBlock(blockId);
}

QSharedPointer<RBlock> RDocument::queryBlock(const QString& blockName) const {
    return storage.queryBlock(blockName);
}

QSharedPointer<RView> RDocument::queryView(RView::Id viewId) const {
    return storage.queryView(viewId);
}

QSharedPointer<RView> RDocument::queryView(const QString& viewName) const {
    return storage.queryView(viewName);
}

QSharedPointer<RLinetype> RDocument::queryLinetype(RLinetype::Id linetypeId) const {
	return storage.queryLinetype(linetypeId);
}

QSharedPointer<RLinetype> RDocument::queryLinetype(const QString& linetypeName) const {
	return storage.queryLinetype(linetypeName);
}


/**
 * \todo refactoring to operation
 *
 * Deselectes all selected entities.
 */
void RDocument::clearSelection(QSet<REntity::Id>* affectedEntities) {
    storage.clearEntitySelection(affectedEntities);
}



/**
 * \todo refactoring to operation
 *
 * \copydoc RStorage::selectEntity
 */
void RDocument::selectEntity(
    REntity::Id entityId,
    bool add,
    QSet<REntity::Id>* affectedEntities) {

    storage.selectEntity(entityId, add, affectedEntities);
}



/**
 * \copydoc RStorage::deselectEntity
 */
void RDocument::deselectEntity(
    REntity::Id entityId,
    QSet<REntity::Id>* affectedEntities) {

    storage.deselectEntity(entityId, affectedEntities);
}


bool RDocument::isSelected(REntity::Id entityId) {
	return storage.isSelected(entityId);
}

/**
 * \todo refactoring to operation
 *
 * \copydoc RStorage::selectEntities
 *
 */
void RDocument::selectEntities(
    QSet<REntity::Id> entityIds,
    bool add,
    QSet<REntity::Id>* affectedEntities) {

    storage.selectEntities(entityIds, add, affectedEntities);
}

bool RDocument::hasSelection() const {
    return storage.hasSelection();
}

/**
 * \copydoc RStorage::getBoundingBox
 */
RBox RDocument::getBoundingBox() {
    return storage.getBoundingBox();
}

RBox RDocument::getSelectionBox() {
    return storage.getSelectionBox();
}

void RDocument::removeFromSpatialIndex(QSharedPointer<RObject> object) {
	QSharedPointer<REntity> entity = object.dynamicCast<REntity> ();
	if (entity.isNull()) {
		return;
	}

    spatialIndex.removeFromIndex(entity->getId(), entity->getBoundingBoxes());
}

/**
 * \todo Is this ever called?
 */
void RDocument::rebuildSpatialIndex(){
    spatialIndex.clear();
	QSet<REntity::Id> result =
        storage.queryAllEntities();
	QSetIterator<REntity::Id> i(result);
	while (i.hasNext()) {
		QSharedPointer<REntity> entity = storage.queryEntityDirect(i.next());
		if (entity.isNull()) {
			continue;
		}
        if (entity->isUndone()) {
            continue;
        }
        if (entity->getBlockId()!=getCurrentBlockId()) {
            continue;
        }

		spatialIndex.addToIndex(
			entity->getId(),
            entity->getBoundingBoxes()
		);
	}
}

/**
 * Called whenever an object has changed.
 *
 * \param isNew True if the given object is a new object. This is used
 *      for optimizations.
 */
/*
void RDocument::updateSpatialIndex(RObject& object, bool isNew) {
    REntity* entity = dynamic_cast<REntity*>(&object);
    if (entity!=NULL) {
        updateSpatialIndex(*entity, isNew);
    }
}
*/

/**
 * Updates the spatial index by removing all entries with the ID
 * of the given entity and adding new entries at the position of the
 * bouding box(es) of the entity.
 *
 * \param isNew True if the given object is a new object. This is used
 *      for optimizations.
 * \todo remove isNew
 */
void RDocument::updateSpatialIndex(REntity& entity, bool /*isNew*/) {
    /*
    if (isNew == false) {
		spatialIndex.removeFromIndex(entity.getId(), entity.getBoundingBoxes());
	}
    */

    spatialIndex.addToIndex(entity.getId(), entity.getBoundingBoxes());
}

/**
 * Undoes the last transaction.
 *
 * \return The transaction that was undone. Higher level callers can use
 *   this set to update scenes, views, etc accordingly.
 */
RTransaction RDocument::undo() {
    return transactionStack.undo();
}



/**
 * Redoes a previously undone transaction.
 *
 * \return Set of affected entity IDs. Higher level callers can use
 *   this set to update scenes, views, etc accordingly.
 */
RTransaction RDocument::redo() {
    return transactionStack.redo();
}



/**
 * Copies selected entities, the layers they're on and blocks they reference
 * to the clipboard document.
 */
/*void RDocument::copySelectionToClipboard(const RVector& reference) {
    copyToDocument(reference, getClipboard(), true, true);
}
*/


/**
 * Copies selected entities, the layers they're on and blocks they reference
 * to the given document.
 *
 * \param reference Reference point for the insertion. The origin of the
 *      source document is placed at the reference point.
 * \param other Destination document.
 * \param selectionOnly Copy only selected entities.
 * \param clear Clear (delete) selection after copying (Cut).
 * \param transaction The copy operation is part of this transaction.
 */
/*
void RDocument::copyToDocument(const RVector& reference,
    RDocument& other, bool selectionOnly, bool clear, 
    RTransaction& transaction) {

    if (clear) {
        other.clear();
    }

    QSet<REntity::Id> entityIds;
    if (selectionOnly) {
         entityIds = querySelectedEntities();
    }
    else {
         entityIds = queryAllEntities();
    }
    QSet<REntity::Id>::iterator it;
    for (it=entityIds.begin(); it!=entityIds.end(); ++it) {
        QSharedPointer<REntity> entity = queryEntityDirect(*it);
        if (entity.isNull() || entity->isUndone()) {
            continue;
        }
        copyToDocument(*entity.data(), reference, other, transaction);
    }

    qDebug() << "RDocumentInterface::copyToDocument:";
    qDebug() << other;
}
*/



/**
 * Copies the given entity (and its layer(s) and block reference(s) to the
 * given document.
 */
/*
void RDocument::copyToDocument(REntity& entity, const RVector& reference,
    RDocument& other, RTransaction& transaction) {

    // add layer the entity is on, if the layer exists it is updated only:
    QSharedPointer<RLayer> oriLayer =
        queryLayer(entity.getLayerId());
    if (oriLayer.isNull()) {
        RDebug::error("RDocument::copyToDocument: "
            "layer of entity is NULL.");
        return;
    }
    QSharedPointer<RLayer> clpLayer = QSharedPointer<RLayer>(oriLayer->clone());
    other.getStorage().setObjectId(*clpLayer.data(), RObject::INVALID_ID);
    clpLayer->setDocument(&other);
    RTransaction t1(other.getStorage(), "", false);
    t1.addObject(clpLayer);
    t1.end();

    // add block the entity belongs to, if the block exists it is updated only:
    QSharedPointer<RBlock> oriBlock =
        queryBlock(entity.getBlockId());
    if (oriBlock.isNull()) {
        RDebug::error("RDocument::copyToDocument: "
            "block of entity is NULL.");
        return;
    }
    QSharedPointer<RBlock> clpBlock = QSharedPointer<RBlock>(oriBlock->clone());
    other.getStorage().setObjectId(*clpBlock.data(), RObject::INVALID_ID);
    clpBlock->setDocument(&other);
    RTransaction t2(other.getStorage(), "", false);
    t2.addObject(clpBlock);
    t2.end();

    // entity is a block reference: add block the entity refers to:
    RBlockReferenceEntity* blockRef =
        dynamic_cast<RBlockReferenceEntity*>(&entity);
    if (blockRef!=NULL) {
        QSharedPointer<RBlock> refBlock =
            queryBlock(blockRef->getReferencedBlockId());
        if (refBlock.isNull()) {
            RDebug::error("RDocument::copyToDocument: "
                "entity references a NULL block.");
            return;
        }
        QSet<REntity::Id> ids = queryBlockEntities(refBlock->getId());
        QSet<REntity::Id>::iterator it;
        for (it=ids.begin(); it!=ids.end(); ++it) {
            QSharedPointer<REntity> e = queryEntityDirect(*it);
            if (e.isNull()) {
                continue;
            }
            copyToDocument(*e.data(), RVector::nullVector, other, transaction);
        }
    }

    // add entity self:
    QSharedPointer<REntity> clpEntity = QSharedPointer<REntity>(entity.clone());
    other.getStorage().setObjectId(*clpEntity.data(), RObject::INVALID_ID);
    clpEntity->setSelected(false);
    clpEntity->setDocument(&other);
    clpEntity->setLayerId(clpLayer->getId());
    clpEntity->setBlockId(clpBlock->getId());
    clpEntity->move(-reference);
    QSharedPointer<RBlockReferenceEntity> clpBlockRef =
        clpEntity.dynamicCast<RBlockReferenceEntity>();
    if (!clpBlockRef.isNull() && blockRef!=NULL) {
        QString blockName = getBlockName(blockRef->getReferencedBlockId());
        RBlock::Id blockId = other.getBlockId(blockName);
        clpBlockRef->setReferencedBlockId(blockId);
    }
    RTransaction t3(other.getStorage(), "", false);
    t3.addObject(clpEntity, false);
    t3.end();
    if (clpEntity->getBlockId()==other.getBlockId("*Model_Space")) {
        other.updateSpatialIndex(*clpEntity.data());
    }
}
*/



/**
 * Pastes the clipboard to the given position in the document.
 */
/*
void RDocument::pasteFromClipboard(const RVector& reference) {
    getClipboard().copyToDocument(reference, *this, false, false);

    qDebug() << "pasteFromClipboard: " << *this;
}
*/



RDocument& RDocument::getClipboard() {
    if (clipboard==NULL) {
        clipboard = new RDocument(
            *(new RMemoryStorage()),
            *(new RSpatialIndexSimple())
        );
    }

    return *clipboard;
}

/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, RDocument& d) {
//    dbg.nospace() << "RDocument(" << QString("%1").arg((int)&d, 0, 16) << ", ";
    dbg.nospace() << d.getStorage();
    return dbg.space();
}

void RDocument::dump() {
    qDebug() << *this;
}
