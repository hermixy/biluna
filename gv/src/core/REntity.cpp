#include "REntity.h"
#include "RBlockReferenceEntity.h"
#include "RDocument.h"

RPropertyTypeId REntity::PropertyType;
RPropertyTypeId REntity::PropertyLayer;
RPropertyTypeId REntity::PropertyLinetype;
RPropertyTypeId REntity::PropertyLineweight;
RPropertyTypeId REntity::PropertyColor;
RPropertyTypeId REntity::PropertyDrawOrder;

void REntity::init() {
    REntity::PropertyType.generateId(typeid(REntity), "", "Type");
    REntity::PropertyLayer.generateId(typeid(REntity), "", "Layer");
    REntity::PropertyLinetype.generateId(typeid(REntity), "Pen", "Linetype");
    REntity::PropertyLineweight.generateId(typeid(REntity), "Pen", "Lineweight");
    REntity::PropertyColor.generateId(typeid(REntity), "Pen", "Color");
    REntity::PropertyDrawOrder.generateId(typeid(REntity), "", "Draw Order");
}

/**
 * \copydoc REntityData::getLineweight
 */
RLineweight::Lineweight REntity::getLineweight(bool resolve,
    RLayer* layer, RBlockReferenceEntity* blockRef) const {

    if (this==(REntity*)blockRef) {
        return getData().getLineweight(resolve, layer, NULL);
    }

    return getData().getLineweight(resolve, layer, blockRef);
}

/**
 * \copydoc REntityData::getColor
 */
RColor REntity::getColor(bool resolve,
    RLayer* layer, RBlockReferenceEntity* blockRef) {

    if (this==(REntity*)blockRef) {
        return getData().getColor(resolve, layer, NULL);
    }

    return getData().getColor(resolve, layer, blockRef);
}

/**
 * \copydoc REntityData::getLineweightInUnits
 */
double REntity::getLineweightInUnits(RLayer* layer, RBlockReferenceEntity* blockRef) const {
    if (this==(REntity*)blockRef) {
        return getData().getLineweightInUnits(layer, NULL);
    }
    return getData().getLineweightInUnits(layer, blockRef);
}

/**
 * \copydoc REntityData::getLinetypeId
 */
RLinetype::Id REntity::getLinetypeId(bool resolve,
    RLayer* layer, RBlockReferenceEntity* blockRef) const {

    if (this==(REntity*)blockRef) {
        return getData().getLinetypeId(resolve, layer, NULL);
    }
    return getData().getLinetypeId(resolve, layer, blockRef);
}

QPair<QVariant, RPropertyAttributes> REntity::getProperty(
        RPropertyTypeId propertyTypeId,
		bool humanReadable, bool noAttributes) {

	if (propertyTypeId == PropertyType) {
        return qMakePair(QVariant(tr("Unknown")), RPropertyAttributes());
	} else if (propertyTypeId == PropertyLayer) {
        if (humanReadable) {
            RDocument* document = getData().getDocument();
			if (document != NULL) {
				RPropertyAttributes attr;
				// TODO
				if (!noAttributes) {
					attr.setChoices(document->getLayerNames());
				}
				return qMakePair(QVariant(document->getLayerName(
						getData().getLayerId())), attr);
			}
		} else {
			return qMakePair(QVariant(getData().getLayerId()),
					RPropertyAttributes());
		}
    } else if (propertyTypeId == PropertyLinetype) {
		if (humanReadable) {
            RDocument* document = getData().getDocument();
            if (document != NULL) {
				QVariant var;
				QSharedPointer<RLinetype> linetype = document->queryLinetype(
						getData().getLinetypeId());
				var.setValue<RLinetype> (*linetype.data());
				return qMakePair(var, RPropertyAttributes());
			}
		} else {
			return qMakePair(QVariant(getData().getLinetypeId()),
					RPropertyAttributes());
		}
    } else if (propertyTypeId == PropertyLineweight) {
    	QVariant v;
    	v.setValue<RLineweight::Lineweight>(getData().getLineweight());
    	return qMakePair(v, RPropertyAttributes());
    } else if (propertyTypeId == PropertyColor) {
		QVariant var;
		var.setValue<RColor> (getData().getColor());
		return qMakePair(var, RPropertyAttributes());
    } else if (propertyTypeId == PropertyDrawOrder) {
		return qMakePair(QVariant(getData().getDrawOrder()), RPropertyAttributes());
	}

	return qMakePair(QVariant(), RPropertyAttributes());
}

bool REntity::setProperty(RPropertyTypeId propertyTypeId,
    const QVariant& value) {

	bool ret = false;

	if (propertyTypeId == PropertyLayer) {
		if (value.type() == QVariant::Int || value.type() == QVariant::LongLong) {
            ret = ret || RObject::setMember(getData().layerId, value.toInt(), true);
		} else if (value.type() == QVariant::String) {
            RDocument* document = getData().getDocument();
            if (document != NULL) {
                ret = ret || RObject::setMember(getData().layerId,
						document->getLayerId(value.toString()), true);
			}
		}
    } else if (propertyTypeId == PropertyLinetype) {
        if (value.type() == QVariant::Int || value.type() == QVariant::LongLong) {
            ret = ret || RObject::setMember(getData().linetypeId, value.toInt(), true);
        } else {
            RDocument* document = getData().getDocument();
            if (document != NULL) {
				RLinetype t = value.value<RLinetype> ();
				int id = document->getLinetypeId(t.getName());
				ret = ret || RObject::setMember(getData().linetypeId, id,
						true);
			}
		}
    } else if (propertyTypeId == PropertyLineweight) {
        ret = ret || RObject::setMember((int&)getData().lineweight, 
            (int)value.value<RLineweight::Lineweight>(), true);
    } else if (propertyTypeId == PropertyColor) {
    	ret = ret || RObject::setMember(getData().color, value, true);
    } else if (propertyTypeId == PropertyDrawOrder) {
    	ret = ret || RObject::setMember(getData().drawOrder, value, true);
	}

	//	ret = ret || RObject::setProperty(data.endPoint.y, value, PropertyEndPointY
	//			== propertyTypeId);

	return ret;
}

/**
 * Stream operator for QDebug
 */
void REntity::print(QDebug dbg) const {
	dbg.nospace() << "REntity(";
	RObject::print(dbg);
	dbg.nospace() 
        << ", layerId: " << getLayerId()
        << ", blockId: " << getBlockId()
        << ", lineweight: " << getLineweight()
        << ", linetypeId: " << getLinetypeId()
        << ", color: " << getColor()
        << ", drawOrder: " << getDrawOrder()
        << ", selectionStatus: " << isSelected()
        << ")";
}
