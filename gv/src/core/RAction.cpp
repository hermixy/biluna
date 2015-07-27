#include "RAction.h"
#include "RDebug.h"
#include "RDocument.h"
#include "REntity.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RGuiAction.h"
#include "RMouseEvent.h"
#include "RTransaction.h"
#include "RWheelEvent.h"
#include "RMainWindow.h"
#include "RDocumentInterface.h"


RAction::RAction(RGuiAction* guiAction)
  : terminated(false), override(false),
    clickMode(RAction::PickCoordinate),
    guiAction(guiAction),
    overrideBase(NULL),
    documentInterface(NULL) {

    if (guiAction!=NULL) {
        if (guiAction->isOverride()) {
            setOverride();
        }
        setUniqueGroup(guiAction->getUniqueGroup());
    }
}



RAction::~RAction() {
}



/**
 * Terminates the action. The action object will be deleted after calling this
 * method.
 */
void RAction::terminate() {
    terminated = true;
}



/**
 * \return True if the action is terminated and can be deleted, false otherwise.
 */
bool RAction::isTerminated() {
    return terminated;
}



void RAction::setOverride() {
    override = true;
}


bool RAction::isOverride() {
    return override;
}

void RAction::setUniqueGroup(const QString& ug) {
    uniqueGroup = ug;
}

QString RAction::getUniqueGroup() {
    return uniqueGroup;
}

void RAction::setOverrideBase(RAction* base) {
    overrideBase = base;
}

RAction* RAction::getOverrideBase() {
    return overrideBase;
}

/**
 * \return List of pointers to the graphics scenes which are attached to the
 *   document interface in which this action is active.
 */
QList<RGraphicsScene*> RAction::getGraphicsScenes() {
    RDocumentInterface* documentInterface = getDocumentInterface();
    if (documentInterface!=NULL) {
        return documentInterface->getGraphicsScenes();
    }
    else {
        return QList<RGraphicsScene*>();
    }
}



/**
 * \return Pointer to the graphics document or NULL if no document is open.
 */
RDocument* RAction::getDocument() {
    RDocumentInterface* documentInterface = getDocumentInterface();
    if (documentInterface!=NULL) {
        return &documentInterface->getDocument();
    }
    else {
    	RDebug::warning(QString("RAction::getDocument: Action has no document."));
        return NULL;
    }
}



/**
 * \return Reference to the document interface that owns this action or NULL
 *      for actions that run on an application level (e.g. file - new).
 */
RDocumentInterface* RAction::getDocumentInterface() {
//    RMainWindow* mainWindow = RMainWindow::getMainWindow();
//    if (mainWindow==NULL) {
//        return NULL;
//    }
//
//    return mainWindow->getDocumentInterface();
    return documentInterface;
}


/**
 * \return Pointer to the storage or NULL if no document is open.
 */
RStorage* RAction::getStorage() {
	RDocumentInterface* documentInterface = getDocumentInterface();
	if (documentInterface != NULL) {
		return &documentInterface->getStorage();
	} else {
		RDebug::warning(QString("RAction::getStorage: Action has no document."));
		return NULL;
	}
}


/**
 * Uses the currently active snap function to snap to the closest
 * object or grid point.
 */
RVector RAction::snap(RMouseEvent& event) {
    RDocumentInterface* documentInterface = getDocumentInterface();
    if (documentInterface!=NULL) {
        return documentInterface->snap(event);
    }
    else {
        return RVector();
    }
}
