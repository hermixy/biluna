#ifndef RSPATIALINDEX_H
#define RSPATIALINDEX_H

#include <stdint.h>

#include <QSet>
#include <QList>

#include "RBox.h"
#include "RDebug.h"
#include "RMath.h"
#include "RRequireHeap.h"
#include "RSpatialIndexVisitor.h"



/**
 * \brief Abstract base class for spatial index implementations.
 *
 * \ingroup core
 * \scriptable
 */
class RSpatialIndex: public RRequireHeap {
public:
	RSpatialIndex(){
	}

	virtual ~RSpatialIndex(){
	}

    virtual void clear() = 0;

	/**
     * Adds a new entry into this spatial index.
     * 
	 * \nonscriptable
	 */
    virtual void addToIndex(
        int id,
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        size_t dataLength=0, const uint8_t* data=NULL
    ) = 0;

	/**
     * Adds a new entry into this spatial index.
     * 
	 * \nonscriptable
	 */
    virtual void addToIndex(
        int id,
        const RBox& bb,
        size_t dataLength=0, const uint8_t* data=NULL
    );

	/**
     * Adds a new entry at multiple positions into this 
     * spatial index.
     * 
	 * \nonscriptable
	 */
    virtual void addToIndex(
        int id,
        const QList<RBox>& bb,
        size_t dataLength=0, const uint8_t* data=NULL
    );

    /**
     * Removes the entry with the given ID from this spatial index.
     */
    virtual void removeFromIndex(int id) = 0;

    /**
     * Removes the entry with the given ID from this spatial index.
     * the bounding box is provided for faster lookup.
     */
    virtual void removeFromIndex(
    		int id,
    		double x1, double y1, double z1,
			double x2, double y2, double z2) = 0;

    /**
     * Removes the entry with the given ID from this spatial index.
     * the bounding box is provided for faster lookup.
     */
    virtual void removeFromIndex(int id, const RBox& bb);

    /**
     * Removes the entry with the given ID from this spatial index.
     * the bounding boxes are provided for faster lookup.
     */
    virtual void removeFromIndex(int id, const QList<RBox>& bb);

    /**
     * Queries the index for all items that are completely inside or intersect
     * with the given box x1,y1,z1,x2,y2,z2.
     *
     * \param x1 lower region boundary of the first dimension.
     * \param y1 lower region boundary of the second dimension.
     * \param z1 lower region boundary of the third dimension.
     * \param x2 higher region boundary of the first dimension.
     * \param y2 higher region boundary of the second dimension.
     * \param z2 higher region boundary of the third dimension.
     * \param dataVisitor The item data is handed back to the caller over
     *      this object (visitor pattern).
     */
    virtual QSet<int> queryIntersected(
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        RSpatialIndexVisitor* dataVisitor=NULL
    ) = 0;

    /**
     * Queries the index for all items that are completely inside or intersect
     * with the given box.
     */
    virtual QSet<int> queryIntersected(
        const RBox& b,
        RSpatialIndexVisitor* dataVisitor=NULL
    );

    /**
     * Queries the index for all items that are completely inside the given
     * box x1,y1,z1,x2,y2,z2.
     *
     * \param x1 lower region boundary of the first dimension.
     * \param y1 lower region boundary of the second dimension.
     * \param z1 lower region boundary of the third dimension.
     * \param x2 higher region boundary of the first dimension.
     * \param y2 higher region boundary of the second dimension.
     * \param z2 higher region boundary of the third dimension.
     * \param dataVisitor The item data is handed back to the caller over
     *      this object (visitor pattern).
     */
    virtual QSet<int> queryContained(
        double x1, double y1, double z1,
        double x2, double y2, double z2,
        RSpatialIndexVisitor* dataVisitor=NULL
    ) = 0;

    /**
     * Queries the index for all items that are completely inside the given
     * box.
     */
    virtual QSet<int> queryContained(
        const RBox& b,
        RSpatialIndexVisitor* dataVisitor=NULL
    );

};

QDebug operator<<(QDebug dbg, RSpatialIndex& si);

Q_DECLARE_METATYPE(RSpatialIndex*)

#endif
