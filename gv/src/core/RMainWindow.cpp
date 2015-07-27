#include <QSettings>
#include <QCoreApplication>
#include <QObject>

#include "RCoordinateListener.h"
#include "RDebug.h"
#include "RDocumentInterface.h"
#include "RFocusListener.h"
#include "RGuiAction.h"
#include "RLayerListener.h"
#include "RPenListener.h"
#include "RBlockListener.h"
#include "RViewListener.h"
#include "RMainWindow.h"
#include "RPropertyListener.h"
#include "RSelectionListener.h"
#include "RSnapListener.h"
#include "RUcsListener.h"
#include "RVector.h"



RMainWindow* RMainWindow::mainWindow = NULL;

RMainWindow::RMainWindow() {
	mainWindow = this;
}

RMainWindow::~RMainWindow() {
	mainWindow = NULL;
}

RMainWindow* RMainWindow::getMainWindow() {
	return mainWindow;
}

bool RMainWindow::hasMainWindow() {
	return mainWindow != NULL;
}

/**
 * \return Pointer to the document interface of the current document
 *      (e.g. the active MDI window) or NULL if no document is open.
 */
RDocumentInterface* RMainWindow::getDocumentInterfaceStatic() {
	if (mainWindow == NULL) {
		return NULL;
	}
	return mainWindow->getDocumentInterface();
}

void RMainWindow::notifyListeners(){
	RDocument* document = getDocument();
	notifyFocusListeners(getDocumentInterface());
	notifySnapListeners(getDocumentInterface());
    notifyTransactionListeners(document);
	notifyPropertyListeners(document);
	notifySelectionListeners(document);
	notifyLayerListeners(getDocumentInterface());
	notifyPenListeners(getDocumentInterface());
	notifyBlockListeners(getDocumentInterface());
	notifyViewListeners(getDocumentInterface());
}

/**
 * Adds a listener for coordinate events. This can for example be a widget that
 * displays the current coordinate. 
 */
void RMainWindow::addCoordinateListener(RCoordinateListener* l) {
	coordinateListeners.push_back(l);
}

/**
 * Notifies all coordinate listeners that the coordinate has changed to \c position.
 */
void RMainWindow::notifyCoordinateListeners(const RVector& wcsPosition,
		const RVector& ucsPosition) {
	QList<RCoordinateListener*>::iterator it;
	for (it = coordinateListeners.begin(); it != coordinateListeners.end(); ++it) {
		(*it)->setCoordinate(wcsPosition, ucsPosition);
	}
}

/**
 * Adds a listener for selection events. This can for example be a widget that
 * displays the number of selected entities.
 */
void RMainWindow::addSelectionListener(RSelectionListener* l) {
	selectionListeners.push_back(l);
}

/**
 * Notifies all selection listeners that the current selection has changed.
 */
void RMainWindow::notifySelectionListeners(RDocument* document) {
	QList<RSelectionListener*>::iterator it;
	for (it = selectionListeners.begin(); it != selectionListeners.end(); ++it) {
        (*it)->updateSelectionListener(document);
	}
}

/**
 * Adds a listener for transaction events. This can for example be an undo or
 * redo button.
 */
void RMainWindow::addTransactionListener(RTransactionListener* l) {
	if (l != NULL) {
		transactionListeners.push_back(l);
	} else {
		RDebug::error(
				"RMainWindow::addTransactionListener(): Listener is NULL.");
	}
}

/**
 * Notifies all transaction listeners.
 */
void RMainWindow::notifyTransactionListeners(RDocument* document) {
	QList<RTransactionListener*>::iterator it;
	for (it = transactionListeners.begin(); it != transactionListeners.end(); ++it) {
        (*it)->updateTransactionListener(document);
	}
}

/**
 * Adds a listener for snap mode changes. This can for example be 
 * a snap status widget.
 */
void RMainWindow::addSnapListener(RSnapListener* l) {
	if (l != NULL) {
		RDebug::debug("RMainWindow::addSnapListener");
		snapListeners.push_back(l);
	} else {
		RDebug::error(
				"RMainWindow::addSnapListener(): Listener is NULL.");
	}
}

/**
 * Notifies all snap mode listeners.
 */
void RMainWindow::notifySnapListeners(RDocumentInterface* documentInterface) {
	QList<RSnapListener*>::iterator it;
	for (it = snapListeners.begin(); it != snapListeners.end(); ++it) {
		(*it)->updateSnap(documentInterface);
	}
}

/**
 * Adds a listener for focus changes.
 */
void RMainWindow::addFocusListener(RFocusListener* l) {
	if (l != NULL) {
		focusListeners.push_back(l);
	} else {
		RDebug::error(
				"RMainWindow::addFocusListener(): Listener is NULL.");
	}
}

/**
 * Notifies all focus listeners.
 */
void RMainWindow::notifyFocusListeners(RDocumentInterface* documentInterface) {
	QList<RFocusListener*>::iterator it;
	for (it = focusListeners.begin(); it != focusListeners.end(); ++it) {
		(*it)->updateFocus(documentInterface);
	}
}

/**
 * Adds a listener for property events. This can for example be a widget that
 * displays the properties of selected entities or the current action.
 */
void RMainWindow::addPropertyListener(RPropertyListener* l) {
	propertyListeners.push_back(l);
}

