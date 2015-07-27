#ifndef RSNAP_H
#define RSNAP_H

#include <QSet>

#include "REntity.h"
#include "RMouseEvent.h"


class RGraphicsView;
class RVector;



/**
 * \brief Abstract base class for all grid and object snap 
 * implementations.
 * A snap class implements an object or grid snap tool. For
 * example to snap to end points of objects.
 *
 * \scriptable
 * \generateScriptShell
 *
 * \ingroup core
 */
class RSnap {
public:
    RSnap() {}
    virtual ~RSnap() {}

    /**
     * \return The snapped position (closest relevant position)
     * for the given mouse position.
     */
    virtual RVector snap(const RVector& position, RGraphicsView& view) = 0;

    /**
     * \overload Provided for convenience.
     */
    virtual RVector snap(RMouseEvent& event) {
        return snap(event.getModelPosition(), event.getGraphicsView());
    }

    virtual void showUiOptions() {}
    virtual void hideUiOptions() {}

    /**
     * \return Set of entity IDs that are relevant for the last performed
     * snap, usually used for highlighting.
     */
    QSet<REntity::Id> getEntityIds() {
        return entityIds;
    }

protected:
    QSet<REntity::Id> entityIds;
};

Q_DECLARE_METATYPE(RSnap*)

#endif
