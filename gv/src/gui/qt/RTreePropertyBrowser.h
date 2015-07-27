#ifndef RTREEPROPERTYBROWSER_H_
#define RTREEPROPERTYBROWSER_H_

#include <QWidget>
//#include <QtTreePropertyBrowser>
#include "qttreepropertybrowser.h"
//#include <QtVariantPropertyManager>
#include "qtvariantproperty.h"
//#include <QtVariantEditorFactory>
//#include <QtEnumPropertyManager>
#include "qtpropertymanager.h"
//#include <QtEnumEditorFactory>
#include "qteditorfactory.h"

#include "RPropertyEditor.h"
#include "RColorEditorFactory.h"
#include "RColorPropertyManager.h"
#include "RLinetypeEditorFactory.h"
#include "RLinetypePropertyManager.h"
#include "RLineweightEditorFactory.h"
#include "RLineweightPropertyManager.h"
#include "RDocument.h"

/**
 * \scriptable
 */
class RTreePropertyBrowser: public QtTreePropertyBrowser,
		public RPropertyEditor {

Q_OBJECT

public:
	RTreePropertyBrowser(QWidget *parent);
	~RTreePropertyBrowser();

public slots:
	void slotEnableGui();
	void slotDisableGui();

protected:
	virtual void updateGui(bool force = false);
	QtProperty* createQtProperty(const QString& propertyKey,
			const QPair<QVariant, RPropertyAttributes>& property);

	virtual void itemChanged(QtBrowserItem* item);
	RPropertyTypeId getPropertyTypeId(QtBrowserItem* item);
	QVariant getPropertyValue(QtBrowserItem* item);
	RPropertyAttributes getPropertyAttributes(QtBrowserItem* item);
	bool isGroupProperty(QtBrowserItem* item);
	bool isValueMixed(QtBrowserItem* item);
	bool isVariantProperty(QtBrowserItem* item);
	bool isColorProperty(QtBrowserItem* item);
	bool isLinetypeProperty(QtBrowserItem* item);
	bool isLineweightProperty(QtBrowserItem* item);
	QString getTitle(QtBrowserItem* item);
	QString getGroupTitle(QtBrowserItem* item);

private:
	bool enabled;
	QtVariantPropertyManager* variantManager;
	QtVariantEditorFactory* variantFactory;

	RColorPropertyManager* colorManager;
	RColorEditorFactory* colorFactory;

	QtEnumPropertyManager* enumManager;
	QtEnumEditorFactory* enumFactory;

	RLinetypePropertyManager* linetypeManager;
	RLinetypeEditorFactory* linetypeFactory;

	RLineweightPropertyManager* lineweightManager;
	RLineweightEditorFactory* lineweightFactory;
};

Q_DECLARE_METATYPE(RTreePropertyBrowser*)

#endif
