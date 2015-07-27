#include "RGlobal.h"
#include "RDebug.h"
#include "RMath.h"
#include "RMainWindow.h"
#include "RPropertyEditor.h"
#include "RPropertyEvent.h"
#include "RDocument.h"
#include "RObject.h"
#include "RColor.h"

QString RPropertyEditor::mixed;

/**
 * Default Constructor.
 */
RPropertyEditor::RPropertyEditor() {
	mixed = tr("*VARIES*");
	guiUpToDate = false;
	updatesDisabled = false;
}

/**
 * Destructor
 */
RPropertyEditor::~RPropertyEditor() {
}

/**
 * Sets the property with the given name to the given value or to 'mixed'
 * if that property exists already with a different value.
 *
 * \param propertyTypeId: Id of the property in the format "Group|Property".
 * \param property: Value and attributes of the property.
 */
void RPropertyEditor::updateProperty(const RPropertyTypeId& ptid,
        RObject& object, RDocument* container) {

	QString propertyGroupTitle = ptid.getPropertyGroupTitle();
	QString propertyTitle = ptid.getPropertyTitle();
//	QPair<QVariant, RPropertyAttributes>& property;

	if (guiUpToDate) {
		combinedProperties.clear();
		guiUpToDate = false;
	}

	// existing group:
	if (combinedProperties.count(propertyGroupTitle) > 0) {
		RPropertyMap& pmap = combinedProperties[propertyGroupTitle];

		// existing property in existing group:
		if (pmap.count(propertyTitle) > 0) {
            QPair<QVariant, RPropertyAttributes> property =
                object.getProperty(ptid, true, true);
			if (property.second.isInvisible()) {
				return;
			}
			// mixed value:
			if (!RS::compare(pmap[propertyTitle], property)) {
				pmap[propertyTitle].first = property.first;
				pmap[propertyTitle].second.setMixed(true);
			}
		} else {
			// new property in existing group:
            QPair<QVariant, RPropertyAttributes> property =
                object.getProperty(ptid, true);
			if (property.second.isInvisible()) {
				return;
			}
			pmap[propertyTitle] = property;
			propertyOrder[propertyGroupTitle].push_back(propertyTitle);
		}
	} else {
		// new property in new group:
		QPair<QVariant, RPropertyAttributes> property = object.getProperty(ptid,
				container, true);
		if (property.second.isInvisible()) {
			return;
		}
		RPropertyMap pmap;
		pmap[propertyTitle] = property;
		combinedProperties[propertyGroupTitle] = pmap;
		groupOrder.push_back(propertyGroupTitle);
		propertyOrder[propertyGroupTitle].push_back(propertyTitle);
	}
}

/**
 * Removes all but the given properties from the combined properties
 * collected so far.
 */
void RPropertyEditor::removeAllButThese(
		const QMultiMap<QString, QString>& propertyTitles) {

	// iterate through all groups of properties (e.g. "Start Point", "End Point", ...):
	QStringList removableGroups;
	RPropertyGroupMap::iterator it;
	for (it = combinedProperties.begin(); it != combinedProperties.end(); ++it) {

		// iterate through all properties in the current group (e.g. "X", "Y"):
		QStringList removableProperties;
		RPropertyMap::iterator it2;
		for (it2 = it.value().begin(); it2 != it.value().end(); ++it2) {
			bool keep = false;

			// check if the current property is among the given properties
			// we want to keep:
			QMultiMap<QString, QString>::const_iterator it3;
			for (it3 = propertyTitles.begin(); it3 != propertyTitles.end(); ++it3) {
				if (it3.key() == it.key() && it3.value() == it2.key()) {
					keep = true;
					break;
				}
			}

			if (keep == false) {
				// schedule property for removal:
				removableProperties.push_back(it2.key());
			}
		}

		// remove all properties in the current group that are scheduled for removal:
		QStringList::iterator it4;
		for (it4 = removableProperties.begin(); it4
				!= removableProperties.end(); ++it4) {
			it.value().remove(*it4);
			propertyOrder[it.key()].removeAll(*it4);
		}

		// schedule empty groups for removal:
		if (it.value().empty()) {
			removableGroups.push_back(it.key());
		}
	}

	// remove all groups that are scheduled for removal:
	QStringList::iterator it5;
	for (it5 = removableGroups.begin(); it5 != removableGroups.end(); ++it5) {
		combinedProperties.remove(*it5);
		groupOrder.removeAll(*it5);
	}
}

