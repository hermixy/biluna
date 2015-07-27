#include "RDocumentInterface.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RMouseEvent.h"
#include "RUcs.h"

RMouseEvent::RMouseEvent(Type type, const RVector& position,
		Qt::MouseButton button, Qt::MouseButtons buttons,
		Qt::KeyboardModifiers modifiers, RGraphicsScene& s, RGraphicsView& v)

:
    QMouseEvent(type, QPoint(position.x, position.y), button, buttons, modifiers),
	RInputEvent(position, s, v) {
}

RMouseEvent::RMouseEvent(const QMouseEvent& mouseEvent, RGraphicsScene& s,
		RGraphicsView& v) :
	QMouseEvent(mouseEvent),
    RInputEvent(RVector(mouseEvent.pos().x(), mouseEvent.pos().y()), s, v) {
}

RMouseEvent::~RMouseEvent() {
}

