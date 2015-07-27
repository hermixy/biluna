#ifndef RFILEEXPORTERREGISTRY_H
#define RFILEEXPORTERREGISTRY_H

class RFileExporter;
class RDocument;

#include <QList>
#include <QMetaType>
#include <QString>
#include <QStringList>



/**
 * \brief Registry of all available file exporters (\ref RFileExporter).
 *
 * File importers must register themselves in this registry to become 
 * available to the user of the application.
 *
 * \ingroup core
 * \scriptable
 */
class RFileExporterRegistry {
public:
    typedef RFileExporter* (*FactoryFunction)(RDocument& document);
    typedef bool (*CheckFunction)(const QString&);

    /**
     * \nonscriptable
     */
    static void registerFileExporter(
        RFileExporterRegistry::FactoryFunction factoryFunction,
        RFileExporterRegistry::CheckFunction checkFunction,
        const QString& filterString
    );

    static RFileExporter* getFileExporter(
        const QString& fileName,
        RDocument& document
    );

    static QStringList getFilterStrings() {
        return filterStrings;
    }

private:
    static QList<FactoryFunction> factoryFunctions;
    static QList<CheckFunction> checkFunctions;
    static QStringList filterStrings;
};

Q_DECLARE_METATYPE(RFileExporterRegistry*)

#endif