/**
 * Updates the property editor to contain the properties of the
 * property owners that are selected for editing in the given
 * property owner container.
 *
 */
void RPropertyEditor::updateFromContainer(RDocument* objectContainer) {
    if (updatesDisabled) {
        return;
    }

	if (objectContainer == NULL) {
		clearEditor();
		return;
	} else {
		combinedProperties.clear();
		propertyOrder.clear();
		groupOrder.clear();
	}

	updatesDisabled = true;

	// add all properties of the property owners in the given container:
	QSet<RObject::Id> objectIds =
        objectContainer->queryAllObjectsFromContainer();
	QSet<RObject::Id>::iterator it;
	for (it = objectIds.begin(); it != objectIds.end(); ++it) {
		updateEditor(*it, false, objectContainer);
	}

	// remove properties that are not shared by all entities:
	for (it = objectIds.begin(); it != objectIds.end(); ++it) {
		QSharedPointer<RObject> object = objectContainer->queryObjectFromContainer(*it);
		if (object.isNull()) {
			continue;
		}

		if (object->isSelectedForPropertyEditing()) {
			QSet<RPropertyTypeId> propertyTypeIds =
					object->getPropertyTypeIds();
			QMultiMap<QString, QString> propertiesToKeep;
			QSet<RPropertyTypeId>::iterator it;
			for (it = propertyTypeIds.begin(); it != propertyTypeIds.end(); ++it) {
				propertiesToKeep.insert(it->getPropertyGroupTitle(),
						it->getPropertyTitle());
			}
			removeAllButThese(propertiesToKeep);
		}
	}

	updateGui();

	updatesDisabled = false;
}

/**
 * Implements update from RPropertyListener.
 */
void RPropertyEditor::updateFromObject(RObject* object, RDocument* container) {
	if (object != NULL) {
		updateEditor(*object, true, container);
	}
}

/**
 * Updates the property widget to include the properties of the given property owner.
 */
void RPropertyEditor::updateEditor(RObject& object, bool doUpdateGui,
        RDocument* container) {
    QList<RPropertyTypeId> propertyTypeIds = object.getPropertyTypeIds().toList();
    qSort(propertyTypeIds);
    QList<RPropertyTypeId>::iterator it;
	for (it = propertyTypeIds.begin(); it != propertyTypeIds.end(); ++it) {
		updateProperty(*it, object, container);

//		QPair<QVariant, RPropertyAttributes> p = object.getProperty(*it,
//				container, true);
//		if (p.second.isVisible()) {
//			updateProperty(it->getPropertyGroupTitle(), it->getPropertyTitle(),
//					p);
//		}
	}
	if (doUpdateGui) {
		updateGui();
	}
}

void RPropertyEditor::clearEditor() {
	combinedProperties.clear();
	propertyOrder.clear();
	groupOrder.clear();
	updateGui(true);
}


void RPropertyEditor::propertyChanged(RPropertyTypeId propertyTypeId, QVariant propertyValue){
	if (RMainWindow::getMainWindow() == NULL) {
		RDebug::warning(QString(
				"RPropertyEditor::itemChanged: mainWindow is NULL"));
		return;
	}
    RPropertyEvent pe(propertyTypeId, propertyValue);
    RMainWindow::getMainWindow()->propertyChangeEvent(pe);
}

/**
 * Updates the property widget to include the property owner with the given ID.
 */
void RPropertyEditor::updateEditor(RObject::Id objectId, bool doUpdateGui,
        RDocument* objectContainer) {

	QSharedPointer<RObject> object = objectContainer->queryObjectFromContainer(objectId);
	if (object.isNull()) {
		return;
	}

	if (!object->isSelectedForPropertyEditing()) {
		return;
	}

	updateEditor(*object, doUpdateGui, objectContainer);
}
