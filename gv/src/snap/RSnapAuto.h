#ifndef RSNAPAUTO_H
#define RSNAPAUTO_H

#include "RSnap.h"

class RMouseEvent;
class RGraphicsView;

/**
 * \brief Auto snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapAuto : public RSnap {
public:
    virtual ~RSnapAuto() {}

    virtual RVector snap(const RVector& position, RGraphicsView& view);
};

Q_DECLARE_METATYPE(RSnapAuto*)

#endif
