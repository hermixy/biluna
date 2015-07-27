#ifndef RSPATIALINDEXVISITOR_H
#define RSPATIALINDEXVISITOR_H

#include <stdint.h>


/**
 * \brief Abstract base class for classes that are interested in
 * the data of spatial index items.
 *
 * \scriptable
 * \ingroup core
 */
class RSpatialIndexVisitor {
public:
    virtual ~RSpatialIndexVisitor() {}
    /**
     * \nonscriptable
     */
    virtual void visitData(
        int id,
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        size_t size, uint8_t* data
    ) = 0;

    virtual void visitNode(
        double x1, double y1, double z1,
        double x2, double y2, double z2
    ) = 0;
};

Q_DECLARE_METATYPE(RSpatialIndexVisitor*)

#endif
