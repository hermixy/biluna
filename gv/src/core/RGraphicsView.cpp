#include <QSettings>

#include "RGraphicsView.h"
#include "RAction.h"
#include "RDebug.h"
#include "RDocument.h"
#include "RGraphicsScene.h"
#include "RMouseEvent.h"
#include "RWheelEvent.h"
#include "RDocumentInterface.h"
#include "RSettings.h"


RGraphicsView::RGraphicsView(RGraphicsScene* scene)
    : grid(NULL),
      navigationAction(NULL),
      margin(-1) {

    offset = RVector(5, 5);
    factor = 10.0;

    setScene(scene);

    displayOnlyCurrentUcs = false;

    autoScalePatterns = RSettings::getAutoScalePatterns();
}



RGraphicsView::~RGraphicsView() {
    if (navigationAction!=NULL) {
        delete navigationAction;
    }
    if (grid!=NULL) {
        delete grid;
        grid = NULL;
    }
    if (scene!=NULL) {
        scene->unregisterView(this);
        scene = NULL;
    }
}

void RGraphicsView::viewportChangeEvent() {
	if (grid == NULL) {
		return;
	}
	grid->update(*this);
}

void RGraphicsView::saveViewport() {
	previousOffset = offset;
	previousFactor = factor;
}

void RGraphicsView::restoreViewport() {
	RVector tmpOffset = previousOffset;
	double tmpFactor = previousFactor;
	saveViewport();
	factor = tmpFactor;
	offset = tmpOffset;
	regenerate();
}

void RGraphicsView::setScene(RGraphicsScene* scene) {
    this->scene = scene;
    if (scene != NULL) {
            scene->registerView(this);
    }
}

/**
 * \return Reference to the graphics scene that is visualized by this view.
 */
RGraphicsScene* RGraphicsView::getScene() {
    return scene;
}


/**
 * If \c on is true, only the current UCS and the entities that are in its range are
 * displayed. The range of a UCS is defined by the length of its axis vectors. This
 * can be used to display only one floor in an architecture application.
 */
void RGraphicsView::setDisplayOnlyCurrentUcs(bool on) {
    displayOnlyCurrentUcs = on;
}



/**
 * \return Reference to the document that is visualized by this view.
 */
RDocument* RGraphicsView::getDocument() {
	if (scene == NULL) {
		return NULL;
	}
    return &scene->getDocument();
}



/**
 * \return Reference to the document interface.
 */
RDocumentInterface* RGraphicsView::getDocumentInterface() {
	if (scene == NULL) {
		return NULL;
	}
    return &scene->getDocumentInterface();
}

void RGraphicsView::autoZoom() {
	if (getDocument() == NULL) {
		return;
	}
    RBox bb = getDocument()->getBoundingBox();
    // TODO unit conversion:
    bb.growXY(getDocument()->getMaxLineweight()/100.0);
    zoomTo(bb, getMargin());
}

bool RGraphicsView::zoomToSelection() {
    RDocument* document = getDocument();
	if (document == NULL) {
		return false;
	}
    RBox selectionBox = document->getSelectionBox();
    if (selectionBox.isValid()) {
        zoomTo(selectionBox, getMargin());
        return true;
    }
    return false;
}

void RGraphicsView::zoomPrevious(){
	restoreViewport();
}

/**
 * Zooms in / out by the given factor towards / from the given center
 * in scene coordinates.
 */
void RGraphicsView::zoom(const RVector& center, double f) {
    saveViewport();

    offset = offset - (center*f - center);
    offset/=f;

    factor*=f;

    regenerate();

    viewportChangeEvent();
}



/**
 * Zooms in by factor 1.2. The center point is at the center of the view.
 */
void RGraphicsView::zoomIn() {
    zoomIn(mapFromView(RVector(getWidth()/2, getHeight()/2)));
}



/**
 * Zooms out by factor 1.2. The center point is at the center of the view.
 */
