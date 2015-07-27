#ifndef RDELETEOBJECTSOPERATION_H
#define RDELETEOBJECTSOPERATION_H

#include <QList>
#include <QSharedPointer>

#include "RDocument.h"
#include "RExporter.h"
#include "ROperation.h"
#include "RTransaction.h"
#include "RVector.h"

class RObject;

/**
 * Implementation of an operation that deletes entities.
 * 
 * \ingroup operations
 * \scriptable
 */
class RDeleteObjectsOperation: public ROperation {
public:
	RDeleteObjectsOperation(bool undoable = true);
	RDeleteObjectsOperation(QList<QSharedPointer<RObject> >& list, bool undoable =
			true);
	virtual ~RDeleteObjectsOperation() {
	}

    void deleteObject(const QSharedPointer<RObject>& o);

	virtual RTransaction apply(RDocument& document) const;
	virtual void preview(RDocument& document, RExporter& exporter) const;

private:
	QList<QSharedPointer<RObject> > list;
};

Q_DECLARE_METATYPE(RDeleteObjectsOperation*)

#endif
