#ifndef RPASTEOPERATION_H
#define RPASTEOPERATION_H

#include "RDocument.h"
#include "REntity.h"
#include "RExporter.h"
#include "RClipboardOperation.h"
#include "RVector.h"


/**
 * Implementation of an operation that pastes the clipboard
 * at a given reference point.
 * 
 * \ingroup operations
 * \scriptable
 */
class RPasteOperation : public RClipboardOperation {
public:
    RPasteOperation(RDocument& sourceDocument);
    virtual ~RPasteOperation() {}

    void setOffset(const RVector& offset) {
        this->offset = offset;
    }
    void setScale(double scale) {
        this->scale = scale;
    }
    void setRotation(double rotation) {
        this->rotation = rotation;
    }
    void setFlipHorizontal(bool flipHorizontal) {
        this->flipHorizontal = flipHorizontal;
    }
    void setFlipVertical(bool flipVertical) {
        this->flipVertical = flipVertical;
    }
    void setToCurrentLayer(bool toCurrentLayer) {
        this->toCurrentLayer = toCurrentLayer;
    }
    void setOverwriteLayers(bool overwriteLayers) {
        this->overwriteLayers = overwriteLayers;
    }
    void setOverwriteBlocks(bool overwriteBlocks) {
        this->overwriteBlocks = overwriteBlocks;
    }

    void setBlockName(const QString& blockName) {
		this->blockName = blockName;
	}

    virtual RTransaction apply(RDocument& document) const;
    virtual void preview(RDocument& document, RExporter& exporter) const;

protected:
    RTransaction previewOrApply(RDocument& document, RExporter* exporter=NULL) const;

private:
    RDocument& sourceDocument;
    RVector offset;
    double scale;
    double rotation;
    bool flipHorizontal;
    bool flipVertical;
    bool toCurrentLayer;
    bool overwriteLayers;
    bool overwriteBlocks;
    QString blockName;
};

Q_DECLARE_METATYPE(RPasteOperation*)

#endif
