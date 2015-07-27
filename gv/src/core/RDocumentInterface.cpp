#include <QtNetwork>

//#include "RAddObjectOperation.h"
#include "RCoordinateEvent.h"
#include "RDocumentInterface.h"
#include "REntityPickEvent.h"
#include "RFileExporter.h"
#include "RFileExporterRegistry.h"
#include "RFileImporter.h"
#include "RFileImporterRegistry.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RLine.h"
#include "RMainWindow.h"
#include "RMemoryStorage.h"
#include "ROperation.h"
#include "RScriptAction.h"
#include "RScriptHandler.h"
#include "RScriptHandlerRegistry.h"
#include "RSettings.h"
#include "RSnap.h"
#include "RSpatialIndexSimple.h"
#include "RStorage.h"
#include "RTransaction.h"
#include "RWheelEvent.h"
    
RDocumentInterface* RDocumentInterface::clipboard = NULL;


RDocumentInterface::RDocumentInterface(RDocument& document)
  : document(document),
    lastKnownViewWithFocus(NULL),
    defaultAction(NULL),
    currentSnap(NULL),
    relativeZero(RVector::invalid),
    cursorPosition(RVector::invalid),
    suspended(false) {

    //setCurrentBlock("*Model_Space");
}

RDocumentInterface::~RDocumentInterface() {
    if (defaultAction!=NULL) {
        defaultAction->finishEvent();
        delete defaultAction;
        defaultAction = NULL;
    }

    while (currentActions.size()>0) {
    	currentActions.top()->finishEvent();
        delete currentActions.top();
        currentActions.pop();
    }

    while (!scenes.isEmpty()) {
        delete scenes.takeFirst();
    }

    if (currentSnap!=NULL) {
        currentSnap->hideUiOptions();
        delete currentSnap;
    }

    QMapIterator<QString, RScriptHandler*> i(scriptHandlers);
	while (i.hasNext()) {
		i.next();
		delete i.value();
	}

    delete &document;
}


void RDocumentInterface::killAllActions() {
	for (int i = 0; i < currentActions.size(); ++i) {
		RAction* a = currentActions.at(i);
		a->terminate();
	}
}


/**
 * \return Reference to underlying document.
 */
RDocument& RDocumentInterface::getDocument() {
    return document;
}

/**
 * \return Reference to underlying storage.
 */
RStorage& RDocumentInterface::getStorage() {
    return document.getStorage();
}


RScriptHandler* RDocumentInterface::getScriptHandler(const QString& extension){
	if (!scriptHandlers.contains(extension)) {
		scriptHandlers[extension] = RScriptHandlerRegistry::createScriptHandler(extension);
	}
	return scriptHandlers[extension];
}

/**
 * \return List of attached graphics scenes.
 */
QList<RGraphicsScene*> RDocumentInterface::getGraphicsScenes() {
    return scenes;
}

/**
 * Adds a listener for coordinate events. This can for example be a widget that
 * displays the current coordinate. 
 */
void RDocumentInterface::addCoordinateListener(RCoordinateListener* l) {
	coordinateListeners.push_back(l);
}

/**
 * Notifies all coordinate listeners that the coordinate has changed to \c position.
 */
void RDocumentInterface::notifyCoordinateListeners(const RVector& wcsPosition,
		const RVector& ucsPosition) {
	QList<RCoordinateListener*>::iterator it;
	for (it = coordinateListeners.begin(); it != coordinateListeners.end(); ++it) {
		(*it)->setCoordinate(wcsPosition, ucsPosition);
	}

    RMainWindow::getMainWindow()->notifyCoordinateListeners(
        wcsPosition, ucsPosition);
}


/**
 * Resets the document to its original, empty state.
 */
void RDocumentInterface::clear() {
    document.clear();
    suspended = false;
    setCurrentBlock("*Model_Space");
}


/**
 * \return The graphics view that currently has the focus or the
 *      first attached view or NULL if no view is attached.
 */
RGraphicsView* RDocumentInterface::getGraphicsViewWithFocus() {
    RGraphicsView* ret = lastKnownViewWithFocus;

    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        QList<RGraphicsView*> views = (*it)->getGraphicsViews();
        QList<RGraphicsView*>::iterator it2;
        for (it2=views.begin(); it2!=views.end(); it2++) {
            if (ret==NULL) {
                ret = *it2;
                continue;
            }
            if ((*it2)->hasFocus()) {
                ret = *it2;
            }
        }
    }

    return ret;
}



/**
 * Sets the action that is active if no other action is active.
 */
void RDocumentInterface::setDefaultAction(RAction* action) {
	defaultAction = action;

	if (defaultAction != NULL) {
		defaultAction->setDocumentInterface(this);
		defaultAction->beginEvent();
	}
}



/**
 * Sets the current action. This action will receive all events
 * until it finishes.
 */
void RDocumentInterface::setCurrentAction(RAction* action) {
    if (action==NULL) {
        return;
    }

    if (!mutex.tryLock()) {
        return;
    }

    action->setDocumentInterface(this);

    if (!action->getUniqueGroup().isNull()) {
        // kill all instances of same action:
        if (currentActions.size() > 0) {
            if (currentActions.top()->getUniqueGroup()==action->getUniqueGroup()) {
                currentActions.top()->terminate();
            }
        }
    }

    deleteTerminatedActions();

    if (!action->isOverride()) {
        // suspend current action:
        if (currentActions.size() > 0) {
            currentActions.top()->suspendEvent();
        } else if (defaultAction != NULL) {
            defaultAction->suspendEvent();
        }
    }
    else {
        if (currentActions.size() > 0) {
            action->setOverrideBase(currentActions.top());
        } else if (defaultAction != NULL) {
            action->setOverrideBase(defaultAction);
        }
    }

    // add new action to action stack:
    currentActions.push(action);
    action->beginEvent();

    mutex.unlock();

    deleteTerminatedActions();
}



