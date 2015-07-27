#include "RTreePropertyBrowser.h"

#include <QTreeWidget>

#include "RMainWindow.h"
#include "RPropertyEvent.h"
#include "RPropertyTypeId.h"
#include "RColor.h"
#include "RLinetype.h"

RTreePropertyBrowser::RTreePropertyBrowser(QWidget *parent) :
	QtTreePropertyBrowser(parent), enabled(true) {

	variantManager = new QtVariantPropertyManager();
	variantFactory = new QtVariantEditorFactory;
	setFactoryForManager(variantManager, variantFactory);

	enumManager = new QtEnumPropertyManager;
	enumFactory = new QtEnumEditorFactory;
	setFactoryForManager(enumManager, enumFactory);

	colorManager = new RColorPropertyManager;
	colorFactory = new RColorEditorFactory;
	setFactoryForManager(colorManager, colorFactory);
	connect(colorFactory, SIGNAL(editorCreated()), this, SLOT(slotDisableGui()));
	connect(colorFactory, SIGNAL(editorDestroyed()), this, SLOT(slotEnableGui()));

	linetypeManager = new RLinetypePropertyManager;
	linetypeFactory = new RLinetypeEditorFactory;
	setFactoryForManager(linetypeManager, linetypeFactory);

	lineweightManager = new RLineweightPropertyManager;
	lineweightFactory = new RLineweightEditorFactory;
	setFactoryForManager(lineweightManager, lineweightFactory);

	QListIterator<QObject*> i(children());
	while (i.hasNext()) {
		QObject* o = i.next();
		if (QString(o->metaObject()->className()) == "QtPropertyEditorView") {
			QTreeWidget* w = qobject_cast<QTreeWidget*> (o);
			if (w == NULL) {
				continue;
			}
			w->setIconSize(QSize(32, 16));
			break;
		}
	}
}

RTreePropertyBrowser::~RTreePropertyBrowser() {
	delete variantManager;
	delete variantFactory;
	delete enumManager;
	delete enumFactory;
	delete colorManager;
	delete colorFactory;
	delete linetypeManager;
	delete linetypeFactory;
	delete lineweightManager;
	delete lineweightFactory;
}

void RTreePropertyBrowser::slotEnableGui() {
	enabled = true;
}

void RTreePropertyBrowser::slotDisableGui() {
	enabled = false;
}


/**
 * Called whenever the user has changed a browser item that represents
 * a property.
 */
void RTreePropertyBrowser::itemChanged(QtBrowserItem* item) {
	QtTreePropertyBrowser::itemChanged(item);

	if (/*updatesDisabled == true ||*/ item == NULL) {
		return;
	}

    updatesDisabled = true;
	RPropertyTypeId propertyTypeId = getPropertyTypeId(item);
	QVariant propertyValue = getPropertyValue(item);
	propertyChanged(propertyTypeId, propertyValue);
    updatesDisabled = false;
}


/**
 * @return Property ID (group and property title) for the given item.
 */
RPropertyTypeId RTreePropertyBrowser::getPropertyTypeId(QtBrowserItem* item) {
	if (isGroupProperty(item)) {
		return -1;
	}
	if (isValueMixed(item)) {
		return -1;
	}

	return RPropertyTypeId::getPropertyTypeId(getGroupTitle(item), getTitle(
			item));
}


RPropertyAttributes RTreePropertyBrowser::getPropertyAttributes(
		QtBrowserItem* item) {
	if (isGroupProperty(item)) {
		return RPropertyAttributes();
	}

	QString groupTitle = getGroupTitle(item);
	if (!combinedProperties.contains(groupTitle)) {
		return RPropertyAttributes();
	}

	QString title = getTitle(item);
	if (!combinedProperties[groupTitle].contains(title)) {
		return RPropertyAttributes();
	}

	return combinedProperties[groupTitle][title].second;
}

/**
 * @return True if the given item is a group item, false otherwise.
 */
