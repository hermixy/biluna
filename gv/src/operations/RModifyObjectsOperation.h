#ifndef RMODIFYOBJECTSOPERATION_H_
#define RMODIFYOBJECTSOPERATION_H_

#include "RAddObjectsOperation.h"

/**
 * Implementation of an operation that adds entities.
 * 
 * \ingroup operations
 * \scriptable
 */
class RModifyObjectsOperation: public RAddObjectsOperation {
public:
	RModifyObjectsOperation(bool undoable = true) :
        RAddObjectsOperation(undoable) {
	}

	RModifyObjectsOperation(QList<QSharedPointer<RObject> >& list,
			bool undoable = true) :
        RAddObjectsOperation(list, false, undoable) {
	}

	virtual ~RModifyObjectsOperation() {
	}
};

Q_DECLARE_METATYPE(RModifyObjectsOperation*);

#endif
