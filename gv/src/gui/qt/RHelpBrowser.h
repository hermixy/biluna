#ifndef RHELPBROWSER_H
#define RHELPBROWSER_H

#include <QtCore/QString>

#include "RSingleton.h"

class QProcess;

/**
 * \ingroup gui_qt
 *
 * \scriptable
 */
class RHelpBrowser: public RSingleton {
	friend class RSingleton;

public:
    RHelpBrowser();
    ~RHelpBrowser();
    void showDocumentation(const QString &file);
    static RHelpBrowser& getInstance();
    
private:
    bool startAssistant();
    QProcess *proc;
};

Q_DECLARE_METATYPE(RHelpBrowser)
Q_DECLARE_METATYPE(RHelpBrowser*)

#endif
