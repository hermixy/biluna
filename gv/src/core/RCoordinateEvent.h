#ifndef RCOORDINATEEVENT_H
#define RCOORDINATEEVENT_H

#include "RInputEvent.h"
#include "RVector.h"

class RGraphicsView;
class RGraphicsScene;

/**
 * Coordinate Event.
 *
 * \ingroup core
 * \scriptable
 */
class RCoordinateEvent : public RInputEvent {
public:
    /**
     * \param position Absolute model coordinate.
     */
    RCoordinateEvent(const RVector& position,
       RGraphicsScene& s, RGraphicsView& v);
};

Q_DECLARE_METATYPE(RCoordinateEvent*)
Q_DECLARE_METATYPE(const RCoordinateEvent*)

#endif
