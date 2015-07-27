#ifndef RINPUTEVENT_H
#define RINPUTEVENT_H

#include <QMouseEvent>

#include "RVector.h"

class RGraphicsScene;
class RGraphicsView;

/**
 * \brief An input event for a \ref RGraphicsScene "graphics scene".
 *
 * The coordinates in a mouse event are available as scene coordinates
 * or screen coordinates from the view the event originated.
 *
 * \ingroup core
 * \scriptable
 */
class RInputEvent {
public:
	RInputEvent(const RVector& position, RGraphicsScene& s, RGraphicsView& v);
	~RInputEvent();

	RVector getModelPosition() const;
//	RVector getUcsPosition() const;
	RVector getScreenPosition() const;

	RGraphicsView& getGraphicsView() const;
	RGraphicsScene& getGraphicsScene() const;

    /**
     * \return true If the position of the event is valid.
     */
    bool isValid() const {
        return modelPosition.valid;
    }

protected:
	RVector modelPosition;
//	RVector ucsPosition;
	RVector screenPosition;

	RGraphicsScene& scene;
	RGraphicsView& view;
};

Q_DECLARE_METATYPE(RInputEvent*)

#endif