/**
 * Notifies all property listeners that the current property has changed.
 */
void RMainWindow::notifyPropertyListeners(RDocument* document) {
	QList<RPropertyListener*>::iterator it;
	for (it = propertyListeners.begin(); it != propertyListeners.end(); ++it) {
        (*it)->updateFromContainer(document);
	}
}

/**
 * Notifies all property listeners that the current property is the given entity.
 */
void RMainWindow::notifyPropertyListeners(RDocument& document, REntity& entity) {
	QList<RPropertyListener*>::iterator it;
	for (it = propertyListeners.begin(); it != propertyListeners.end(); ++it) {
		(*it)->updateFromObject((RObject*) &entity, &document);
	}
}

/**
 * Notifies all property listeners that no properties are currently relevant.
 */
void RMainWindow::notifyPropertyListeners() {
	QList<RPropertyListener*>::iterator it;
	for (it = propertyListeners.begin(); it != propertyListeners.end(); ++it) {
		(*it)->clearEditor();
	}
}

void RMainWindow::propertyChangeEvent(RPropertyEvent& event) {
	RDocumentInterface* documentInterface = getDocumentInterface();
	if (documentInterface == NULL) {
		return;
	}

	documentInterface->propertyChangeEvent(event);
}

/**
 * Adds a listener for UCS change / add / remove events. 
 * This can for example be a widget that displays all UCS objects.
 */
void RMainWindow::addUcsListener(RUcsListener* l) {
	ucsListeners.push_back(l);
}

/**
 * Notifies all UCS listeners that at least one UCS object has changed.
 */
void RMainWindow::notifyUcsListeners(RDocumentInterface* documentInterface) {
	QList<RUcsListener*>::iterator it;
	for (it = ucsListeners.begin(); it != ucsListeners.end(); ++it) {
		(*it)->update(documentInterface);
	}
}

/**
 * Adds a listener for layer change / add / remove events.
 */
void RMainWindow::addLayerListener(RLayerListener* l) {
	layerListeners.push_back(l);
}

/**
 * Notifies all layer listeners that at least one layer object has changed.
 */
void RMainWindow::notifyLayerListeners(RDocumentInterface* documentInterface) {
	QList<RLayerListener*>::iterator it;
	for (it = layerListeners.begin(); it != layerListeners.end(); ++it) {
		(*it)->updateLayers(documentInterface);
	}
}

/**
 * Adds a listener for block change / add / remove events.
 */
void RMainWindow::addBlockListener(RBlockListener* l) {
	blockListeners.push_back(l);
}

/**
 * Notifies all block listeners that at least one block object has changed.
 */
void RMainWindow::notifyBlockListeners(RDocumentInterface* documentInterface) {
	QList<RBlockListener*>::iterator it;
	for (it = blockListeners.begin(); it != blockListeners.end(); ++it) {
		(*it)->updateBlocks(documentInterface);
	}
}

/**
 * Adds a listener for view change / add / remove events.
 */
void RMainWindow::addViewListener(RViewListener* l) {
	viewListeners.push_back(l);
}

/**
 * Notifies all view listeners that at least one view object has changed.
 */
void RMainWindow::notifyViewListeners(RDocumentInterface* documentInterface) {
	QList<RViewListener*>::iterator it;
	for (it = viewListeners.begin(); it != viewListeners.end(); ++it) {
		(*it)->updateViews(documentInterface);
	}
}

/**
 * Adds a listener for pen change events.
 */
void RMainWindow::addPenListener(RPenListener* l) {
	penListeners.push_back(l);
}

/**
 * Notifies all pen listeners that the current pen has changed.
 */
void RMainWindow::notifyPenListeners(RDocumentInterface* documentInterface) {
    QList<RPenListener*>::iterator it;
	for (it = penListeners.begin(); it != penListeners.end(); ++it) {
        (*it)->updatePen(documentInterface);
	}
}

/**
 * Called immediately after the user has activated a new UCS to be used as current UCS.
 */
void RMainWindow::ucsSetEvent(const QString& ucsName) {
	RDocumentInterface* documentInterface = getDocumentInterface();
	if (documentInterface == NULL) {
		return;
	}

	documentInterface->ucsSetEvent(ucsName);
}

void RMainWindow::commandEvent(const QString& command){
	QString commandLower = command.toLower();
	if(!RGuiAction::triggerByCommand(commandLower)){
		handleUserWarning(QObject::tr("Unknown command '%1'").arg(commandLower));
	}
}

/**
 * Restores the application window settings (size, position, ...).
 */
void RMainWindow::readSettings() {
	int x = QSettings().value("Appearance/Position.X", -1).toInt();
	int y = QSettings().value("Appearance/Position.Y", -1).toInt();
	int width = QSettings().value("Appearance/Width", 800).toInt();
	int height = QSettings().value("Appearance/Height", 600).toInt();
	if (x != -1 && y != -1) {
		move(x, y);
	}

	resize(width, height);
}

/**
 * Stores the application window settings (size, position, ...).
 */
void RMainWindow::writeSettings() {
	QSettings().setValue("Appearance/Position.X", getPositionX());
	QSettings().setValue("Appearance/Position.Y", getPositionY());
	QSettings().setValue("Appearance/Width", getWidth());
	QSettings().setValue("Appearance/Height", getHeight());
}

