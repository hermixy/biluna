#include "RBlock.h"
#include "RDocument.h"

RPropertyTypeId RBlock::PropertyName;

RBlock::RBlock() :
	frozen(false) {
}

RBlock::RBlock(RDocument* document, const QString& name,
    const RVector& origin) :
    RObject(document), name(name), frozen(false), origin(origin) {
}

RBlock::~RBlock() {
}

void RBlock::init() {
	RBlock::PropertyName.generateId(typeid(RBlock), "", "Name");
}

RBlock* RBlock::clone() {
	return new RBlock(*this);
}

void RBlock::setName(const QString& n) {
	name = n;
}

bool RBlock::setProperty(RPropertyTypeId propertyTypeId,
    const QVariant& value) {

	bool ret = false;

	ret = RObject::setMember(name, value, PropertyName == propertyTypeId);
	//	ret = ret || RObject::setProperty(frozen, value, PropertyFrozen == propertyTypeId);

	return ret;
}

QPair<QVariant, RPropertyAttributes> RBlock::getProperty(
        RPropertyTypeId propertyTypeId,
        bool /*humanReadable*/, bool /*noAttributes*/) {

	if (propertyTypeId == PropertyName) {
		return qMakePair(QVariant(name), RPropertyAttributes());
	}

	return qMakePair(QVariant(), RPropertyAttributes());
}

bool RBlock::isSelectedForPropertyEditing() {
	return false;
}

void RBlock::print(QDebug dbg) const {
	dbg.nospace() << "RBlock(";
	RObject::print(dbg);
	dbg.nospace() << ", name: " << getName()
			<< ", origin: " << getOrigin()
			<< ")";
}
