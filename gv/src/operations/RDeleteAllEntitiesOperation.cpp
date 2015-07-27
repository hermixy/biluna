#include "RDocument.h"
#include "RDeleteAllEntitiesOperation.h"

RDeleteAllEntitiesOperation::RDeleteAllEntitiesOperation(bool undoable) :
	ROperation(undoable) {
}

RTransaction RDeleteAllEntitiesOperation::apply(RDocument& document) const {
	RTransaction transaction(document.getStorage(), "Deleting object(s)", undoable);
	QSet<RObject::Id> ids =
        document.queryAllEntities();

	QSetIterator<RObject::Id> i(ids);
	while (i.hasNext()) {
		transaction.deleteObject(i.next(), &document);
	}

	transaction.end();
	return transaction;
}

void RDeleteAllEntitiesOperation::preview(RDocument& /*document*/, RExporter& /*exporter*/) const {
}
