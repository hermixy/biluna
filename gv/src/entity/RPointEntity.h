#ifndef RPOINTENTITY_H
#define RPOINTENTITY_H

#include "REntity.h"
#include "RPointData.h"

class RDocument;
class RExporter;

/**
 * \scriptable
 * \ingroup entity
 */
class RPointEntity: public REntity {

public:
    static RPropertyTypeId PropertyType;
    static RPropertyTypeId PropertyLayer;
    static RPropertyTypeId PropertyLinetype;
    static RPropertyTypeId PropertyLineweight;
    static RPropertyTypeId PropertyColor;
    static RPropertyTypeId PropertyDrawOrder;
    static RPropertyTypeId PropertyPositionX;
    static RPropertyTypeId PropertyPositionY;
    static RPropertyTypeId PropertyPositionZ;

public:
    RPointEntity(RDocument* document, const RPointData& data,
                 RObject::Id objectId = RObject::INVALID_ID);
    ~RPointEntity();

    static void init();

    virtual RPointEntity* clone() {
        return new RPointEntity(*this);
    }

    bool setProperty(RPropertyTypeId propertyTypeId, const QVariant& value);
	QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);

	virtual void exportEntity(RExporter& e) const;

	virtual RPointData& getData() {
		return data;
	}

	virtual const RPointData& getData() const {
		return data;
	}

	RVector getPosition() const {
		return data.getPosition();
	}

protected:
	virtual void print(QDebug dbg) const;

protected:
	RPointData data;
};

Q_DECLARE_METATYPE(RPointEntity*)

#endif
