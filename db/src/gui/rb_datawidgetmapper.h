/*****************************************************************
 * $Id: rb_datawidgetmapper.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Sep 1, 2009 4:34:25 PM - rutger
 *
 * Copyright (C) 2009 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_DATAWIDGETMAPPER_H
#define RB_DATAWIDGETMAPPER_H

#include <QDataWidgetMapper>
#include <QMap>
#include <QVector>
#include "db_global.h"

class QComboBox;
class RB_MmProxy;

class DB_EXPORT RB_DataWidgetMapper : public QDataWidgetMapper {

    Q_OBJECT

public:
    RB_DataWidgetMapper(RB_MmProxy* sfpm);
    virtual ~RB_DataWidgetMapper();

    virtual void setRootIndex(const QModelIndex &index);
    virtual void addMapping(QWidget* widget, int section);
    virtual void addMapping(QWidget* widget, int section,
                            const QByteArray& propertyName);
    virtual void removeMapping(QWidget* widget);
    virtual void clearMapping();
    virtual void setProxyModel(RB_MmProxy* sfpm);

public slots:
    void setCurrentModelIndex(const QModelIndex& idx);
    void toPrevious();
    void toNext();

    void slotRemoveMapping(QObject* obj);
    void slotClearWidgets();
    void slotActivateWidgets(int row);

#ifdef Q_OS_MAC
protected slots:
    // HACK: for MAC OSX only
    void slotFixedComboIndexChanged(int index);
    void slotRelationComboIndexChanged(int index);
#endif

private:
    void updateSelectionModel(int row);

    QVector<QObject*> mObjectList;
    RB_MmProxy* mProxyModel;
    /** Map of comboboxes mapped with currentIndex property */
    QMap<QComboBox*, int> mComboBoxMap;

};

#endif // RB_DATAWIDGETMAPPER_H
