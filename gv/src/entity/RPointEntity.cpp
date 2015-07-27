#include "RPointEntity.h"
#include "RExporter.h"
#include "RPoint.h"

RPropertyTypeId RPointEntity::PropertyType;
RPropertyTypeId RPointEntity::PropertyLayer;
RPropertyTypeId RPointEntity::PropertyLinetype;
RPropertyTypeId RPointEntity::PropertyLineweight;
RPropertyTypeId RPointEntity::PropertyColor;
RPropertyTypeId RPointEntity::PropertyDrawOrder;
RPropertyTypeId RPointEntity::PropertyPositionX;
RPropertyTypeId RPointEntity::PropertyPositionY;
RPropertyTypeId RPointEntity::PropertyPositionZ;


RPointEntity::RPointEntity(RDocument* document, const RPointData& data,
		RObject::Id objectId) :
	REntity(document, objectId), data(document, data) {
}

RPointEntity::~RPointEntity() {
}

void RPointEntity::init() {
    RPointEntity::PropertyType.generateId(typeid(RPointEntity), REntity::PropertyType);
    RPointEntity::PropertyLayer.generateId(typeid(RPointEntity), REntity::PropertyLayer);
    RPointEntity::PropertyLinetype.generateId(typeid(RPointEntity), REntity::PropertyLinetype);
    RPointEntity::PropertyLineweight.generateId(typeid(RPointEntity), REntity::PropertyLineweight);
    RPointEntity::PropertyColor.generateId(typeid(RPointEntity), REntity::PropertyColor);
    RPointEntity::PropertyDrawOrder.generateId(typeid(RPointEntity), REntity::PropertyDrawOrder);
    RPointEntity::PropertyPositionX.generateId(typeid(RPointEntity), "Position", "X");
    RPointEntity::PropertyPositionY.generateId(typeid(RPointEntity), "Position", "Y");
    RPointEntity::PropertyPositionZ.generateId(typeid(RPointEntity), "Position", "Z");
}

bool RPointEntity::setProperty(RPropertyTypeId propertyTypeId,
		const QVariant& value) {
	bool ret = REntity::setProperty(propertyTypeId, value);
	ret = ret || RObject::setMember(data.position.x, value, PropertyPositionX
			== propertyTypeId);
	ret = ret || RObject::setMember(data.position.y, value, PropertyPositionY
			== propertyTypeId);
	ret = ret || RObject::setMember(data.position.z, value, PropertyPositionZ
			== propertyTypeId);
	return ret;
}

QPair<QVariant, RPropertyAttributes> RPointEntity::getProperty(
		RPropertyTypeId propertyTypeId, bool humanReadable, bool noAttributes) {
	if (propertyTypeId == PropertyType) {
		return qMakePair(QVariant(tr("Point")), RPropertyAttributes(
				RPropertyAttributes::ReadOnly));
	} else if (propertyTypeId == PropertyPositionX) {
		return qMakePair(QVariant(data.position.x), RPropertyAttributes());
	} else if (propertyTypeId == PropertyPositionY) {
		return qMakePair(QVariant(data.position.y), RPropertyAttributes());
	} else if (propertyTypeId == PropertyPositionZ) {
		return qMakePair(QVariant(data.position.z), RPropertyAttributes());
	}
	return REntity::getProperty(propertyTypeId, humanReadable, noAttributes);
}


void RPointEntity::exportEntity(RExporter& e) const {
    /*
	e.setEntityColor(getColor());
	e.setLineweight(getLineweight());
	e.setStyle(Qt::SolidLine);
    */
    e.setEntityAttributes();
    e.exportPoint(data);
}

void RPointEntity::print(QDebug dbg) const {
	dbg.nospace() << "RPointEntity(";
	REntity::print(dbg);
	dbg.nospace() << ", position: " << getPosition() << ")";
}
