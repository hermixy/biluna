#ifndef RVIEWLISTENER_H
#define RVIEWLISTENER_H

class RDocumentInterface;



/**
 * \brief Abstract base class for classes that are interested 
 * in being notified whenever a view has been added or changed.
 * This can for example be a widget that lists all views.
 *
 * \ingroup core
 * \scriptable
 */
class RViewListener {
public:
    virtual ~RViewListener() {}

    /**
     * This method is called whenever at least one view in the given
     * document has changed.
     */
    virtual void updateViews(RDocumentInterface* documentInterface) = 0;

    /**
     * This method is called to signal that no document is currently active.
     */
    virtual void clearViews() = 0;
};

Q_DECLARE_METATYPE(RViewListener*)

#endif
