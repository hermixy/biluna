#ifndef RSPATIALINDEXVISITORADAPTER_H
#define RSPATIALINDEXVISITORADAPTER_H

#include "RDebug.h"
#include "RSpatialIndexVisitor.h"


/**
 * \brief Base class for classes that are interested in
 * the data of spatial index items. Not abstract, so it
 * can be used in script environments with prototype based
 * OOP as base class.
 *
 * \ingroup core
 * \scriptable
 * \generateScriptShell
 */
class RSpatialIndexVisitorAdapter : public RSpatialIndexVisitor {
public:
    virtual ~RSpatialIndexVisitorAdapter() {}

    virtual void visitData(
        int id,
        double x1, double y1, double z1,
        double x2, double y2, double z2
    ) {
    }

    /**
     * \nonscriptable
     */
    virtual void visitData(
        int id,
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        size_t size, uint8_t* data
    ) {
        visitData(id, x1,y1,z1, x2,y2,z2);
    }


    virtual void visitNode(
        double x1, double y1, double z1,
        double x2, double y2, double z2
    ) {
    }
};

Q_DECLARE_METATYPE(RSpatialIndexVisitorAdapter*)

#endif
