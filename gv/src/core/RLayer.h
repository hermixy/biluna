#ifndef RLAYER_H
#define RLAYER_H

#include <QString>
#include <QColor>
#include <QDebug>

#include "RGlobal.h"
#include "RObject.h"
#include "RVector.h"
#include "RPropertyTypeId.h"
#include "RLinetype.h"
#include "RColor.h"
#include "RLineweight.h"

class RDocument;

/**
 * Represents a layer in a drawing.
 *
 * \ingroup core
 * \scriptable
 */
class RLayer: public RObject {
public:
    static RPropertyTypeId PropertyName;
    static RPropertyTypeId PropertyFrozen;
    static RPropertyTypeId PropertyLocked;
    static RPropertyTypeId PropertyColor;
    static RPropertyTypeId PropertyLinetype;
    static RPropertyTypeId PropertyLineweight;

public:
    RLayer();

    RLayer(RDocument* document, const QString& name, bool frozen = false,
    bool locked = false, const RColor& color = Qt::black,
    RLinetype::Id linetype = RLinetype::INVALID_ID,
    RLineweight::Lineweight lineweight = RLineweight::Weight000);

    virtual ~RLayer();

    static void init();

    virtual RLayer* clone();

    QString getName() const {
        return name;
    }

    void setName(const QString& n);

    bool isFrozen() const {
        return frozen;
    }

    void setFrozen(bool on) {
        frozen = on;
    }

    bool isLocked() const {
        return locked;
    }

    void setLocked(bool on) {
        locked = on;
    }

    RColor getColor() const {
        return color;
    }

    void setColor(const RColor& c) {
        color = c;
    }

    RLinetype::Id getLinetypeId() const {
        return linetype;
    }

    void setLinetypeId(RLinetype::Id lt) {
        linetype = lt;
    }

    RLineweight::Lineweight getLineweight() const {
        return lineweight;
    }

    void setLineweight(RLineweight::Lineweight lw) {
        lineweight = lw;
    }

    virtual QPair<QVariant, RPropertyAttributes> getProperty(
        RPropertyTypeId propertyTypeId,
                    bool humanReadable = false, bool noAttributes = false);
    virtual bool setProperty(RPropertyTypeId propertyTypeId,
        const QVariant& value);

    virtual bool isSelectedForPropertyEditing();

private:
    RDocument* document;
    QString name;
    bool frozen;
    bool locked;
    RColor color;
    RLinetype::Id linetype;
    RLineweight::Lineweight lineweight;
};

QDebug operator<<(QDebug dbg, const RLayer& l);

Q_DECLARE_METATYPE(QSharedPointer<RLayer>)
Q_DECLARE_METATYPE(QSharedPointer<RLayer>*)
Q_DECLARE_METATYPE(RLayer)
Q_DECLARE_METATYPE(RLayer*)

#endif
