/****************************************************************************
**
** Copyright (C) 2007 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifdef RS_PROPERTYEDITOR

#include "qg_propertyeditor.h"

#include <QHeaderView>
#include <QLayout>
#include <QMetaProperty>
#include <QTableWidget>

#include <QtBrowserItem>
#include <QtProperty>
#include <QtVariantPropertyManager>
#include <QtEnumPropertyManager>
#include <QtEnumEditorFactory>

#include "rs_propertyownercontainerinterface.h"
#include "rs.h"
#include "rs_modification.h"


#include "qg_dialogfactory.h"
#include "qg_mainwindow.h"

char QG_PropertyEditor::separator = '|';

/**
 * Default Constructor. 
 */
QG_PropertyEditor::QG_PropertyEditor(QWidget* parent)
        : QtTreePropertyBrowser(parent) {
    
    guiUpToDate = false;
    updatesDisabled = false;
    variantManager = new QtVariantPropertyManager();
    enumManager = new QtEnumPropertyManager();

    variantFactory = new QtVariantEditorFactory();
    enumFactory = new QtEnumEditorFactory();

    setFactoryForManager(variantManager, variantFactory);
    setFactoryForManager(enumManager, enumFactory);
    
    QPalette p = palette();
    p.setColor(QPalette::Highlight, Qt::darkBlue);
    p.setColor(QPalette::HighlightedText, Qt::white);
    setPalette(p);

    mixedProperty = QVariant(tr("mixed"));
    singlePropertyOwnerMode = false;
    propertyOwner = NULL;
    propertyOwnerContainer = NULL;
}



/**
 * Destructor
 */
QG_PropertyEditor::~QG_PropertyEditor() {
    delete variantManager;
    delete variantFactory;
    delete enumManager;
    delete enumFactory;
}



/**
 * Sets the property with the given name to the given value or to mixed 
 * if that property exists already with a different value.
 *
 * @param propertyId: Id of the property in the format "Group|Property".
 * @param value: Value of the property.
 */
void QG_PropertyEditor::setProperty(const QString& propertyId, 
    const QVariant& value, const RS_PropertyAttributes& atts) {

    if (guiUpToDate) {
        combinedProperties.clear();
        guiUpToDate = false;
    }

    RS_PropertyId pId(propertyId);

    RS_DEBUG->print("QG_PropertyEditor::setProperty: "
        "groupTitle: %s\ntitle: %s", (const char*)pId.groupTitle.toLatin1(),
        (const char*)pId.propertyTitle.toLatin1());
    
    // existing group:
    if (combinedProperties.contains(pId.groupTitle)) {
        RS_DEBUG->print("QG_PropertyEditor::setProperty: "
            "existing group: %s", (const char*)pId.groupTitle.toLatin1());
        QG_UniquePropertyHash& p = combinedProperties[pId.groupTitle];
        
        // existing property in existing group:
        if (p.contains(pId.propertyTitle)) {
            RS_DEBUG->print("QG_PropertyEditor::setProperty: "
                "existing property: %s", (const char*)pId.propertyTitle.toLatin1());
            // mixed value:
            if (p[pId.propertyTitle].value != value) {
                p[pId.propertyTitle].value = mixedProperty;

                // todo: GET THIS INFO FROM PROPERTY OWNER:
                // never allow changing of entity type, mixed as only choice:
                if (pId.propertyTitle==RS_ENTITY_TYPE) {
                    p[pId.propertyTitle].atts.choices = 
                        QStringList() << mixedProperty.toString();
                }
                else {
                    if (p[pId.propertyTitle].atts.choices.count()!=0) {
                        p[pId.propertyTitle].atts.choices =
                            QStringList() << atts.choices << mixedProperty.toString();
                    }
                }
            }
        }

        // new property in existing group:
        else {
            RS_DEBUG->print("QG_PropertyEditor::setProperty: "
                "   new property: %s", (const char*)pId.propertyTitle.toLatin1());
            /*
            if (atts.isAngle) {
                p[pId.propertyTitle].value = RS_Math::rad2deg(value.toDouble());
            }
            else {
                p[pId.propertyTitle].value = value;
            }
            */
            p[pId.propertyTitle].value = value;
            p[pId.propertyTitle].atts = atts;
            propertyOrder[pId.groupTitle] << pId.propertyTitle;
        }
    }
    
    // new property in new group:
    else {
        RS_DEBUG->print("QG_PropertyEditor::setProperty: "
            "new group: %s", (const char*)pId.groupTitle.toLatin1());
        RS_DEBUG->print("QG_PropertyEditor::setProperty: "
            "   new property: %s", (const char*)pId.propertyTitle.toLatin1());
            
        QG_UniquePropertyHash p;
        /*
        if (atts.isAngle) {
            p[pId.propertyTitle].value = RS_Math::rad2deg(value.toDouble());
        }
        else {
            p[pId.propertyTitle].value = value;
        }
        */
        p[pId.propertyTitle].value = value;
        p[pId.propertyTitle].atts = atts;
        combinedProperties[pId.groupTitle] = p;
        groupOrder << pId.groupTitle;
        propertyOrder[pId.groupTitle] << pId.propertyTitle;
    }

    // debugging:
    RS_DEBUG->print("QG_PropertyEditor::setProperty:");
    QStringList groupKeys = combinedProperties.keys();
    for (int i=0; i<groupKeys.size(); ++i) {
        RS_DEBUG->print(QString("%1").arg(groupKeys[i]));
        QStringList propertyKeys = combinedProperties[groupKeys[i]].keys();
        for (int k=0; k<propertyKeys.size(); ++k) {
            RS_DEBUG->print(QString("   %1").arg(propertyKeys[k]));
            RS_DEBUG->print(QString("      value: %1")
                .arg(combinedProperties[groupKeys[i]][propertyKeys[k]].value.toString()));
            RS_DEBUG->print(QString("      choices: %1")
                .arg(combinedProperties[groupKeys[i]][propertyKeys[k]].atts.choices.join("|")));
        }
    }
}



