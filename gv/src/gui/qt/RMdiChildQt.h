#ifndef RMDICHILDQT_H
#define RMDICHILDQT_H

#include <QWidget>

#include "RDebug.h"

class RDocument;
class RDocumentInterface;

/**
 * \brief MDI child window.
 *
 * \ingroup gui_qt
 *
 * \scriptable
 */
class RMdiChildQt: public QWidget {
Q_OBJECT

public:
	RMdiChildQt(QWidget* parent = NULL);
	~RMdiChildQt();

	void setDocumentInterface(RDocumentInterface* di);

	RDocumentInterface* getDocumentInterface();
	RDocument* getDocument();

    /*
    void focusIn();
    void focusOut();
    */

protected:
    /*
    virtual void focusInEvent(QFocusEvent* e);
    virtual void focusOutEvent(QFocusEvent* e);
    */
	virtual QSize sizeHint() const;

private:
	RDocumentInterface* documentInterface;
	static RDocumentInterface* diLast;
};

Q_DECLARE_METATYPE(RMdiChildQt*)

#endif