/**
 * \return Pointer to the current action or NULL. Used by
 *      script interfaces.
 */
RAction* RDocumentInterface::getCurrentAction() {
    if (currentActions.size()>0) {
        return currentActions.top();
    }

    return NULL;
}


void RDocumentInterface::suspend() {
    if (currentActions.size() > 0) {
		currentActions.top()->suspendEvent();
	} else if (defaultAction != NULL) {
		defaultAction->suspendEvent();
	}

    if (currentSnap!=NULL) {
        currentSnap->hideUiOptions();
    }

    suspended = true;
}



void RDocumentInterface::resume() {
    if (currentSnap!=NULL) {
        currentSnap->showUiOptions();
    }

    if (currentActions.size()>0) {
        currentActions.top()->resumeEvent();
    }
    else if (defaultAction!=NULL) {
        defaultAction->resumeEvent();
    }

    suspended = false;
}


/**
 * Deletes all actions that have been terminated.
 */
void RDocumentInterface::deleteTerminatedActions() {
    // prevents recursion (crash when e.g. keeping minus key pressed to zoom out):
    if (!mutex.tryLock()) {
        return;
    }

    bool removed = false;

    while (currentActions.size()>0 && currentActions.top()->isTerminated()) {
        cursorPosition = RVector::invalid;
        RAction* currentAction = currentActions.top();
        currentAction->finishEvent(); // crash
        setClickMode(RAction::PickingDisabled);
        // remember GUI action group:
        QString group;
        if (currentAction->getGuiAction()!=NULL &&
            !currentAction->getGuiAction()->getGroup().isEmpty() &&
            currentAction->isOverride()) {
            group = currentAction->getGuiAction()->getGroup();
        }

        currentActions.pop();
        delete currentAction;

        if (!group.isEmpty()) {
            RGuiAction::triggerGroupDefault(group);
        }
        removed = true;
    }

    // if one or more actions have been terminated, resume previous action
    // or default action:
    if (removed) {
        if (currentActions.size()>0) {
            currentActions.top()->resumeEvent();
        }
        else if (defaultAction!=NULL) {
            defaultAction->resumeEvent();
        }
    }

    mutex.unlock();
}



/**
 * Sets the click mode of the current action to the given mode.
 *
 * \see ClickMode
 */
void RDocumentInterface::setClickMode(RAction::ClickMode m) {
    if (currentActions.size()>0) {
        currentActions.top()->setClickMode(m);
    }
    else {
        if (defaultAction!=NULL) {
            defaultAction->setClickMode(m);
        }
    }

    if (currentSnap!=NULL) {
        if (m==RAction::PickCoordinate) {
            currentSnap->showUiOptions();
        }
        else {
            currentSnap->hideUiOptions();
        }
    }
}


void RDocumentInterface::setCursor(const QCursor& cursor) {
	QList<RGraphicsScene*>::iterator it;
	for (it = scenes.begin(); it != scenes.end(); it++) {
		(*it)->setCursor(cursor);
	}
}

/**
 * Registers a scene with this document interface.
 */
void RDocumentInterface::registerScene(RGraphicsScene& scene) {
    scenes.push_back(&scene);
}



/**
 * Unregisters a scene from this document interface.
 */
void RDocumentInterface::unregisterScene(RGraphicsScene& scene) {
    scenes.removeOne(&scene);
}



/**
 * Regenerates all scenes attached to this document interface by exporting
 * the document into them.
 */
void RDocumentInterface::regenerateScenes() {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->regenerate();
    }
}



/**
 * Regenerates the given part of all scenes attached to this document
 * interface by exporting the given list of entities into them.
 */
void RDocumentInterface::regenerateScenes(QSet<REntity::Id>& entityIds) {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->regenerate(entityIds);
    }
}



/**
 * \overload
 */
void RDocumentInterface::regenerateScenes(REntity::Id entityId) {
    QSet<RObject::Id> s;
    s.insert(entityId);
    regenerateScenes(s);
}



/**
 * Regenerates all views.
 */
void RDocumentInterface::regenerateViews(bool force) {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->regenerateViews(force);
    }
}



/**
 * Repaints all views.
 */
void RDocumentInterface::repaintViews() {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->repaintViews();
    }
}


void RDocumentInterface::keyPressEvent(QKeyEvent& event) {
	if (currentActions.size() > 0) {
		currentActions.top()->keyPressEvent(event);
    } else if (defaultAction != NULL) {
		defaultAction->keyPressEvent(event);
	} else {
		event.ignore();
	}

	deleteTerminatedActions();
}

void RDocumentInterface::keyReleaseEvent(QKeyEvent& event) {
	if (currentActions.size() > 0) {
		currentActions.top()->keyReleaseEvent(event);
	} else if (defaultAction != NULL) {
		defaultAction->keyReleaseEvent(event);
	} else {
		event.ignore();
	}

	deleteTerminatedActions();
}


/**
 * Forwards the given mouse move \c event to the current action.
 */
void RDocumentInterface::mouseMoveEvent(RMouseEvent& event) {
	clearPreview();

	if (currentActions.size() > 0) {
		currentActions.top()->mouseMoveEvent(event);
		previewClickEvent(*currentActions.top(), event);
	} else if (defaultAction != NULL) {
		defaultAction->mouseMoveEvent(event);
		previewClickEvent(*defaultAction, event);
	} else {
		event.ignore();
	}

	repaintViews();
}


/**
 * Forwards the given mouse press \c event to the current action.
 */
