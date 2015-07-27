#include "RFileImporterRegistry.h"

#include "RDebug.h"
    
    
QList<RFileImporterRegistry::FactoryFunction> RFileImporterRegistry::factoryFunctions;
QList<RFileImporterRegistry::CheckFunction> RFileImporterRegistry::checkFunctions;
QStringList RFileImporterRegistry::filterStrings;



/**
 * Registers a file importer.
 *
 * \param factoryFunction The function that can be used to instantiate
 *      the importer.
 * \param checkFunction The function tht can be used to check whether
 *      the importer can import a given file.
 */
void RFileImporterRegistry::registerFileImporter(
    FactoryFunction factoryFunction,
    CheckFunction checkFunction,
    const QString& filterString) {

    factoryFunctions.push_back(factoryFunction);
    checkFunctions.push_back(checkFunction);
    filterStrings.push_back(filterString);
}



/**
 * \return A new instance of the first file importer found that
 *      can handle the given file or NULL if no suitable importer 
 *      can be found.
 */
RFileImporter* RFileImporterRegistry::getFileImporter(const QString& fileName,
		RDocument& document) {

	QList<CheckFunction>::iterator itc;
	QList<FactoryFunction>::iterator itf;

	for (itc = checkFunctions.begin(), itf = factoryFunctions.begin(); itc
			!= checkFunctions.end(), itf != factoryFunctions.end(); ++itc, ++itf) {
		bool suitable = (*itc)(fileName);
		if (suitable) {
			return (*itf)(document);
		}
	}
	RDebug::warning("RFileImporterRegistry::getFileImporter: "
		"NO suitable importer found");

	return NULL;
}

