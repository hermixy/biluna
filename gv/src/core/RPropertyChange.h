#ifndef RPROPERTYCHANGE_H
#define RPROPERTYCHANGE_H

#include <QVariant>

#include "RObject.h"
#include "RPropertyTypeId.h"

class RStorage;
class RExporter;

/**
 * Stores the change that was made to a property of an object.
 */
class RPropertyChange {
public:
	RPropertyChange(RPropertyTypeId propertyTypeId, QVariant oldValue,
			QVariant newValue) :
		propertyTypeId(propertyTypeId), oldValue(oldValue), newValue(newValue) {
	}

	RPropertyTypeId propertyTypeId;
	QVariant oldValue;
	QVariant newValue;
};

QDebug operator<<(QDebug dbg, RPropertyChange& p);

typedef QMultiMap<int, RPropertyChange> _RMapIntPropertyChange;
Q_DECLARE_METATYPE(_RMapIntPropertyChange*)
Q_DECLARE_METATYPE(RPropertyChange*)

#endif
