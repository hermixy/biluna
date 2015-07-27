#ifndef RPROPERTYEDITOR_H
#define RPROPERTYEDITOR_H

#include <QCoreApplication>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>

#include "RDocument.h"
#include "RObject.h"
#include "RPropertyListener.h"


/**
 *  base class for property editors. Provides basic functionality that
 * is common to any property editor implementation.
 *
 * \ingroup core
 * \scriptable
 */
class RPropertyEditor: public RPropertyListener {

	Q_DECLARE_TR_FUNCTIONS(RPropertyEditor)

public:
	RPropertyEditor();
	virtual ~RPropertyEditor();

	// from RPropertyListener interface:
    virtual void updateFromContainer(RDocument* container);
    virtual void updateFromObject(RObject* object, RDocument* container =
			NULL);
	virtual void clearEditor();
	void propertyChanged(RPropertyTypeId propertyTypeId, QVariant propertyValue);

protected:
	virtual void updateEditor(RObject& object, bool doUpdateGui,
            RDocument* container = NULL);

	/**
	 * Updates the user interface of this property editor. This is the
	 * only part of the property editor that depends on the user interface
	 * and GUI toolkit that is available.
	 */
	virtual void updateGui(bool force = false) = 0;

	void updateProperty(const RPropertyTypeId& ptid, RObject& object,
            RDocument* container);
	void removeAllButThese(const QMultiMap<QString, QString>& propertyTitles);
	void updateEditor(RObject::Id objectId, bool doUpdateGui = true,
            RDocument* objectContainer = NULL);

public:
	static QString mixed;

protected:
	typedef QMap<QString, QPair<QVariant, RPropertyAttributes> > RPropertyMap;
	typedef QMap<QString, RPropertyMap> RPropertyGroupMap;

	QStringList groupOrder;
	QMap<QString, QStringList> propertyOrder;
	RPropertyGroupMap combinedProperties;
	bool guiUpToDate;
	bool updatesDisabled;
};

Q_DECLARE_METATYPE(RPropertyEditor*)

#endif
