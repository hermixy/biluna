#include "RSpatialIndexSimple.h"
#include "RBox.h"

RSpatialIndexSimple::RSpatialIndexSimple() {
}

RSpatialIndexSimple::~RSpatialIndexSimple() {
}

void RSpatialIndexSimple::clear() {
    si.clear();
}

void RSpatialIndexSimple::addToIndex(
    int id,
    double x1, double y1, double z1,
    double x2, double y2, double z2,
    size_t /*dataLength*/, const uint8_t* /*data*/) {
    
    si.insert(id, RBox(RVector(x1,y1,z1), RVector(x2,y2,z2)));
}

void RSpatialIndexSimple::removeFromIndex(int id) {
    /*
    QList<RBox> keys = si.keys(id);
    for (int i = 0; i < keys.size(); ++i) {
        si.remove(keys.at(i), id);
    }
    */
    si.remove(id);
}

void RSpatialIndexSimple::removeFromIndex(
        int id,
        double /*x1*/, double /*y1*/, double /*z1*/,
        double /*x2*/, double /*y2*/, double /*z2*/) {
    
    //RBox box(RVector(x1,y1,z1),RVector(x2,y2,z2));
    si.remove(id);
}

QSet<int> RSpatialIndexSimple::queryIntersected(
    double x1, double y1, double z1,
    double x2, double y2, double z2,
    RSpatialIndexVisitor* /*dataVisitor*/) {
    
    RBox box(RVector(x1,y1,z1),RVector(x2,y2,z2));
    QSet<int> res;
    QMultiMap<int, RBox>::iterator it = si.begin();
    while (it != si.end()) {
        if (box.intersects(it.value())) {
            res.insert(it.key());
        }
        ++it;
    }

    return res;
}

QSet<int> RSpatialIndexSimple::queryContained(
    double x1, double y1, double z1,
    double x2, double y2, double z2,
    RSpatialIndexVisitor* /*dataVisitor*/) {

    RBox box(RVector(x1,y1,z1),RVector(x2,y2,z2));
    QSet<int> res;
    QMultiMap<int, RBox>::iterator it = si.begin();
    while (it != si.end()) {
        if (box.contains(it.value())) {
            res.insert(it.key());
        }
        ++it;
    }

    return res;
}