void RGraphicsView::zoomOut() {
    zoomOut(mapFromView(RVector(getWidth()/2, getHeight()/2)));
}



/**
 * Zooms in by factor 1.2. The given \c center point stays
 * at the same position.
 */
void RGraphicsView::zoomIn(const RVector& center) {
    zoom(center, 1.2);
}



/**
 * Zooms out by factor 1.0/1.2. The given \c center point stays
 * at the same position.
 */
void RGraphicsView::zoomOut(const RVector& center) {
    zoom(center, 1.0/1.2);
}



/**
 * Zooms the view in a way that all entities are within the visible area of the view.
 * The view is updated.
 */
void RGraphicsView::zoomTo(const RBox& window, int margin) {
	saveViewport();

	RBox viewWindow = mapToView(window);
	RVector size = viewWindow.getSize();
	RVector f;

	if (size.x > 1.0e-6) {
		f.x = (getWidth() - 2 * margin) / size.x;
	} else {
		f.x = 1.0;
	}

	if (size.y > 1.0e-6) {
		f.y = (getHeight() - 2 * margin) / size.y;
	} else {
		f.y = 1.0;
	}

	f.x = f.y = std::min<double>(f.x, f.y);

	if (f.x < 1.0e-6) {
		f.x = f.y = 1.0;
	}

	factor *= f.x;

	centerTo(window);
}

RVector RGraphicsView::getOffset() const {
	return offset;
}

void RGraphicsView::setOffset(const RVector& offset) {
	this->offset = offset;
	regenerate();
	viewportChangeEvent();
}

void RGraphicsView::setFactor(double f) {
    this->factor = f;
    regenerate();
    viewportChangeEvent();
}

void RGraphicsView::startPan() {
	saveViewport();
}

/**
 * Pans (scrolls) by the given amount \c delta which is a vector in screen
 * coordinates (pixels).
 */
void RGraphicsView::pan(const RVector& delta) {
    double dx = mapDistanceFromView(delta.x);
    double dy = mapDistanceFromView(delta.y);
    offset += RVector(dx, -dy);
    regenerate();
    viewportChangeEvent();
}



/**
 * Centers the view on the given \c box. The view is updated.
 *
 * \param box Box to move into the center of the drawing in model coordinates.
 */
void RGraphicsView::centerTo(const RBox& box) {
    RVector center = box.getCenter();
    RVector boxCenter = mapFromView(RVector(getWidth(), getHeight())/2.0);
    offset -= (center - boxCenter);
    regenerate();
    viewportChangeEvent();
}



/**
 * \return Four vectors with the coordiantes of the four corners of the
 *      view mapped to scene coordiantes.
 */
QList<RVector> RGraphicsView::mapCornersFromView() {
    QList<RVector> ret;

    ret.append(mapFromView(RVector(0,0,0)));
    ret.append(mapFromView(RVector(getWidth(),0,0)));
    ret.append(mapFromView(RVector(0,getHeight(),0)));
    ret.append(mapFromView(RVector(getWidth(),getHeight(),0)));

    return ret;
}



RBox RGraphicsView::getBox() {
    QList<RVector> corners = mapCornersFromView();

    return RBox(
        RVector::getMinimum(
            RVector::getMinimum(corners[0], corners[1]),
            RVector::getMinimum(corners[2], corners[3])
        ),
        RVector::getMaximum(
            RVector::getMaximum(corners[0], corners[1]),
            RVector::getMaximum(corners[2], corners[3])
        )
    );
}



/**
 * \return The minimum coordinate that is currently visible in the view.
 */
RVector RGraphicsView::getMinimum() {
    QList<RVector> corners = mapCornersFromView();

    return RVector::getMinimum(
        RVector::getMinimum(corners[0], corners[1]),
        RVector::getMinimum(corners[2], corners[3])
    );
}



/**
 * \return The maximum coordinate that is currently visible in the view.
 */
RVector RGraphicsView::getMaximum() {
    QList<RVector> corners = mapCornersFromView();

    return RVector::getMaximum(
        RVector::getMaximum(corners[0], corners[1]),
        RVector::getMaximum(corners[2], corners[3])
    );
}

