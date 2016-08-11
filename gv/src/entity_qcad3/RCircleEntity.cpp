#include "RCircleEntity.h"
#include "RExporter.h"
#include "RPoint.h"

RPropertyTypeId RCircleEntity::PropertyType;
RPropertyTypeId RCircleEntity::PropertyLayer;
RPropertyTypeId RCircleEntity::PropertyLinetype;
RPropertyTypeId RCircleEntity::PropertyLineweight;
RPropertyTypeId RCircleEntity::PropertyColor;
RPropertyTypeId RCircleEntity::PropertyDrawOrder;
RPropertyTypeId RCircleEntity::PropertyCenterX;
RPropertyTypeId RCircleEntity::PropertyCenterY;
RPropertyTypeId RCircleEntity::PropertyCenterZ;
RPropertyTypeId RCircleEntity::PropertyRadius;



RCircleEntity::RCircleEntity(RDocument* document, const RCircleData& data,
		RObject::Id objectId) :
	REntity(document, objectId), data(document, data) {
}

RCircleEntity::~RCircleEntity() {
}

void RCircleEntity::init() {
    RCircleEntity::PropertyType.generateId(typeid(RCircleEntity), REntity::PropertyType);
    RCircleEntity::PropertyLayer.generateId(typeid(RCircleEntity), REntity::PropertyLayer);
    RCircleEntity::PropertyLinetype.generateId(typeid(RCircleEntity), REntity::PropertyLinetype);
    RCircleEntity::PropertyLineweight.generateId(typeid(RCircleEntity), REntity::PropertyLineweight);
    RCircleEntity::PropertyColor.generateId(typeid(RCircleEntity), REntity::PropertyColor);
    RCircleEntity::PropertyDrawOrder.generateId(typeid(RCircleEntity), REntity::PropertyDrawOrder);
    RCircleEntity::PropertyCenterX.generateId(typeid(RCircleEntity), "Center", "X");
    RCircleEntity::PropertyCenterY.generateId(typeid(RCircleEntity), "Center", "Y");
    RCircleEntity::PropertyCenterZ.generateId(typeid(RCircleEntity), "Center", "Z");
    RCircleEntity::PropertyRadius.generateId(typeid(RCircleEntity), "", "Radius");
}

bool RCircleEntity::setProperty(RPropertyTypeId propertyTypeId,
		const QVariant& value) {
	bool ret = REntity::setProperty(propertyTypeId, value);
	ret = ret || RObject::setMember(data.center.x, value, PropertyCenterX
			== propertyTypeId);
	ret = ret || RObject::setMember(data.center.y, value, PropertyCenterY
			== propertyTypeId);
	ret = ret || RObject::setMember(data.center.z, value, PropertyCenterZ
			== propertyTypeId);
	ret = ret || RObject::setMember(data.radius, value, PropertyRadius
			== propertyTypeId);
	return ret;
}

QPair<QVariant, RPropertyAttributes> RCircleEntity::getProperty(
		RPropertyTypeId propertyTypeId, bool humanReadable, bool noAttributes) {
	if (propertyTypeId == PropertyType) {
		return qMakePair(QVariant(tr("Circle")), RPropertyAttributes(
				RPropertyAttributes::ReadOnly));
	} else if (propertyTypeId == PropertyCenterX) {
		return qMakePair(QVariant(data.center.x), RPropertyAttributes());
	} else if (propertyTypeId == PropertyCenterY) {
		return qMakePair(QVariant(data.center.y), RPropertyAttributes());
	} else if (propertyTypeId == PropertyCenterZ) {
		return qMakePair(QVariant(data.center.z), RPropertyAttributes());
	} else if (propertyTypeId == PropertyRadius) {
		return qMakePair(QVariant(data.radius), RPropertyAttributes());
	}
	return REntity::getProperty(propertyTypeId, humanReadable, noAttributes);
}


void RCircleEntity::exportEntity(RExporter& e) const {
    /*
	e.setEntityColor(getColor());
	e.setLineweight(getLineweight());
	e.setStyle(Qt::SolidLine);
    */
    e.setEntityAttributes();
    e.exportCircle(data);
}

void RCircleEntity::print(QDebug dbg) const {
	dbg.nospace() << "RCircleEntity(";
	REntity::print(dbg);
	dbg.nospace() << ", center: " << getCenter();
//	dbg.nospace() << ", radius: " << getRadius() << ")";
}
