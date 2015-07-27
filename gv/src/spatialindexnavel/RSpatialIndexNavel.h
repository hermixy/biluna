#ifndef RSPATIALINDEXNAVEL_H
#define RSPATIALINDEXNAVEL_H

#include <QSet>
#include <QList>

#include "RSpatialIndex.h"

#include "spatialindex/include/SpatialIndex.h"

/**
 * \ingroup spatialindex
 */
class RSiGetRegionVisitor: public RSpatialIndexVisitor {
public:
	RSiGetRegionVisitor(int id) :
		id(id), found(false) {
	}
	virtual ~RSiGetRegionVisitor() {
	}
	virtual void visitData(int id, 
        double x1, double y1, double z1, 
        double x2, double y2, double z2, 
        size_t /*size*/, uint8_t* /*data*/) {

		if (this->id == id) {
			double p1[] = { x1, y1, z1 };
			double p2[] = { x2, y2, z2 };

			region = SpatialIndex::Region(p1, p2, 3);

			found = true;
		}
	}
    virtual void visitNode(
        double /*x1*/, double /*y1*/, double /*z1*/,
        double /*x2*/, double /*y2*/, double /*z2*/) {}

public:
	int id;
	SpatialIndex::Region region;
	bool found;
};



/**
 * \brief Spatial index class. 
 * Each instance of this class offers a spatial index.
 * This class wraps the API of the "spatial index" library.
 *
 * \ingroup spatialindex
 * \scriptable
 */
class RSpatialIndexNavel: public RSpatialIndex {
protected:
	/**
	 * Internal representation of a rectangular region.
	 * Depends on the external spatial index library.
	 *
	 * \ingroup qcadspatial
	 */
	class RSiRegion: public SpatialIndex::Region {
	public:
		RSiRegion(double x1, double y1, double z1, double x2, double y2,
				double z2);
	};

	/**
	 * \brief Internal node and data visitor.
	 * Depends on the external spatial index library.
	 * \see RSpatialIndexVisitor
	 *
	 * \ingroup qcadspatial
	 */
	class Visitor: public SpatialIndex::IVisitor {
	public:
		Visitor(QSet<int>& ids, RSpatialIndexVisitor* dataVisitor = NULL) :
			ids(ids), dataVisitor(dataVisitor) {
		}

		void visitNode(const SpatialIndex::INode& n) {
			//printf("Visitor::visitNode: %Ld\n", n.getIdentifier());
			if (dataVisitor != NULL) {
                SpatialIndex::IShape* shape;
                n.getShape(&shape);
                if (shape == NULL) {
                    fprintf(stderr, "error: node has no shape\n");
                    return;
                }
                SpatialIndex::Region* region =
                        dynamic_cast<SpatialIndex::Region*> (shape);
                if (region == NULL) {
                    fprintf(stderr,
                            "error: node shape in spacial index is not a SpatialIndex::Region\n");
                    return;
                }
				dataVisitor->visitNode(region->m_pLow[0],
						region->m_pLow[1], region->m_pLow[2],
						region->m_pHigh[0], region->m_pHigh[1],
						region->m_pHigh[2]);
            }
		}

		void visitData(const SpatialIndex::IData& d) {

			if (ids.contains(d.getIdentifier())) {
                /*
				qFatal("Visitor.visitData: double entry found in spatial index, identifier: %d", 
                    (int)d.getIdentifier());
				Q_ASSERT(false);
                */
			}

			ids.insert(d.getIdentifier());

			if (dataVisitor != NULL) {
                SpatialIndex::IShape* shape;
                d.getShape(&shape);
                if (shape == NULL) {
                    fprintf(stderr, "error: data node has no shape\n");
                    return;
                }
                SpatialIndex::Region* region =
                        dynamic_cast<SpatialIndex::Region*> (shape);
                if (region == NULL) {
                    fprintf(stderr,
                            "error: shape in spacial index is not a SpatialIndex::Region\n");
                    return;
                }

				size_t len;
				uint8_t* data;
				d.getData(len, &data);
				dataVisitor->visitData(d.getIdentifier(), region->m_pLow[0],
						region->m_pLow[1], region->m_pLow[2],
						region->m_pHigh[0], region->m_pHigh[1],
						region->m_pHigh[2],
						//region->getX1(), region->getY1(), region->getZ1(),
						//region->getX2(), region->getY2(), region->getZ2(),
						len, data);
                delete data;
                //delete region;
                delete shape;
			}
		}

		void visitData(std::vector<const SpatialIndex::IData*>& v) {
			std::vector<const SpatialIndex::IData*>::iterator it;
			for (it = v.begin(); it != v.end(); it++) {
				printf("Visitor::visitData[]: %Ld\n", (*it)->getIdentifier());
			}
		}

	private:
		QSet<int>& ids;
		RSpatialIndexVisitor* dataVisitor;
	};

public:
	RSpatialIndexNavel();
	~RSpatialIndexNavel();

    virtual void clear();

	/**
	 * \nonscriptable
	 */
	void addToIndex(int id, double x1, double y1, double z1, double x2,
			double y2, double z2, size_t dataLength = 0, const uint8_t* data =
					NULL);

	void removeFromIndex(int id);
    virtual void removeFromIndex(
    		int id,
    		double x1, double y1, double z1,
			double x2, double y2, double z2);

	QSet<int> queryIntersected(double x1, double y1, double z1,
			double x2, double y2, double z2, RSpatialIndexVisitor* dataVisitor =
					NULL);
	QSet<int> queryContained(double x1, double y1, double z1,
			double x2, double y2, double z2, RSpatialIndexVisitor* dataVisitor =
					NULL);

protected:
    /**
	 * \nonscriptable
     */
	void addToIndex(int id, const RSiRegion& region, size_t dataLength = 0,
			const uint8_t* data = NULL);
    /**
	 * \nonscriptable
     */
	QSet<int> queryIntersected(const RSiRegion& region,
			RSpatialIndexVisitor* dataVisitor = NULL);
    /**
	 * \nonscriptable
     */
	QSet<int> queryContained(const RSiRegion& region,
			RSpatialIndexVisitor* dataVisitor = NULL);

protected:
    void init();
    void uninit();

protected:
	SpatialIndex::ISpatialIndex* tree;
	SpatialIndex::IStorageManager* buff;

};

Q_DECLARE_METATYPE(RSpatialIndexNavel*)

#endif
