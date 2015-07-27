#include <QBrush>
#include "qtpropertybrowserutils_p.h"

#include "RPropertyEditor.h"
#include "RLinetypePropertyManager.h"
#include "RDebug.h"

RLinetypePropertyManager::RLinetypePropertyManager(QObject* parent) :
	QtAbstractPropertyManager(parent) {

}

void RLinetypePropertyManager::setValue(QtProperty* property,
		const RLinetype& linetype) {
	this->linetype = linetype;
	emit propertyChanged(property);
	emit valueChanged(property, linetype);
}

void RLinetypePropertyManager::setMixedValue(QtProperty* property) {
    setValue(property, RLinetype());
}

void RLinetypePropertyManager::initializeProperty(QtProperty* /*property*/) {

}

void RLinetypePropertyManager::uninitializeProperty(QtProperty* /*property*/) {

}

QIcon RLinetypePropertyManager::valueIcon(const QtProperty* /*property*/) const {
	if (linetype.isValid()) {
		return RLinetype::getIcon(linetype);
	}
	return QIcon();
}

QString RLinetypePropertyManager::valueText(const QtProperty* /*property*/) const {
	if (linetype.isValid()) {
		return RLinetype::getTitle(linetype);
	}
	return RPropertyEditor::mixed;
}

RLinetype RLinetypePropertyManager::value(const QtProperty* /*property*/) const {
	return linetype;
}
