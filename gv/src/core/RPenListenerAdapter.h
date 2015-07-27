#ifndef RPENLISTENERADAPTER_H_
#define RPENLISTENERADAPTER_H_

#include <QObject>

#include "RDocumentInterface.h"
#include "RPenListener.h"

/**
 * \scriptable
 */
class RPenListenerAdapter: public QObject, public RPenListener {
Q_OBJECT

public:
	RPenListenerAdapter(QObject* parent=NULL) : QObject(parent) { }
	virtual ~RPenListenerAdapter(){ }

    virtual void updatePen(RDocumentInterface* documentInterface) {
		emit penUpdated(documentInterface);
	}

    virtual void clearPen() {
        emit penCleared();
    }

signals:
	void penUpdated(RDocumentInterface* documentInterface);
    void penCleared();
};

Q_DECLARE_METATYPE(RPenListenerAdapter*)

#endif
