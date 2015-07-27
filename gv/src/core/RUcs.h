#ifndef RUCS_H
#define RUCS_H

#include "RObject.h"
#include "RVector.h"
#include "RPropertyTypeId.h"



/**
 * Represents a user coordinate system (UCS) in a drawing.
 * The current UCS of a drawing defines the current workplane
 * and coordinate system that is being used for all input.
 *
 * \ingroup core
 */
class RUcs : public RObject {
public:
	static RPropertyTypeId PropertyName;
	static RPropertyTypeId PropertyOriginX;
	static RPropertyTypeId PropertyOriginY;
	static RPropertyTypeId PropertyOriginZ;
	static RPropertyTypeId PropertyXAxisDirectionX;
	static RPropertyTypeId PropertyXAxisDirectionY;
	static RPropertyTypeId PropertyXAxisDirectionZ;
	static RPropertyTypeId PropertyYAxisDirectionX;
	static RPropertyTypeId PropertyYAxisDirectionY;
	static RPropertyTypeId PropertyYAxisDirectionZ;

public:
    RUcs() : origin(0,0,0), xAxisDirection(1,0,0), yAxisDirection(0,1,0) {
    }

    RUcs(
        RDocument* document,
        const QString& name,
        const RVector& origin,
        const RVector& xAxisDirection,
        const RVector& yAxisDirection
    ) : RObject(document),
        name(name),
        origin(origin),
        xAxisDirection(xAxisDirection),
        yAxisDirection(yAxisDirection) {
   }

    virtual ~RUcs() {
    }

    static void init();

    virtual RUcs* clone() {
        return new RUcs(*this);
    }

    virtual bool isSelectedForPropertyEditing() {
        return false;
    }

    virtual QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);
    virtual bool setProperty(RPropertyTypeId propertyTypeId,
            const QVariant& value);

    void setOrigin(const RVector& o) {
        origin = o;
    }

    void setXAxisDirection(const RVector& x) {
        xAxisDirection = x;
    }

    void setYAxisDirection(const RVector& y) {
        yAxisDirection = y;
    }

    RVector getZAxisDirection();

    RVector mapFromUcs(const RVector& positionUcs);
    RVector mapToUcs(const RVector& positionWcs);

public:
    QString name;
    RVector origin;
    RVector xAxisDirection;
    RVector yAxisDirection;
};

Q_DECLARE_METATYPE(RUcs)
Q_DECLARE_METATYPE(RUcs*)
Q_DECLARE_METATYPE(QSharedPointer<RUcs>)

#endif
