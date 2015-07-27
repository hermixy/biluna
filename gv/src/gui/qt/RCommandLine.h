#ifndef RCOMMANDLINE_H_
#define RCOMMANDLINE_H_

#include <QEvent>
#include <QLineEdit>
#include <QMetaType>
#include <QStringList>

/**
 * \scriptable
 */
class RCommandLine: public QLineEdit {

Q_OBJECT

signals:
	void clearHistory();
	void commandConfirmed(const QString& command);
	void completeCommand(const QString& command);

public:
	RCommandLine(QWidget* parent = 0);
	QString getLastCommand();

protected:
	virtual void keyPressEvent(QKeyEvent * event);
	virtual bool event(QEvent* event);

private:
	QStringList history;
	QStringList::iterator it;
};

Q_DECLARE_METATYPE(RCommandLine*)

#endif
