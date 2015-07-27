#ifndef RSCRIPTHANDLERREGISTRY_H
#define RSCRIPTHANDLERREGISTRY_H

#include <QList>
#include <QMetaType>
#include <QString>

#include "RNonCopyable.h"
#include "RDocumentInterface.h"
#include "RAction.h"
#include "RGuiAction.h"
#include "RScriptAction.h"
#include "RMainWindow.h"
#include "RSingleton.h"


/**
 * Script Handler Registry for global Script Handlers.
 *
 * \ingroup core
 */
class RScriptHandlerRegistry: public RSingleton {

	friend class RSingleton;

public:
	typedef RScriptHandler* (*FactoryFunction)();

    virtual ~RScriptHandlerRegistry();

    static void registerScriptHandler(FactoryFunction factoryFunction,
			const QList<QString>& fileExtensions);

    static RScriptHandler* createScriptHandler(const QString& extension);
	static RScriptHandler* getGlobalScriptHandler(const QString& extension);

protected:
    static QMap<QString, RScriptHandler*> globalScriptHandlers;
    static QMap<QString, FactoryFunction> factoryFunctions;
};

#endif
