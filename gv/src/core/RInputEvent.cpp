#include "RInputEvent.h"
#include "RDocumentInterface.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RUcs.h"

RInputEvent::RInputEvent(const RVector& position, RGraphicsScene& s, RGraphicsView& v)
	: screenPosition(position), scene(s), view(v) {

	modelPosition = view.mapFromView(position);

//	RDocumentInterface& documentInterface = scene.getDocumentInterface();
//	RUcs ucs = documentInterface.getCurrentUcs();
//	ucsPosition = ucs.mapToUcs(modelPosition);
}

RInputEvent::~RInputEvent() {
}

RVector RInputEvent::getModelPosition() const {
	return modelPosition;
}

//RVector RInputEvent::getUcsPosition() const {
//	return ucsPosition;
//}

RVector RInputEvent::getScreenPosition() const {
	return screenPosition;
}

RGraphicsView& RInputEvent::getGraphicsView() const {
	return view;
}

RGraphicsScene& RInputEvent::getGraphicsScene() const {
	return scene;
}
