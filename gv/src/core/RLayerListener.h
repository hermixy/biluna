#ifndef RLAYERLISTENER_H
#define RLAYERLISTENER_H

class RDocumentInterface;



/**
 * \brief Abstract base class for classes that are interested 
 * in being notified whenever a layer has been added or changed.
 * This can for example be a widget that lists all layers.
 *
 * \ingroup core
 * \scriptable
 */
class RLayerListener {
public:
    virtual ~RLayerListener() {}

    /**
     * This method is called whenever at least one layer in the given
     * document has changed.
     */
    virtual void updateLayers(RDocumentInterface* documentInterface) = 0;

    /**
     * This method is called to signal that no document is currently active.
     */
    virtual void clearLayers() = 0;
};

Q_DECLARE_METATYPE(RLayerListener*)

#endif