void RDocumentInterface::mousePressEvent(RMouseEvent& event) {
	if (currentActions.size() > 0) {
		currentActions.top()->mousePressEvent(event);
	} else if (defaultAction != NULL) {
		defaultAction->mousePressEvent(event);
	} else {
		event.ignore();
	}

	deleteTerminatedActions();
}

/**
 * Forwards the given mouse release \c event to the current action.
 */
void RDocumentInterface::mouseReleaseEvent(RMouseEvent& event) {
	if (currentActions.size() > 0) {
		currentActions.top()->mouseReleaseEvent(event);
		handleClickEvent(*currentActions.top(), event);
	} else if (defaultAction != NULL) {
		defaultAction->mouseReleaseEvent(event);
		handleClickEvent(*defaultAction, event);
	} else {
		event.ignore();
	}

	deleteTerminatedActions();
}

void RDocumentInterface::coordinateEvent(RCoordinateEvent& event) {
    if (!event.isValid()) {
        return;
    }
    if (currentActions.size() > 0) {
        currentActions.top()->coordinateEvent(event);
    } else if (defaultAction != NULL) {
        defaultAction->coordinateEvent(event);
    }

    deleteTerminatedActions();
}

void RDocumentInterface::coordinateEventPreview(RCoordinateEvent& event) {
    if (!event.isValid()) {
        return;
    }
    if (currentActions.size() > 0) {
        currentActions.top()->coordinateEventPreview(event);
    } else if (defaultAction != NULL) {
        defaultAction->coordinateEventPreview(event);
    }

    cursorPosition = event.getModelPosition();
    deleteTerminatedActions();
}



/**
 * Helper function for \ref mouseReleaseEvent. Triggers an appropriate
 * higher level event for mouse clicks for the given \c action.
 * The event type depends on the action's current \ref ClickMode.
 */
void RDocumentInterface::handleClickEvent(RAction& action, RMouseEvent& event) {
	if (event.button() == Qt::LeftButton) {
		switch (action.getClickMode()) {
		case RAction::PickCoordinate: {
                RCoordinateEvent ce(snap(event),
                    event.getGraphicsScene(), event.getGraphicsView());
                if (ce.isValid()) {
                    cursorPosition = ce.getModelPosition();
                    action.coordinateEvent(ce);
                }
            }
            break;

		case RAction::PickEntity: {
                cursorPosition = RVector::invalid;
                REntity::Id entityId = getClosestEntity(event);
                if (entityId!=REntity::INVALID_ID) {
                    REntityPickEvent pe(entityId, event.getModelPosition(),
                        event.getGraphicsScene(), event.getGraphicsView());
                    action.entityPickEvent(pe);
                }
            }
            break;

		case RAction::PickingDisabled: {
                cursorPosition = RVector::invalid;
            }
            break;
		}
	} else if (event.button() == Qt::RightButton) {
		action.escapeEvent();
	}

    // right click or any other events might lead to termination of the action:
	deleteTerminatedActions();
}



/**
 * Helper function for \ref mouseMoveEvent. Triggers an appropriate
 * preview event for the given action and the current click mode the
 * action is in.
 */
void RDocumentInterface::previewClickEvent(RAction& action, RMouseEvent& event) {
    switch (action.getClickMode()) {
		case RAction::PickCoordinate: {
            RCoordinateEvent ce(snap(event),
                event.getGraphicsScene(), event.getGraphicsView());
			if (ce.isValid()) {
                cursorPosition = ce.getModelPosition();
				action.coordinateEventPreview(ce);

				if (RMainWindow::hasMainWindow()) {
					notifyCoordinateListeners(
						cursorPosition, cursorPosition);
				}
			}
        }
        break;

		case RAction::PickEntity: {
			cursorPosition = RVector::invalid;
			REntity::Id entityId = getClosestEntity(event);
			if (entityId != -1) {
                REntityPickEvent pe(entityId, event.getModelPosition(),
                    event.getGraphicsScene(), event.getGraphicsView());
                action.entityPickEventPreview(pe);
			}

			if (RMainWindow::hasMainWindow()) {
				notifyCoordinateListeners(
					event.getModelPosition(), event.getModelPosition());
			}
		}
		break;

		case RAction::PickingDisabled: {
			cursorPosition = RVector::invalid;

			if (RMainWindow::hasMainWindow()) {
				notifyCoordinateListeners(
					event.getModelPosition(), event.getModelPosition());
			}
		}
		break;
	}

    deleteTerminatedActions();
}



/**
 * Forwards the given mouse wheel \c event to the current action.
 */
void RDocumentInterface::wheelEvent(RWheelEvent& event) {
	if (currentActions.size() > 0) {
		currentActions.top()->wheelEvent(event);
	} else if (defaultAction != NULL) {
		defaultAction->wheelEvent(event);
	} else {
		event.ignore();
	}

	deleteTerminatedActions();
}

/**
 * Forwards the given tablet \c event to the current action.
 */
void RDocumentInterface::tabletEvent(RTabletEvent& event) {
	if (currentActions.size() > 0) {
		currentActions.top()->tabletEvent(event);
	} else if (defaultAction != NULL) {
		defaultAction->tabletEvent(event);
	} else {
		event.ignore();
	}

	deleteTerminatedActions();
}

/**
 * Forwards the given \c event to the current action to signal that a
 * property value has been changed.
 */
void RDocumentInterface::propertyChangeEvent(RPropertyEvent& event) {
	if (currentActions.size() > 0) {
		currentActions.top()->propertyChangeEvent(event);
	} else if (defaultAction != NULL) {
		defaultAction->propertyChangeEvent(event);
	}

	deleteTerminatedActions();
}



/**
 * Called immediately after the user has activated a new UCS to be used as current UCS.
 */
