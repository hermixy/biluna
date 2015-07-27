#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QProcess>

#include <QtWidgets/QMessageBox>

#include "RDebug.h"
#include "RHelpBrowser.h"

RHelpBrowser::RHelpBrowser() :
	proc(0) {
}

RHelpBrowser::~RHelpBrowser() {
	RDebug::debug("~RHelpBrowser()");
	if (proc && proc->state() == QProcess::Running) {
		RDebug::debug("~RHelpBrowser(): running");
		proc->terminate();
		proc->waitForFinished(3000);
	}
	delete proc;
}

RHelpBrowser& RHelpBrowser::getInstance() {
	return RSingleton::getInstance<RHelpBrowser>();
}

void RHelpBrowser::showDocumentation(const QString &page) {
	if (!startAssistant())
		return;

	QByteArray ba("SetSource ");
	ba.append("qthelp://com.trolltech.examples.simpletextviewer/doc/");

	proc->write(ba + page.toLocal8Bit() + '\0');
}

bool RHelpBrowser::startAssistant() {
	if (!proc)
		proc = new QProcess();

	if (proc->state() != QProcess::Running) {
		QString app = QString("support") + QDir::separator() + QString("bin")
				+ QDir::separator();
#if !defined(Q_OS_MAC)
		app += QLatin1String("assistant");
#else
		app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

		QStringList args;
		args << QLatin1String("-collectionFile") << QLatin1String(
				"support/doc/manual/simpletextviewer.qhc") << QLatin1String(
				"-enableRemoteControl");

		proc->start(app, args);
		//		proc->start("/usr/bin/xeyes");

		if (!proc->waitForStarted()) {
			QMessageBox::critical(0, QObject::tr("Simple Text Viewer"),
					QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
			return false;
		}
	}
	return true;
}
