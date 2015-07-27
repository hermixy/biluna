#ifndef RSINGLETON_H_
#define RSINGLETON_H_

#include <QApplication>
#include <QObject>
#include <QMap>
#include <QString>
#include <typeinfo>
#include "RDebug.h"

/**
 * \ingroup core
 */
class RSingleton {
public:
	virtual ~RSingleton() {
	}
	template<class Derived>
        static Derived& getInstance();
        static void cleanUp();

protected:
	RSingleton();

private:
	static QMap<QString, RSingleton*> map;
};

Q_DECLARE_METATYPE(RSingleton*)

template<class Derived>
Derived& RSingleton::getInstance() {
	if (!map.contains(typeid(Derived).name())) {
		map[typeid(Derived).name()] = new Derived;
	}
	Derived* d = dynamic_cast<Derived*> (map[typeid(Derived).name()]);
	if (d == NULL) {
		Q_ASSERT("RSingleton::getInstance(): \"Derived\" in not a singleton");
	}
	return *d;
}

#endif /* RSINGLETON_H_ */
