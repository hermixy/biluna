#include "RBlockReferenceEntity.h"
#include "RExporter.h"
#include "RLine.h"

RPropertyTypeId RBlockReferenceEntity::PropertyType;
RPropertyTypeId RBlockReferenceEntity::PropertyLayer;
RPropertyTypeId RBlockReferenceEntity::PropertyLinetype;
RPropertyTypeId RBlockReferenceEntity::PropertyLineweight;
RPropertyTypeId RBlockReferenceEntity::PropertyColor;
RPropertyTypeId RBlockReferenceEntity::PropertyBlock;
RPropertyTypeId RBlockReferenceEntity::PropertyPositionX;
RPropertyTypeId RBlockReferenceEntity::PropertyPositionY;
RPropertyTypeId RBlockReferenceEntity::PropertyPositionZ;
RPropertyTypeId RBlockReferenceEntity::PropertyScaleX;
RPropertyTypeId RBlockReferenceEntity::PropertyScaleY;
RPropertyTypeId RBlockReferenceEntity::PropertyScaleZ;
RPropertyTypeId RBlockReferenceEntity::PropertyRotation;


RBlockReferenceEntity::RBlockReferenceEntity(RDocument* document,
    const RBlockReferenceData& data, RObject::Id objectId) :
    REntity(document, objectId), data(document, data),
    recursionDepth(0) {
//	if (data.getDocument() == NULL) {
//		return;
//	}
//	RBlock::Id id = &this->data.blockId;
//	while (data.getDocument()->getBlockName(id) != "*Model_Space") {
//		QSharedPointer<RBlock> block = data.getDocument()->queryBlock(id);
//		if (block.isNull()) {
//			break;
//		}
//		id = block->getId();
//	}
}

RBlockReferenceEntity::~RBlockReferenceEntity() {
}

void RBlockReferenceEntity::init() {
    RBlockReferenceEntity::PropertyType.generateId(
        typeid(RBlockReferenceEntity), REntity::PropertyType);
    RBlockReferenceEntity::PropertyLayer.generateId(
        typeid(RBlockReferenceEntity), REntity::PropertyLayer);
    RBlockReferenceEntity::PropertyLinetype.generateId(
        typeid(RBlockReferenceEntity), REntity::PropertyLinetype);
    RBlockReferenceEntity::PropertyLineweight.generateId(
        typeid(RBlockReferenceEntity), REntity::PropertyLineweight);
    RBlockReferenceEntity::PropertyColor.generateId(
        typeid(RBlockReferenceEntity), REntity::PropertyColor);
    RBlockReferenceEntity::PropertyBlock.generateId(
        typeid(RBlockReferenceEntity), "", "Block");
    RBlockReferenceEntity::PropertyPositionX.generateId(
        typeid(RBlockReferenceEntity), "Position", "X");
    RBlockReferenceEntity::PropertyPositionY.generateId(
        typeid(RBlockReferenceEntity), "Position", "Y");
    RBlockReferenceEntity::PropertyPositionZ.generateId(
        typeid(RBlockReferenceEntity), "Position", "Z");
    RBlockReferenceEntity::PropertyScaleX.generateId(
        typeid(RBlockReferenceEntity), "Scale", "X");
    RBlockReferenceEntity::PropertyScaleY.generateId(
        typeid(RBlockReferenceEntity), "Scale", "Y");
    RBlockReferenceEntity::PropertyScaleZ.generateId(
        typeid(RBlockReferenceEntity), "Scale", "Z");
    RBlockReferenceEntity::PropertyRotation.generateId(
        typeid(RBlockReferenceEntity), "", "Angle");
}

