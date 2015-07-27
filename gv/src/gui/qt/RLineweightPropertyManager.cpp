#include <QBrush>
#include "qtpropertybrowserutils_p.h"

#include "RPropertyEditor.h"
#include "RLineweightPropertyManager.h"
#include "RDebug.h"

RLineweightPropertyManager::RLineweightPropertyManager(QObject* parent) :
	QtAbstractPropertyManager(parent), lw(RLineweight::WeightInvalid) {

}

void RLineweightPropertyManager::setValue(QtProperty* property,
		RLineweight::Lineweight lw) {
	this->lw = lw;
	emit propertyChanged(property);
	emit valueChanged(property, lw);
}

void RLineweightPropertyManager::setMixedValue(QtProperty* property) {
    setValue(property, RLineweight::WeightInvalid);
}

void RLineweightPropertyManager::initializeProperty(QtProperty* /*property*/) {

}

void RLineweightPropertyManager::uninitializeProperty(QtProperty* /*property*/) {

}

QIcon RLineweightPropertyManager::valueIcon(const QtProperty* /*property*/) const {
	if (lw == RLineweight::WeightInvalid) {
		return QIcon();
	}
	return RLineweight::getIcon(lw);
}

QString RLineweightPropertyManager::valueText(const QtProperty* /*property*/) const {
	if (lw != RLineweight::WeightInvalid) {
		return RLineweight::getName(lw);
	}
	return RPropertyEditor::mixed;
}

RLineweight::Lineweight RLineweightPropertyManager::value(const QtProperty* /*property*/) const {
	return lw;
}
