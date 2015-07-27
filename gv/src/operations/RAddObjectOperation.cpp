#include "RDocument.h"
#include "RAddObjectOperation.h"

RAddObjectOperation::RAddObjectOperation(QSharedPointer<RObject> object,
                                         bool useCurrentAttributes, bool undoable)
    : RAddObjectsOperation(QList<QSharedPointer<RObject> > () << object,
          useCurrentAttributes, undoable) { }
