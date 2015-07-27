#ifndef RSNAPLISTENER_H
#define RSNAPLISTENER_H

#include "RSnap.h"

class RDocumentInterface;
class REntity;



/**
 * \brief Abstract base class for classes that are interested in the current 
 * snap mode.
 *
 * \ingroup core
 * \scriptable
 */
class RSnapListener {
public:
    virtual ~RSnapListener() {}

    /**
     * Called by the document whenever the current snap mode changes.
     */
    virtual void updateSnap(RDocumentInterface* di) = 0;
};

Q_DECLARE_METATYPE(RSnapListener*)

#endif
