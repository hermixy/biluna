#include "RLayer.h"
#include "RDocument.h"

RPropertyTypeId RLayer::PropertyName;
RPropertyTypeId RLayer::PropertyFrozen;
RPropertyTypeId RLayer::PropertyLocked;
RPropertyTypeId RLayer::PropertyColor;
RPropertyTypeId RLayer::PropertyLinetype;
RPropertyTypeId RLayer::PropertyLineweight;

RLayer::RLayer() {
}

RLayer::RLayer(RDocument* document, const QString& name,
    bool frozen, bool locked, const RColor& color,
    RLinetype::Id linetype, RLineweight::Lineweight lineweight) :
    RObject(document), name(name), frozen(frozen), locked(locked),
    color(color), linetype(linetype), lineweight(lineweight) {
}

RLayer::~RLayer() {
}

void RLayer::init() {
    RLayer::PropertyName.generateId(typeid(RLayer), "", "Name");
    RLayer::PropertyFrozen.generateId(typeid(RLayer), "", "Frozen");
    RLayer::PropertyLocked.generateId(typeid(RLayer), "", "Locked");
    RLayer::PropertyColor.generateId(typeid(RLayer), "", "Color");
    RLayer::PropertyLinetype.generateId(typeid(RLayer), "", "Linetype");
    RLayer::PropertyLineweight.generateId(typeid(RLayer), "", "Lineweight");
}

RLayer* RLayer::clone() {
    return new RLayer(*this);
}

void RLayer::setName(const QString& n) {
    if (name == "0") {
        return;
    }
    name = n;
}

bool RLayer::setProperty(RPropertyTypeId propertyTypeId,
    const QVariant& value) {

	bool ret = false;

	if (PropertyName == propertyTypeId && name == "0") {
		return false;
	}

	ret = RObject::setMember(name, value, PropertyName == propertyTypeId);
	ret = ret || RObject::setMember(frozen, value, PropertyFrozen == propertyTypeId);
	ret = ret || RObject::setMember(locked, value, PropertyLocked == propertyTypeId);
	ret = ret || RObject::setMember(color, value, PropertyColor == propertyTypeId);

	if (propertyTypeId == PropertyLinetype) {
		if (value.type() == QVariant::Int || value.type() == QVariant::LongLong) {
			ret = ret || RObject::setMember(linetype, value.toInt(), true);
        } else if (value.type() == QVariant::String) {
			if (document != NULL) {
				ret = ret || RObject::setMember(linetype,
						document->getLinetypeId(value.toString()), true);
			}
		}
	}

	ret = ret || RObject::setMember((int&) lineweight, value,
			PropertyLineweight == propertyTypeId);

	return ret;
}

QPair<QVariant, RPropertyAttributes> RLayer::getProperty(
        RPropertyTypeId propertyTypeId, bool humanReadable,
		bool noAttributes) {

	if (propertyTypeId == PropertyName) {
		return qMakePair(QVariant(name), RPropertyAttributes());
	}
	if (propertyTypeId == PropertyFrozen) {
		return qMakePair(QVariant(frozen), RPropertyAttributes());
	}
	if (propertyTypeId == PropertyLocked) {
		return qMakePair(QVariant(locked), RPropertyAttributes());
	}
	if (propertyTypeId == PropertyColor) {
		return qMakePair(QVariant(color), RPropertyAttributes());
	}

	if (propertyTypeId == PropertyLinetype) {
		if (humanReadable) {
            if (document != NULL) {
				RPropertyAttributes attr;
				if (!noAttributes) {
					attr.setChoices(document->getLinetypeNames());
				}
				return qMakePair(QVariant(document->getLinetypeName(
						linetype)), attr);
			}
		} else {
			return qMakePair(QVariant(linetype), RPropertyAttributes());
		}
	}

	if (propertyTypeId == PropertyLineweight) {
		return qMakePair(QVariant(lineweight), RPropertyAttributes());
	}

	return qMakePair(QVariant(), RPropertyAttributes());
}

bool RLayer::isSelectedForPropertyEditing(){
	return false;
}


/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, const RLayer& l) {
    dbg.nospace()
       << "RLayer("
       << (RObject&) l
       << ", name: " << l.getName()
       << ", color: " << l.getColor()
       << ", lineweight: " << l.getLineweight()
       << ", linetype: " << l.getLinetypeId()
       << ")";
	return dbg.space();
}
