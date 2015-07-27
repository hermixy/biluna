#ifndef RENTITYDATA_H
#define RENTITYDATA_H

#include <QList>

#include "RBlock.h"
#include "RBox.h"
#include "RColor.h"
#include "RLayer.h"
#include "RLine.h"
#include "RMatrix.h"
#include "RObject.h"
#include "RView.h"

class RBlockReferenceEntity;
class RDocument;
class REntity;
class RExporter;


/**
 * Base class for all entity data classes. Such classes are
 * used to separate functionality from data. Data objects may
 * for example be used to perform certain calculations. A
 * custom entity might use the same data class as a standard
 * entity to provide similar behavior (e.g. a wall might
 * behave similar like a line entity).
 *
 * \ingroup core
 * \scriptable
 */
class REntityData {

    friend class REntity;

public:
    REntityData(RDocument* document=NULL);

    virtual ~REntityData() { }

    RDocument* getDocument() {
    	return document;
    }

    void setDocument(RDocument* document) {
        this->document = document;
    }

    /**
     * \return Shapes that represent the geometry of this entity.
     * \nonscriptable
     */
    virtual QList<const RShape*> getShapes() const {
        return QList<const RShape*>();
    }

    /**
     * \return Shapes that represent the geometry of this entity.
     * \nonscriptable
     */
    virtual QList<RShape*> getShapes() {
        QList<RShape*> ret;
        QList<const RShape*> c = ((const REntityData*)(this))->getShapes();
        for (int i=0; i<c.size(); i++) {
            ret.append(const_cast<RShape*>(c.at(i)));
        }
        return ret;
    }

    virtual RBox getBoundingBox() const;

    /**
     * \return Vector of bounding boxes that contain this entity.
     * This is used for complex entities such as block references
     * to further optimize algorithms that depend on bounding boxes
     * (e.g. spatial index algorithms). The default implementation
     * returns the bounding box that contains the whole entity.
     */
    virtual QList<RBox> getBoundingBoxes() const {
        return QList<RBox>() << getBoundingBox();
    }

    /**
     * \return True if the entity is currently selected. This can for example
     *      influence the color in which the entity is exported.
     */
    virtual bool isSelected() const {
        return selectionStatus;
    }

    /**
     * Selects or deselects this entity.
     */
    virtual void setSelected(bool on) {
        selectionStatus = on;
    }

    /**
     * \return The draw order of this entity. Entities are usually drawn in
     * ascending order of this number.
     */
    virtual int getDrawOrder() const {
        return drawOrder;
    }

    /**
     * Sets the draw order of this entity.
     */
    virtual void setDrawOrder(int drawOrder) {
        this->drawOrder = drawOrder;
    }

    virtual void setLayerId(RLayer::Id layerId) {
        this->layerId = layerId;
    }

    RLayer::Id getLayerId() const {
    	return layerId;
    }

    virtual void setBlockId(RBlock::Id blockId) {
        this->blockId = blockId;
    }

    RLayer::Id getBlockId() const {
    	return blockId;
    }

    virtual void setLinetypeId(RLinetype::Id linetypeId) {
        this->linetypeId = linetypeId;
    }

    virtual void setLinetype(RLinetype linetype);

    RLinetype::Id getLinetypeId() const {
        return linetypeId;
    }

    RLinetype::Id getLinetypeId(bool resolve,
        RLayer* layer, RBlockReferenceEntity* blockRef) const;

    RLineweight::Lineweight getLineweight() const {
        return lineweight;
    }

    RLineweight::Lineweight getLineweight(bool resolve,
        RLayer* layer, RBlockReferenceEntity* blockRef) const;

    double getLineweightInUnits(RLayer* layer, RBlockReferenceEntity* blockRef) const;

    void setLineweight(RLineweight::Lineweight lineweight){
        this->lineweight = lineweight;
    }

    void setColor(const RColor& color) {
        this->color = color;
    }

    RColor getColor() const {
        return color;
    }

    RColor getColor(bool resolve, RLayer* layer,
                    RBlockReferenceEntity* blockRef) const;

    /**
     * \return Vector of reference points of this entity. Reference points
     *      can usually be moved around by the user to change the geometry
     *      of the entity.
     */
    virtual QList<RVector> getReferencePoints(
        RS::ProjectionRenderingHint hint=RS::Top) const = 0;

    virtual QList<RVector> getEndPoints() const;
    virtual QList<RVector> getMiddlePoints() const;
    virtual QList<RVector> getCenterPoints() const;
    virtual QList<RVector> getPointsWithDistanceToEnd(
        double distance) const;

    virtual RVector getClosestPointOnEntity(const RVector& point) const;

    //virtual QList<RVector> getIntersectionPoints(RDocument& document) const;

    virtual QList<RVector> getIntersectionPoints(const REntityData& other,
        bool limited = true) const;

    virtual RVector getVectorTo(const RVector& point,
        bool limited=true) const;

    /**
     * \return The distance vector from this entity to the given line.
     *
     * \param line the line to which the distance was measured
     *
     * \param limited: If true, an invalid vector is returned if the
     *   closest point on the entity is outside of the entity (e.g. in
     *   the extension line of a line or outside the start / end angle
     *   of an arc.
     *
     * \todo remove
     */
    //virtual RVector getVectorTo(const RLine& line, bool limited=true) const = 0;

    virtual double getDistanceTo(const RVector& point, bool limited = true) const;

    /**
     * \return The shortest distance from this entity to the given line.
     * \todo remove
     */
    //virtual double getDistanceTo(const RLine& line, bool limited = true) const {
    //    return getVectorTo(line, limited).getMagnitude();
    //}

    /**
     * \return True if this entity is completely inside the given box,
     *     false otherwise. Only geometry is accounted for, not the line width.
     */
    virtual bool isInside(const RBox& box) const {
        return box.contains(getBoundingBox());
    }

    virtual bool intersectsWith(const RShape& shape) const;

    /**
     * Moves the given reference point to the given target point or does nothing
     * if this entity has no reference point as the given location.
     *
     * \return True if a reference point has been moved successfully,
     *        false otherwise.
     */
    virtual bool moveReferencePoint(
        const RVector& referencePoint, const RVector& targetPoint
    ) = 0;

    virtual bool move(const RVector& offset);
    virtual bool rotate(double rotation, const RVector& center = RVector());
    virtual bool scale(const RVector& scaleFactors, const RVector& center = RVector());
    virtual bool scale(double scaleFactor, const RVector& center = RVector());
    virtual bool mirror(const RLine& axis);
    virtual bool mirror(const RVector& axis1, const RVector& axis2);
    virtual bool flipHorizontal();
    virtual bool flipVertical();

protected:
    RDocument* document;
    bool selectionStatus;
    int drawOrder;
    RLayer::Id layerId;
    RBlock::Id blockId;
    RLinetype::Id linetypeId;
    RLineweight::Lineweight lineweight;
    RColor color;
};

Q_DECLARE_METATYPE(REntityData*)

#endif
