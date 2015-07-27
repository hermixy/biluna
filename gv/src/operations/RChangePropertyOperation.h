#ifndef RCHANGEPROPERTYOPERATION_H
#define RCHANGEPROPERTYOPERATION_H

#include <QVariant>

#include "RDocument.h"
#include "RExporter.h"
#include "ROperation.h"
#include "RPropertyTypeId.h"
#include "RTransaction.h"

/**
 * Implementation of an operation that changes a given property to
 * a given value for the selection in a document.
 * 
 * \ingroup operations
 * \scriptable
 */
class RChangePropertyOperation : public ROperation {
public:
    RChangePropertyOperation(
        const RPropertyTypeId& propertyTypeId, const QVariant& value
    );
    virtual ~RChangePropertyOperation() {}
    
    virtual RTransaction apply(RDocument& document) const;
    virtual void preview(RDocument& document, RExporter& exporter) const;

private:
    RPropertyTypeId propertyTypeId;
    QVariant value;
};

Q_DECLARE_METATYPE(RChangePropertyOperation*)

#endif