bool RBlockReferenceEntity::setProperty(RPropertyTypeId propertyTypeId,
        const QVariant& value) {
    bool ret = REntity::setProperty(propertyTypeId, value);
	ret = ret || RObject::setMember(data.position.x, value, PropertyPositionX
			== propertyTypeId);
	ret = ret || RObject::setMember(data.position.y, value, PropertyPositionY
			== propertyTypeId);
	ret = ret || RObject::setMember(data.position.z, value, PropertyPositionZ
			== propertyTypeId);
	ret = ret || RObject::setMember(data.scaleFactors.x, value, PropertyScaleX
			== propertyTypeId);
	ret = ret || RObject::setMember(data.scaleFactors.y, value, PropertyScaleY
			== propertyTypeId);
	ret = ret || RObject::setMember(data.scaleFactors.z, value, PropertyScaleZ
			== propertyTypeId);
	ret = ret || RObject::setMember(data.scaleFactors.z, value, PropertyScaleZ
			== propertyTypeId);
	ret = ret || RObject::setMember(data.rotation, value, PropertyRotation
			== propertyTypeId);

	if (propertyTypeId == PropertyBlock) {
        if (value.type() == QVariant::Int ||
            value.type() == QVariant::LongLong) {

            ret = ret || RObject::setMember(
                getData().referencedBlockId, value.toInt(),	true);
		} else if (value.type() == QVariant::String) {
            RDocument* document = getData().getDocument();
			if (document != NULL) {
				ret = ret || RObject::setMember(getData().referencedBlockId,
						document->getBlockId(value.toString()), true);
			}
		}
	}

	return ret;
}

QPair<QVariant, RPropertyAttributes> RBlockReferenceEntity::getProperty(
        RPropertyTypeId propertyTypeId,
        bool humanReadable,	bool noAttributes) {

	if (propertyTypeId == PropertyType) {
		return qMakePair(QVariant(tr("Block Reference")), 
            RPropertyAttributes(RPropertyAttributes::ReadOnly));
	} else if (propertyTypeId == PropertyPositionX) {
		return qMakePair(QVariant(data.position.x), RPropertyAttributes());
	} else if (propertyTypeId == PropertyPositionY) {
		return qMakePair(QVariant(data.position.y), RPropertyAttributes());
	} else if (propertyTypeId == PropertyPositionZ) {
		return qMakePair(QVariant(data.position.z), RPropertyAttributes());
	} else if (propertyTypeId == PropertyScaleX) {
		return qMakePair(QVariant(data.scaleFactors.x), RPropertyAttributes());
	} else if (propertyTypeId == PropertyScaleY) {
		return qMakePair(QVariant(data.scaleFactors.y), RPropertyAttributes());
	} else if (propertyTypeId == PropertyScaleZ) {
		return qMakePair(QVariant(data.scaleFactors.z), RPropertyAttributes());
	} else if (propertyTypeId == PropertyRotation) {
		return qMakePair(QVariant(data.rotation), 
            RPropertyAttributes(RPropertyAttributes::Angle));
	} else if (propertyTypeId == PropertyBlock) {
		if (humanReadable) {
            RDocument* document = getData().getDocument();
			if (document != NULL) {
				RPropertyAttributes attr;
				// TODO
				if (!noAttributes) {
					attr.setChoices(document->getBlockNames());
				}
				return qMakePair(QVariant(document->getBlockName(
						getData().getReferencedBlockId())), attr);
			}
		} else {
			return qMakePair(QVariant(getData().getReferencedBlockId()),
					RPropertyAttributes());
		}
	}
    return REntity::getProperty(propertyTypeId, humanReadable, noAttributes);
}

void RBlockReferenceEntity::exportEntity(RExporter& e) const {
    const RDocument* document = getDocument();
    if (document==NULL) {
        return;
    }

    recursionDepth++;

    /*
    if (recursionDepth>10) {
        recursionDepth--;
        RDebug::error("RBlockReferenceEntity::exportEntity: "
            "maximum recursion depth reached");
        return;
    }
    */

    QSet<REntity::Id> ids =
        document->queryBlockEntities(data.referencedBlockId);
	QSet<REntity::Id>::iterator it;
	for (it = ids.begin(); it != ids.end(); it++) {
        QSharedPointer<REntity> entity = data.queryEntity(*it);
        if (entity.isNull()) {
            RDebug::error("RBlockReferenceEntity::exportEntity: "
                "entity %d is NULL", *it);
            continue;
        }
        //document->getStorage().setObjectId(*entity.data(), getId());
        //entity->setId(getId());
        e.exportEntity(*entity);
        //entity->exportEntity(e);
	}

    recursionDepth--;
}

void RBlockReferenceEntity::print(QDebug dbg) const {
	dbg.nospace() << "RBlockReferenceEntity(";
	REntity::print(dbg);
    dbg.nospace() << ", position: " << getPosition();
    dbg.nospace() << ", referencedBlockId: " << getReferencedBlockId();
    dbg.nospace() << ")";
}
