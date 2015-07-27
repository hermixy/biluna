#ifndef RBLOCKREFERENCEENTITY_H
#define RBLOCKREFERENCEENTITY_H

#include "REntity.h"
#include "RBlockReferenceData.h"

class RDocument;
class RExporter;

/**
 * \scriptable
 * \ingroup entity
 */
class RBlockReferenceEntity: public REntity {

public:
    static RPropertyTypeId PropertyType;
    static RPropertyTypeId PropertyLayer;
    static RPropertyTypeId PropertyLinetype;
    static RPropertyTypeId PropertyLineweight;
    static RPropertyTypeId PropertyColor;
    static RPropertyTypeId PropertyBlock;
    static RPropertyTypeId PropertyPositionX;
    static RPropertyTypeId PropertyPositionY;
    static RPropertyTypeId PropertyPositionZ;
    static RPropertyTypeId PropertyScaleX;
    static RPropertyTypeId PropertyScaleY;
    static RPropertyTypeId PropertyScaleZ;
    static RPropertyTypeId PropertyRotation;

public:
    RBlockReferenceEntity(RDocument* document, const RBlockReferenceData& data,
			RObject::Id objectId = RObject::INVALID_ID);
	~RBlockReferenceEntity();

    static void init();

    virtual RBlockReferenceEntity* clone() {
        return new RBlockReferenceEntity(*this);
    }

    bool setProperty(RPropertyTypeId propertyTypeId, const QVariant& value);
	QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);

	virtual void exportEntity(RExporter& e) const;

	virtual RBlockReferenceData& getData() {
		return data;
	}

	virtual const RBlockReferenceData& getData() const {
		return data;
	}

    RVector getPosition() const {
       return data.getPosition();
    }

    void setReferencedBlockId(RBlock::Id blockId) {
        data.setReferencedBlockId(blockId);
    }

	RBlock::Id getReferencedBlockId() const {
		return data.getReferencedBlockId();
	}

	/**
	 * \nonscriptable
	 */
	virtual void print(QDebug dbg) const;

protected:
	RBlockReferenceData data;
    mutable int recursionDepth;
};

Q_DECLARE_METATYPE(RBlockReferenceEntity*)

#endif
