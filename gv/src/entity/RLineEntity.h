#ifndef RLINEENTITY_H
#define RLINEENTITY_H

#include "REntity.h"
#include "RLineData.h"

class RDocument;
class RExporter;

/**
 * \scriptable
 * \ingroup entity
 */
class RLineEntity: public REntity {

public:
    static RPropertyTypeId PropertyType;
    static RPropertyTypeId PropertyLayer;
    static RPropertyTypeId PropertyLinetype;
    static RPropertyTypeId PropertyLineweight;
    static RPropertyTypeId PropertyColor;
    static RPropertyTypeId PropertyDrawOrder;
    static RPropertyTypeId PropertyStartPointX;
    static RPropertyTypeId PropertyStartPointY;
    static RPropertyTypeId PropertyStartPointZ;
    static RPropertyTypeId PropertyEndPointX;
    static RPropertyTypeId PropertyEndPointY;
    static RPropertyTypeId PropertyEndPointZ;

public:
    RLineEntity(RDocument* document, const RLineData& data,
        RObject::Id objectId = RObject::INVALID_ID);
    ~RLineEntity();

    static void init();

    virtual RLineEntity* clone() {
        return new RLineEntity(*this);
    }

    bool setProperty(RPropertyTypeId propertyTypeId, const QVariant& value);
	QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);

	virtual void exportEntity(RExporter& e) const;

	virtual RLineData& getData() {
		return data;
	}

	virtual const RLineData& getData() const {
		return data;
	}

	RVector getStartPoint() const {
		return data.getStartPoint();
	}

	RVector getEndPoint() const {
		return data.getEndPoint();
	}

protected:
	virtual void print(QDebug dbg) const;

protected:
	RLineData data;
};

Q_DECLARE_METATYPE(RLineEntity*)

#endif
