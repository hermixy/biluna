#ifndef RSCRIPTACTION_H
#define RSCRIPTACTION_H

#include "RAction.h"
#include "RMainWindow.h"

/**
 * Script backed action.
 *
 * \ingroup core
 * \scriptable
 * \abstract
 */
class RScriptAction: public RAction {

public:
	RScriptAction(const QString& scriptFile, RGuiAction* guiAction);
	QString getScriptFile() {
		return scriptFile;
	}

protected:
	QString scriptFile;
};

Q_DECLARE_METATYPE(RScriptAction*)

#endif
