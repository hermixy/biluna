#ifndef RMODIFYOBJECTOPERATION_H_
#define RMODIFYOBJECTOPERATION_H_

#include "RAddObjectOperation.h"

/**
 * Implementation of an operation that adds entities.
 * 
 * \ingroup operations
 * \scriptable
 */
class RModifyObjectOperation: public RAddObjectOperation {
public:
	RModifyObjectOperation(QSharedPointer<RObject> object, bool undoable = true) :
        RAddObjectOperation(object, false, undoable) {
	}
	virtual ~RModifyObjectOperation() {
	}
};

Q_DECLARE_METATYPE(RModifyObjectOperation*);

#endif /* RMODIFYOBJECTOPERATION_H_ */
