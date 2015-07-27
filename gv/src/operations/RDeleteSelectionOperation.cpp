#include <QSet>

#include "RDocument.h"
#include "RDeleteSelectionOperation.h"
#include "RExporter.h"


RDeleteSelectionOperation::RDeleteSelectionOperation() {
}

RTransaction RDeleteSelectionOperation::apply(RDocument& document) const {
    RTransaction transaction(document.getStorage(), "Delete / Cut Selection");

    QSet<REntity::Id> selectedEntities = document.querySelectedEntities();
    QSet<REntity::Id>::iterator it;
    for (it=selectedEntities.begin(); it!=selectedEntities.end(); it++) {
        transaction.deleteObject(*it, &document);
    }

    transaction.end();
    return transaction;
}

void RDeleteSelectionOperation::preview(RDocument& document, RExporter& exporter) const {
}

