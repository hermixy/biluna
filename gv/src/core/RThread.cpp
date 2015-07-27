#include "RThread.h"

#include "RDebug.h"

RThread::RThread(QObject * parent) :
	QThread(parent) {
}

RThread::~RThread() {
}

void RThread::start() {
	QThread::start();
}

void RThread::run() {
	emit dorun();
	//	QThread::run();
	//	int i=0;
	//	while (i<100000) {
	//		RDebug::debug(QString("RThread::run: %1").arg(i));
	//		++i;
	//	}
	//	exec();
}

void RThread::yieldCurrentThread() {
	QThread::yieldCurrentThread();
}
