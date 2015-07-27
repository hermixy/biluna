#ifndef RADDOBJECTSOPERATION_H
#define RADDOBJECTSOPERATION_H

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
class RAddObjectsOperation: public ROperation {
public:
    RAddObjectsOperation(bool undoable = true);
    RAddObjectsOperation(QList<QSharedPointer<RObject> >& list,
        bool useCurrentAttributes = true, bool undoable = true);
    virtual ~RAddObjectsOperation() {
    }

    void addObject(const QSharedPointer<RObject>& object,
    bool useCurrentAttributes = true);

    virtual RTransaction apply(RDocument& document) const;
    virtual void preview(RDocument& document, RExporter& exporter) const;

protected:
    RTransaction previewOrApply(RDocument& document, RExporter* exporter=NULL) const;

private:
    QList<QPair<QSharedPointer<RObject>, bool> > list;
};

Q_DECLARE_METATYPE(RAddObjectsOperation*)

#endif
