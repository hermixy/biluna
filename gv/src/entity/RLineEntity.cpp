#include "RLineEntity.h"
#include "RExporter.h"
#include "RLine.h"

RPropertyTypeId RLineEntity::PropertyType;
RPropertyTypeId RLineEntity::PropertyLayer;
RPropertyTypeId RLineEntity::PropertyLinetype;
RPropertyTypeId RLineEntity::PropertyLineweight;
RPropertyTypeId RLineEntity::PropertyColor;
RPropertyTypeId RLineEntity::PropertyDrawOrder;
RPropertyTypeId RLineEntity::PropertyStartPointX;
RPropertyTypeId RLineEntity::PropertyStartPointY;
RPropertyTypeId RLineEntity::PropertyStartPointZ;
RPropertyTypeId RLineEntity::PropertyEndPointX;
RPropertyTypeId RLineEntity::PropertyEndPointY;
RPropertyTypeId RLineEntity::PropertyEndPointZ;


RLineEntity::RLineEntity(RDocument* document, const RLineData& data,
		RObject::Id objectId) :
    REntity(document, objectId), data(document, data) {
}

RLineEntity::~RLineEntity() {
}

void RLineEntity::init() {
    RLineEntity::PropertyType.generateId(typeid(RLineEntity), REntity::PropertyType);
    RLineEntity::PropertyLayer.generateId(typeid(RLineEntity), REntity::PropertyLayer);
    RLineEntity::PropertyLinetype.generateId(typeid(RLineEntity), REntity::PropertyLinetype);
    RLineEntity::PropertyLineweight.generateId(typeid(RLineEntity), REntity::PropertyLineweight);
    RLineEntity::PropertyColor.generateId(typeid(RLineEntity), REntity::PropertyColor);
    RLineEntity::PropertyDrawOrder.generateId(typeid(RLineEntity), REntity::PropertyDrawOrder);
    RLineEntity::PropertyStartPointX.generateId(typeid(RLineEntity), "Start Point", "X");
    RLineEntity::PropertyStartPointY.generateId(typeid(RLineEntity), "Start Point", "Y");
    RLineEntity::PropertyStartPointZ.generateId(typeid(RLineEntity), "Start Point", "Z");
    RLineEntity::PropertyEndPointX.generateId(typeid(RLineEntity), "End Point", "X");
    RLineEntity::PropertyEndPointY.generateId(typeid(RLineEntity), "End Point", "Y");
    RLineEntity::PropertyEndPointZ.generateId(typeid(RLineEntity), "End Point", "Z");
}

bool RLineEntity::setProperty(RPropertyTypeId propertyTypeId,
        const QVariant& value) {
    bool ret = REntity::setProperty(propertyTypeId, value);
    ret = ret || RObject::setMember(data.startPoint.x, value, PropertyStartPointX
			== propertyTypeId);
    ret = ret || RObject::setMember(data.startPoint.y, value, PropertyStartPointY
			== propertyTypeId);
    ret = ret || RObject::setMember(data.startPoint.z, value, PropertyStartPointZ
			== propertyTypeId);
    ret = ret || RObject::setMember(data.endPoint.x, value, PropertyEndPointX
			== propertyTypeId);
    ret = ret || RObject::setMember(data.endPoint.y, value, PropertyEndPointY
			== propertyTypeId);
    ret = ret || RObject::setMember(data.endPoint.z, value, PropertyEndPointZ
			== propertyTypeId);
	return ret;
}

QPair<QVariant, RPropertyAttributes> RLineEntity::getProperty(
        RPropertyTypeId propertyTypeId, bool humanReadable,
		bool noAttributes) {
	if (propertyTypeId == PropertyType) {
		return qMakePair(QVariant(tr("Line")), 
            RPropertyAttributes(RPropertyAttributes::ReadOnly));
	} else if (propertyTypeId == PropertyStartPointX) {
		return qMakePair(QVariant(data.startPoint.x), RPropertyAttributes());
	} else if (propertyTypeId == PropertyStartPointY) {
		return qMakePair(QVariant(data.startPoint.y), RPropertyAttributes());
	} else if (propertyTypeId == PropertyStartPointZ) {
		return qMakePair(QVariant(data.startPoint.z), RPropertyAttributes());
	} else if (propertyTypeId == PropertyEndPointX) {
		return qMakePair(QVariant(data.endPoint.x), RPropertyAttributes());
	} else if (propertyTypeId == PropertyEndPointY) {
		return qMakePair(QVariant(data.endPoint.y), RPropertyAttributes());
	} else if (propertyTypeId == PropertyEndPointZ) {
		return qMakePair(QVariant(data.endPoint.z), RPropertyAttributes());
	}
    return REntity::getProperty(propertyTypeId, humanReadable, noAttributes);
}


void RLineEntity::exportEntity(RExporter& e) const {
    //e.setEntityColor(getColor(true, e.getEntityStack()));
	// TODO scale according to unit
    //e.setLineweight(getLineweight());
    e.setEntityAttributes();
    e.exportLine(data);
}

void RLineEntity::print(QDebug dbg) const {
	dbg.nospace() << "RLineEntity(";
	REntity::print(dbg);
	dbg.nospace() << ", startPoint: " << getStartPoint() << ", endPoint: "
			<< getEndPoint() << ")";
}
