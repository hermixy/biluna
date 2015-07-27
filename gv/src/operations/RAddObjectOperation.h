#ifndef RADDOBJECTOPERATION_H
#define RADDOBJECTOPERATION_H

#include <QSharedPointer>

#include "RAddObjectsOperation.h"
#include "ROperation.h"
#include "RVector.h"

class RObject;

/**
 * Implementation of an operation that adds entities.
 * 
 * \ingroup operations
 * \scriptable
 */
class RAddObjectOperation: public RAddObjectsOperation {

public:
    RAddObjectOperation(QSharedPointer<RObject> object,
                        bool useCurrentAttributes = true, bool undoable = true);
    virtual ~RAddObjectOperation() { }
};

Q_DECLARE_METATYPE(RAddObjectOperation*)

#endif
