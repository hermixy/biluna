#ifndef RLAYERLISTENERADAPTER_H_
#define RLAYERLISTENERADAPTER_H_

#include <QObject>

#include "RDocumentInterface.h"
#include "RLayerListener.h"

/**
 * \scriptable
 */
class RLayerListenerAdapter: public QObject, public RLayerListener {
Q_OBJECT

public:
	RLayerListenerAdapter(QObject* parent=NULL) : QObject(parent) { }
	virtual ~RLayerListenerAdapter(){ }

    virtual void updateLayers(RDocumentInterface* documentInterface) {
		emit layersUpdated(documentInterface);
	}

    virtual void clearLayers() {
        emit layersCleared();
    }

signals:
	void layersUpdated(RDocumentInterface* documentInterface);
    void layersCleared();
};

Q_DECLARE_METATYPE(RLayerListenerAdapter*)

#endif
