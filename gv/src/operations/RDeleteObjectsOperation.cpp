#include "RDocument.h"
#include "RDeleteObjectsOperation.h"

RDeleteObjectsOperation::RDeleteObjectsOperation(bool undoable) :
	ROperation(undoable), list() {
}

RDeleteObjectsOperation::RDeleteObjectsOperation(
		QList<QSharedPointer<RObject> >& list, bool undoable) :
	ROperation(undoable), list(list) {
}

void RDeleteObjectsOperation::deleteObject(const QSharedPointer<RObject>& o) {
	list.append(o);
}

RTransaction RDeleteObjectsOperation::apply(RDocument& document) const {
	RTransaction transaction(document.getStorage(), "Deleting object(s)",
			undoable);
	for (int i = 0; i < list.size(); ++i) {
		if (list[i].isNull()) {
			RDebug::error(
					QString("RDeleteObjectsOperation::apply: list contains NULL object"));
			continue;
		}
		transaction.deleteObject(list[i]->getId(), &document);
	}
	transaction.end();
	return transaction;
}

void RDeleteObjectsOperation::preview(RDocument& /*document*/,
		RExporter& /*exporter*/) const {
}
