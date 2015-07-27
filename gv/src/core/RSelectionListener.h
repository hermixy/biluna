#ifndef RSELECTIONLISTENER_H
#define RSELECTIONLISTENER_H

class RDocument;



/**
 * \brief Abstract base class for classes that are interested in the current 
 * clipboard contents and want to be notified whenever the clipboard changes.
 *
 * \ingroup core
 */
class RSelectionListener {
public:
    virtual ~RSelectionListener() {}

    /**
     * Called by the document whenever the current clipboard changes.
     */
    virtual void updateSelectionListener(const RDocument* document) = 0;
};

Q_DECLARE_METATYPE(RSelectionListener*)

#endif
