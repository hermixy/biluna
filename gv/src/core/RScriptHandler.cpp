#include <QDir>

#include "RScriptHandler.h"
#include "RDebug.h"
#include "RScriptHandlerRegistry.h"


QString RScriptHandler::autostartScriptName = "autostart";


/**
 * Looks up and initializes the autostart script(s) available for this
 * script handler.
 * This method calls \ref initScript for all files with a
 * supported file extension (see \ref getSupportedFileExtensions).
 */
void RScriptHandler::init() {
	QList<QString> extensions = getSupportedFileExtensions();
	QList<QString>::iterator it;
	for (it = extensions.begin(); it != extensions.end(); ++it) {
		QString scriptFile = "scripts" + QString(QDir::separator())
				+ autostartScriptName + "." + (*it);
		if (QFileInfo(scriptFile).exists()) {
			doScript(scriptFile);
		}
	}
}

/**
 * Runs the given script file as application level action.
 */
void RScriptHandler::triggerActionApplicationLevel(const QString& scriptFile,
		RGuiAction* guiAction) {
	RScriptHandler* h = RScriptHandlerRegistry::getGlobalScriptHandler(QFileInfo(scriptFile).suffix());
	if (h == NULL) {
		RDebug::warning("RScriptHandler::triggerActionApplicationLevel: "
			"no script handler found for scriptFile: %s",
				(const char*) scriptFile.toUtf8());
		return;
	}

	h->createActionApplicationLevel(scriptFile, guiAction);
}
