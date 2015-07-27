#ifndef RCLIPBOARDLISTENER_H
#define RCLIPBOARDLISTENER_H

class RDocument;



/**
 * \brief Abstract base class for classes that are interested in the current 
 * clipboard contents and want to be notified whenever the clipboard changes.
 *
 * \ingroup core
 */
class RClipboardListener {
public:
    virtual ~RClipboardListener() {}

    /**
     * Called by the document whenever the current clipboard changes.
     */
    virtual void updateClipboardListener(const RDocument* document) = 0;
};

Q_DECLARE_METATYPE(RClipboardListener*)

#endif
