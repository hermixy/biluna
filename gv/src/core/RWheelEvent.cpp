#include "RWheelEvent.h"

#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RUcs.h"
#include "RDocumentInterface.h"

RWheelEvent::RWheelEvent(const RVector& position, int delta,
		Qt::MouseButtons buttons, Qt::KeyboardModifiers modifiers,
		Qt::Orientation orient, RGraphicsScene& s, RGraphicsView& v) :
	QWheelEvent(QPoint(position.x, position.y), delta, buttons, modifiers,
			orient),
    RInputEvent(position, s, v) {

}

RWheelEvent::RWheelEvent(const QWheelEvent& wheelEvent, RGraphicsScene& s,
		RGraphicsView& v) :
    QWheelEvent(wheelEvent),
	RInputEvent(RVector(wheelEvent.pos().x(), wheelEvent.pos().y()), s, v) {
}

RWheelEvent::~RWheelEvent() {
}
