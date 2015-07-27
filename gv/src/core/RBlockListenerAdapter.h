#ifndef RBLOCKLISTENERADAPTER_H_
#define RBLOCKLISTENERADAPTER_H_

#include <QObject>

#include "RDocumentInterface.h"
#include "RVector.h"
#include "RBlockListener.h"

/**
 * This block listener adapter emits signals on block events.
 * Used to simplify listening to block events from QObjects,
 * for instance from an ECMA script.
 *
 * \scriptable
 */
class RBlockListenerAdapter: public QObject, public RBlockListener {
Q_OBJECT

public:
	RBlockListenerAdapter(QObject* parent=NULL) : QObject(parent) { }
	virtual ~RBlockListenerAdapter(){ }

    virtual void updateBlocks(RDocumentInterface* documentInterface) {
		emit blocksUpdated(documentInterface);
	}

    virtual void clearBlocks() {
        emit blocksCleared();
    }

signals:
	void blocksUpdated(RDocumentInterface* documentInterface);
    void blocksCleared();
};

Q_DECLARE_METATYPE(RBlockListenerAdapter*)

#endif
