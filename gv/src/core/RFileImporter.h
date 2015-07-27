#ifndef RFILEIMPORTER_H
#define RFILEIMPORTER_H

#include "RImporter.h"

#include <QString>



/**
 * \brief Base class for file importers. 
 *
 * File importers import files of a specific format into an \ref RDocument.
 *
 * \ingroup core
 * \scriptable
 */
class RFileImporter : public RImporter {
public:
    RFileImporter(RDocument& document);
    virtual ~RFileImporter();
    
    /**
     * Must be implemented by file importers to import the given file into
     * the document.
     */
    virtual bool importFile(const QString& fileName) = 0;
};

Q_DECLARE_METATYPE(RFileImporter*)

#endif
