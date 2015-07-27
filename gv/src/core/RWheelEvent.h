#ifndef RWHEELEVENT_H
#define RWHEELEVENT_H

#include <QWheelEvent>

#include "RInputEvent.h"

class RGraphicsView;
class RGraphicsScene;

/**
 * \brief A mouse wheel event for a \ref RGraphicsScene "graphics scene".
 *
 * The coordinates in a mouse wheel event are available as scene 
 * coordinates or screen coordinates from the view the event originated.
 *
 * \ingroup core
 * \scriptable
 */
class RWheelEvent: public QWheelEvent, public RInputEvent {
public:
	RWheelEvent(const RVector& position, int delta, Qt::MouseButtons buttons,
			Qt::KeyboardModifiers modifiers, Qt::Orientation orient,
			RGraphicsScene& s, RGraphicsView& v);

	RWheelEvent(const QWheelEvent& wheelEvent, RGraphicsScene& s,
			RGraphicsView& v);

	~RWheelEvent();

};

Q_DECLARE_METATYPE(RWheelEvent*)

#endif
