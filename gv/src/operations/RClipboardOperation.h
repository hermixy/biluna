#ifndef RCLIPBOARDOPERATION_H
#define RCLIPBOARDOPERATION_H

#include "RDocument.h"
#include "REntity.h"
#include "RExporter.h"
#include "ROperation.h"
#include "RVector.h"


/**
 * Base class for clipboard related operations (copy, cut, paste).
 * 
 * \ingroup operations
 * \scriptable
 * \abstract
 */
class RClipboardOperation : public ROperation {
public:
    RClipboardOperation();
    virtual ~RClipboardOperation() {}
    
protected:
    void copy(
        RDocument& src, RDocument& dest,
        const RVector& offset,
        double scale,
        double rotation,
        bool flipHorizontal,
        bool flipVertical,
        bool toCurrentLayer,
        bool toCurrentBlock,
        bool overwriteLayers,
        bool overwriteBlocks,
        const QString& blockName,
        RTransaction& transaction,
        bool selectionOnly, bool clear
    ) const;
    void copyEntity(
        REntity& entity,
        RDocument& src, RDocument& dest,
        const RVector& offset,
        double scale,
        double rotation,
        bool flipHorizontal,
        bool flipVertical,
        bool toCurrentLayer,
        bool toCurrentBlock,
        bool overwriteLayers,
        bool overwriteBlocks,
        const QString& blockName,
        RTransaction& transaction
    ) const;
};

Q_DECLARE_METATYPE(RClipboardOperation*)

#endif
