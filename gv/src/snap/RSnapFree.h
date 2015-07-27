#ifndef RSNAPFREE_H
#define RSNAPFREE_H

#include "RSnap.h"

class RGraphicsView;
class RMouseEvent;

/**
 * \brief Free snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapFree : public RSnap {
public:
    virtual ~RSnapFree() {}

    virtual RVector snap(const RVector& position, RGraphicsView& view);
};

Q_DECLARE_METATYPE(RSnapFree*)

#endif
