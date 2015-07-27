#ifndef RCOPYOPERATION_H
#define RCOPYOPERATION_H

#include "RDocument.h"
#include "REntity.h"
#include "RExporter.h"
#include "RClipboardOperation.h"
#include "RVector.h"


/**
 * Implementation of an operation that copies the selection of the
 * given document to the clipboard
 * 
 * \ingroup operations
 * \scriptable
 */
class RCopyOperation : public RClipboardOperation {
public:
    RCopyOperation(const RVector& offset, RDocument& src);
    virtual ~RCopyOperation() {}
    
    virtual RTransaction apply(RDocument& document) const;
    virtual void preview(RDocument& document, RExporter& exporter) const;

private:
    RVector offset;
    RDocument& src;
};

Q_DECLARE_METATYPE(RCopyOperation*)

#endif
