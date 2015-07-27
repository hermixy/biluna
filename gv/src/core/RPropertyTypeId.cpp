#include "RPropertyTypeId.h"
#include "RObject.h"

long int RPropertyTypeId::counter = 0;
long int RPropertyTypeId::invalid = -1;

QMap<QString, QSet<RPropertyTypeId> >
		RPropertyTypeId::propertyTypeByObjectMap;
QMap<long int, QPair<QString, QString> > RPropertyTypeId::titleMap;

/**
 * Creates a new unique property type id. Property type IDs are unique
 * across class boundaries (for application scope).
 * The IDs are dynamically generated and only guaranteed to remain
 * the same during one application session. These IDs should never
 * appears in a file format.
 *
 * \param classInfo class info object for the class to which this
 * property belongs, e.g. typeid(ClassName) or typeid(objectName).
 * This is used to remember to which class a property belongs.
 */
/*
RPropertyTypeId::RPropertyTypeId(const std::type_info& classInfo,
		const QString& groupTitle, const QString& title) {
	id = counter++;
	propertyTypeByObjectMap[classInfo.name()].insert(*this);
	titleMap[id].first = groupTitle;
	titleMap[id].second = title;
}
*/

RPropertyTypeId::RPropertyTypeId(const RPropertyTypeId& other){
	id = other.id;
}

RPropertyTypeId::RPropertyTypeId(long int id){
	this->id = id;
}

/*
RPropertyTypeId::RPropertyTypeId(const std::type_info& classInfo,
		const RPropertyTypeId& other) {
	id = other.id;
	propertyTypeByObjectMap[classInfo.name()].insert(*this);
}
*/

void RPropertyTypeId::generateId(const std::type_info& classInfo,
		const QString& groupTitle, const QString& title) {
    if (id!=-1) {
        RDebug::error("RPropertyTypeId::generateId: property already initialized");
        return;
    }

    if(getPropertyTypeId(groupTitle, title).isValid()){
        generateId(classInfo, getPropertyTypeId(groupTitle, title));
        return;
    }

	id = counter++;
	propertyTypeByObjectMap[classInfo.name()].insert(*this);
	titleMap[id].first = groupTitle;
	titleMap[id].second = title;
}

void RPropertyTypeId::generateId(const std::type_info& classInfo,
		const RPropertyTypeId& other) {
    if (id!=-1) {
        RDebug::error("RPropertyTypeId::generateId: property already initialized");
        return;
    }

	id = other.id;
	propertyTypeByObjectMap[classInfo.name()].insert(*this);
}

RPropertyTypeId RPropertyTypeId::getPropertyTypeId(const QString& groupTitle,
		const QString& title) {
	QMap<long int, QPair<QString, QString> >::iterator i;
	for (i = titleMap.begin(); i != titleMap.end(); ++i) {
		if (i.value().first == groupTitle && i.value().second == title) {
			return RPropertyTypeId(i.key());
		}
	}
	return RPropertyTypeId(-1);
}

/**
 * \return The property group title of the given property.
 */
QString RPropertyTypeId::getPropertyGroupTitle() const {
	return titleMap[id].first;
}

/**
 * \return The property title of the given property.
 */

QString RPropertyTypeId::getPropertyTitle() const {
	return titleMap[id].second;
}

bool RPropertyTypeId::isValid(){
    return id != invalid;
}

/**
 * \return The internal ID of this property.
 */
long int RPropertyTypeId::getId() const {
	return id;
}

/**
 * \retval true: if the given class has the given property
 */
bool RPropertyTypeId::hasPropertyType(const std::type_info& classInfo,
		RPropertyTypeId propertyTypeId) {
	if (!propertyTypeByObjectMap.contains(classInfo.name())) {
		return false;
	}
	return propertyTypeByObjectMap[classInfo.name()].contains(propertyTypeId);
}

/**
 * Gets all property type IDs that where registered for the given
 * class.
 *
 * \param classInfo The class info, e.g. typeid(ObjectA)
 *
 * \return a set of property type ID pointers or an empty set if
 * the class has no registered property types.
 */
QSet<RPropertyTypeId> RPropertyTypeId::getPropertyTypeIds(
		const std::type_info& classInfo) {
	if (propertyTypeByObjectMap.contains(classInfo.name())) {
		return propertyTypeByObjectMap[classInfo.name()];
	}
	RDebug::error(
			QString(
					"RPropertyIdRegistry::getPropertyTypeIds: no properties registered for class %1") .arg(
					classInfo.name()));
	return QSet<RPropertyTypeId> ();
}


/**
 * == operator
 */
bool RPropertyTypeId::operator ==(const RPropertyTypeId& other) const {
	return id == other.id;
}

/**
 * < operator
 */
bool RPropertyTypeId::operator <(const RPropertyTypeId& other) const {
	return id < other.id;
}

uint qHash(RPropertyTypeId propertyTypeId) {
	return qHash(propertyTypeId.getId());
}
