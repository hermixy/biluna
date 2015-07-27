#ifndef RDELETEALLENTITIESOBJECTSOPERATION_H
#define RDELETEALLENTITIESOBJECTSOPERATION_H

#include <QList>
#include <QSharedPointer>

#include "RDocument.h"
#include "RExporter.h"
#include "ROperation.h"
#include "RTransaction.h"
#include "RVector.h"

class RObject;

/**
 * Implementation of an operation that adds entities.
 * 
 * \ingroup operations
 * \scriptable
 */
class RDeleteAllEntitiesOperation: public ROperation {
public:
	RDeleteAllEntitiesOperation(bool undoable = true);
	virtual ~RDeleteAllEntitiesOperation() {
	}

	virtual RTransaction apply(RDocument& document) const;
	virtual void preview(RDocument& document, RExporter& exporter) const;
};

Q_DECLARE_METATYPE(RDeleteAllEntitiesOperation*)

#endif
