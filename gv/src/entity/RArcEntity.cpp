#include "RArcEntity.h"
#include "RExporter.h"
#include "RPoint.h"

RPropertyTypeId RArcEntity::PropertyType;
RPropertyTypeId RArcEntity::PropertyLayer;
RPropertyTypeId RArcEntity::PropertyLinetype;
RPropertyTypeId RArcEntity::PropertyLineweight;
RPropertyTypeId RArcEntity::PropertyColor;
RPropertyTypeId RArcEntity::PropertyDrawOrder;
RPropertyTypeId RArcEntity::PropertyCenterX;
RPropertyTypeId RArcEntity::PropertyCenterY;
RPropertyTypeId RArcEntity::PropertyCenterZ;
RPropertyTypeId RArcEntity::PropertyRadius;
RPropertyTypeId RArcEntity::PropertyStartAngle;
RPropertyTypeId RArcEntity::PropertyEndAngle;
RPropertyTypeId RArcEntity::PropertyReversed;

RArcEntity::RArcEntity(RDocument* document, const RArcData& data,
		RObject::Id objectId) :
	REntity(document, objectId), data(document, data) {
}

RArcEntity::~RArcEntity() {
}

void RArcEntity::init() {
    RArcEntity::PropertyType.generateId(typeid(RArcEntity), REntity::PropertyType);
    RArcEntity::PropertyLayer.generateId(typeid(RArcEntity), REntity::PropertyLayer);
    RArcEntity::PropertyLinetype.generateId(typeid(RArcEntity), REntity::PropertyLinetype);
    RArcEntity::PropertyLineweight.generateId(typeid(RArcEntity), REntity::PropertyLineweight);
    RArcEntity::PropertyColor.generateId(typeid(RArcEntity), REntity::PropertyColor);
    RArcEntity::PropertyDrawOrder.generateId(typeid(RArcEntity), REntity::PropertyDrawOrder);
    RArcEntity::PropertyCenterX.generateId(typeid(RArcEntity), "Center", "X");
    RArcEntity::PropertyCenterY.generateId(typeid(RArcEntity), "Center", "Y");
    RArcEntity::PropertyCenterZ.generateId(typeid(RArcEntity), "Center", "Z");
    RArcEntity::PropertyRadius.generateId(typeid(RArcEntity), "", "Radius");
    RArcEntity::PropertyStartAngle.generateId(typeid(RArcEntity), "", "Start Angle");
    RArcEntity::PropertyEndAngle.generateId(typeid(RArcEntity), "", "End Angle");
    RArcEntity::PropertyReversed.generateId(typeid(RArcEntity), "", "Reversed");
}

bool RArcEntity::setProperty(RPropertyTypeId propertyTypeId,
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
	ret = ret || RObject::setMember(data.startAngle, value, PropertyStartAngle
			== propertyTypeId);
	ret = ret || RObject::setMember(data.endAngle, value, PropertyEndAngle
			== propertyTypeId);
	ret = ret || RObject::setMember(data.reversed, value, PropertyReversed
			== propertyTypeId);
	return ret;
}

QPair<QVariant, RPropertyAttributes> RArcEntity::getProperty(
		RPropertyTypeId propertyTypeId, bool humanReadable, bool noAttributes) {
	if (propertyTypeId == PropertyType) {
		return qMakePair(QVariant(tr("Arc")), RPropertyAttributes(
				RPropertyAttributes::ReadOnly));
	} else if (propertyTypeId == PropertyCenterX) {
		return qMakePair(QVariant(data.center.x), RPropertyAttributes());
	} else if (propertyTypeId == PropertyCenterY) {
		return qMakePair(QVariant(data.center.y), RPropertyAttributes());
	} else if (propertyTypeId == PropertyCenterZ) {
		return qMakePair(QVariant(data.center.z), RPropertyAttributes());
	} else if (propertyTypeId == PropertyRadius) {
		return qMakePair(QVariant(data.radius), RPropertyAttributes());
	} else if (propertyTypeId == PropertyStartAngle) {
		return qMakePair(QVariant(data.startAngle), RPropertyAttributes(
				RPropertyAttributes::Angle));
	} else if (propertyTypeId == PropertyEndAngle) {
		return qMakePair(QVariant(data.endAngle), RPropertyAttributes(
				RPropertyAttributes::Angle));
	} else if (propertyTypeId == PropertyReversed) {
		return qMakePair(QVariant(data.reversed), RPropertyAttributes());
	}

	return REntity::getProperty(propertyTypeId, humanReadable, noAttributes);
}


void RArcEntity::exportEntity(RExporter& e) const {
    /*
	e.setEntityColor(getColor());
	e.setLineweight(getLineweight());
	e.setStyle(Qt::SolidLine);
    */
    e.setEntityAttributes();
    e.exportArc(data);
}

void RArcEntity::print(QDebug dbg) const {
	dbg.nospace() << "RArcEntity(";
	REntity::print(dbg);
	dbg.nospace() << ", center: " << getCenter();
	dbg.nospace() << ", radius: " << getRadius();
	dbg.nospace() << ", startAngle: " << getStartAngle();
	dbg.nospace() << ", endAngle: " << getEndAngle();
	dbg.nospace() << ", reversed: " << isReversed() << ")";
}