/**
 * Removes all but the given properties.
 */
void QG_PropertyEditor::removeAllButThese(const QStringList& propertyIds) {

    RS_DEBUG->print("QG_PropertyEditor::removeAllButThese");
    RS_DEBUG->print(propertyIds);

    QList<QString> groupKeys = combinedProperties.keys();
    for (int i=0; i<groupKeys.size(); ++i) {
        
        QList<QString> propertyKeys = combinedProperties[groupKeys[i]].keys();
        for (int k=0; k<propertyKeys.size(); ++k) {
            bool contains = false;
            for (int n=0; n<propertyIds.size(); ++n) {
                RS_PropertyId pId(propertyIds[n]);
                if (pId.groupTitle==groupKeys[i] && 
                    pId.propertyTitle==propertyKeys[k]) {
                    contains = true;
                    break;
                }
            }
            if (contains==false) {
                combinedProperties[groupKeys[i]].remove(propertyKeys[k]);
                if (propertyOrder.contains(groupKeys[i])) {
                    propertyOrder[groupKeys[i]].removeAll(propertyKeys[k]);
                }
            }
        }
        if (combinedProperties[groupKeys[i]].count()==0) {
            combinedProperties.remove(groupKeys[i]);
            groupOrder.removeAll(groupKeys[i]);
        }
    }
}



/**
 * Updates the property editor to contain the properties of the 
 * property owners that are selected for editing in the given
 * property owner container.
 *
 * @param includeGeometry true: include geometry attributes,
 *      false: don't include geometry (e.g. during drag'n'drop)
 */
