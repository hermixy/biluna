#ifndef RPROPERTYEVENT_H
#define RPROPERTYEVENT_H

#include <QVariant>

#include "RPropertyTypeId.h"
#include "RPropertyAttributes.h"



/**
 * \brief A property change event is fired whenever a user interface
 * component was used to change the value of a property.
 *
 * \ingroup core
 * \scriptable
 * \copyable
 */
class RPropertyEvent {
public:
	RPropertyEvent() :
		propertyTypeId(-1) {
	}

    /**
     * \param propertyTypeId ID of the property that was changed.
     * \param value New value of the property.
     */
    RPropertyEvent(RPropertyTypeId propertyTypeId, const QVariant& value)
        : propertyTypeId(propertyTypeId), value(value) {
    }

    /**
     * \return The position of the event in real graphic measures.
     */
    RPropertyTypeId getPropertyTypeId() const {
        return propertyTypeId;
    }

    /**
     * \return true If the position of the coordinate event is valid.
     */
    QVariant getValue() const {
        return value;
    }

private:
    RPropertyTypeId propertyTypeId;
    QVariant value;
};

Q_DECLARE_METATYPE(RPropertyEvent)
Q_DECLARE_METATYPE(RPropertyEvent*)
Q_DECLARE_METATYPE(const RPropertyEvent*)

#endif
