#ifndef RDELETESELECTIONOPERATION_H
#define RDELETESELECTIONOPERATION_H

#include "RDocument.h"
#include "REntity.h"
#include "RExporter.h"
#include "RClipboardOperation.h"
#include "RVector.h"


/**
 * Implementation of an operation that deletes the selection of the
 * given document to the clipboard.
 * 
 * \ingroup operations
 * \scriptable
 */
class RDeleteSelectionOperation : public RClipboardOperation {
public:
    RDeleteSelectionOperation();
    virtual ~RDeleteSelectionOperation() {}
    
    virtual RTransaction apply(RDocument& document) const;
    virtual void preview(RDocument& document, RExporter& exporter) const;
};

Q_DECLARE_METATYPE(RDeleteSelectionOperation*)

#endif