void RDocumentInterface::ucsSetEvent(const QString& ucsName) {
    setCurrentUcs(ucsName);
}

/**
 * Sets the given pen style for all scenes.
 */
/*
void RDocumentInterface::setPen(const QPen& pen) {
	QList<RGraphicsScene*>::iterator it;
	for (it = scenes.begin(); it != scenes.end(); it++) {
		(*it)->setPen(pen);
	}
}
*/

bool RDocumentInterface::importUrl(const QUrl& url) {
    // URL points to local file:
    if (url.scheme()=="file") {
        QString filePath = url.toLocalFile();
        return importFile(filePath);
    }

    QNetworkAccessManager* manager = new QNetworkAccessManager();
	QNetworkReply* reply = manager->get(QNetworkRequest(url));
	do {
        // dangerous: processing events here allows user to 'interrupt'
        // by sending events (mouse moves, etc)
        //QApplication::processEvents();
	} while (reply->isRunning());
	QByteArray data = reply->readAll();
//	QString str(data);
//	qDebug() << "RDocumentInterface::importUrl: "<<str;

    QString suffix = QFileInfo(url.path()).suffix();

	QTemporaryFile file(QDir::tempPath() + QDir::separator() + "qcad_XXXXXX." + suffix);
	if (file.open()) {
		file.write(data);
		file.close();
		return importFile(file.fileName());
	}
	return false;
}

/**
 * Imports the given file if there is a file importer registered for that
 * file type.
 */
bool RDocumentInterface::importFile(const QString& fileName) {
	RFileImporter* fileImporter = RFileImporterRegistry::getFileImporter(
			fileName, document);
	if (fileImporter == NULL) {
            RDebug::error(QString("No file importer found for file %1").arg(fileName));
		return false;
	}
	bool ret = false;
	if (fileImporter->importFile(fileName)) {
		document.setFileName(fileName);
//		document.resetTransactionStack();
		ret = true;
    } else {
        RDebug::error(QString("Import for file %1 failed").arg(fileName));
    }

	delete fileImporter;
	if (RMainWindow::getMainWindow() != NULL) {
		RMainWindow::getMainWindow()->notifyListeners();
    }
	return ret;
}

bool RDocumentInterface::exportFile(const QString& fileName) {
	RFileExporter* fileExporter = RFileExporterRegistry::getFileExporter(
			fileName, document);
	if (fileExporter == NULL) {
		return false;
	}
	bool ret = false;
	if (fileExporter->exportFile(fileName)) {
		document.setFileName(fileName);
		ret = true;
	}
    delete fileExporter;
	return ret;
}


/**
 * Transaction based undo.
 */
void RDocumentInterface::undo() {
    RTransaction t = document.undo();

    //document.rebuildSpatialIndex();

    //QSet<REntity::Id> entityIds = t.getAffectedEntites();
    QSet<RObject::Id> objectIds = t.getAffectedObjects();
    //regenerateScenes(entityIds);
    objectChangeEvent(objectIds);

    if (RMainWindow::hasMainWindow()) {
    	RMainWindow::getMainWindow()->notifyTransactionListeners(&document);
    	RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
    }
}



/**
 * Transaction based redo.
 */
void RDocumentInterface::redo() {
    RTransaction t = document.redo();

    //document.rebuildSpatialIndex();

    //QSet<REntity::Id> entityIds = t.getAffectedEntites();
    QSet<RObject::Id> objectIds = t.getAffectedObjects();
    //regenerateScenes(entityIds);
    objectChangeEvent(objectIds);

    if (RMainWindow::hasMainWindow()) {
    	RMainWindow::getMainWindow()->notifyTransactionListeners(&document);
    	RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
    }
}



/**
 * Sets the current snap object.
 * The document interface takes ownership of the object.
 */
void RDocumentInterface::setSnap(RSnap* snap) {
    if (currentSnap!=NULL) {
        currentSnap->hideUiOptions();
        delete currentSnap;
    }

    currentSnap = snap;
    if (!suspended && currentSnap!=NULL) {
        currentSnap->showUiOptions();
    }
    //RMainWindow::getMainWindow()->notifySnapListeners(this);
}



/**
 * Gets the current snap object.
 */
RSnap* RDocumentInterface::getSnap() {
    return currentSnap;
}



/**
 * Uses the current snap to snap the given \c position
 * to a grid point, end point, etc.
 *
 * \return Coordinate to which was snapped.
 */
RVector RDocumentInterface::snap(RMouseEvent& event) {
    if (currentSnap!=NULL) {
        RVector ret = currentSnap->snap(event);
        
        QSet<REntity::Id> entityIds = currentSnap->getEntityIds();
        QSet<REntity::Id>::iterator it;
        for (it=entityIds.begin(); it!=entityIds.end(); ++it) {
            highlightEntity(*it);
        }

        return ret;
    }

    return event.getModelPosition();
}




/**
 * \return ID of the entity that is the closest to the mouse cursor
 *      of the given event.
 *
 * The event is also used to determine the maximum distance from the
 * cursor to the entity in the view in which the event originated.
 */
REntity::Id RDocumentInterface::getClosestEntity(RMouseEvent& event) {
    RGraphicsView& view = event.getGraphicsView();
    RVector modelPosition = event.getModelPosition();

    // range is 10 pixels in model units:
    double modelRange = view.mapDistanceFromView(10);

    return getClosestEntity(modelPosition, modelRange);
}



REntity::Id RDocumentInterface::getClosestEntity(const RVector& position,
		double range) {

    return document.queryClosestXY(position, range);
}



/*
REntity::Id RDocumentInterface::getClosestEntityInUcs(
    const RVector& position,
    double range) {

    double zMin = getCurrentUcs().origin.z;
    double zMax = getCurrentUcs().origin.z + getCurrentUcs().getZAxisDirection().getMagnitude();

    return document.queryClosest(
        position,
        range,
        zMin,
        zMax
    );
}
*/