void QG_PropertyEditor::update(
    RS_PropertyOwnerContainerInterface* propertyOwnerContainer, 
    bool includeGeometry) {

    if (propertyOwnerContainer==NULL) {
        return;
    }
    
    updatesDisabled = true;
    
    RS_DEBUG->print("QG_PropertyEditor::update");
    
    // add all properties of the property owners in the given container:
    for (RS_PropertyOwner* e=propertyOwnerContainer->firstPropertyOwner();
            e!=NULL;
            e = propertyOwnerContainer->nextPropertyOwner()) {

        if (e->isSelectedForPropertyEditing()) {
            updateFor(e, includeGeometry);
        }
    }
    
    // remove properties that are not shared by all entities:
    for (RS_PropertyOwner* e=propertyOwnerContainer->firstPropertyOwner();
            e!=NULL;
            e = propertyOwnerContainer->nextPropertyOwner()) {

        if (e->isSelectedForPropertyEditing()) {
            QStringList propertyIds = e->getPropertyNames(true);
            removeAllButThese(propertyIds);
        }
    }

    singlePropertyOwnerMode = false;
    propertyOwner = NULL;
    this->propertyOwnerContainer = propertyOwnerContainer;

    updateGui();
    
    updatesDisabled = false;
    
    RS_DEBUG->print("QG_PropertyEditor::update: OK");
}



/**
 * Updates the GUI tree according to the current state of the 'combinedProperties'.
 */
void QG_PropertyEditor::updateGui() {
    RS_DEBUG->print("QG_PropertyEditor::updateGui");
    setUpdatesEnabled(false);

    variantManager->clear();
    enumManager->clear();

    // update GUI:
    for (int i=0; i<groupOrder.count(); ++i) {
        RS_DEBUG->print("QG_PropertyEditor::updateGui: i: %d", i);

        QString groupKey = groupOrder.at(i);
        RS_DEBUG->print(QString("%1").arg(groupKey));

        QtVariantProperty* groupProperty = NULL;
        
        if (groupKey.isEmpty()==false) {
            groupProperty = variantManager->addProperty(
                QtVariantPropertyManager::groupTypeId(),
                groupKey);
            addProperty(groupProperty);
        }
        
        if (!propertyOrder.contains(groupKey) ||
            !combinedProperties.contains(groupKey)) {
            RS_DEBUG->print("QG_PropertyEditor::updateGui: "
                "key '%s' not found", (const char*)groupKey.toLatin1());
            continue;
        }

        for (int k=0; k<propertyOrder[groupKey].count(); ++k) {
            RS_DEBUG->print("QG_PropertyEditor::updateGui: k: %d", k);

            QString propertyKey = propertyOrder[groupKey].at(k);
            RS_DEBUG->print("QG_PropertyEditor::updateGui: key: %s", (const char*)propertyKey.toLatin1());

            if (!combinedProperties.contains(groupKey) || 
                !combinedProperties[groupKey].contains(propertyKey)) {
                RS_DEBUG->print("QG_PropertyEditor::updateGui: "
                    "key '%s'/'%s' not found", 
                    (const char*)groupKey.toLatin1(),
                    (const char*)propertyKey.toLatin1());
                continue;
            }

            QVariant v; 
            if (combinedProperties[groupKey][propertyKey].atts.isAngle) {
                v = RS_Math::rad2deg(
                        combinedProperties[groupKey][propertyKey].value.toDouble()
                    );
            }
            else {
                v = combinedProperties[groupKey][propertyKey].value;
            }
            QStringList choices = 
                combinedProperties[groupKey][propertyKey].atts.choices;

            RS_DEBUG->print(
                QString("   %1: '%2'").arg(propertyKey).arg(v.toString()));

            QtProperty* property;

            // add property control as variant editor:
            if (choices.count()==0) {
                RS_DEBUG->print("QG_PropertyEditor::updateGui: "
                    "add property control as variant editor");
                property = variantManager->addProperty(v.type(), propertyKey);
                if (property!=NULL) {
                    ((QtVariantProperty*)property)->setValue(v);
                }
            }
            
            // add property control as enum editor (combo box):
            else {
                RS_DEBUG->print("QG_PropertyEditor::updateGui: "
                    "add property control as enum editor");
                property = enumManager->addProperty(propertyKey);
                RS_DEBUG->print("QG_PropertyEditor::updateGui: "
                    "value for '%s' is: '%s'", 
                    (const char*)propertyKey.toLatin1(),
                    (const char*)v.toString().toLatin1());

                if (property!=NULL) {
                    choices.sort();
                    int cIndex = choices.indexOf(v.toString());
                    if (cIndex!=-1) {
                        enumManager->setEnumNames(property, choices);
                        enumManager->setValue(property, cIndex);
                    }
                    else {
                        // add value that is not yet in choices to possible choices:
                        choices << v.toString();
                        choices.sort();
                        enumManager->setEnumNames(property, choices);
                        cIndex = choices.indexOf(v.toString());
                        enumManager->setValue(property, cIndex);
                    }
                }
                RS_DEBUG->print("QG_PropertyEditor::updateGui: enum editor added");
            }
                
            if (property!=NULL) {
                //variantManager->setAttribute(property, "decimals", 4);
                if (groupProperty!=NULL) {
                    groupProperty->addSubProperty(property);
                }
                else {
                    addProperty(property);
                }
            }
        }
    }
        
    setUpdatesEnabled(true);

    //combinedProperties.clear();
    groupOrder.clear();
    propertyOrder.clear();

    guiUpToDate = true;
    
    RS_DEBUG->print("QG_PropertyEditor::updateGui: OK");
}



