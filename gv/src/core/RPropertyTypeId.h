#ifndef RPROPERTYTYPEID_H_
#define RPROPERTYTYPEID_H_

#include <typeinfo>

#include <QMap>
#include <QMetaType>
#include <QSet>
#include <QString>

#include "RDebug.h"

/**
 * Represents unique property type IDs. Every property type an object can have
 * has such an unique ID.
 *
 * E.g. all objects of type car may have a property
 * type called brand. This property type is uniquely identified by an instance
 * of this class. All instances of this class are automatically unique.
 *
 * \code
 * class Car {
 * public:
 * static RPropertyTypeId propertyBrand;
 * }
 *
 * RPropertyTypeId Car::propertyBrand(typeid(Car));
 * \endcode
 *
 * \ingroup core
 *
 * \scriptable
 * \copyable
 */
class RPropertyTypeId {
public:
	/**
	 * \ nonscriptable
	 */
	//RPropertyTypeId(const std::type_info& classInfo, const QString& groupTitle,
	//		const QString& title);
	/**
	 * \ nonscriptable
	 */
	//RPropertyTypeId(const std::type_info& classInfo, const RPropertyTypeId& other);

	RPropertyTypeId(const RPropertyTypeId& other);
	RPropertyTypeId(long int id = invalid);

	long int getId() const;

    bool isValid();

	/**
	 * \nonscriptable
	 */
    void generateId(const std::type_info& classInfo, 
        const QString& groupTitle, const QString& title);
	/**
	 * \nonscriptable
	 */
    void generateId(const std::type_info& classInfo, 
        const RPropertyTypeId& other);

	/**
	 * \nonscriptable
	 */
	static QSet<RPropertyTypeId> getPropertyTypeIds(
			const std::type_info& classInfo);
	/**
	 * \nonscriptable
	 */
	static bool hasPropertyType(const std::type_info& classInfo,
			RPropertyTypeId propertyTypeId);
	QString getPropertyGroupTitle() const;
	QString getPropertyTitle() const;
	static RPropertyTypeId getPropertyTypeId(const QString& groupTitle,
			const QString& title);

	bool operator ==(const RPropertyTypeId& other) const;
	bool operator <(const RPropertyTypeId& other) const;

private:
	long int id;

	static long int invalid;
	static long int counter;
	static QMap<QString, QSet<RPropertyTypeId> > propertyTypeByObjectMap;
	static QMap<long int, QPair<QString, QString> > titleMap;
};

uint qHash(RPropertyTypeId propertyTypeId);

Q_DECLARE_METATYPE(RPropertyTypeId)
Q_DECLARE_METATYPE(RPropertyTypeId*)
Q_DECLARE_METATYPE(QSet<RPropertyTypeId>)
Q_DECLARE_METATYPE(QList<RPropertyTypeId>)

#endif /* RPROPERTYTYPEID_H_ */