/**
 * \override
 */
void RDocumentInterface::highlightEntity(REntity::Id entityId) {
	QSharedPointer<REntity> entity = document.queryEntity(entityId);
    if (entity.isNull()) {
        return;
    }

    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->highlightEntity(*entity);
    }
}



/**
 * Highlights the given reference point.
 */
void RDocumentInterface::highlightReferencePoint(const RVector& position) {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->highlightReferencePoint(position);
    }
}



/**
 * Selects the given entity and updates the scenes accordingly.
 *
 * \param add True to add to the current selection, false otherwise.
 *
 * \todo refactor into operations
 */
void RDocumentInterface::selectEntity(REntity::Id entityId, bool add) {
    QSet<RObject::Id> objectIds;
    document.selectEntity(entityId, add, &objectIds);
    regenerateScenes(objectIds);

    if (RMainWindow::hasMainWindow()) {
		RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
		RMainWindow::getMainWindow()->notifySelectionListeners(&document);
    }
}



/**
 * Deselects the given entity and updates the scenes accordingly.
 */
void RDocumentInterface::deselectEntity(REntity::Id entityId) {
    QSet<RObject::Id> objectIds;
    document.deselectEntity(entityId, &objectIds);
    regenerateScenes(objectIds);

    if (RMainWindow::hasMainWindow()) {
        RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
        RMainWindow::getMainWindow()->notifySelectionListeners(&document);
    }
}



/**
 * Selects the entity that is closest and within a fixed range of the
 * mouse cursor.
 *
 * \param event Typically a mouse move event.
 * \param add True to add to the correct selection, false otherwise.
 *
 * \todo refactor into operations
 */
/*
void RDocumentInterface::selectClosestEntity(
    RMouseEvent& event,
    bool add) {

    RGraphicsView& view = event.getGraphicsView();
    RVector position = event.getModelPosition();

    int rangePixels = RSettings::getSnapRange();
    double range = view.mapDistanceFromView(rangePixels);

    REntity::Id entityId =
        document.queryClosest(
            position,
            range
        );

    if (entityId!=-1) {
        selectEntity(entityId, add);
    }
    else {
        if (!add) {
            clearEntitySelection();
        }
    }
}
*/



/*
void RDocumentInterface::selectClosestEntity(
    RMouseEvent& event,
    const RLine& line,
    bool add) {

    RGraphicsView& view = event.getGraphicsView();

    int rangePixels = RSettings::getSnapRange();
    double range = view.mapDistanceFromView(rangePixels);

    REntity::Id entityId =
        document.queryClosestToLine(
            line,
            range
        );

    if (entityId!=-1) {
        selectEntity(entityId, add);
    }
    else {
        if (!add) {
            clearEntitySelection();
        }
    }
}
*/



/**
 * Selects all entities inside the given box and in the current UCS range
 * (e.g. on the current floor).
 *
 * \todo refactor into operations
 */
/*
void RDocumentInterface::selectBoxInUcs(const RBox& box, bool add) {
    double z1 = box.c1.z;
    double z2 = box.c2.z;
    RBox smallerBox(
        RVector(box.c1.x, box.c1.y, z1),
        RVector(box.c2.x, box.c2.y, z2)
    );

//    RDebug::debug("RDocumentInterface::selectBoxInUcs: box: "
//        "%f/%f/%f - %f/%f/%f",
//        box.c1.x, box.c1.y, z1,
//        box.c2.x, box.c2.y, z2);

    QSet<REntity::Id> entityIds;

    if (box.c2.x<box.c1.x) {
        entityIds = document.queryIntersectedEntities(smallerBox);
    }
    else {
        entityIds = document.queryEntitiesContainedXYIntersectedZ(smallerBox);
    }

    QSet<REntity::Id> affectedEntities;
    document.selectEntities(entityIds, add, &affectedEntities);
    regenerateScenes(affectedEntities);

    if (RMainWindow::hasMainWindow()) {
		RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
		RMainWindow::getMainWindow()->notifySelectionListeners(&document);
    }
}
*/


void RDocumentInterface::selectBoxXY(const RBox& box, bool add) {
    QSet<REntity::Id> entityIds;

    if (box.c2.x<box.c1.x) {
        entityIds = document.queryIntersectedEntitiesXY(box);
    }
    else {
        entityIds = document.queryContainedEntitiesXY(box);
    }

    QSet<REntity::Id> affectedEntities;
    document.selectEntities(entityIds, add, &affectedEntities);
    regenerateScenes(affectedEntities);

    if (RMainWindow::hasMainWindow()) {
        RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
        RMainWindow::getMainWindow()->notifySelectionListeners(&document);
    }
}


/**
 * Clears the current selection and updates the scenes / views accordingly.
 *
 * \todo refactor into operations
 */
void RDocumentInterface::clearSelection() {
    QSet<REntity::Id> entityIds;
    document.clearSelection(&entityIds);
    regenerateScenes(entityIds);

    if (RMainWindow::hasMainWindow()) {
		RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
		RMainWindow::getMainWindow()->notifySelectionListeners(&document);
	}
}

/**
 * \return True if the document has selected entities.
 */
bool RDocumentInterface::hasSelection() {
    return document.hasSelection();
}

/**
 * Adds a box to the preview that represents a zoom box displayed
 * while drawing a window to magnify an area.
 */
