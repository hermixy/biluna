#ifndef RENTITY_H
#define RENTITY_H

#include <QList>
#include <QStack>

#include "RBox.h"
#include "RDebug.h"
#include "RMatrix.h"
#include "RObject.h"
#include "REntityData.h"
#include "RLayer.h"
#include "RColor.h"
#include "RVector.h"

class RDocument;
class REntity;
class RBlockReferenceEntity;
class RExporter;


/**
 * Base class for all entity classes.
 *
 * Entities can be exported using an implementor of the exporter
 * interface by calling \ref exportEntity.
 *
 * \ingroup core
 * \scriptable
 */
class REntity : public RObject {

    Q_DECLARE_TR_FUNCTIONS(REntity)

public:
	static RPropertyTypeId PropertyType;
	static RPropertyTypeId PropertyLayer;
    static RPropertyTypeId PropertyLinetype;
    static RPropertyTypeId PropertyLineweight;
    static RPropertyTypeId PropertyColor;
    static RPropertyTypeId PropertyDrawOrder;

public:
    REntity(RDocument* document, Id objectId=-1) : RObject(document, objectId) {}
    virtual ~REntity() {}

    static void init();

    void setDocument(RDocument* document) {
        RObject::setDocument(document);
        getData().setDocument(document);
    }

    virtual REntity* clone() = 0;

    /**
     * \return Reference to the data object of the entity.
     */
    virtual REntityData& getData() = 0;

    /**
     * \return Const reference to the data object of the entity.
     */
    virtual const REntityData& getData() const = 0;

    /**
     * \copydoc REntityData::isSelected
     */
    bool isSelected() const {
        return getData().isSelected();
    }

    /**
     * \copydoc REntityData::setSelected
     */
    void setSelected(bool on) {
        getData().setSelected(on);
    }

    /**
     * \copydoc REntityData::setDrawOrder
     */
    void setDrawOrder(int drawOrder) {
        getData().setDrawOrder(drawOrder);
    }

    /**
     * \copydoc REntityData::getDrawOrder
     */
    int getDrawOrder() const {
        return getData().getDrawOrder();
    }

    /**
     * \copydoc REntityData::setLayerId
     */
    void setLayerId(RLayer::Id layerId) {
		getData().setLayerId(layerId);
	}

    /**
     * \copydoc REntityData::getLayerId
     */
    RLayer::Id getLayerId() const {
		return getData().getLayerId();
	}

    /**
     * \copydoc REntityData::setBlockId
     */
    void setBlockId(RBlock::Id blockId) {
    	getData().setBlockId(blockId);
    }

    /**
     * \copydoc REntityData::getBlockId
     */
    RBlock::Id getBlockId() const {
    	return getData().getBlockId();
    }

    /**
     * \copydoc REntityData::setLinetypeId
     */
    void setLinetypeId(RLinetype::Id linetypeId) {
        getData().setLinetypeId(linetypeId);
    }

    /**
     * \copydoc REntityData::setLinetype
     */
    void setLinetype(RLinetype linetype) {
        getData().setLinetype(linetype);
    }

    /**
     * \copydoc REntityData::getLinetypeId
     */
    RLinetype::Id getLinetypeId() const {
        return getData().getLinetypeId();
    }

    RLinetype::Id getLinetypeId(bool resolve,
        RLayer* layer, RBlockReferenceEntity* blockRef) const;

    /**
     * \copydoc REntityData::setLineweight
     */
    void setLineweight(RLineweight::Lineweight lineweight) {
        getData().setLineweight(lineweight);
    }

    /**
     * \copydoc REntityData::getLineweight
     */
    RLineweight::Lineweight getLineweight() const {
        return getData().getLineweight();
    }

    RLineweight::Lineweight getLineweight(bool resolve,
        RLayer* layer, RBlockReferenceEntity* blockRef) const;

    double getLineweightInUnits(RLayer* layer, RBlockReferenceEntity* blockRef) const;

    /**
     * \copydoc REntityData::setColor
     */
    void setColor(const RColor& color) {
    	getData().setColor(color);
    }

    /**
     * \copydoc REntityData::getColor
     */
    RColor getColor() const {
        return getData().getColor();
    }

    RColor getColor(bool resolve, RLayer* layer, RBlockReferenceEntity* blockRef);

    /**
     * \copydoc REntityData::getBoundingBox
     */
    virtual RBox getBoundingBox() const {
        return getData().getBoundingBox();
    }

    /**
     * \copydoc REntityData::getBoundingBoxes
     */
    virtual QList<RBox> getBoundingBoxes() const {
        return getData().getBoundingBoxes();
    }

