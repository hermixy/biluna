#ifndef RLINETYPE_H
#define RLINETYPE_H

#include <QColor>
#include <QCoreApplication>
#include <QIcon>
#include <QMetaType>
#include <QString>

#include "RLinetypePattern.h"
#include "RObject.h"
#include "RPropertyTypeId.h"
#include "RVector.h"

class RDocument;



/**
 * Represents a linetype in a drawing. A linetype is an object (RObject).
 * A drawings contains and owns a set of linetypes.
 *
 * \ingroup core
 * \scriptable
 * \copyable
 */
class RLinetype: public RObject {

	Q_DECLARE_TR_FUNCTIONS(RLinetype);

public:
	static RPropertyTypeId PropertyName;

public:
    RLinetype(RDocument* document=NULL);
    RLinetype(RDocument* document, const QString& name);

	virtual ~RLinetype();

    static void init();

    virtual RLinetype* clone() {
        return new RLinetype(*this);
    }

    QString getName() const {
        return name;
    }

    void setName(const QString& n) {
        name = n;
    }

    bool isValid() const;

    static QList<QPair<QString, RLinetype> > getList(bool onlyFixed = false);

	virtual QPair<QVariant, RPropertyAttributes> getProperty(
            RPropertyTypeId propertyTypeId,
			bool humanReadable = false, bool noAttributes = false);
	virtual bool setProperty(RPropertyTypeId propertyTypeId,
            const QVariant& value);

	virtual bool isSelectedForPropertyEditing();

	static QIcon getIcon(const RLinetype& linetype);
	static QString getTitle(const RLinetype& linetype);

	bool operator==(const RLinetype & linetype) const;
	bool operator!=(const RLinetype & linetype) const;
	bool operator<(const RLinetype & linetype) const;

private:
	static void init(const QString& cn, const RLinetype& c);

private:
	QString name;
	static QList<QPair<QString, RLinetype> > list;
	static QMap<RLinetype, QIcon> iconMap;
};

QDebug operator<<(QDebug dbg, const RLinetype& l);

Q_DECLARE_METATYPE(RLinetype)
Q_DECLARE_METATYPE(RLinetype*)
Q_DECLARE_METATYPE(QSharedPointer<RLinetype>)
Q_DECLARE_METATYPE(QSharedPointer<RLinetype>*)
typedef QPair<QString, RLinetype> _RPairStringRLinetype;
Q_DECLARE_METATYPE(QList< _RPairStringRLinetype >)

#endif