void RGraphicsView::handleKeyPressEvent(QKeyEvent& event) {
	if (scene == NULL) {
		return;
	}
    scene->handleKeyPressEvent(event);
	if (navigationAction != NULL) {
		navigationAction->keyPressEvent(event);
	}
    event.ignore();
}

void RGraphicsView::handleKeyReleaseEvent(QKeyEvent& event) {
	if (scene == NULL) {
		return;
	}
    scene->handleKeyReleaseEvent(event);
	if (navigationAction != NULL) {
		navigationAction->keyReleaseEvent(event);
	}
    event.ignore();
}


/**
 * This should be called by the mouse move event handler of a higher level
 * GUI toolkit. Events are forwarded to the scene and the navigation action
 * that is attached to this view.
 */
void RGraphicsView::handleMouseMoveEvent(RMouseEvent& event) {
	if (scene != NULL) {
        scene->handleMouseMoveEvent(event);
	}
	if (navigationAction != NULL) {
		navigationAction->mouseMoveEvent(event);
	}
}



/**
 * This should be called by the mouse press event handler of a higher level
 * GUI toolkit. Events are forwarded to the scene and the navigation action
 * that is attached to this view.
 */
void RGraphicsView::handleMousePressEvent(RMouseEvent& event) {
	if (scene == NULL) {
		return;
	}
    scene->handleMousePressEvent(event);
    if (navigationAction!=NULL) {
        navigationAction->mousePressEvent(event);
    }
}



/**
 * This should be called by the mouse release event handler of a higher level
 * GUI toolkit. Events are forwarded to the scene and the navigation action
 * that is attached to this view.
 */
void RGraphicsView::handleMouseReleaseEvent(RMouseEvent& event) {
	if (scene == NULL) {
		return;
	}
    scene->handleMouseReleaseEvent(event);
    if (navigationAction!=NULL) {
        navigationAction->mouseReleaseEvent(event);
    }
}



/**
 * This should be called by the mouse wheel event handler of a higher level
 * GUI toolkit. Events are forwarded to the scene and the navigation action
 * that is attached to this view.
 */
void RGraphicsView::handleWheelEvent(RWheelEvent& event) {
	if (scene == NULL) {
		return;
	}
    scene->handleWheelEvent(event);
    if (navigationAction!=NULL) {
        navigationAction->wheelEvent(event);
    }
}

/**
 * This should be called by the tablet event handler of a higher level
 * GUI toolkit. Events are forwarded to the scene and the navigation action
 * that is attached to this view.
 */
void RGraphicsView::handleTabletEvent(RTabletEvent& event) {
	if (scene == NULL) {
		return;
	}
    scene->handleTabletEvent(event);
    if (navigationAction!=NULL) {
        navigationAction->tabletEvent(event);
    }
}

/**
 * Sets an action object that is typically used to handle all
 * navigation interaction (zooming, panning, ...). All events that are
 * received by this view are first sent to the graphics scene
 * and then to this action if they were not consumed yet.
 */
void RGraphicsView::setNavigationAction(RAction* action) {
    if (navigationAction!=NULL) {
        navigationAction->suspendEvent();
        navigationAction->finishEvent();
        delete navigationAction;
    }

    navigationAction = action;

    if (navigationAction!=NULL) {
        navigationAction->beginEvent();
    }
}



/**
 * Finds the reference point that is the closest to the given screen
 * coordinate (in pixels).
 *
 * \param range Maximum distance in pixels.
 *
 * \return The closest referecene point in model coordiantes.
 */
RVector RGraphicsView::getClosestReferencePoint(const RVector& screenPosition,
		int range) {

	RVector ret = RVector::invalid;
	if (scene == NULL) {
		return ret;
	}

	double minDist = (double) range;

	QMultiMap<REntity::Id, RVector>& referencePoints =
			scene->getReferencePoints();
	QMultiMap<REntity::Id, RVector>::iterator it;
	for (it = referencePoints.begin(); it != referencePoints.end(); it++) {
		RVector rp = mapToView(it.value());

		double dist = screenPosition.getDistanceTo(rp);
		if (dist < minDist) {
			minDist = dist;
			ret = it.value();
		}
	}

	return ret;
}



