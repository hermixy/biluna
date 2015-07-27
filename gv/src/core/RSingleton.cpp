#include "RSingleton.h"

QMap<QString, RSingleton*> RSingleton::map;

RSingleton::RSingleton() {
	static bool called = false;
        if (!called) {
//            QObject::connect(qApp, SIGNAL(aboutToQuit()), &cleaner,
//                             SLOT(cleanUp()));
            called = true;
        }
    }


void RSingleton::cleanUp() {
	QMapIterator<QString, RSingleton*> i(RSingleton::map);
	while (i.hasNext()) {
		i.next();
		delete i.value();
	}
	RSingleton::map.clear();
}
