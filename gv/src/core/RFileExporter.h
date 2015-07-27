#ifndef RFILEEXPORTER_H
#define RFILEEXPORTER_H

#include "RExporter.h"

#include <QString>



/**
 * \brief Base class for file exporters. 
 *
 * File exporters export \ref RDocument objects into files of a specific format.
 *
 * \ingroup core
 * \scriptable
 */
class RFileExporter : public RExporter {
public:
    RFileExporter(RDocument& document);
    virtual ~RFileExporter();
    
    /**
     * Must be implemented by file exporters to export the document
     * into the given file.
     */
    virtual bool exportFile(const QString& fileName) = 0;
};

Q_DECLARE_METATYPE(RFileExporter*)

#endif
