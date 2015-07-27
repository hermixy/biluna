#ifndef RBLOCK_H
#define RBLOCK_H

#include <QString>
#include <QDebug>

#include "RGlobal.h"
#include "RObject.h"
#include "RVector.h"
#include "RPropertyTypeId.h"

class RDocument;

/**
 * Represents a block definition in a drawing.
 *
 * \ingroup core
 * \scriptable
 */
class RBlock: public RObject {
public:
	static RPropertyTypeId PropertyName;

public:
	RBlock();

    RBlock(RDocument* document, const QString& name, const RVector& origin);

	virtual ~RBlock();

	static void init();

	virtual RBlock* clone();

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

    void setOrigin(const RVector& origin) {
		this->origin = origin;
	}

	RVector getOrigin() const {
		return origin;
	}

	virtual QPair<QVariant, RPropertyAttributes> getProperty(
			RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);
	virtual bool setProperty(RPropertyTypeId propertyTypeId,
            const QVariant& value);

	virtual bool isSelectedForPropertyEditing();

protected:
	/**
	 * \nonscriptable
	 */
	virtual void print(QDebug dbg) const;

private:
	QString name;
	bool frozen;
	RVector origin;
};


Q_DECLARE_METATYPE(QSharedPointer<RBlock>)
Q_DECLARE_METATYPE(QSharedPointer<RBlock>*)
Q_DECLARE_METATYPE(RBlock)
Q_DECLARE_METATYPE(RBlock*)

#endif
