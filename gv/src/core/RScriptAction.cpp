#include "RDebug.h"
#include "RScriptAction.h"


    
RScriptAction::RScriptAction(const QString& scriptFile, RGuiAction* guiAction)
  : RAction(guiAction), 
    scriptFile(scriptFile) {
}
