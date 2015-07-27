#ifndef RMAINWINDOWQT_H
#define RMAINWINDOWQT_H

#include <QMainWindow>
#include <QKeyEvent>
#include "RMainWindow.h"

class QMdiArea;
class QMdiSubWindow;
class QProgressBar;

class RCoordinateListener;
class RDocument;
class RDocumentInterface;
class RFocusListener;
class RLayerListener;
class RBlockListener;
class RViewListener;
class RMdiChildQt;
class RPropertyListener;
class RSelectionListener;
class RSnapListener;
class RUcsListener;
class RPenListener;

/**
 * \brief Base class for a Qt based MDI main application window.
 *
 * \ingroup gui_qt
 *
 * \scriptable
 */
class RMainWindowQt: public QMainWindow, public RMainWindow {
Q_OBJECT

public:
	RMainWindowQt(QWidget* parent = 0);
	static RMainWindowQt* getMainWindow();
	virtual int getWidth();
	virtual int getHeight();
	virtual int getPositionX();
	virtual int getPositionY();
	virtual void resize(int width, int height);
	virtual void move(int x, int y);
	virtual RDocument* getDocument();
	virtual RDocumentInterface* getDocumentInterface();
	virtual void handleUserMessage(const QString& message);
	virtual void handleUserWarning(const QString& message, bool messageBox =
			false);
	virtual void handleUserCommand(const QString& message);
	RMdiChildQt* getMdiChild();
	QTabBar* getTabBar();
	QMdiArea* getMdiArea();

	virtual void cancelProgress();
	virtual void setProgress(int value);
	virtual void setProgressEnd();
	virtual void setProgressText(const QString& text = "");

	virtual void setLeftMouseTip(const QString& text = "");
	virtual void setRightMouseTip(const QString& text = "");

    virtual void escapeEvent();

public slots:
	void show();

signals:
	void leftMouseTip(const QString& message);
	void rightMouseTip(const QString& message);
	void progress(int value);
	void progressEnd();
	void progressText(const QString& label);
	void progressCanceled();
	void userMessage(const QString& message);
	void userWarning(const QString& message, bool messageBox);
	void userCommand(const QString& message);
    void escape();
    void drop(QDropEvent* event);
    void dragEnter(QDragEnterEvent* event);

protected:
	virtual void closeEvent(QCloseEvent* e);
    //virtual void keyPressEvent(QKeyEvent* e);
	virtual void dropEvent(QDropEvent* event);
	virtual void dragEnterEvent(QDragEnterEvent* event);
	virtual void readSettings();
	virtual void writeSettings();

protected slots:
	void updateGuiActions(QMdiSubWindow* mdiChild = NULL);
	void updateScenes(QMdiSubWindow* mdiChild);
	void notifyListenersSlot(QMdiSubWindow* mdiChild);
	void suspendAndResume(QMdiSubWindow* mdiChild);

protected:
	QMdiArea* mdiArea;
};

Q_DECLARE_METATYPE(RMainWindowQt*)

#endif
