#ifndef RTRANSACTIONLISTENER_H
#define RTRANSACTIONLISTENER_H

class RDocument;
class REntity;



/**
 * \brief Abstract base class for classes that are interested in the current 
 * transaction stack.
 *
 * \ingroup core
 * \scriptable
 */
class RTransactionListener {
public:
    virtual ~RTransactionListener() {}

    /**
     * Called by the document whenever the current transaction stack changes.
     */
    virtual void updateTransactionListener(const RDocument* document) = 0;
};

Q_DECLARE_METATYPE(RTransactionListener*)

#endif
