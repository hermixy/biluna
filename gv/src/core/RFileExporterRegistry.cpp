#include "RFileExporterRegistry.h"

#include "RDebug.h"
    
    
QList<RFileExporterRegistry::FactoryFunction> RFileExporterRegistry::factoryFunctions;
QList<RFileExporterRegistry::CheckFunction> RFileExporterRegistry::checkFunctions;
QStringList RFileExporterRegistry::filterStrings;



/**
 * Registers a file exporter.
 *
 * \param factoryFunction The function that can be used to instantiate
 *      the importer.
 * \param checkFunction The function that can be used to check whether
 *      the importer can import a given file.
 */
void RFileExporterRegistry::registerFileExporter(
    FactoryFunction factoryFunction,
    CheckFunction checkFunction,
    const QString& filterString) {

    factoryFunctions.push_back(factoryFunction);
    checkFunctions.push_back(checkFunction);
    filterStrings.push_back(filterString);
}



/**
 * \return A new instance of the first file exporter found that
 *      can handle the given file or NULL if no suitable exporter
 *      can be found.
 */
RFileExporter* RFileExporterRegistry::getFileExporter(const QString& fileName,
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
	RDebug::warning("RFileExporterRegistry::getFileExporter: "
		"No suitable exporter found");

	return NULL;
}

