#ifndef RSCRIPTHANDLER_H
#define RSCRIPTHANDLER_H

#include <QList>
#include <QMetaType>
#include <QString>

#include "RNonCopyable.h"
#include "RDocumentInterface.h"
#include "RAction.h"
#include "RGuiAction.h"
#include "RScriptAction.h"
#include "RMainWindow.h"



/**
 * Abstract base class for all script handlers.
 *
 * \ingroup core
 */
class RScriptHandler {

public:

    RScriptHandler(){ };

    virtual ~RScriptHandler(){ };

    /**
     * \return The file extensions of script files that are supported by
     *      this script handler.
     */
    virtual QList<QString> getSupportedFileExtensions() = 0;

    virtual void init();

    /**
     * Runs the given script file.
     */
    virtual void doScript(const QString& scriptFile) = 0;

    static void triggerActionApplicationLevel(const QString& scriptFile,
			RGuiAction* guiAction = NULL);

    /**
     * Creates a script based action for this script handler.
     */
    virtual void
			createActionDocumentLevel(const QString& scriptFile,
					RGuiAction* guiAction,
					RDocumentInterface* documentInterface = NULL) = 0;
	virtual void createActionApplicationLevel(const QString& scriptFile,
			RGuiAction* guiAction) = 0;

    /**
     * Creates a new document level action by instantiating class T and attaching the
     * action to the document interface of the currently active document.
     * Document level actions typically effect only the current document.
     * Document level actions stay active until they are explicitly finished.
     */
    template<class T>
	void createActionDocumentLevelT(const QString& scriptFile,
			RGuiAction* guiAction, RDocumentInterface* documentInterface) {
		if (documentInterface == NULL) {
		    documentInterface = RMainWindow::getDocumentInterfaceStatic();
		    if (documentInterface == NULL) {
		    	RDebug::error("RScriptHandler::createActionDocumentLevelT(): No document interface given or found.");
		    	return;
		    }
		}

		RScriptAction* a = new T(scriptFile, guiAction);
		a->setDocumentInterface(documentInterface);
		documentInterface->setCurrentAction(a);
	}

    /**
     * Creates a new application level action by instantiating class T. Application level
     * actions typically do not effect any documents. Application level actions cannot
     * be attached to a document interface or anywhere else, so the action is instantiated,
     * \ref beginEvent is called and then the action is terminated immediately
     * (\ref finishEvent is called).
     */
    template<class T>
    void createActionApplicationLevelT(const QString& scriptFile, RGuiAction* guiAction) {
        T action(scriptFile, guiAction);
        action.beginEvent();
        action.finishEvent();
    }

protected:
    static QString autostartScriptName;
};

#endif
