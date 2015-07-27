#include "RImporter.h"

#include "RDocument.h"
#include "REntity.h"
#include "RObject.h"
//#include "RLineEntity.h"


RImporter::RImporter(RDocument& document) :
	document(document), transaction(document.getStorage(), "Importing", false) {
}

RImporter::~RImporter() {
}

/**
 * Starts the import. Reimplementations should call this base
 * implementation first since this starts a transaction.
 */
void RImporter::startImport() {
}

/**
 * Imports an entity into the document.
 */
void RImporter::importObject(QSharedPointer<RObject> object) {
    transaction.addObject(object, false);
}

/**
 * Ends the import. Reimplementations should call this base
 * implementation first since this commits the transaction
 * after importing entities into the document.
 */
void RImporter::endImport() {
	transaction.end();
	document.rebuildSpatialIndex();
}

void RImporter::setCurrentBlockId(RBlock::Id id) {
	this->blockId = id;
}

RBlock::Id RImporter::getCurrentBlockIdId() {
	return blockId;
}

RDocument& RImporter::getDocument(){
	return document;
}
