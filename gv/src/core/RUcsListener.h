#ifndef RUCSLISTENER_H
#define RUCSLISTENER_H

class RDocumentInterface;



/**
 * \brief Abstract base class for classes that are interested 
 * in being notified whenever a UCS has been added or changed.
 * This can for example be a widget that lists all UCSs.
 *
 * \ingroup core
 */
class RUcsListener {
public:
    virtual ~RUcsListener() {}

    /**
     * This method is called whenever at least one UCS in the given
     * document has changed.
     */
    virtual void update(RDocumentInterface* documentInterface) = 0;

    /**
     * This method is called to signal that no document is currently active.
     */
    virtual void clear() = 0;
};

Q_DECLARE_METATYPE(RUcsListener*)

#endif
