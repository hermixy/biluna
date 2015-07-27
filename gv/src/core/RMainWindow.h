#ifndef RMAINWINDOW_H
#define RMAINWINDOW_H

#include "RTransactionListener.h"
#include "RPropertyEvent.h"

class RBlockListener;
class RCoordinateListener;
class RDocument;
class RDocumentInterface;
class REntity;
class RFocusListener;
class RLayerListener;
class RPenListener;
class RPropertyListener;
class RSelectionListener;
class RSnapListener;
class RUcsListener;
class RVector;
class RViewListener;

/**
 * \brief Base class for an MDI main application window.
 *
 * \ingroup core
 * \scriptable
 */
class RMainWindow {

public:
	RMainWindow();
	virtual ~RMainWindow();

	static RMainWindow* getMainWindow();

	static bool hasMainWindow();

	/**
	 * \return Pointer to the document that is currently open and has
	 *      the focus or NULL if no document is open.
	 */
	virtual RDocument* getDocument() = 0;

	/**
	 * \return Pointer to the document interface of the document that is
	 *      currently open and has the focus or NULL if no document is open.
	 */
	virtual RDocumentInterface* getDocumentInterface() = 0;
	static RDocumentInterface* getDocumentInterfaceStatic();

	void notifyListeners();

	void addCoordinateListener(RCoordinateListener* l);
	void notifyCoordinateListeners(const RVector& wcsPosition,
			const RVector& ucsPosition);

	void addSelectionListener(RSelectionListener* l);
	void notifySelectionListeners(RDocument* document);

	void addPropertyListener(RPropertyListener* l);
	void notifyPropertyListeners(RDocument* document);
	void notifyPropertyListeners(RDocument& document, REntity& entity);
	void notifyPropertyListeners();

	void addTransactionListener(RTransactionListener* l);
	void notifyTransactionListeners(RDocument* document);

	void addSnapListener(RSnapListener* l);
	void notifySnapListeners(RDocumentInterface* documentInterface);

	void addFocusListener(RFocusListener* l);
	void notifyFocusListeners(RDocumentInterface* documentInterface);

    void propertyChangeEvent(RPropertyEvent& event);

	void addUcsListener(RUcsListener* l);
	void notifyUcsListeners(RDocumentInterface* documentInterface);

	void addLayerListener(RLayerListener* l);
	void notifyLayerListeners(RDocumentInterface* documentInterface);

	void addBlockListener(RBlockListener* l);
	void notifyBlockListeners(RDocumentInterface* documentInterface);

	void addViewListener(RViewListener* l);
	void notifyViewListeners(RDocumentInterface* documentInterface);

	void addPenListener(RPenListener* l);
	void notifyPenListeners(RDocumentInterface* documentInterface);

	void ucsSetEvent(const QString& ucsName);

	void commandEvent(const QString& command);

    virtual void escapeEvent() = 0;

	/**
	 * \return Width of the main application window.
	 */
	virtual int getWidth() = 0;
	/**
	 * \return Height of the main application window.
	 */
	virtual int getHeight() = 0;
	/**
	 * \return X position of the main application window
	 *      (distance from left of screen).
	 */
	virtual int getPositionX() = 0;
	/**
	 * \return Y position of the main application window
	 *      (distance from top of screen).
	 */
	virtual int getPositionY() = 0;

	/**
	 * Resizes the application window to the given \c width and \c height.
	 */
	virtual void resize(int width, int height) = 0;
	/**
	 * Moves the application window to the given \c x / \c y position.
	 */
	virtual void move(int x, int y) = 0;

	virtual void handleUserMessage(const QString& message) = 0;
	virtual void handleUserWarning(const QString& message, bool messageBox =
			false) = 0;
	virtual void handleUserCommand(const QString& message) = 0;

	virtual void setProgress(int value) = 0;
	virtual void setProgressEnd() = 0;
	virtual void setProgressText(const QString& text = "") = 0;
	virtual void setProgressEnabled(bool on) {
		progressEnabled = on;
	}

	virtual void setLeftMouseTip(const QString& text = "")=0;
	virtual void setRightMouseTip(const QString& text = "")=0;

protected:
	virtual void readSettings();
	virtual void writeSettings();

protected:
	static RMainWindow* mainWindow;
	bool progressEnabled;

	QList<RCoordinateListener*> coordinateListeners;
	QList<RSelectionListener*> selectionListeners;
	QList<RPropertyListener*> propertyListeners;
	QList<RUcsListener*> ucsListeners;
	QList<RLayerListener*> layerListeners;
	QList<RBlockListener*> blockListeners;
	QList<RViewListener*> viewListeners;
	QList<RPenListener*> penListeners;
	QList<RTransactionListener*> transactionListeners;
	QList<RSnapListener*> snapListeners;
	QList<RFocusListener*> focusListeners;
};

Q_DECLARE_METATYPE(RMainWindow*)

#endif
