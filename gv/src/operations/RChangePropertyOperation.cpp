#include <QSet>

#include "RChangePropertyOperation.h"
#include "RDocument.h"

RChangePropertyOperation::RChangePropertyOperation(
		const RPropertyTypeId& propertyTypeId, const QVariant& value) :
	propertyTypeId(propertyTypeId), value(value) {
}

RTransaction RChangePropertyOperation::apply(RDocument& document) const {
	RTransaction transaction(document.getStorage(), "Changing property");

    QVariant val = value;

    // optimization: change layer ID instead of changing layer name:
    if (propertyTypeId==REntity::PropertyLayer && value.type() == QVariant::String) {
        val = QVariant(document.getLayerId(value.toString()));
    }

	QSet<REntity::Id> selectedEntities = document.querySelectedEntities();
	QSet<REntity::Id>::iterator it;
	for (it = selectedEntities.begin(); it != selectedEntities.end(); it++) {
		QSharedPointer<REntity> entity = document.queryEntity(*it);
		if (entity.isNull()) {
			continue;
		}

		// apply operation to entity:
        bool modified = entity->setProperty(propertyTypeId, val);

		if (modified) {
            transaction.addObject(entity, false,
                QSet<RPropertyTypeId>() << propertyTypeId);
		}
	}
	transaction.end();

	return transaction;
}

void RChangePropertyOperation::preview(RDocument& /*document*/, RExporter& /*exporter*/) const {
	// No preview for property editing
}