/**
 * \overload
 */
RVector RGraphicsView::getClosestReferencePoint(
    REntity::Id entityId, const RVector& screenPosition) {

    RVector ret = RVector::invalid;
    double minDist = RMAXDOUBLE;

	if (scene == NULL) {
		return ret;
	}

	if (getDocument() == NULL) {
		return ret;
	}
	QSharedPointer<REntity> entity = getDocument()->queryEntity(entityId);
    if (entity.isNull()) {
        return ret;
    }

    QList<RVector> referencePoints = entity->getReferencePoints(
			scene->getProjectionRenderingHint());
    QList<RVector>::iterator it;
    for (it=referencePoints.begin(); it!=referencePoints.end(); it++) {
        RVector rp = mapToView(*it);

        double dist = screenPosition.getDistanceTo(rp);
        if (dist<minDist) {
            minDist = dist;
            ret = (*it);
        }
    }

    return ret;
}



/**
 * Finds the entity that is the closest to the given screen
 * coordinate (in pixels).
 *
 * \param range Maximum distance in pixels.
 *
 * \return The closest entity or NULL.
 */
REntity::Id RGraphicsView::getClosestEntity(
    const RVector& screenPosition, int range) {

    RVector modelPosition = mapFromView(screenPosition);
    double modelRange = mapDistanceFromView(range);

//    RDebug::debug("RGraphicsView::getClosestEntity: "
//        "modelPosition: %f/%f, modelRange: %f",
//        modelPosition.x, modelPosition.y, modelRange);

    if (getDocumentInterface() == NULL) {
		return REntity::INVALID_ID;
	}
	return getDocumentInterface()->getClosestEntity(modelPosition, modelRange);
}



/**
 * Finds the entity that is the closest to the given screen
 * coordinate (in pixels) and inside the current UCS (read floor).
 *
 * \param range Maximum distance in pixels.
 *
 * \return The closest entity or NULL.
 */
/*
REntity::Id RGraphicsView::getClosestEntityInUcs(
    const RVector& screenPosition, int range) {

    RVector modelPosition = mapFromView(screenPosition);
    double modelRange = mapDistanceFromView(range);

    if(getDocumentInterface()==NULL){
    	return REntity::INVALID_ID;
    }
    return getDocumentInterface()->getClosestEntityInUcs(modelPosition, modelRange);
}
*/



/**
 * Maps the given \c box (e.g. a 3d bounding box) to a 2d box
 * in view coordinates (pixels).
 */
RBox RGraphicsView::mapToView(const RBox& box) {
    QList<RVector> boxCorners = box.getCorners();

    RVector minView(RMAXDOUBLE, RMAXDOUBLE, RMAXDOUBLE);
    RVector maxView(RMINDOUBLE, RMINDOUBLE, RMINDOUBLE);
    RVector corner;

    for (int i=0; i<8; i++) {
        corner = mapToView(boxCorners[i]);
        minView = RVector::getMinimum(corner, minView);
        maxView = RVector::getMaximum(corner, maxView);
    }

    return RBox(minView, maxView);
}


void RGraphicsView::setBackgroundColor(const QColor& col) {
    backgroundColor = col;
}

QColor RGraphicsView::getBackgroundColor() {
    if (!backgroundColor.isValid()) {
        backgroundColor = RSettings::getColor(RSettings::BackgroundColor).value();
    }
    return backgroundColor;
}

void RGraphicsView::setMargin(int m) {
    margin = m;
}

int RGraphicsView::getMargin() {
    if (margin==-1) {
        margin = RSettings::getValue(RSettings::GraphicsViewMargin).toInt();
    }
    return margin;
}
