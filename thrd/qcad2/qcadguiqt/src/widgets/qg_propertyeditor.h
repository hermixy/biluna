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

#ifndef QG_PROPERTYEDITOR_H
#define QG_PROPERTYEDITOR_H


#include <QKeyEvent>
#include <QWidget>
#include <QVariant>
#include <QHash>
#include <QString>

#include <QtTreePropertyBrowser>

#include "rs.h"
#include "rs_propertyowner.h"

class QTableWidget;
class QtAbstractPropertyBrowser;
class QtBrowserItem;
class QtProperty;
class QtVariantEditorFactory;
class QtEnumEditorFactory;
class QtVariantPropertyManager;
class QtEnumPropertyManager;

class RS_PropertyOwnerContainerInterface;
    
    
    
/*
class QG_UniquePropertyHash : public QHash<QString, QVariant> {
public:
    RS_PropertyAttributes atts;
};
*/



class QG_Variant {
public:
    QVariant value;
    //QStringList choices;
    RS_PropertyAttributes atts;
};

/**
 * A property editor.
 */
class QG_PropertyEditor : public QtTreePropertyBrowser {
    Q_OBJECT

public:
    QG_PropertyEditor(QWidget* parent=0);
    virtual ~QG_PropertyEditor();

    void setProperty(const QString& propertyId, 
        const QVariant& value, const RS_PropertyAttributes& atts);
    void removeAllButThese(const QStringList& propertyIds);

    void update(RS_PropertyOwnerContainerInterface* propertyOwnerContainer, bool includeGeometry=true);
    void updateGui();
    void updateFor(RS_PropertyOwner* propertyOwner, bool includeGeometry=true);

    bool isVariantProperty(QtBrowserItem* item);
    bool isGroupProperty(QtBrowserItem* item);
    bool isValueMixed(QtBrowserItem* item);
    QString getPropertyName(QtBrowserItem* item);
    QVariant getPropertyValue(QtBrowserItem* item);
    void setPropertyValue(QtBrowserItem* item, QVariant value);

    //static RS_PropertyId parsePropertyId(const QString& propertyId);

protected:
    virtual void keyPressEvent(QKeyEvent* e);

signals:
    void escape();
    void signalPropertyChanged(RS_PropertyOwner* propertyOwner, 
        const QString& name, const QVariant& value);
    void signalPropertyChanged(RS_PropertyOwnerContainerInterface* propertyOwnerContainer, 
        const QString& name, const QVariant& value);

protected:
    virtual void itemChanged(QtBrowserItem* item);

private:
    QStringList groupOrder;
    QHash<QString, QStringList> propertyOrder;
    typedef QHash<QString, QG_Variant> QG_UniquePropertyHash;
    typedef QHash<QString, QG_UniquePropertyHash> QG_UniquePropertyGroupHash;
    QG_UniquePropertyGroupHash combinedProperties;
    bool guiUpToDate;

    QtVariantPropertyManager* variantManager;
    QtEnumPropertyManager* enumManager;
    QtVariantEditorFactory* variantFactory;
    QtEnumEditorFactory* enumFactory;
    bool updatesDisabled;
    QVariant mixedProperty;
    bool singlePropertyOwnerMode;
    RS_PropertyOwner* propertyOwner;
    RS_PropertyOwnerContainerInterface* propertyOwnerContainer;
    
    static char separator;
};

#endif

#endif
