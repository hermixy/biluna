#ifndef RMOUSEEVENT_H
#define RMOUSEEVENT_H

#include <QMouseEvent>

#include "RVector.h"
#include "RInputEvent.h"

class RGraphicsScene;
class RGraphicsView;

/**
 * \brief A mouse event for a \ref RGraphicsScene "graphics scene".
 *
 * The coordinates in a mouse event are available as scene coordinates
 * of screen coordinates from the view the event originated.
 *
 * \ingroup core
 * \scriptable
 */
class RMouseEvent: public QMouseEvent, public RInputEvent {
public:
	RMouseEvent(QEvent::Type type, const RVector& position,
			Qt::MouseButton button, Qt::MouseButtons buttons,
			Qt::KeyboardModifiers modifiers, RGraphicsScene& s,
			RGraphicsView& v);

	RMouseEvent(const QMouseEvent& mouseEvent, RGraphicsScene& s,
			RGraphicsView& v);

	~RMouseEvent();
};

Q_DECLARE_METATYPE(RMouseEvent*)

#endif
