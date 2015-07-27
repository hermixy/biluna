#include "RNavigationAction.h"

RNavigationAction::RNavigationAction(RGuiAction* guiAction) 
    : RActionAdapter(guiAction), 
    panning(false) {

    /*
    var mdiChild = RMainWindowQt.getMainWindow().getMdiChild();
    if (mdiChild!=null) {
        this.hruler = mdiChild.findChild("horizontalRuler");
        this.vruler = mdiChild.findChild("verticalRuler");
    }
    else {
        debug("MDI child is NULL");
    }
    */
}

void RNavigationAction::mousePressEvent(RMouseEvent& event) {
	if (event.button() == Qt::MidButton ||
        ( event.button() == Qt::LeftButton && 
          event.modifiers() == Qt::ControlModifier)) {
		panOrigin = event.getScreenPosition();
		panning = true;
		event.getGraphicsView().startPan();
	}
}

void RNavigationAction::mouseReleaseEvent(RMouseEvent& event) {
	if (event.button() == Qt::MidButton ||
        event.button() == Qt::LeftButton) {
		panning = false;
	}
}

void RNavigationAction::mouseMoveEvent(RMouseEvent& event) {
	if (panning &&
        ( event.buttons() == Qt::MidButton ||
          ( event.buttons() == Qt::LeftButton &&
            event.modifiers() == Qt::ControlModifier )
        )) {

        RVector panTarget = event.getScreenPosition();
        RVector panDelta = panTarget - panOrigin;
        if (fabs(panDelta.x) > 1 || fabs(panDelta.y) > 1) {
            event.getGraphicsView().pan(panDelta);
            panOrigin = panTarget;
        }
	}

    /*
    if (this.hruler!=undefined) {
        this.hruler.update();
    }
    if (this.vruler!=undefined) {
        this.vruler.update();
    }
    */
}

/*
Default.prototype.wheelEvent = function(event) {
	var view = event.getGraphicsView();

	switch (event.modifiers().valueOf()) {
	
	// scroll up / down:
	case Qt.ControlModifier.valueOf():
		view.pan(new RVector(0, event.delta() / 2));
		break;

	// scroll left / right:
	case Qt.ShiftModifier.valueOf():
		view.pan(new RVector(event.delta() / 2, 0));
		break;

	// zoom in / out:
	case Qt.NoModifier.valueOf():
		// avoid previews at wrong scales:
		// documentInterface.clearPreview();

		position = event.getModelPosition();
		if (event.delta() > 0) {
			view.zoomIn(position);
		} else if (event.delta() < 0) {
			view.zoomOut(position);
		}
		break;
	}

}
*/