/**
 * Updates the property widget to include the given propertyOwner.
 */
void QG_PropertyEditor::updateFor(RS_PropertyOwner* e, bool includeGeometry) {
    RS_DEBUG->print("QG_PropertyEditor::updateFor");
    if (e==NULL) {
        return;
    }
        
    QStringList propertyIds = e->getPropertyNames(includeGeometry);
    for (int i=0; i<propertyIds.size(); ++i) {
        QString propertyId = propertyIds.at(i);
        if (e->getPropertyAttributes(propertyId).isVisible) {
            setProperty(propertyId, e->getProperty(propertyId), 
                e->getPropertyAttributes(propertyId));
            RS_DEBUG->print(
                QString("QG_PropertyEditor::updateFor: "
                "setting property: '%1' to '%2'")
                .arg(propertyId).arg(e->getProperty(propertyId).toString()));
        }
    }
    
    singlePropertyOwnerMode = true;
    propertyOwner = e;
    propertyOwnerContainer = NULL;
}



/**
 * Called whenever the user has changed a browser item that represents 
 * a property.
 */
void QG_PropertyEditor::itemChanged(QtBrowserItem* item) {
    QtTreePropertyBrowser::itemChanged(item);

    if (updatesDisabled==true || item==NULL) {
        return;
    }

    QString propertyName = getPropertyName(item);
    QVariant propertyValue = getPropertyValue(item);

    RS_PropertyId pid(propertyName);
    
    RS_DEBUG->print("QG_PropertyEditor::itemChanged: %s: %s. isAngle: %d", 
        (const char*)propertyName.toLatin1(), 
        (const char*)propertyValue.toString().toLatin1(),
        combinedProperties[pid.groupTitle][pid.propertyTitle].atts.isAngle);

    if (propertyName=="") {
        return;
    }
    
    // only change one entity that is currently being drawn. no undo / redo:
    if (singlePropertyOwnerMode) {
        if (propertyOwner==NULL) {
            return;
        }

        propertyOwner->setProperty(propertyName, propertyValue);
        setPropertyValue(item, propertyOwner->getProperty(propertyName));
        emit signalPropertyChanged(propertyOwner, propertyName, propertyValue);
    }

    // change properties of all selected entities with undo / redo handling:
    else {
        if (propertyOwnerContainer==NULL /*|| graphicView==NULL*/) {
            return;
        }

        propertyOwnerContainer->changeProperty(propertyName, propertyValue);
        
        emit signalPropertyChanged(propertyOwnerContainer, propertyName, propertyValue);
    }
    
    RS_DEBUG->print("QG_PropertyEditor::itemChanged: OK");
}

    

/**
 * @return True if the given item contains a variant property.
 */
bool QG_PropertyEditor::isVariantProperty(QtBrowserItem* item) {
    QtAbstractPropertyManager* manager = item->property()->propertyManager();
    if (QString(manager->metaObject()->className())=="QtVariantPropertyManager") {
        return true;
    }
    return false;
}


/**
 * @return True if the given item is a group item.
 */
