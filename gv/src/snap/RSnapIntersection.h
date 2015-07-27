#ifndef RSNAPINTERSECTION_H
#define RSNAPINTERSECTION_H

#include "RSnap.h"
#include "REntity.h"

class RGraphicsView;
class RMouseEvent;

/**
 * \brief Intersection snapper implementation.
 *
 * \scriptable
 *
 * \ingroup snap
 */
class RSnapIntersection : public RSnap {
public:
    RSnapIntersection();
    virtual ~RSnapIntersection() {}

    virtual RVector snap(const RVector& position, RGraphicsView& view);
};

Q_DECLARE_METATYPE(RSnapIntersection*)

#endif
