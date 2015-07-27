#ifndef RCIRCLENTITY_H
#define RCIRCLENTITY_H

#include "REntity.h"
#include "RCircleData.h"

class RDocument;
class RExporter;

/**
 * \scriptable
 * \ingroup entity
 */
class RCircleEntity: public REntity {

public:
    static RPropertyTypeId PropertyType;
    static RPropertyTypeId PropertyLayer;
    static RPropertyTypeId PropertyLinetype;
    static RPropertyTypeId PropertyLineweight;
    static RPropertyTypeId PropertyColor;
    static RPropertyTypeId PropertyDrawOrder;
    static RPropertyTypeId PropertyCenterX;
    static RPropertyTypeId PropertyCenterY;
    static RPropertyTypeId PropertyCenterZ;
    static RPropertyTypeId PropertyRadius;

public:
    RCircleEntity(RDocument* document, const RCircleData& data,
                  RObject::Id objectId = RObject::INVALID_ID);
    ~RCircleEntity();

    static void init();

    virtual RCircleEntity* clone() {
        return new RCircleEntity(*this);
    }

    bool setProperty(RPropertyTypeId propertyTypeId, const QVariant& value);
	QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);

	virtual void exportEntity(RExporter& e) const;

	virtual RCircleData& getData() {
		return data;
	}

	virtual const RCircleData& getData() const {
		return data;
	}

	RVector getCenter() const {
		return data.getCenter();
	}

	double getRadius() {
		return data.getRadius();
	}

protected:
	virtual void print(QDebug dbg) const;

protected:
	RCircleData data;
};

Q_DECLARE_METATYPE(RCircleEntity*)

#endif
