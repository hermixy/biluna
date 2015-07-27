#include "RObject.h"
#include "RPropertyTypeId.h"

const RObject::Id RObject::INVALID_ID = -1;


/**
 * Sets the given property.
 * \param variable a reference to the (member) variable we want to set
 * \param value the new value
 * \param condition the variable is only set to the new value if condition is true
 * \retval true: if condition is true and the value was successfully converted
 * to the same type as the variable.
 */
bool RObject::setMember(QString& variable, const QVariant& value,
                        bool condition) {
    if (!condition) {
            return false;
    }
    variable = value.toString();
    return true;
}

/**
 * \overload
 */
bool RObject::setMember(double& variable, const QVariant& value,
                        bool condition) {
    if (!condition) {
        return false;
    }
    bool ok;
    double d = value.toDouble(&ok);
    if (!ok) {
        RDebug::warning(
                        QString("RObject::setProperty: '%1' is not a double").arg(
                                        value.toString()));
        return false;
    }
    variable = d;
    return true;
}

/**
 * \overload
 */
bool RObject::setMember(int& variable, const QVariant& value,
		bool condition) {
	if (!condition) {
		return false;
	}
	bool ok;
	int n = value.toInt(&ok);
	if (!ok) {
		RDebug::warning(
				QString("RObject::setProperty: '%1' is not an integer").arg(
						value.toString()));
		return false;
	}
	variable = n;
	return true;
}

/**
 * \overload
 */
bool RObject::setMember(bool& variable, const QVariant& value,
		bool condition) {
	if (!condition) {
		return false;
	}
	variable = value.toBool();
	return true;
}

/**
 * \return A set of all property IDs of properties that are available for this
 *      object.
 *      The IDs that are returned can be translated into a group title and
 *      a property title using \ref getPropertyGroupTitle and \ref getPropertyTitle.
 */
QSet<RPropertyTypeId> RObject::getPropertyTypeIds() {
	return RPropertyTypeId::getPropertyTypeIds(typeid(*this));
}

/**
 * Stream operator for QDebug
 */
void RObject::print(QDebug dbg) const {
	dbg.nospace() << "RObject("
			<< "id: " << getId()
			<< ", address: "
            //<< QString("0x%1").arg((int) this, 0, 16)
			<< ", undone: " << (int)isUndone()
			<< ")";
}
