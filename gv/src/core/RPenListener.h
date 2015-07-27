#ifndef RPENLISTENER_H
#define RPENLISTENER_H

class RDocumentInterface;



/**
 * \brief Abstract base class for classes that are interested 
 * in being notified whenever the current pen changes.
 * This can for example be a widget that displays the current pen.
 *
 * \ingroup core
 * \scriptable
 */
class RPenListener {
public:
    virtual ~RPenListener() {}

    /**
     * This method is called whenever the current pen has changed.
     */
    virtual void updatePen(RDocumentInterface* documentInterface) = 0;

    /**
     * This method is called to signal that no document is currently active.
     */
    virtual void clearPen() = 0;
};

Q_DECLARE_METATYPE(RPenListener*)

#endif
