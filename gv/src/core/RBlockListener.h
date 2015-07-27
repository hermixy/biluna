#ifndef RBLOCKLISTENER_H
#define RBLOCKLISTENER_H

class RDocumentInterface;



/**
 * \brief Abstract base class for classes that are interested 
 * in being notified whenever a block has been added or changed.
 * This can for example be a widget that lists all blocks.
 *
 * \ingroup core
 * \scriptable
 */
class RBlockListener {
public:
    virtual ~RBlockListener() {}

    /**
     * This method is called whenever at least one block in the given
     * document has changed.
     */
    virtual void updateBlocks(RDocumentInterface* documentInterface) = 0;

    /**
     * This method is called to signal that no document is currently active.
     */
    virtual void clearBlocks() = 0;
};

Q_DECLARE_METATYPE(RBlockListener*)

#endif
