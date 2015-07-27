#include <QMenu>
#include <QSettings>
#include <QToolBar>
#include <QShortcutEvent>
#include <QFileInfo>

#include "RDebug.h"
#include "RGuiActionQt.h"
#include "RScriptHandler.h"





/**
 * \param text: Menu text, tooltip text.
 */
RGuiActionQt::RGuiActionQt(const QString& text, QObject* parent) :
	QAction(text, parent), RGuiAction(), sortOrder(-1) {

    //RDebug::debug("RGuiActionQt::RGuiActionQt: text: %s", text.c_str());
    
    initTexts();
    
    connect(this, SIGNAL(triggered()),
            this, SLOT(slotTrigger()));

    setCheckable(true);

    setIcon("scripts/Empty.svg");
}



RGuiActionQt::~RGuiActionQt() {
}


void RGuiActionQt::setScriptFile(const QString& sf) {
	RGuiAction::setScriptFile(sf);
	setObjectName(QFileInfo(sf).baseName());
}

void RGuiActionQt::triggerAction() {
	emit triggered();
}

void RGuiActionQt::setShortcutText(const QString& text) {
	shortcutText = text;
	initTexts();
}

void RGuiActionQt::initTexts() {
    QString textAndKeycode = text();
//    QKeySequence sc = shortcut();

    // Remove previously added keycode:
/*
#ifdef Q_OS_MACX
    if (textAndKeycode.indexOf(QLatin1Char(']'))!=-1) {
        textAndKeycode = textAndKeycode.mid(textAndKeycode.indexOf(']')+2);
    }
#else
    if (textAndKeycode.indexOf(QLatin1Char('\t'))!=-1) {
        textAndKeycode = textAndKeycode.left(textAndKeycode.indexOf('\t'));
    }
#endif
*/

    QString textOnly = textAndKeycode;
    textOnly.replace('&', "");

    // Use tab to separate menu text from keycodes.
    //   The menu text with keycode looks like this: "Line\tli".
    //   This does not work under OS X. Text after tab (\t) is
    //   ignored.
    /*
    QString kcs = getKeycodeString();
    if (!kcs.isEmpty()) {
#ifdef Q_OS_MACX
        textAndKeycode = QString("[%1] %2").arg(kcs).arg(textAndKeycode);
#else
        textAndKeycode += QLatin1Char('\t');
        textAndKeycode += kcs;
#endif
    }
    */
#ifndef Q_OS_MACX
    // Override shortcut text (does not work for Mac OS X):
    if (!shortcutText.isEmpty()) {
        if (textAndKeycode.indexOf(QLatin1Char('\t'))!=-1) {
            textAndKeycode = textAndKeycode.left(textAndKeycode.indexOf('\t'));
        }
        textAndKeycode += QLatin1Char('\t');
        textAndKeycode += shortcutText;
    }
#endif

    setText(textAndKeycode);

    QString kcs = shortcutText.isEmpty() ? shortcut().toString() : shortcutText;
    if (!kcs.isEmpty()) {
        QAction::setToolTip(getToolTip(textOnly, kcs.replace(", ", "")));
    }
    else {
//        if (!QAction::shortcut().toString().isEmpty()) {
//            QAction::setToolTip(
//                QString("%1 "
//                    "<span style=\"color: gray; font-size: small\">%2</span>")
//                    .arg(textOnly).arg(QAction::shortcut().toString()));
//        }
//        else {
            QAction::setToolTip(textOnly);
//        }
    }
}


