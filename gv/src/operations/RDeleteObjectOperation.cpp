#include "RDocument.h"
#include "RDeleteObjectOperation.h"

RDeleteObjectOperation::RDeleteObjectOperation(QSharedPointer<RObject> object, bool undoable) :
	RDeleteObjectsOperation(QList<QSharedPointer<RObject> > () << object, undoable) {
}