void RDocumentInterface::addZoomBoxToPreview(const RBox& box) {
    QList<RVector> boxCorners = box.getCorners();

    QList<RGraphicsScene*>::iterator it;
    for (it = scenes.begin(); it != scenes.end(); it++) {
        (*it)->beginPreview();
        (*it)->setColor(RSettings::getColor(RSettings::ZoomBoxColor));
        (*it)->setLineweight(RLineweight::Weight000);
        (*it)->setStyle(Qt::DashLine);

        /*
        // bottom:
        (*it)->exportQuad(
            boxCorners[0],
            boxCorners[1],
            boxCorners[2],
            boxCorners[3]
        );
        // top:
        (*it)->exportQuad(
            boxCorners[4],
            boxCorners[5],
            boxCorners[6],
            boxCorners[7]
        );
        */
        for (int i=0; i<4; ++i) {
            // 4 lines of box at z==point1.z:
            (*it)->exportLine(RLine(boxCorners[i], boxCorners[(i+1)%4]));
            // 4 lines of box at z==point2.z:
            //(*it)->exportLine(RLine(boxCorners[i+4], boxCorners[(i+1)%4+4]));
            // 4 vertical lines:
            //(*it)->exportLine(RLine(boxCorners[i], boxCorners[i+4]));
            // sides:
            /*
            (*it)->exportQuad(
                boxCorners[i],
                boxCorners[(i+1)%4],
                boxCorners[(i+1)%4+4],
                boxCorners[i+4]
            );
            */
        }
        (*it)->endPreview();
    }
}



/**
 * Adds a box to the preview that represents a selection box displayed
 * while drawing a window to select all entities inside.
 */
void RDocumentInterface::addSelectionBoxToPreview(const RBox& box) {
    //QList<RVector> boxCorners = box.getCorners();

    QList<RGraphicsScene*>::iterator it;
    for (it = scenes.begin(); it != scenes.end(); it++) {
        (*it)->beginPreview();
		(*it)->setColor(RSettings::getColor(RSettings::SelectionBoxColor));
		(*it)->setLineweight(RLineweight::Weight000);
		if (box.c2.x < box.c1.x) {
			(*it)->setStyle(Qt::CustomDashLine);
            QVector<qreal> dashes;
			dashes << 10 << 5;
			(*it)->setDashPattern(dashes);
		} else {
			(*it)->setStyle(Qt::SolidLine);
		}
        if (box.c2.x < box.c1.x) {
			// cross selection
			(*it)->setBrush(RSettings::getColor(RSettings::SelectionBoxBackgroundCrossColor));
		} else {
			(*it)->setBrush(RSettings::getColor(RSettings::SelectionBoxBackgroundColor));
		}
//        (*it)->exportQuad(
//            RVector(box.c1.x, box.c1.y, box.c1.z),
//            RVector(box.c2.x, box.c1.y, box.c1.z),
//            RVector(box.c2.x, box.c2.y, box.c1.z),
//            RVector(box.c1.x, box.c2.y, box.c1.z)
//        );
        (*it)->exportRectangle(box.c1, box.c2);
		(*it)->endPreview();
    }

//    for (it = scenes.begin(); it != scenes.end(); it++) {
//        (*it)->beginPreview();
//        (*it)->setColor(RSettings::getSelectionBoxColor());
//        (*it)->setWidth(0);
//        if (box.c2.x < box.c1.x) {
//			(*it)->setStyle(Qt::CustomDashLine);
//			QList<qreal> dashes;
//			dashes << 10 << 5;
//			(*it)->setDashPattern(dashes);
//		} else {
//			(*it)->setStyle(Qt::SolidLine);
//		}
//        for (unsigned int i = 0; i < 4; ++i) {
//			(*it)->exportLine(RLine(boxCorners[i], boxCorners[(i + 1) % 4]));
//		}
//        (*it)->endPreview();
//    }
}


/**
 * Exports the given shape (currently only lines supported)
 * to the preview.
 *
 * \todo Support other shapes.
 */
void RDocumentInterface::addAuxShapeToPreview(RShape& shape) {
    QList<RGraphicsScene*>::iterator it;
    for (it = scenes.begin(); it != scenes.end(); it++) {
        (*it)->beginPreview();
        (*it)->setColor(RSettings::getColor(RSettings::AuxShapeColor));
        (*it)->setLineweight(RLineweight::Weight000);
        (*it)->setStyle(Qt::CustomDashLine);
        QVector<qreal> dashes;
        dashes << 10 << 5 << 2 << 5;
        (*it)->setDashPattern(dashes);

        RLine* line = dynamic_cast<RLine*>(&shape);
        if (line!=NULL) {
            (*it)->exportLine(*line);
        }
        // TODO: support other shapes

        (*it)->endPreview();
    }
}

/**
 * Adds the given entity to the preview of all scenes / view.
 */
void RDocumentInterface::addEntityToPreview(REntity& entity) {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->beginPreview();
        (*it)->exportEntity(entity);
        (*it)->endPreview();
    }
}


/**
 * \return The relative zero position in model coordinates.
 */
RVector RDocumentInterface::getRelativeZero() {
    return relativeZero;
}


void RDocumentInterface::setRelativeZero(const RVector& p) {
    relativeZero = p;
}

/**
 * \return The last known mouse cursor position in model coordinates.
 */
RVector RDocumentInterface::getCursorPosition() {
    return cursorPosition;
}


void RDocumentInterface::setCursorPosition(const RVector& p) {
    cursorPosition = p;
}

/**
 * After calling this function, all exports go into the preview of the
 * scene instead of the scene itself.
 *
 * \see endPreview
 */
void RDocumentInterface::beginPreview() {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->beginPreview();
    }
}



/**
 * After calling this function, all exports go into the scene
 * again and not the preview anymore.
 *
 * \see beginPreview
 */
void RDocumentInterface::endPreview() {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->endPreview();
    }
}



