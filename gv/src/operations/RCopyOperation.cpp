#include <QSet>

#include "RDocument.h"
#include "RCopyOperation.h"
#include "RExporter.h"


RCopyOperation::RCopyOperation(const RVector& offset, RDocument& src)
    : offset(offset), src(src) {
}



RTransaction RCopyOperation::apply(RDocument& document) const {
    RTransaction transaction(
        RDocument::getClipboard().getStorage(), "Copy to clipboard");

    copy(
        src, document,
        offset,
        1.0, 0.0,             // scale, angle
        false,                // flipHorizontal
        false,                // flipVertical
        false,                // toCurrentLayer
        false,                // toCurrentBlock
        true,                 // overwriteLayers
        true,                 // overwriteBlocks
        QString(),            // blockName
        transaction,
        true,                 // selectionOnly
        true                  // clear
    );

    transaction.end();
    return transaction;
}



void RCopyOperation::preview(RDocument& document, RExporter& exporter) const {
}

