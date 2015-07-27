#ifndef RDELETEOBJECTOPERATION_H
#define RDELETEOBJECTOPERATION_H

#include <QSharedPointer>

#include "RDeleteObjectsOperation.h"
#include "ROperation.h"
#include "RVector.h"

class RDocument;
class RExporter;
class RObject;

/**
 * Implementation of an operation that deletes entities.
 * 
 * \ingroup operations
 * \scriptable
 */
class RDeleteObjectOperation: public RDeleteObjectsOperation {
public:
	RDeleteObjectOperation(QSharedPointer<RObject> object, bool undoable = true);
	virtual ~RDeleteObjectOperation() {
	}
};

Q_DECLARE_METATYPE(RDeleteObjectOperation*)

#endif
