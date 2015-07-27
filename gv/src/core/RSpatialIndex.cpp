#include "RSpatialIndex.h"

void RSpatialIndex::addToIndex(int id, const QList<RBox>& bb,
    size_t dataLength, const uint8_t* data) {

    for (int i = 0; i < bb.size(); ++i) {
        addToIndex(
            id,
            bb.at(i),
            dataLength, data
        );
    }
}

void RSpatialIndex::addToIndex(int id, const RBox& bb, 
    size_t dataLength, const uint8_t* data) {

    addToIndex(
        id, 
        bb.c1.x, bb.c1.y, bb.c1.z,
        bb.c2.x, bb.c2.y, bb.c2.z,
        dataLength,
        data
    );
}

void RSpatialIndex::removeFromIndex(int id, const RBox& bb) {
    removeFromIndex(
        id, 
        bb.c1.x, bb.c1.y, bb.c1.z,
        bb.c2.x, bb.c2.y, bb.c2.z
    );
}

void RSpatialIndex::removeFromIndex(int id, const QList<RBox>& bb) {
    for (int i = 0; i < bb.size(); ++i) {
        removeFromIndex(
            id, 
            bb.at(i)
        );
    }
}

QSet<int> RSpatialIndex::queryIntersected(const RBox& b, 
    RSpatialIndexVisitor* dataVisitor) {

    return queryIntersected(
        b.c1.x, b.c1.y, b.c1.z,
        b.c2.x, b.c2.y, b.c2.z,
        dataVisitor
    );
}

QSet<int> RSpatialIndex::queryContained(const RBox& b, 
    RSpatialIndexVisitor* dataVisitor){

    return queryContained(
        b.c1.x, b.c1.y, b.c1.z,
        b.c2.x, b.c2.y, b.c2.z,
        dataVisitor
    );
}

/**
 * Stream operator for QDebug
 */
QDebug operator<<(QDebug dbg, RSpatialIndex& si) {
	dbg.nospace() << "RSpatialIndex(";

	QSet<int> result = si.queryContained(
        -RMAXDOUBLE, -RMAXDOUBLE, -RMAXDOUBLE,
	    RMAXDOUBLE, RMAXDOUBLE, RMAXDOUBLE);
	QSetIterator<int> i(result);
	while (i.hasNext()) {
		dbg.nospace() << i.next() << ", ";
	}

	dbg.nospace() << ")";
	return dbg.space();
}