    /**
     * \copydoc REntityData::isInside
     */
    virtual bool isInside(const RBox& box) const {
        return getData().isInside(box);
    }

    /**
     * \copydoc REntityData::intersectsWith
     */
    virtual bool intersectsWith(const RShape& shape) const {
        return getData().intersectsWith(shape);
    }

    /**
     * \copydoc REntityData::getDistanceTo(const RVector& point)
     */
    virtual double getDistanceTo(const RVector& point, bool limited = true) const {
        return getData().getDistanceTo(point, limited);
    }

    /**
     * \copydoc REntityData::getVectorTo
     */
    virtual RVector getVectorTo(const RVector& point, bool limited = true) const {
		return getData().getVectorTo(point, limited);
	}

    /**
     * \copydoc REntityData::getReferencePoints
     */
    virtual QList<RVector> getReferencePoints(RS::ProjectionRenderingHint hint=RS::Top) const {
        return getData().getReferencePoints(hint);
    }

    /**
     * \copydoc REntityData::getEndPoints
     */
    virtual QList<RVector> getEndPoints() const {
        return getData().getEndPoints();
    }

    /**
     * \copydoc REntityData::getMiddlePoints
     */
    virtual QList<RVector> getMiddlePoints() const {
        return getData().getMiddlePoints();
    }

    /**
     * \copydoc REntityData::getCenterPoints
     */
    virtual QList<RVector> getCenterPoints() const {
        return getData().getCenterPoints();
    }

    /**
     * \copydoc REntityData::getOnEntityPoints
     */
    virtual RVector getClosestPointOnEntity(const RVector& point) const {
        return getData().getClosestPointOnEntity(point);
    }

    /**
     * \copydoc REntityData::getPointsWithDistanceToEnd
     */
    virtual QList<RVector> getPointsWithDistanceToEnd(double distance) const {
        return getData().getPointsWithDistanceToEnd(distance);
    }

    /**
     * \copydoc REntityData::getIntersectionPoints(RDocument&)
     */
    /*
    virtual QList<RVector> getIntersectionPoints(RDocument& document) const {
        return getData().getIntersectionPoints(document);
    }
    */

    /**
     * \copydoc REntityData::getIntersectionPoints(const RShape&, bool)
     */
    virtual QList<RVector> getIntersectionPoints(const REntity& other,
        bool limited = true) const {

        return getData().getIntersectionPoints(other.getData(), limited);
    }

    /**
     * \copydoc REntityData::moveReferencePoint
     */
    virtual bool moveReferencePoint(
        const RVector& referencePoint, const RVector& targetPoint) {
        return getData().moveReferencePoint(referencePoint, targetPoint);
    }

    /**
     * \copydoc REntityData::move
     */
    virtual bool move(const RVector& offset) {
        return getData().move(offset);
    }

    /**
     * \copydoc REntityData::rotate
     */
    virtual bool rotate(double rotation, const RVector& center = RVector()) {
        return getData().rotate(rotation, center);
    }

    /**
     * \copydoc REntityData::scale
     */
    virtual bool scale(const RVector& scaleFactors,
        const RVector& center = RVector()) {

        return getData().scale(scaleFactors, center);
    }

    /**
     * \copydoc REntityData::scale
     */
    virtual bool scale(double scaleFactor,
        const RVector& center = RVector()) {

        return getData().scale(scaleFactor, center);
    }

    /**
     * \copydoc REntityData::mirror
     */
    virtual bool mirror(const RLine& axis) {
        return getData().mirror(axis);
    }

    /**
     * \copydoc REntityData::mirror
     */
    virtual bool mirror(const RVector& axis1, const RVector& axis2) {
        return getData().mirror(axis1, axis2);
    }

    /**
     * \copydoc REntityData::flipHorizontal
     */
    virtual bool flipHorizontal() {
        return getData().flipHorizontal();
    }

    /**
     * \copydoc REntityData::flipVertical
     */
    virtual bool flipVertical() {
        return getData().flipVertical();
    }

    /**
     * Exports the entity to the given exporter. The exporter might be a file
     * exporter, a graphics scene or any other platform one can export entities
     * to.
     */
    virtual void exportEntity(RExporter& e) const = 0;

    // from RObject:
    virtual bool isSelectedForPropertyEditing() {
        return isSelected();
    }

    virtual QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);

	virtual bool setProperty(RPropertyTypeId propertyTypeId,
            const QVariant& value);

protected:
	virtual void print(QDebug dbg) const;
};

Q_DECLARE_METATYPE(REntity*)
Q_DECLARE_METATYPE(QSharedPointer<REntity>)
Q_DECLARE_METATYPE(QSharedPointer<REntity>*)
Q_DECLARE_METATYPE(QStack<REntity*>)

#endif

