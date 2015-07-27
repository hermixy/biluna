#include "RSpatialIndexNavel.h"
#include "RDebug.h"


#ifdef _MSC_VER
#define RSIMAXDOUBLE 1e10
#define RSIMINDOUBLE -1e10
#else
#define RSIMAXDOUBLE std::numeric_limits<double>::max()
#define RSIMINDOUBLE -std::numeric_limits<double>::max()
#endif



/**
 * Internal.
 */
RSpatialIndexNavel::RSiRegion::RSiRegion(
    double x1, double y1, double z1,
    double x2, double y2, double z2) {

    double p1[] = { 
        std::min(x1, x2), 
        std::min(y1, y2), 
        std::min(z1, z2) 
    };
    double p2[] = { 
        std::max(x1, x2), 
        std::max(y1, y2), 
        std::max(z1, z2) 
    };

    (*(SpatialIndex::Region*)this) = SpatialIndex::Region(p1, p2, 3);
}



/**
 * Creates a 2d spatial index in memory (r-tree, star shape).
 */
RSpatialIndexNavel::RSpatialIndexNavel() {
    init();
}



RSpatialIndexNavel::~RSpatialIndexNavel() {
    uninit();
}


void RSpatialIndexNavel::init() {
    buff = SpatialIndex::StorageManager::createNewMemoryStorageManager();

    SpatialIndex::id_type indexIdentifier;
    int dimension = 3;
    double minimumLoad = 0.1;
    size_t indexCapacity = 100;
    size_t leafCapacity = 100;
    tree = SpatialIndex::RTree::createNewRTree(
        *buff,
        minimumLoad,
        indexCapacity,
        leafCapacity,
        dimension,
        SpatialIndex::RTree::RV_RSTAR,
        indexIdentifier
    );
}



void RSpatialIndexNavel::uninit() {
    delete tree;
    delete buff;
    tree = NULL;
    buff = NULL;
}


void RSpatialIndexNavel::clear() {
    uninit();
    init();
}
    
    
/**
 * Adds an item to the index.
 *
 * \param id ID of the item.
 * \param x1 lower value of the first dimension.
 * \param y1 lower value of the second dimension.
 * \param z1 lower value of the third dimension.
 * \param x2 higher value of the first dimension.
 * \param y2 higher value of the second dimension.
 * \param z2 higher value of the third dimension.
 * \param dataLength Legth of the data in bytes or 0.
 * \param data Pointer to the data associated with the item. The
 *      index makes a deep copy of the data, the caller is responsible 
 *      for deleting the data. NULL indicates that the item data is stored
 *      externally (e.g. in a map or list).
 */
void RSpatialIndexNavel::addToIndex(
    int id,
    double x1, double y1, double z1,
    double x2, double y2, double z2,
    size_t dataLength, const uint8_t* data) {

    addToIndex(
        id, 
        RSpatialIndexNavel::RSiRegion(x1, y1, z1, x2, y2, z2), 
        dataLength, 
        data
    );
}



/**
 * Internal.
 */
void RSpatialIndexNavel::addToIndex(
    int id,
    const RSpatialIndexNavel::RSiRegion& region,
    size_t dataLength, const uint8_t* data) {

    tree->insertData(dataLength, data, region, id);
}



void RSpatialIndexNavel::removeFromIndex(int id, double x1, double y1, double z1,
		double x2, double y2, double z2) {

	double p1[] = { x1, y1, z1 };
	double p2[] = { x2, y2, z2 };
	SpatialIndex::Region r(p1, p2, 3);
	if(!tree->deleteData(r, id)){
        //RDebug::printBacktrace();
		RDebug::warning(QString(
            "RSpatialIndexNavel::removeFromIndex: "
            "entry not found, id: %1, %2,%3,%4 / %5,%6,%7")
            .arg(id)
            .arg(x1)
            .arg(y1)
            .arg(z1)
            .arg(x2)
            .arg(y2)
            .arg(z2)
        );
	}
}

/**
 *  * \todo refactoring
 *
 * Removes the item with the given ID from this spatial index.
 */
void RSpatialIndexNavel::removeFromIndex(int id) {
    RSiGetRegionVisitor v(id);
    QSet<int> res =
        queryIntersected(
            RSIMINDOUBLE, RSIMINDOUBLE, RSIMINDOUBLE,
            RSIMAXDOUBLE, RSIMAXDOUBLE, RSIMAXDOUBLE,
            &v
        );

    if (v.found) {
        tree->deleteData(v.region, id);
    }
}



QSet<int> RSpatialIndexNavel::queryIntersected(
    double x1, double y1, double z1,
    double x2, double y2, double z2,
    RSpatialIndexVisitor* dataVisitor) {

    return queryIntersected(
        RSpatialIndexNavel::RSiRegion(x1,y1,z1, x2,y2,z2), 
        dataVisitor
    );
}



/**
 * Internal.
 */
QSet<int> RSpatialIndexNavel::queryIntersected(
    const RSpatialIndexNavel::RSiRegion& region, 
    RSpatialIndexVisitor* dataVisitor) {

    QSet<int> result;
    RSpatialIndexNavel::Visitor visitor(result, dataVisitor);
    tree->intersectsWithQuery(region, visitor);
    return result;
}



QSet<int> RSpatialIndexNavel::queryContained(
    double x1, double y1, double z1, 
    double x2, double y2, double z2,
    RSpatialIndexVisitor* dataVisitor) {

    return queryContained(
        RSpatialIndexNavel::RSiRegion(x1, y1, z1, x2, y2, z2), 
        dataVisitor
    );
}



/**
 * Internal.
 */
QSet<int> RSpatialIndexNavel::queryContained(
    const RSpatialIndexNavel::RSiRegion& region, 
    RSpatialIndexVisitor* dataVisitor) {

    QSet<int> result;
    RSpatialIndexNavel::Visitor visitor(result, dataVisitor);
    tree->containsWhatQuery(region, visitor);
    return result;
}