/**
 * Clears the preview of all scenes.
 */
void RDocumentInterface::clearPreview() {
    cursorPosition = RVector::invalid;
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        (*it)->clearPreview();
    }
}



/**
 * \return True if the previews of all scenes are empty.
 * This is used to detect if the views needs to be redrawn
 * after a mouse move event.
 */
bool RDocumentInterface::isPreviewEmpty() {
    QList<RGraphicsScene*>::iterator it;
    for (it=scenes.begin(); it!=scenes.end(); it++) {
        if ((*it)->isPreviewEmpty()==false) {
            return false;
        }
    }

    return true;
}



/**
 * Notifies all property listeners that the properties of the given entity should
 * be shown.
 */
void RDocumentInterface::showPropertiesOf(REntity& entity) {
	if (RMainWindow::hasMainWindow()) {
		RMainWindow::getMainWindow()->notifyPropertyListeners(document, entity);
	}
}



/**
 * Notifies all property listeners that no properties are relevant at this
 * point. This can for example clear the property editor and other property
 * listeners.
 */
void RDocumentInterface::clearProperties() {
	if (RMainWindow::hasMainWindow()) {
		RMainWindow::getMainWindow()->notifyPropertyListeners();
	}
}



/**
 * Zooms in at the view that currently has the focus.
 */
void RDocumentInterface::zoomIn() {
    RGraphicsView* view = getGraphicsViewWithFocus();
    if (view!=NULL) {
        view->zoomIn();
    }
}



/**
 * Zooms out at the view that currently has the focus.
 */
void RDocumentInterface::zoomOut() {
    RGraphicsView* view = getGraphicsViewWithFocus();
    if (view!=NULL) {
        view->zoomOut();
    }
}



/**
 * Auto zooms in the view that currently has the focus.
 */
void RDocumentInterface::autoZoom() {
	RGraphicsView* view = getGraphicsViewWithFocus();
	if (view != NULL) {
		view->autoZoom();
	}
    else {
        RDebug::warning("RDocumentInterface::autoZoom: "
            "No view with focus found.");
    }
}

bool RDocumentInterface::zoomToSelection() {
	RGraphicsView* view = getGraphicsViewWithFocus();
	if (view != NULL) {
		return view->zoomToSelection();
	}
    return false;
}

/**
 * Zooms to the given \c region.
 */
void RDocumentInterface::zoomTo(const RBox& region, int margin) {
	RGraphicsView* view = getGraphicsViewWithFocus();
	if (view != NULL) {
		view->zoomTo(region, margin);
	}
}

void RDocumentInterface::zoomPrevious(){
	RGraphicsView* view = getGraphicsViewWithFocus();
	if (view != NULL) {
		view->zoomPrevious();
	}
}


/**
 * Previews the given operation by calling \ref ROperation::preview
 * for every scene that is attached to this document interface.
 */
void RDocumentInterface::previewOperation(const ROperation& operation) {
	QList<RGraphicsScene*>::iterator it;
	for (it = scenes.begin(); it != scenes.end(); it++) {
		(*it)->beginPreview();
		operation.preview(document, **it);
		(*it)->endPreview();
	}
}



/**
 * Applies the given operation to the document. The operation might
 * for example do something with the current selection.
 */
RTransaction RDocumentInterface::applyOperation(const ROperation& operation) {

    RTransaction t = operation.apply(document);
    if (t.isFailed()) {
        RDebug::error(QString(
                      "RDocumentInterface::applyOperation: transaction failed"));
        return t;
    }

    QSet<RObject::Id> objectIds = t.getAffectedObjects();
    //QSet<REntity::Id> entityIds = t.getAffectedEntites();

    clearPreview();
    //regenerateScenes(entityIds);
    objectChangeEvent(objectIds);

    if (RMainWindow::hasMainWindow() && !isClipboard()) {
        RMainWindow::getMainWindow()->notifyTransactionListeners(&document);
        // selection might have changed or disappeared (delete, cut):
        RMainWindow::getMainWindow()->notifyPropertyListeners(&document);
    }
    return t;
}



/**
 * Triggers an objectChangeEvent for every object in the given set.
 */
void RDocumentInterface::objectChangeEvent(QSet<RObject::Id>& objectIds) {
    bool ucsHasChanged = false;
    bool layerHasChanged = false;
    bool blockHasChanged = false;
    bool viewHasChanged = false;
    bool entityHasChanged = false;

    QSet<RObject::Id>::iterator it;
    for (it=objectIds.begin(); it!=objectIds.end(); ++it) {
    	QSharedPointer<RObject> object = document.queryObject(*it);
        if (object.isNull()) {
            continue;
        }

        QSharedPointer<RUcs> ucs = object.dynamicCast<RUcs> ();
        if(!ucs.isNull()){
        	ucsHasChanged = true;
        	continue;
        }

        QSharedPointer<RLayer> layer = object.dynamicCast<RLayer> ();
        if (!layer.isNull()) {
			layerHasChanged = true;
			QSet<RObject::Id> ids =
                document.queryLayerEntities(*it);
            // don't regenerate entities here that are to be regenerated anyway:
            ids -= objectIds;
            regenerateScenes(ids);
			continue;
		}

        QSharedPointer<RBlock> block = object.dynamicCast<RBlock> ();
		if (!block.isNull()) {
			blockHasChanged = true;
			QSet<RObject::Id> ids = document.queryBlockReferences(*it);
            // don't regenerate block references that are to be regenerated anyway:
            ids -= objectIds;
			regenerateScenes(ids);
			continue;
		}

        QSharedPointer<RView> view = object.dynamicCast<RView> ();
        if (!view.isNull()) {
			viewHasChanged = true;
			continue;
		}

        QSharedPointer<REntity> entity = object.dynamicCast<REntity> ();
		if (!entity.isNull()) {
			if (entity->isUndone()) {
				document.removeFromSpatialIndex(entity);
			} else {
				document.updateSpatialIndex(*entity);
			}
			entityHasChanged = true;
            regenerateScenes(entity->getId());

            // regenerate block references that point to the parent block of this entity:
            QSet<RObject::Id> ids = document.queryBlockReferences(entity->getBlockId());
            // don't regenerate block references that are to be regenerated anyway:
            ids -= objectIds;
            regenerateScenes(ids);
			continue;
		}
    }

    // notify listeners if this is not the clipboard document interface:
    if (RMainWindow::hasMainWindow() && this!=clipboard) {
		if (ucsHasChanged) {
			RMainWindow::getMainWindow()->notifyUcsListeners(this);
		}
		if (layerHasChanged) {
			RMainWindow::getMainWindow()->notifyLayerListeners(this);
		}
		if (blockHasChanged) {
			RMainWindow::getMainWindow()->notifyBlockListeners(this);
		}
		if (viewHasChanged) {
			RMainWindow::getMainWindow()->notifyViewListeners(this);
		}
	}
}


