#ifndef RARCENTITY_H
#define RARCENTITY_H

#include "REntity.h"
#include "RArcData.h"

class RDocument;
class RExporter;

/**
 * \scriptable
 * \ingroup entity
 */
class RArcEntity: public REntity {

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
	static RPropertyTypeId PropertyStartAngle;
	static RPropertyTypeId PropertyEndAngle;
	static RPropertyTypeId PropertyReversed;

public:
    RArcEntity(RDocument* document, const RArcData& data,
        RObject::Id objectId = RObject::INVALID_ID);
	~RArcEntity();

    static void init();

    virtual RArcEntity* clone() {
        return new RArcEntity(*this);
    }

    bool setProperty(RPropertyTypeId propertyTypeId, const QVariant& value);
	QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);

	virtual void exportEntity(RExporter& e) const;

	virtual RArcData& getData() {
		return data;
	}

	virtual const RArcData& getData() const {
		return data;
	}

	RVector getCenter() const {
		return data.getCenter();
	}

	double getRadius() const {
		return data.getRadius();
	}

	double getStartAngle() const {
		return data.getStartAngle();
	}

	double getEndAngle() const {
		return data.getEndAngle();
	}

	bool isReversed() const {
		return data.isReversed();
	}

protected:
	virtual void print(QDebug dbg) const;

protected:
	RArcData data;
};

Q_DECLARE_METATYPE(RArcEntity*)

#endif