QString RGuiActionQt::getToolTip(const QString& text, const QString& shortcut) {
    return QString("%1 "
                "<span style=\"color: gray; font-size: small\">%2</span>")
                .arg(text)
                .arg(shortcut);
}

    
/*
QString RGuiActionQt::getKeycodeString() {
    QString ret;

#ifdef Q_OS_MACX
    bool defaultShowKeyCodes = 0;
#else
    bool defaultShowKeyCodes = 1;
#endif

    QSettings settings;
    bool showKeycodes = settings.value("Appearance/ShowKeycodes", defaultShowKeyCodes).toBool();
    bool showShortcuts = settings.value("Appearance/ShowShortcuts", 0).toBool();
    
    if (!showKeycodes && !showShortcuts) {
        return ret;
    }
    
	if (!shortcutText.isEmpty()) {
		return shortcutText;
	}
	QString sc = shortcut().toString();

    bool gotKeycodeToShow = 
        (keycodeList.size()>0 && keycodeList.begin()->size()!=0 && showKeycodes);
    
    // keycode (usually two letter code):
    if (gotKeycodeToShow) {
        ret += *(keycodeList.begin());
    }
    
    // add shortcut (e.g. Ctrl+X):
    if (sc.length() != 0 && showShortcuts) {
		if (gotKeycodeToShow) {
			ret += QString(", ");
		}
		QList<QKeySequence> scs = shortcuts();
		for (int i = 0; i < scs.count(); ++i) {
			ret += QString(scs.at(i).toString().toUtf8());
			if (i != scs.count() - 1) {
				ret += QString(", ");
			}
		}
	}

    return ret;
}
*/



void RGuiActionQt::setIcon(const QString& iconFile) {
   QAction::setIcon(QIcon(iconFile));
}



/*
void RGuiActionQt::setShortcut(const QString& sc) {
    shortcutList = sc.split(',');

    QList<QKeySequence> shortcutKeys;
    QStringList::iterator it;
    for (it=shortcutList.begin(); it<shortcutList.end(); ++it) {
        shortcutKeys.append(QKeySequence(*it));
    }
    QAction::setShortcuts(shortcutKeys);

    initTexts();
}
*/



void RGuiActionQt::setShortcut(const QKeySequence& shortcut) {
    //setShortcut(QString(shortcut.toString().toUtf8()));
    QAction::setShortcut(shortcut);
    initTexts();
}



void RGuiActionQt::setToolTip(const QString& tip) {
    QAction::setToolTip(tip);
    initTexts();
}

void RGuiActionQt::setStatusTip(const QString& tip) {
    QAction::setStatusTip(tip);
    initTexts();
}

void RGuiActionQt::setEnabled(bool on) {
	if (on == false) {
		setChecked(false);
	}
	QAction::setEnabled(on);
}

void RGuiActionQt::setChecked(bool on) {
    //if (isChecked()!=on) {
        //blockSignals(true);
        //RGuiAction::setChecked(on);
        QAction::setChecked(on);
        //blockSignals(false);
    //}
}

bool RGuiActionQt::isChecked() {
    return QAction::isChecked();
}

void RGuiActionQt::setSortOrder(int sortOrder) {
	this->sortOrder = sortOrder;
}

int RGuiActionQt::getSortOrder() {
	return sortOrder;
}

void RGuiActionQt::addToMenu(QMenu* menu) {
    addToWidget(menu);
}

void RGuiActionQt::addToToolBar(QToolBar* tb) {
    addToWidget(tb);
    QWidget* w = tb->widgetForAction(this);
    if (w != NULL) {
		w->setObjectName("ToolButton" + objectName());
	}
}

void RGuiActionQt::addToWidget(QWidget* w) {
    if (w==NULL) {
        RDebug::error("RGuiActionQt::addToWidget: widget is NULL");
        return;
    }

    QListIterator<QAction*> i(w->actions());
	while (i.hasNext()) {
		RGuiActionQt* a = dynamic_cast<RGuiActionQt*> (i.next());
		if (a == NULL) {
			continue;
		}
		if (getSortOrder() >= 0 && getSortOrder() < a->getSortOrder()) {
			w->insertAction(a, this);
			return;
		}
	}

    w->addAction(this);
}

RGuiActionQt* RGuiActionQt::getByScriptFile(const QString& scriptFile) {
    return dynamic_cast<RGuiActionQt*>(RGuiAction::getByScriptFile(scriptFile));
}
