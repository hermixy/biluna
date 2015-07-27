#ifndef RSPATIALINDEXSIMPLE_H
#define RSPATIALINDEXSIMPLE_H

#include <QSet>
#include <QList>

#include "RSpatialIndex.h"
#include "RDebug.h"
#include "RMath.h"



/**
 * \brief Simple spatial index implementation.
 *
 * \ingroup core
 * \scriptable
 */
class RSpatialIndexSimple: public RSpatialIndex {
public:
	RSpatialIndexSimple();
	~RSpatialIndexSimple();

    virtual void clear();

	/**
	 * \nonscriptable
	 */
    virtual void addToIndex(
        int id,
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        size_t dataLength=0, const uint8_t* data=NULL
    );

    virtual void removeFromIndex(int id);
    virtual void removeFromIndex(
    		int id,
    		double x1, double y1, double z1,
			double x2, double y2, double z2);

    virtual QSet<int> queryIntersected(
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        RSpatialIndexVisitor* dataVisitor=NULL
    );

    virtual QSet<int> queryContained(
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        RSpatialIndexVisitor* dataVisitor=NULL
    );

protected:
    //QMultiHash<RBox, int> si;
    QMultiMap<int, RBox> si;
};

//QDebug operator<<(QDebug dbg, RSpatialIndexSimple& si);

Q_DECLARE_METATYPE(RSpatialIndexSimple*)

#endif
