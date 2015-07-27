#ifndef RBLOCKREFERENCEDATA_H
#define RBLOCKREFERENCEDATA_H

#include "RBox.h"
#include "RDocument.h"
#include "REntity.h"
#include "RVector.h"
#include "RBlock.h"

/**
 * \scriptable
 * \ingroup entity
 */
class RBlockReferenceData: public REntityData {

friend class RBlockReferenceEntity;

public:
    RBlockReferenceData();
    RBlockReferenceData(RDocument* document, const RBlockReferenceData& data);
    RBlockReferenceData(RBlock::Id referencedBlockId,
			const RVector& position, const RVector& scaleFactors, double angle);

    virtual RBox getBoundingBox() const;
    virtual QList<RBox> getBoundingBoxes() const;

    virtual QList<RVector> getReferencePoints(
        RS::ProjectionRenderingHint /*hint*/ = RS::Top) const;
    virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual RVector getClosestPointOnEntity(const RVector& point) const;
    virtual QList<RVector> getIntersectionPoints(const REntityData& other, 
        bool limited = true) const;
    virtual RVector getVectorTo(const RVector& point, 
        bool limited = true) const;
    //virtual RVector getVectorTo(const RLine& line,
    //    bool limited = true) const;
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance) const;

    virtual bool intersectsWith(const RTriangle& triangle) const;

    virtual bool moveReferencePoint(const RVector& referencePoint, 
        const RVector& targetPoint);
    virtual bool move(const RVector& offset);
    virtual bool rotate(double rotation, const RVector& center = RVector());
    virtual bool mirror(const RLine& axis);
    virtual bool scale(const RVector& scaleFactors, const RVector& center = RVector());
    virtual bool flipHorizontal();
    virtual bool flipVertical();

    void setReferencedBlockId(RBlock::Id blockId) {
        referencedBlockId = blockId;
    }

    RBlock::Id getReferencedBlockId() const {
        return referencedBlockId;
    }

    RVector getPosition() const {
		return position;
	}

    RVector getScaleFactors() const {
		return scaleFactors;
	}

    double getRotation() const {
		return rotation;
	}

protected:
    QSharedPointer<REntity> queryEntity(REntity::Id entityId) const;

private:
    RBlock::Id referencedBlockId;
    RVector position;
    RVector scaleFactors;
    double rotation;
};

Q_DECLARE_METATYPE(RBlockReferenceData*)

#endif
