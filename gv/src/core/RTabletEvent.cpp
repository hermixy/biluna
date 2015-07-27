#include "RDocumentInterface.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RTabletEvent.h"
#include "RUcs.h"


/*
RTabletEvent::RTabletEvent(
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
		RGraphicsView& v)

    :   QTabletEvent(type, QPoint(position.x, position.y), button, buttons, modifiers),
    	screenPosition(position),
        scene(s),
        view(v) {

    modelPosition = view.mapFromView(position);

    RDocumentInterface& documentInterface = view.getDocumentInterface();
    RUcs ucs = documentInterface.getCurrentUcs();
    ucsPosition = ucs.mapToUcs(modelPosition);
}
*/


RTabletEvent::RTabletEvent(const QTabletEvent& tabletEvent, RGraphicsScene& s,
		RGraphicsView& v) :
	QTabletEvent(tabletEvent), screenPosition(tabletEvent.pos().x(),
			tabletEvent.pos().y()), scene(s), view(v) {

}


RTabletEvent::~RTabletEvent() {
}



RVector RTabletEvent::getModelPosition() const {
    return modelPosition;
}



RVector RTabletEvent::getUcsPosition() const {
    return ucsPosition;
}


RVector RTabletEvent::getScreenPosition() const {
    return screenPosition;
}

RGraphicsView& RTabletEvent::getGraphicsView() const {
    return view;
}


RGraphicsScene& RTabletEvent::getGraphicsScene() const {
    return scene;
}
