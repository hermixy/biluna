#ifndef RVIEWLISTENERADAPTER_H_
#define RVIEWLISTENERADAPTER_H_

#include <QObject>

#include "RDocumentInterface.h"
#include "RVector.h"
#include "RViewListener.h"

/**
 * \scriptable
 */
class RViewListenerAdapter: public QObject, public RViewListener {
Q_OBJECT

public:
	RViewListenerAdapter(QObject* parent=NULL) : QObject(parent) { }
	virtual ~RViewListenerAdapter(){ }

    virtual void updateViews(RDocumentInterface* documentInterface) {
		emit viewsUpdated(documentInterface);
	}

    virtual void clearViews() {
        emit viewsCleared();
    }

signals:
	void viewsUpdated(RDocumentInterface* documentInterface);
    void viewsCleared();
};

Q_DECLARE_METATYPE(RViewListenerAdapter*)

#endif
