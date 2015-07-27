#include "RDebug.h"
#include "RDocumentInterface.h"
#include "REntity.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"



RGraphicsScene::RGraphicsScene(RDocumentInterface& documentInterface)
    : RExporter(documentInterface.getDocument()),
      documentInterface(documentInterface),
      exportToPreview(false),
      previewIsEmpty(true),
      highlightedReferencePoint(RVector::invalid) {

    documentInterface.registerScene(*this);
}



RGraphicsScene::~RGraphicsScene() {
    while (!views.isEmpty()) {
        delete views.takeFirst();
    }
}



/**
 * \return Document interface this scene is attached to.
 */
RDocumentInterface& RGraphicsScene::getDocumentInterface() {
    return documentInterface;
}


/**
 * \return List of all views that are attached to this scene.
 */
QList<RGraphicsView*> RGraphicsScene::getGraphicsViews() {
    return views;
}



/**
 * Registers a view with this scene.
 */
void RGraphicsScene::registerView(RGraphicsView* view) {
    if(view==NULL){
        RDebug::error("RGraphicsScene::registerView: view is NULL");
        return;
    }
    views.push_back(view);
    if (documentInterface.getLastKnownViewWithFocus()==NULL) {
        documentInterface.setLastKnownViewWithFocus(view);
    }
}



/**
 * Removes the given view from this scene.
 */
void RGraphicsScene::unregisterView(RGraphicsView* view) {
    views.removeOne(view);
}



void RGraphicsScene::beginPreview() {
    exportToPreview = true;
    previewIsEmpty = false;
}



void RGraphicsScene::endPreview() {
    exportToPreview = false;
}



/**
 * Clears the preview.
 */
void RGraphicsScene::clearPreview() {
    previewIsEmpty = true;
    highlightedReferencePoint = RVector::invalid;
}



bool RGraphicsScene::isPreviewEmpty() {
    return previewIsEmpty;
}



/**
 * Regenerates this scene from the entity data by exporting the whole document into
 * it. This can for example regenerate all painter paths or all triangles
 * of the scene. This is called if the scene changes fundamentally or when the
 * entities that are affected by an update are not known.
 */
void RGraphicsScene::regenerate() {
    exportEntities();
    regenerateViews(true);
}



/**
 * Regenerates part of this scene from the entity data by exporting the given
 * entities into it. This is called if the scene changes only partly and
 * the entities that are affected by the update are known.
 */
void RGraphicsScene::regenerate(QSet<REntity::Id>& affectedEntities) {
    exportEntities(affectedEntities);
    regenerateViews(true);
}



/**
 * Regenerates all views attached to this scene.
 */
void RGraphicsScene::regenerateViews(bool force) {
    QList<RGraphicsView*>::iterator it;
    for (it=views.begin(); it!=views.end(); it++) {
        (*it)->regenerate(force);
    }
}



/**
 * Repaints all views attached to this scene.
 */
void RGraphicsScene::repaintViews() {
    QList<RGraphicsView*>::iterator it;
    for (it=views.begin(); it!=views.end(); it++) {
        (*it)->repaint();
    }
}


void RGraphicsScene::setCursor(const QCursor& cursor) {
	QList<RGraphicsView*>::iterator it;
	for (it = views.begin(); it != views.end(); it++) {
		(*it)->setCursor(cursor);
	}
}


void RGraphicsScene::handleKeyPressEvent(QKeyEvent& event){
	documentInterface.keyPressEvent(event);
}

void RGraphicsScene::handleKeyReleaseEvent(QKeyEvent& event){
	documentInterface.keyReleaseEvent(event);
}


/**
 * Forwards the given mouse event to the document interface.
 */
void RGraphicsScene::handleMouseMoveEvent(RMouseEvent& event) {
    documentInterface.mouseMoveEvent(event);
}



/**
 * Forwards the given mouse event to the document interface.
 */
void RGraphicsScene::handleMousePressEvent(RMouseEvent& event) {
    documentInterface.mousePressEvent(event);
}



/**
 * Forwards the given mouse event to the document interface.
 */
void RGraphicsScene::handleMouseReleaseEvent(RMouseEvent& event) {
    documentInterface.mouseReleaseEvent(event);
}



/**
 * Forwards the given mouse event to the document interface.
 */
void RGraphicsScene::handleWheelEvent(RWheelEvent& event) {
    documentInterface.wheelEvent(event);
}

/**
 * Forwards the given tablet event to the document interface.
 */
void RGraphicsScene::handleTabletEvent(RTabletEvent& event) {
    documentInterface.tabletEvent(event);
}



/**
 * Adds the reference points of the entity that is currently being exported
 * to the internal \c referencePoints map.
 */
void RGraphicsScene::exportReferencePoints() {
    // don't change reference points if we're only dealing with a preview:
    if (exportToPreview) {
        return;
    }

    const REntity* entity = getBlockRefOrEntity();
    if (entity==NULL) {
        return;
    }

    // remove all previous reference points of this entity:
    referencePoints.remove(entity->getId());

    if (entity->isUndone() || entity->isSelected()==false) {
        return;
    }

    // get list of reference points:
    QList<RVector> ref = entity->getReferencePoints(getProjectionRenderingHint());

    // export reference points:
    QList<RVector>::iterator it;
    for (it=ref.begin(); it!=ref.end(); ++it) {
        referencePoints.insert(entity->getId(), *it);
    }
}



void RGraphicsScene::exportCurrentEntity() {
    RExporter::exportCurrentEntity();
    exportReferencePoints();
}



void RGraphicsScene::unexportEntity(REntity::Id entityId) {
    RExporter::unexportEntity(entityId);
    referencePoints.remove(entityId);
}

void RGraphicsScene::highlightReferencePoint(const RVector& position) {
	highlightedReferencePoint = position;
}

RVector RGraphicsScene::getHighlightedReferencePoint(){
	return highlightedReferencePoint;
}

/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, RGraphicsScene& gs) {
    //dbg.nospace() << "RGraphicsScene(" << QString("%1").arg((int)&gs, 0, 16) << ")";
    return dbg.space();
}
