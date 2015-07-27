#include "RView.h"
#include "RDocument.h"

RPropertyTypeId RView::PropertyName;
RPropertyTypeId RView::PropertyCenterPoint;
RPropertyTypeId RView::PropertyWidth;
RPropertyTypeId RView::PropertyHeight;

RView::RView() {
}

RView::RView(RDocument* document, const QString& name, RVector centerPoint,
		double width, double height) :
    RObject(document), name(name), centerPoint(centerPoint), width(width), height(height) {

}


RView::~RView() {
}

void RView::init() {
    RView::PropertyName.generateId(typeid(RView), "", "Name");
    RView::PropertyCenterPoint.generateId(typeid(RView), "", "Center Point");
    RView::PropertyWidth.generateId(typeid(RView), "", "Width");
    RView::PropertyHeight.generateId(typeid(RView), "", "Height");
}

RView* RView::clone() {
    return new RView(*this);
}

bool RView::setProperty(RPropertyTypeId propertyTypeId,
    const QVariant& value) {
	bool ret = false;
	ret = RObject::setMember(name, value, PropertyName == propertyTypeId);
	ret = ret || RObject::setMember(centerPoint, value, PropertyCenterPoint == propertyTypeId);
	ret = ret || RObject::setMember(width, value, PropertyWidth == propertyTypeId);
	ret = ret || RObject::setMember(height, value, PropertyHeight == propertyTypeId);
	return ret;
}

QPair<QVariant, RPropertyAttributes> RView::getProperty(
        RPropertyTypeId propertyTypeId, bool /*humanReadable*/,
        bool /*noAttributes*/) {

	if (propertyTypeId == PropertyName) {
		return qMakePair(QVariant(name), RPropertyAttributes());
	}
	if (propertyTypeId == PropertyCenterPoint) {
		return qMakePair( QVariant::fromValue(centerPoint), RPropertyAttributes());
	}
	if (propertyTypeId == PropertyWidth) {
		return qMakePair(QVariant(width), RPropertyAttributes());
	}
	if (propertyTypeId == PropertyHeight) {
		return qMakePair(QVariant(height), RPropertyAttributes());
	}

	return qMakePair(QVariant(), RPropertyAttributes());
}

bool RView::isSelectedForPropertyEditing(){
	return false;
}


/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, const RView& v) {
	dbg.nospace() << "RView(" << (RObject&) v
			<< ", name: " << v.getName()
			<< ", centerPoint: " << v.getCenterPoint()
			<< ", width: " << v.getWidth()
			<< ", height: " << v.getHeight()
			<< ")";
	return dbg.space();
}
