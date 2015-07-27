#ifndef RTHREAD_H_
#define RTHREAD_H_

#include <QMetaType>
#include <QThread>

#include "RRunner.h"

/**
 * \scriptable
 * _generateScriptShell
 */
class RThread: public QThread {

Q_OBJECT

public:
	RThread(QObject * parent = 0);
	virtual ~RThread();

	void start();

	static void yieldCurrentThread();

signals:
	void dorun();

protected:
	virtual void run();
};

Q_DECLARE_METATYPE(RThread*)

#endif /* RTHREAD_H_ */
