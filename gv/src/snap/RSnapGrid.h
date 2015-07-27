#ifndef RSNAPGRID_H
#define RSNAPGRID_H

#include "RSnap.h"

class RGraphicsView;
class RMouseEvent;

/**
 * \brief Grid snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapGrid : public RSnap {
public:
    virtual ~RSnapGrid() {}

    virtual RVector snap(const RVector& position, RGraphicsView& view);
};

Q_DECLARE_METATYPE(RSnapGrid*)

#endif
