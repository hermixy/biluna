#ifndef RIMPORTER_H
#define RIMPORTER_H

#include "RTransaction.h"

class RDocument;
class REntity;
class RVector;

/**
 * \brief Base class for importers. 
 *
 * Importers import entities from a source (e.g. file, DB, network, ...) 
 * into a graphics document. The implementing importer is expected to call 
 * the import functions to add objects (e.g. entities) to the document. 
 * Custom import functions may be implemented to support custom entity types 
 * supported by a specific importer.
 *
 * \ingroup core
 * \scriptable
 */
class RImporter {
public:
	RImporter(RDocument& document);
	virtual ~RImporter();

	virtual void startImport();
	virtual void endImport();

	virtual void importObject(QSharedPointer<RObject> object);

	virtual void setCurrentBlockId(RBlock::Id id);
	virtual RBlock::Id getCurrentBlockIdId();

	RDocument& getDocument();

protected:
	RDocument& document;
	RTransaction transaction;
	RBlock::Id blockId;
};

Q_DECLARE_METATYPE(RImporter*)

#endif
