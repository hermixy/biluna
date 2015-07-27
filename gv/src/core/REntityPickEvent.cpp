#include "REntity.h"
#include "RDocumentInterface.h"
#include "REntityPickEvent.h"
#include "RGraphicsScene.h"
#include "RGraphicsView.h"
#include "RUcs.h"
#include "RVector.h"



REntityPickEvent::REntityPickEvent(REntity::Id entityId, const RVector& position,
   RGraphicsScene& s, RGraphicsView& v) : RInputEvent(position, s, v) {
    this->entityId = entityId;
    modelPosition = position;
    screenPosition = v.mapToView(position);

//    RDocumentInterface& documentInterface = s.getDocumentInterface();
//    RUcs ucs = documentInterface.getCurrentUcs();
//    ucsPosition = ucs.mapToUcs(modelPosition);
}

