#include "RCoordinateEvent.h"
#include "RDocumentInterface.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RUcs.h"

RCoordinateEvent::RCoordinateEvent(const RVector& position,
   RGraphicsScene& s, RGraphicsView& v) : RInputEvent(position, s, v) {

    modelPosition = position;
    screenPosition = v.mapToView(position);

//    RDocumentInterface& documentInterface = s.getDocumentInterface();
//    RUcs ucs = documentInterface.getCurrentUcs();
//    ucsPosition = ucs.mapToUcs(modelPosition);
}