bool RTreePropertyBrowser::isGroupProperty(QtBrowserItem* item) {
	if (!isVariantProperty(item)) {
		return false;
	}

	QtProperty* p = variantManager->variantProperty(item->property());
	if (p == NULL) {
		return false;
	}

	QtVariantProperty* vp = dynamic_cast<QtVariantProperty*> (p);
	if (vp == NULL) {
		return false;
	}

	if (vp->propertyType() == QtVariantPropertyManager::groupTypeId()) {
		return true;
	} else {
		return false;
	}
}


/**
 * @return True if the given item contains a variant property,
 *      false otherwise (enum item).
 */
bool RTreePropertyBrowser::isVariantProperty(QtBrowserItem* item) {
	QtAbstractPropertyManager* manager = item->property()->propertyManager();
	if (QString(manager->metaObject()->className())
			== "QtVariantPropertyManager") {
		return true;
	}
	return false;
}


bool RTreePropertyBrowser::isColorProperty(QtBrowserItem* item) {
	QtAbstractPropertyManager* manager = item->property()->propertyManager();
	if (QString(manager->metaObject()->className()) == "RColorPropertyManager") {
		return true;
	}
	return false;
}

bool RTreePropertyBrowser::isLinetypeProperty(QtBrowserItem* item) {
	QtAbstractPropertyManager* manager = item->property()->propertyManager();
	if (QString(manager->metaObject()->className()) == "RLinetypePropertyManager") {
		return true;
	}
	return false;
}

bool RTreePropertyBrowser::isLineweightProperty(QtBrowserItem* item) {
	QtAbstractPropertyManager* manager = item->property()->propertyManager();
	if (QString(manager->metaObject()->className()) == "RLineweightPropertyManager") {
		return true;
	}
	return false;
}

/**
 * @return True if the value of the given item is mixed, false otherwise.
 *      Mixed values are shown as text line editors or combo boxes
 *      with the text 'mixed'.
 */
bool RTreePropertyBrowser::isValueMixed(QtBrowserItem* item) {
	QtProperty* p = item->property();
	if (p->valueText() == mixed) {
		QString groupTitle = getGroupTitle(item);
		QString title = getTitle(item);
		if (combinedProperties.contains(groupTitle)
				&& combinedProperties[groupTitle].contains(title)) {
			if(combinedProperties[groupTitle][title].second.getAllowMixedValue()){
				return false;
			}
		}
		return true;
	}

	return false;
}



/**
 * @return Current property value for the given item.
 */
QVariant RTreePropertyBrowser::getPropertyValue(QtBrowserItem* item) {
    // find out complete property id:
    if (isGroupProperty(item)) {
        return QVariant();
    }

    if (isValueMixed(item)) {
        return QVariant();
    }

    if (isVariantProperty(item)) {
		QtVariantProperty* p =
				variantManager->variantProperty(item->property());
		RPropertyAttributes attr = getPropertyAttributes(item);
        if (attr.isAngleType()) {
            qDebug() << "RTreePropertyBrowser::getPropertyValue: "
                << p->value().toString();
            qDebug() << "RTreePropertyBrowser::getPropertyValue: "
                << RMath::eval(p->value().toString());
            qDebug() << "RTreePropertyBrowser::getPropertyValue: "
                << RMath::deg2rad(RMath::eval(p->value().toString()));
            return RMath::deg2rad(RMath::eval(p->value().toString()));
		}
		return p->value();
	}

    if(isColorProperty(item)){
    	QtProperty* p = item->property();
    	RColor color = colorManager->value(p);
    	QVariant v;
    	v.setValue<RColor>(color);
    	return v;
    }

    if(isLinetypeProperty(item)){
    	QtProperty* p = item->property();
    	RLinetype linetype = linetypeManager->value(p);
    	QVariant v;
    	v.setValue<RLinetype>(linetype);
    	return v;
    }

    if(isLineweightProperty(item)){
    	QtProperty* p = item->property();
    	RLineweight::Lineweight lineweight = lineweightManager->value(p);
    	QVariant v;
    	v.setValue<RLineweight::Lineweight>(lineweight);
    	return v;
    }

	QtProperty* p = item->property();
	//RPropertyOwner::debugProperty("RPropertyEditorQt::getPropertyValue 2", "", p->valueText());
	return QVariant(p->valueText());

}

