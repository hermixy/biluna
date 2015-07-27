#include <QDir>

#include "RScriptHandlerRegistry.h"
#include "RDebug.h"
#include "RScriptHandler.h"


QMap<QString, RScriptHandler*> RScriptHandlerRegistry::globalScriptHandlers;
QMap<QString, RScriptHandlerRegistry::FactoryFunction> RScriptHandlerRegistry::factoryFunctions;


RScriptHandlerRegistry::~RScriptHandlerRegistry(){
	RDebug::debug("RScriptHandlerRegistry::~RScriptHandlerRegistry()");
	QMapIterator<QString, RScriptHandler*> i(globalScriptHandlers);
	while (i.hasNext()) {
		i.next();
		delete i.value();
	}
}

/**
 * Registers a script handler.
 *
 * \param factoryFunction The function that can be used to instantiate
 *      the script handler.
 */
void RScriptHandlerRegistry::registerScriptHandler(FactoryFunction factoryFunction,
		const QList<QString>& fileExtensions) {

	QList<QString>::const_iterator it;
	for (it = fileExtensions.begin(); it != fileExtensions.end(); ++it) {
		if (factoryFunctions.count(*it) != 0) {
			RDebug::warning("RScriptHandlerRegistry::registerScriptHandler: "
				"duplicate registration of file extension: '%s'",
					(const char*) it->toUtf8());
			continue;
		}
		factoryFunctions[*it] = factoryFunction;
	}
}

/**
 * \return A new instance of the first script handler found that
 *      can handle the given file or NULL if no suitable handler
 *      can be found.
 */
RScriptHandler* RScriptHandlerRegistry::createScriptHandler(const QString& extension) {

	if (factoryFunctions.count(extension) != 0) {
		return factoryFunctions[extension]();
	}

	return NULL;
}

RScriptHandler* RScriptHandlerRegistry::getGlobalScriptHandler(
		const QString& extension) {
	if (globalScriptHandlers.count(extension) == 0) {
		RScriptHandler* handler = createScriptHandler(extension);
		if (handler == NULL) {
			RDebug::debug(
					QString(
							"RScriptHandlerRegistry::getGlobalScriptHandler: "
                            "Creation of Script Handler for %1 failed.").arg(
							extension));
			return NULL;
		}
		globalScriptHandlers[extension] = handler;
	}
	return globalScriptHandlers[extension];
}
