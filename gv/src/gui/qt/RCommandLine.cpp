#include <QEvent>
#include <QKeyEvent>

#include "RCommandLine.h"
#include "RDebug.h"

RCommandLine::RCommandLine(QWidget* parent) :
	QLineEdit(parent), it(history.end()) {
}

QString RCommandLine::getLastCommand() {
	if (history.isEmpty()) {
		return QString();
	}
	return history.last();
}

bool RCommandLine::event(QEvent* event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* ke = dynamic_cast<QKeyEvent*> (event);
		if (ke->key() == Qt::Key_Tab) {
			emit completeCommand(text());
			return true;
		}
	}
	return QLineEdit::event(event);
}

void RCommandLine::keyPressEvent(QKeyEvent * event) {
	switch (event->key()) {
	case Qt::Key_L:
		if (event->modifiers() == Qt::ControlModifier) {
			emit clearHistory();
			return;
		}
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return: {
		QString t = text();
		emit commandConfirmed(t);
		if (!t.isEmpty() && (history.isEmpty() || history.last() != t)) {
			history.append(t);
			it = history.end();
		}
		}
		break;
	case Qt::Key_Up:
		if (it != history.begin()) {
			it--;
			setText(*it);
		}
		return;
		break;
	case Qt::Key_Down:
		if (it != history.end()) {
			it++;
			if (it != history.end()) {
				setText(*it);
			} else {
				clear();
			}
		}
		return;
		break;
	case Qt::Key_Escape:
		if (text().isEmpty()) {
			event->ignore();
		} else {
			clear();
		}
		break;
	default:
		break;
	}
	QLineEdit::keyPressEvent(event);
}