QString RTreePropertyBrowser::getTitle(QtBrowserItem* item) {
	return item->property()->propertyName();
}

QString RTreePropertyBrowser::getGroupTitle(QtBrowserItem* item) {
	QtBrowserItem* parent = item->parent();
	if (parent == NULL) {
		return QString();
	}
	return parent->property()->propertyName();
}



/**
 * Converts the given \c QVariant (coming from the GUI) into an appropriate
 * \c QVariant if possible.
 */
//QVariant RTreePropertyBrowser::toPropertyValue(const QVariant& v) {
//	switch (v.type()) {
//	case QVariant::Bool:
//		return QVariant(v.toBool());
//		break;
//
//	case QVariant::Int:
//		return QVariant(v.toInt());
//		break;
//
//	case QVariant::Double:
//		return QVariant(v.toDouble());
//		break;
//
//	case QVariant::String:
//		return QVariant((const char*) v.toString().toLatin1());
//		break;
//
//	default:
//		return QVariant();
//		break;
//	}
//}


/**
 * \return A \c QtProperty that represents the given property.
 */
QtProperty* RTreePropertyBrowser::createQtProperty(const QString& propertyKey,
		const QPair<QVariant, RPropertyAttributes>& property) {

	QVariant qvariant;

	// angles are displayed and edited in degrees:
	if (property.second.isAngleType()) {
        qvariant = RMath::angleToString(property.first.toDouble());
	} else {
		qvariant = property.first;
	}

	QSet<QString> choices = property.second.getChoices();

	QStringList qchoices;
	QSet<QString>::iterator it3;
	for (it3 = choices.begin(); it3 != choices.end(); it3++) {
		qchoices << (*it3);
	}

	QtProperty* qproperty = NULL;

	//if (property.first.canConvert<RLineweight::Lineweight> ()) {
	//	RDebug::debug(QString("RTreePropertyBrowser::createQtProperty: "));
	//}

	if (property.second.isReadOnly()) {
        qproperty = enumManager->addProperty(propertyKey);
        if (qproperty != NULL) {
            qproperty->setEnabled(false);
            enumManager->setEnumNames(qproperty, QStringList() << property.first.toString());
        }
        return qproperty;
    }

	if (property.second.isMixed() && choices.size() == 0
			&& !property.first.canConvert<RColor> ()
			&& !property.first.canConvert<RLinetype> ()
			&& !property.first.canConvert<RLineweight::Lineweight> () ) {
		qproperty = variantManager->addProperty(QVariant::String, propertyKey);
		if (qproperty != NULL) {
			((QtVariantProperty*) qproperty)->setValue(mixed);
		}
		return qproperty;
	}

	if (choices.size() == 0) {
		// add property control as variant editor:
		qproperty = variantManager->addProperty(qvariant.type(), propertyKey);

		if (qproperty != NULL) {
			((QtVariantProperty*) qproperty)->setValue(qvariant);

			// show 4 decimals for double values:
			if (qvariant.type() == QVariant::Double) {
				variantManager->setAttribute(qproperty, "decimals", 4);
			}
//			if (property.second.isAngleType()) {
//				variantManager->setAttribute(qproperty, "maximum", 360);
//				variantManager->setAttribute(qproperty, "minimum", -360);
//			}
			return qproperty;
		}
		if (property.first.canConvert<RColor> ()) {
			qproperty = colorManager->addProperty(propertyKey);
			if (property.second.isMixed()) {
				colorManager->setMixedValue(qproperty);
			} else {
				colorManager->setValue(qproperty,
						property.first.value<RColor> ());
			}
			return qproperty;
		}
		if (property.first.canConvert<RLinetype> ()) {
			qproperty = linetypeManager->addProperty(propertyKey);
			if (property.second.isMixed()) {
				linetypeManager->setMixedValue(qproperty);
			} else {
				linetypeManager->setValue(qproperty, property.first.value<
						RLinetype> ());
			}
			return qproperty;
		}
		if (property.first.canConvert<RLineweight::Lineweight> ()) {
			qproperty = lineweightManager->addProperty(propertyKey);
			if (property.second.isMixed()) {
				lineweightManager->setMixedValue(qproperty);
			} else {
				lineweightManager->setValue(qproperty, property.first.value<
						RLineweight::Lineweight> ());
			}
			return qproperty;
		}

		RDebug::warning(QString("RTreePropertyBrowser::createQtProperty: qproperty is NULL"));
		return qproperty;
	}

	// add property control as enum editor (combo box):
	qproperty = enumManager->addProperty(propertyKey);
	if (qproperty != NULL) {
		QRegExp rx(QRegExp::escape(qvariant.toString()),
				   property.second.getIgnoreCase()?Qt::CaseInsensitive:Qt::CaseSensitive);
		int cIndex = qchoices.indexOf(rx);
		if (property.second.isMixed()) {
			rx.setPattern(QRegExp::escape(mixed));
			qchoices.prepend(mixed);
			qvariant = mixed;
		} else if (cIndex == -1) {
			// add value that is not yet in choices to possible choices:
			qchoices << qvariant.toString();
		}
		qchoices.sort();
		enumManager->setEnumNames(qproperty, qchoices);
		cIndex = qchoices.indexOf(rx);
		enumManager->setValue(qproperty, cIndex);
	}

	return qproperty;
}