void RDocumentInterface::setCurrentColor(const RColor& color) {
    document.setCurrentColor(color);
    /*
    if (RMainWindow::hasMainWindow()) {
        RMainWindow::getMainWindow()->notifyPenListeners(this);
    }
    */
}

RColor RDocumentInterface::getCurrentColor() {
    return document.getCurrentColor();
}

void RDocumentInterface::setCurrentLineweight(RLineweight::Lineweight lw) {
    document.setCurrentLineweight(lw);
}

RLineweight::Lineweight RDocumentInterface::getCurrentLineweight() {
    return document.getCurrentLineweight();
}

void RDocumentInterface::setCurrentLinetype(RLinetype lt) {
    document.setCurrentLinetype(lt);
}

RLinetype RDocumentInterface::getCurrentLinetype() {
    return document.getCurrentLinetype();
}


/**
 * Sets the current layer based on the given layer name.
 */
void RDocumentInterface::setCurrentLayer(const QString& layerName) {
    document.setCurrentLayer(layerName);
    if (RMainWindow::hasMainWindow()) {
    	RMainWindow::getMainWindow()->notifyLayerListeners(this);
    }
}



/**
 * Sets the current Layer that is in use for all views attached to 
 * this document interface.
 */
void RDocumentInterface::setCurrentLayer(const RLayer& layer) {
    setCurrentLayer(layer.getName());
}

/**
 * Sets the current block based on the given block name.
 */
void RDocumentInterface::setCurrentBlock(const QString& blockName) {
    document.setCurrentBlock(blockName);
    if (RMainWindow::hasMainWindow()) {
    	RMainWindow::getMainWindow()->notifyBlockListeners(this);
    }
    regenerateScenes();
    document.rebuildSpatialIndex();
}

/**
 * Sets the current block that is in use for all views attached to
 * this document interface.
 */
void RDocumentInterface::setCurrentBlock(const RBlock& block) {
    setCurrentBlock(block.getName());
}

/**
 * Sets the current view based on the given view name.
 */
void RDocumentInterface::setCurrentView(const QString& viewName) {
    document.setCurrentView(viewName);
    if (RMainWindow::hasMainWindow()) {
    	RMainWindow::getMainWindow()->notifyViewListeners(this);
    }
}

/**
 * Sets the current block that is in use for all views attached to
 * this document interface.
 */
/*
void RDocumentInterface::setCurrentView(const RView& view) {
    setCurrentView(view.getName());
}
*/

/**
 * Sets the current UCS based on the given UCS name.
 * \todo refactor to store current UCS in document / storage like layer
 */
void RDocumentInterface::setCurrentUcs(const QString& ucsName) {
    currentUcsName = ucsName;

    QSharedPointer<RUcs> ucs = document.queryUcs(ucsName);
    if (ucs.isNull()) {
        RDebug::warning("RDocumentInterface::setCurrentUcs: "
            "UCS with name '%s' not found.", (const char*)ucsName.toUtf8());
        return;
    }

    setCurrentUcs(*ucs);
}



/**
 * Sets the current UCS (user coordinate system) that is in use
 * for all views attached to this document interface.
 */
void RDocumentInterface::setCurrentUcs(const RUcs& ucs) {
    currentUcs = ucs;
    regenerateViews(true);
    if (RMainWindow::hasMainWindow()) {
    	RMainWindow::getMainWindow()->notifyUcsListeners(this);
    }
}



/**
 * \return The current UCS (user coordinate system).
 */
RUcs RDocumentInterface::getCurrentUcs() {
    return currentUcs;
}



/**
 * \return The current UCS name.
 */
QString RDocumentInterface::getCurrentUcsName() {
    return currentUcsName;
}


RDocumentInterface& RDocumentInterface::getClipboard() {
    if (clipboard==NULL) {
        clipboard = new RDocumentInterface(RDocument::getClipboard());
    }

    return *clipboard;
}

/**
 * Copies selected entities, the layers they're on and blocks they reference
 * to the clipboard document.
 */
/*
void RDocumentInterface::copySelectionToClipboard(const RVector& reference) {
    document.copySelectionToClipboard(reference);
    getClipboard().regenerateScenes();
    getClipboard().autoZoom();
}
*/

/**
 * Pastes the clipboard to the given position in the document.
 */
/*
void RDocumentInterface::pasteFromClipboard(const RVector& reference) {
    document.pasteFromClipboard(reference);
    regenerateScenes();
    if (RMainWindow::hasMainWindow()) {
        RMainWindow::getMainWindow()->notifyListeners();
    }
}
*/
