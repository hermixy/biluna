#ifndef RPROPERTYATTRIBUTES_H
#define RPROPERTYATTRIBUTES_H

#include <QString>
#include <QSet>
#include <QMap>
#include <QPair>
#include <QVariant>

#include "RDebug.h"
#include "RMath.h"

/**
 * Structure to transfer attributes about property types.
 * For example a property 'Side' might have the two choices
 * 'Left' and 'Right'. These choices can be stored in a
 * \c RPropertyAttributes object. Other attributes define if a property
 * is visible, if it is an angle (such properties require
 * conversion between radiants and degrees), etc.
 *
 * \ingroup core
 */
class RPropertyAttributes {
public:
    enum Option {
        NoOptions = 0x0,
        ReadOnly = 0x1,
        Invisible = 0x2,
        Angle = 0x4,
        AffectsOtherProperties = 0x8,
        IgnoreCase = 0x10,
        AllowMixedValue = 0x20
    };
    Q_DECLARE_FLAGS(Options, Option)

public:
	RPropertyAttributes() : options(NoOptions), mixed(false) {}

	RPropertyAttributes(RPropertyAttributes::Options options) :
        options(options), mixed(false) {
	}

    void setOption(RPropertyAttributes::Option option, bool on);

	bool isReadOnly() const {
		return options.testFlag(ReadOnly);
	}

    void setReadOnly(bool readOnly) {
        setOption(ReadOnly, readOnly);
    }

	bool isInvisible() const {
		return options.testFlag(Invisible);
	}

	void setInvisible(bool invisible) {
        setOption(Invisible, invisible);
	}

	bool isAngleType() const {
		return options.testFlag(Angle);
	}

	void setAngleType(bool angle) {
        setOption(Angle, angle);
	}

    bool getIgnoreCase() const {
		return options.testFlag(IgnoreCase);
    }

    void setIgnoreCase(bool ignoreCase){
        setOption(IgnoreCase, ignoreCase);
    }

	bool affectsOtherProperties() const {
		return options.testFlag(AffectsOtherProperties);
	}

	void setAffectsOtherProperties(bool affectsOtherProperties) {
        setOption(AffectsOtherProperties, affectsOtherProperties);
	}

	void setAllowMixedValue(bool allowMixedValue) {
        setOption(AllowMixedValue, allowMixedValue);
	}

	bool getAllowMixedValue() {
		return options.testFlag(AllowMixedValue);
	}

	QSet<QString> getChoices() const {
		return choices;
	}

	void setChoices(QSet<QString> choices) {
		this->choices = choices;
	}

	void mixWith(const RPropertyAttributes & other);

	bool isMixed() const {
		return mixed;
	}

	void setMixed(bool mixed) {
		this->mixed = mixed;
	}

    bool operator == (const RPropertyAttributes& other) const {
		if(isInvisible() != other.isInvisible()){ return false; }
		if(isAngleType() != other.isAngleType()){ return false; }
		if(affectsOtherProperties() != other.affectsOtherProperties()){ return false; }
		if(choices != other.choices){ return false; }
		return true;
	}

	bool operator !=(const RPropertyAttributes& other) const {
		return !operator ==(other);
	}


private:
    RPropertyAttributes::Options options;
	bool mixed;
	QSet<QString> choices;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(RPropertyAttributes::Options)

typedef QPair<QVariant, RPropertyAttributes> _RPairVariantPropertyAttributes;
Q_DECLARE_METATYPE(_RPairVariantPropertyAttributes*)
Q_DECLARE_METATYPE(_RPairVariantPropertyAttributes)

#endif
