#include <QtWidgets>
#include <QTabBar>
#include <QProgressBar>
#include <QLabel>

#include "RMainWindowQt.h"
#include "RMdiChildQt.h"
#include "RDebug.h"
#include "RDocumentInterface.h"
#include "RGuiAction.h"

RMainWindowQt::RMainWindowQt(QWidget* parent) :
    QMainWindow(parent), RMainWindow() {

    mdiArea = new QMdiArea(this);
    mdiArea->setViewMode(QMdiArea::TabbedView);
    mdiArea->setObjectName("MdiArea");
    setCentralWidget(mdiArea);
    setWindowTitle(tr("RMainWindowQt"));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
        this, SLOT(updateScenes(QMdiSubWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
        this, SLOT(updateGuiActions(QMdiSubWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
        this, SLOT(notifyListenersSlot(QMdiSubWindow*)));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
        this, SLOT(suspendAndResume(QMdiSubWindow*)));
}

void RMainWindowQt::handleUserMessage(const QString& message){
    emit userMessage(message);
}

void RMainWindowQt::handleUserWarning(const QString& message, bool messageBox){
    emit userWarning(message, messageBox);
}

void RMainWindowQt::handleUserCommand(const QString& message){
    emit userCommand(message);
}

RMainWindowQt* RMainWindowQt::getMainWindow() {
    return (RMainWindowQt*)mainWindow;
}

void RMainWindowQt::notifyListenersSlot(QMdiSubWindow* mdiChild) {
    static QMdiSubWindow* lastMdiChild = NULL;

    if (lastMdiChild != mdiChild) {
        RMainWindow::notifyListeners();
        lastMdiChild = mdiChild;
    }
}

void RMainWindowQt::updateGuiActions(QMdiSubWindow*) {
    RMdiChildQt* mdi = getMdiChild();
    bool hasDocument = (mdi != NULL);

    for (int i = 0; i < RGuiAction::getActions().size(); ++i) {
        RGuiAction* a = RGuiAction::getActions().at(i);
        if (a->getRequiresDocument()) {
            a->setEnabled(hasDocument);
        }
    }
}

void RMainWindowQt::suspendAndResume(QMdiSubWindow* mdiChild) {
    RMdiChildQt* mdi = getMdiChild();
    if (mdi==NULL) {
        return;
    }
    RDocumentInterface* di = mdi->getDocumentInterface();
    if (di==NULL) {
        return;
    }
    QList<QMdiSubWindow *> list = mdiArea->subWindowList();
    for (int i = 0; i < list.size(); ++i) {
        if (list.at(i)!=NULL && list.at(i)!=mdiChild) {
            RMdiChildQt* mdiOther = dynamic_cast<RMdiChildQt*>(list.at(i)->widget());
            if (mdiOther==NULL) {
                continue;
            }
            RDocumentInterface* diOther = mdiOther->getDocumentInterface();
            if (diOther==NULL) {
                continue;
            }
            diOther->suspend();
        }
    }
    di->resume();
}

void RMainWindowQt::updateScenes(QMdiSubWindow* mdiChild) {
    static QMdiSubWindow* lastMdiChild = NULL;

    if (mdiChild == NULL) {
        //RDebug::error("RMainWindowQt::updateScenes: mdiChild=NULL");
        return;
    }

    if (lastMdiChild != mdiChild) {
        RDocumentInterface* documentInterface = getDocumentInterface();
        if (documentInterface == NULL) {
            RDebug::error("RMainWindowQt::updateScenes: documentInterface=NULL");
            return;
        }

        documentInterface->regenerateScenes();

        lastMdiChild = mdiChild;
    }
}

void RMainWindowQt::closeEvent(QCloseEvent*){
    writeSettings();
}

void RMainWindowQt::dragEnterEvent(QDragEnterEvent* event) {
	emit dragEnter(event);
}

void RMainWindowQt::dropEvent(QDropEvent* event) {
	emit drop(event);
}

void RMainWindowQt::show(){
    readSettings();
    QMainWindow::show();
}

/*
void RMainWindowQt::keyPressEvent(QKeyEvent* e) {
    static QTime ts;
    static QString firstKey;

    switch (e->key()) {
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Alt:
    case Qt::Key_AltGr:
    case Qt::Key_CapsLock:
    case Qt::Key_Meta:
    case Qt::Key_Menu:
    case Qt::Key_Space:
    case Qt::Key_Return:
        QMainWindow::keyPressEvent(e);
        return;
        break;
    case Qt::Key_Escape:
        firstKey = "";
        QMainWindow::keyPressEvent(e);
        return;
        break;
    default:
        e->ignore();
        break;
    }

    QString keyString = QKeySequence(e->key()).toString().toLower();
    RDebug::debug(QString("RMainWindowQt::keyPressEvent: keyString: '%1'").arg(keyString));
    QTime now = QTime::currentTime();

    if (ts.msecsTo(now) < 2000) {
        QString code = firstKey + keyString;
        RDebug::debug(QString("RMainWindowQt::keyPressEvent: code: '%1'").arg(code));
        if (RGuiAction::triggerByKeycode(code)) {
            firstKey = "";
            return;
        } else {
            ts = now;
            firstKey = code;
            if (code.length() >= 3) {
                firstKey = "";
                return;
            }
        }
    } else {
        ts = now;
        firstKey = keyString;
        if (RGuiAction::triggerByKeycode(firstKey)) {
            firstKey = "";
            return;
        }
    }
    QMainWindow::keyPressEvent(e);
}
*/

int RMainWindowQt::getWidth() {
    return width();
}

int RMainWindowQt::getHeight() {
    return height();
}

int RMainWindowQt::getPositionX() {
    return x();
}

int RMainWindowQt::getPositionY() {
    return y();
}

void RMainWindowQt::resize(int width, int height) {
    QMainWindow::resize(width, height);
}

void RMainWindowQt::move(int x, int y) {
    QMainWindow::move(x, y);
}

RDocument* RMainWindowQt::getDocument() {
    RMdiChildQt* mdi = getMdiChild();
    if (mdi != NULL) {
        return mdi->getDocument();
    }
    return NULL;
}

RDocumentInterface* RMainWindowQt::getDocumentInterface() {
    RMdiChildQt* mdi = getMdiChild();
    if (mdi != NULL) {
        return mdi->getDocumentInterface();
    }
    return NULL;
}

RMdiChildQt* RMainWindowQt::getMdiChild() {
    QMdiSubWindow* currentSubWindow = mdiArea->currentSubWindow();
    if (currentSubWindow == NULL && mdiArea->subWindowList().isEmpty()) {
        return NULL;
    }
    currentSubWindow = mdiArea->subWindowList(QMdiArea::StackingOrder).last();
    if (currentSubWindow == NULL) {
        return NULL;
    }

    return dynamic_cast<RMdiChildQt*> (currentSubWindow->widget());
}

QMdiArea* RMainWindowQt::getMdiArea(){
    return mdiArea;
}

QTabBar* RMainWindowQt::getTabBar() {
    QTabBar* tb = findChild<QTabBar*>();
    return tb;
}

void RMainWindowQt::setProgress(int value) {
	if (!progressEnabled) {
		return;
	}
    static int lastProgress = -1;
    if (lastProgress==-1 || abs(value-lastProgress)>=5) {
        emit progress(value);
        lastProgress = value;
    }
}

void RMainWindowQt::cancelProgress() {
	emit progressCanceled();
}

void RMainWindowQt::setProgressEnd() {
	if (!progressEnabled) {
		return;
	}
	emit progressEnd();
}

void RMainWindowQt::setProgressText(const QString& text) {
	if (!progressEnabled) {
		return;
	}
	emit progressText(text);
}

void RMainWindowQt::setRightMouseTip(const QString& text) {
	emit rightMouseTip(text);
}

void RMainWindowQt::setLeftMouseTip(const QString& text) {
	emit leftMouseTip(text);
}

void RMainWindowQt::escapeEvent() {
    emit escape();
}

/**
 * Restores the application window settings (size, position, ...).
 */
void RMainWindowQt::readSettings() {
    RMainWindow::readSettings();
    restoreState(QSettings().value("Appearance/DockappWindows").toByteArray());

    bool on = QSettings().value("Appearance/FullScreen", false).toBool();
	if (bool(windowState() & Qt::WindowFullScreen) == on) {
		return;
	}

	if (on) {
		setWindowState(windowState() | Qt::WindowFullScreen);
		//statusBar()->hide();
		//menuBar()->hide();
	} else {
		setWindowState(windowState() & ~Qt::WindowFullScreen);
		//menuBar()->show();
		//statusBar()->show();
	}
}

/**
 * Stores the application window settings (size, position, ...).
 */
void RMainWindowQt::writeSettings() {
    RMainWindow::writeSettings();
    QSettings().setValue("Appearance/DockappWindows", saveState());
    QSettings().setValue("Appearance/FullScreen", isFullScreen());
}

