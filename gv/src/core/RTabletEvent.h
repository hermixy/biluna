#ifndef RTABLETEVENT_H
#define RTABLETEVENT_H

#include <QTabletEvent>

#include "RVector.h"

class RGraphicsScene;
class RGraphicsView;



/**
 * \brief A tablet event for a \ref RGraphicsScene "graphics scene".
 *
 * The coordinates in a mouse event are available as scene coordinates
 * of screen coordinates from the view the event originated.
 *
 * \ingroup core
 * \scriptable
 */
class RTabletEvent: public QTabletEvent {
public:
	/*
    RTabletEvent(
			QEvent::Type type,
			const RVector& position,
			int device,
			int pointerType,
			qreal pressure,
			int xTilt,
			int yTilt,
			qreal tangentialPressure,
			qreal rotation,
			int z,
			Qt::KeyboardModifiers keyState,
			qint64 uniqueID,
			RGraphicsScene& s,
			RGraphicsView& v);
			*/

    RTabletEvent(
    		const QTabletEvent& tabletEvent,
            RGraphicsScene& s,
            RGraphicsView& v
    		);

    ~RTabletEvent();

    RVector getModelPosition() const;
    RVector getUcsPosition() const;
    RVector getScreenPosition() const;

    RGraphicsView& getGraphicsView() const;
    RGraphicsScene& getGraphicsScene() const;

protected:
    RVector modelPosition;
    RVector ucsPosition;
    RVector screenPosition;
    
    RGraphicsScene& scene;
    RGraphicsView& view;
};

Q_DECLARE_METATYPE(RTabletEvent*)

#endif
