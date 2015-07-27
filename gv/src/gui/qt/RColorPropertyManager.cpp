#include <QBrush>
#include "qtpropertybrowserutils_p.h"

#include "RPropertyEditor.h"
#include "RColorPropertyManager.h"
#include "RDebug.h"

RColorPropertyManager::RColorPropertyManager(QObject* parent) :
	QtAbstractPropertyManager(parent) {

}

void RColorPropertyManager::setValue(QtProperty* property, const RColor& color) {
	this->color = color;
	emit propertyChanged(property);
	emit valueChanged(property, color);
}

void RColorPropertyManager::setMixedValue(QtProperty* property) {
    setValue(property, RColor());
}

void RColorPropertyManager::initializeProperty(QtProperty* /*property*/) {

}

void RColorPropertyManager::uninitializeProperty(QtProperty* /*property*/) {

}

QIcon RColorPropertyManager::valueIcon(const QtProperty* /*property*/) const {
	if (color.isValid()) {
		return RColor::getIcon(color);
	}
	return QIcon();
}

QString RColorPropertyManager::valueText(const QtProperty* /*property*/) const {
	if(color.isValid()){
		return color.getName();
	}
	return RPropertyEditor::mixed;
}

RColor RColorPropertyManager::value(const QtProperty* /*property*/) const {
	return color;
}
