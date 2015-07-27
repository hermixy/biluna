#include <QtWidgets>

#include "RMdiChildQt.h"
#include "RDebug.h"
#include "RDocumentInterface.h"
#include "RMainWindowQt.h"


RDocumentInterface* RMdiChildQt::diLast = NULL;

/**
 * Creates a new MDI child window for interaction with the given document.
 * The MDI window takes ownership of the document.
 */

RMdiChildQt::RMdiChildQt(QWidget* parent) :
        QWidget(parent), documentInterface(NULL) {

        setAttribute(Qt::WA_DeleteOnClose);
        setFocusPolicy(Qt::WheelFocus);
}

RMdiChildQt::~RMdiChildQt() {
	if (documentInterface != NULL) {
        if (diLast==documentInterface) {
            diLast = NULL;
        }
		delete documentInterface;
	}
}

void RMdiChildQt::setDocumentInterface(RDocumentInterface* di){
	documentInterface = di;
}

RDocumentInterface* RMdiChildQt::getDocumentInterface() {
	if (documentInterface == NULL) {
		RDebug::warning(
				QString("RMdiChildQt::getDocumentInterface(): documentInterface is NULL"));
		return NULL;
	}
	return documentInterface;
}

RDocument* RMdiChildQt::getDocument() {
	if (documentInterface == NULL) {
		RDebug::warning(QString("RMdiChildQt::getDocument(): documentInterface is NULL"));
		return NULL;
	}
	return &documentInterface->getDocument();
}

QSize RMdiChildQt::sizeHint() const {
	return QSize(640, 480);
}

/*
void RMdiChildQt::focusIn() {
    focusInEvent(NULL);
}

void RMdiChildQt::focusOut() {
    focusOutEvent(NULL);
}
*/

/*
// not good: focus is lost when clicking into options toolbar:
void RMdiChildQt::focusInEvent(QFocusEvent* e) {
	if (documentInterface == NULL) {
		RDebug::warning(QString("RMdiChildQt::focusInEvent(): documentInterface is NULL"));
		return;
	}
    / *
    if (diLast!=NULL) {
        diLast->suspend();
    }
    * /
	documentInterface->resume();
}

void RMdiChildQt::focusOutEvent(QFocusEvent* e) {
	if (documentInterface == NULL) {
		RDebug::warning(QString("RMdiChildQt::focusOutEvent(): documentInterface is NULL"));
		return;
	}
	documentInterface->suspend();
    //diLast = documentInterface;
}
*/