void RTreePropertyBrowser::updateGui(bool force) {
	if (enabled == false && force == false) {
		if (!combinedProperties.isEmpty()) {
			return;
		}
	}

	setUpdatesEnabled(false);

	variantManager->clear();
	enumManager->clear();
	colorManager->clear();
	linetypeManager->clear();

	// iterate through all property groups:
	QList<QString>::iterator it;
	for (it = groupOrder.begin(); it != groupOrder.end(); ++it) {
		QString groupKey = (*it);

		QtVariantProperty* groupProperty = NULL;

		if (groupKey.size() != 0) {
			// create property browser item for current group:
			groupProperty = variantManager->addProperty(
					QtVariantPropertyManager::groupTypeId(), groupKey);
			addProperty(groupProperty);
		}

		if (propertyOrder.count(groupKey) == 0 || combinedProperties.count(
				groupKey) == 0) {
			RDebug::error("RPropertyEditorQt::updateGui: "
				"key '%s' not found", (const char*) groupKey.toUtf8());
			continue;
		}

		// iterate through all properties in the current group:
		QList<QString>::iterator it2;
		for (it2 = propertyOrder[groupKey].begin(); it2
				!= propertyOrder[groupKey].end(); ++it2) {
			QString propertyKey = (*it2);

			if (combinedProperties.count(groupKey) == 0
					|| combinedProperties[groupKey].count(propertyKey) == 0) {
				RDebug::error("RPropertyEditorQt::updateGui: "
					"key '%s'/'%s' not found", (const char*) groupKey.toUtf8(),
						(const char*) propertyKey.toUtf8());
				continue;
			}

			QtProperty* qproperty = createQtProperty(propertyKey,
					combinedProperties[groupKey][propertyKey]);
			if (qproperty != NULL) {
				if (groupProperty != NULL) {
					groupProperty->addSubProperty(qproperty);
				} else {
					addProperty(qproperty);
				}
			} else {
				RDebug::warning(QString(
						"RTreePropertyBrowser::updateGui: qproperty is NULL"));
			}
		}
	}

	setUpdatesEnabled(true);

	groupOrder.clear();
	propertyOrder.clear();

	guiUpToDate = true;
}
