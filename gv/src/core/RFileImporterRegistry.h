#ifndef RFILEIMPORTERREGISTRY_H
#define RFILEIMPORTERREGISTRY_H

class RFileImporter;
class RDocument;

#include <QString>
#include <QList>
#include <QMetaType>
#include <QStringList>


/**
 * \brief Registry of all available file importers (\ref RFileImporter). 
 *
 * File importers must register themselves in this registry to become 
 * available to the user of the application.
 *
 * \ingroup core
 * \scriptable
 */
class RFileImporterRegistry {
public:
    typedef RFileImporter* (*FactoryFunction)(RDocument& document);
    typedef bool (*CheckFunction)(const QString&);

    /**
     * \nonscriptable
     */
    static void registerFileImporter(
        RFileImporterRegistry::FactoryFunction factoryFunction,
        RFileImporterRegistry::CheckFunction checkFunction,
        const QString& filterString
    );

    static RFileImporter* getFileImporter(
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

Q_DECLARE_METATYPE(RFileImporterRegistry*)

#endif