bool QG_PropertyEditor::isGroupProperty(QtBrowserItem* item) {
    if (isVariantProperty(item)) {
        QtProperty* p = variantManager->variantProperty(item->property());
        if (p!=NULL) {
            QtVariantProperty* vp = (QtVariantProperty*)p;
            if (vp->propertyType()==QtVariantPropertyManager::groupTypeId()) {
                return true;
            }
        }
    }

    return false;
}

    

/**
 * @return True if the value of the given item is mixed.
 */
bool QG_PropertyEditor::isValueMixed(QtBrowserItem* item) {
    QtProperty* p = item->property();
    if (p->valueText()==mixedProperty.toString()) {
        return true;
    }

    return false;
}



/**
 * @return Property name for the given item.
 */
QString QG_PropertyEditor::getPropertyName(QtBrowserItem* item) {
    if (isGroupProperty(item)) {
        return "";
    }
    
    if (isValueMixed(item)) {
        return "";
    }

    QString pName;
    QtProperty* p = item->property();
    QtBrowserItem* parent = item->parent();
    if (parent==NULL) {
        pName = p->propertyName();
    }
    else {
        pName = QString("%1%2%3")
            .arg(parent->property()->propertyName())
            .arg(separator)
            .arg(p->propertyName());
    }
    
    RS_DEBUG->print("QG_PropertyEditor::getPropertyName: "
        "pName: %s", (const char*)pName.toLatin1());

    return pName;
}



/**
 * @return Property value for the given item.
 */
QVariant QG_PropertyEditor::getPropertyValue(QtBrowserItem* item) {
    // find out complete property id:
    if (isGroupProperty(item)) {
        return "";
    }
    
    if (isValueMixed(item)) {
        return "";
    }
    
    if (isVariantProperty(item)) {
        QtVariantProperty* p = variantManager->variantProperty(item->property());
        RS_PropertyOwner::debugProperty("QG_PropertyEditor::getPropertyValue 1", "", p->value());
        return p->value();
    }
    else {
        QtProperty* p = item->property();
        RS_PropertyOwner::debugProperty("QG_PropertyEditor::getPropertyValue 2", "", p->valueText());
        return p->valueText();
    }
}



void QG_PropertyEditor::setPropertyValue(QtBrowserItem* item, QVariant value) {
    RS_DEBUG->print("QG_PropertyEditor::setPropertyValue: "
        "'%s'", (const char*)value.toString().toLatin1());
    
    if (isGroupProperty(item)) {
        return;
    }
    
    if (isValueMixed(item)) {
        return;
    }
    
    if (isVariantProperty(item)) {
        QtVariantProperty* p = variantManager->variantProperty(item->property());
        p->setValue(value);
    }
}



/**
 * @return Parsed values from the given property id string 
 * (e.g. 'Wall|Type[Exterior|Interior]' parses into groupTitle: 'Wall', propertyTitle: 'Type',
 * enumValues: 'Exterior', 'Interior').
 */
/*
RS_PropertyId QG_PropertyEditor::parsePropertyId(const QString& propertyId) {
    RS_DEBUG->print("QG_PropertyEditor::parsePropertyId: %s", 
        (const char*)propertyId.toLatin1());
    RS_PropertyId ret;
    
    QStringList nameList = propertyId.split('|');
    if (nameList.count()>1) {
        ret.groupTitle = nameList[0];
        ret.propertyTitle = nameList[nameList.count()-1];
    }
    else {
        ret.groupTitle = "";
        ret.propertyTitle = nameList[0];
    }
    
    RS_DEBUG->print("QG_PropertyEditor::parsePropertyId: "
        "groupTitle: %s", (const char*)ret.groupTitle.toLatin1());
    RS_DEBUG->print("QG_PropertyEditor::parsePropertyId: "
        "propertyTitle: %s", (const char*)ret.propertyTitle.toLatin1());

    RS_DEBUG->print("QG_PropertyEditor::parsePropertyId: OK");
    return ret;
}
*/



/**
 * Escape releases focus.
 */
void QG_PropertyEditor::keyPressEvent(QKeyEvent* e) {
    switch (e->key()) {
    case Qt::Key_Escape:
        emit escape();
        break;
        
    default:
        QtTreePropertyBrowser::keyPressEvent(e);
        break;
    }
}

#endif
