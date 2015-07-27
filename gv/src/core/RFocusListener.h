#ifndef RFOCUSLISTENER_H
#define RFOCUSLISTENER_H

class RDocumentInterface;
class REntity;



/**
 * \brief Abstract base class for classes that are interested in the current 
 * focus.
 *
 * \ingroup core
 * \scriptable
 */
class RFocusListener {
public:
    virtual ~RFocusListener() {}

    /**
     * Called by the document whenever the focus changed from one MDI
     * to another.
     *
     * \param di The document interface that has now the focus.
     */
    virtual void updateFocus(RDocumentInterface* di) = 0;
};

Q_DECLARE_METATYPE(RFocusListener*)

#endif
