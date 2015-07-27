#ifndef RENTITYPICKEVENT_H
#define RENTITYPICKEVENT_H

#include "RInputEvent.h"
#include "REntity.h"
#include "RVector.h"

class RGraphicsView;
class RGraphicsScene;

/**
 * Entity pick event.
 *
 * \ingroup core
 * \scriptable
 */
class REntityPickEvent : public RInputEvent {
public:
    /**
     * \param entityId ID of picked entity.
     * \param pos Absolute coordinate.
     */
    REntityPickEvent(REntity::Id entityId, const RVector& position,
       RGraphicsScene& s, RGraphicsView& v);

    /**
     * \return The ID of the picked entity or REntity::INVALID_ID.
     */
    REntity::Id getEntityId() const {
        return entityId;
    }

    /**
     * \return true If the entity ID of the event is valid.
     */
    bool isValid() const {
        return entityId!=REntity::INVALID_ID;
    }

private:
    REntity::Id entityId;
};

Q_DECLARE_METATYPE(REntityPickEvent*)
Q_DECLARE_METATYPE(const REntityPickEvent*)

#endif
