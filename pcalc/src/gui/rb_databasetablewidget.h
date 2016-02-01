/*****************************************************************
 * $Id: rb_databasetablewidget.h 0001 2016-01-24T16:47:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#ifndef RB_DATABASETABLEWIDGET_H
#define RB_DATABASETABLEWIDGET_H

#include "rb_widget.h"


namespace Ui {
class RB_DatabaseTableWidget;
}

class RB_DatabaseTableWidget : public RB_Widget {

    Q_OBJECT

public:
    RB_DatabaseTableWidget(QWidget* parent = 0);
    virtual ~RB_DatabaseTableWidget();

    void init();
    void setCodeManufacturer(RB_ObjectBase* obj);
    void setType(RB_ObjectBase* obj);

public slots:
    virtual void on_pbSelectManuf_clicked() = 0;
    virtual void on_pbClearManuf_clicked() = 0;
    virtual void on_pbSelectType_clicked() = 0;
    virtual void on_pbClearType_clicked() = 0;

protected:
    QString mCodeManufId;
    QString mTypeId;

private:
    Ui::RB_DatabaseTableWidget* ui;

};


#include <QtCore/QAbstractTableModel>
#include <QtCore/QHash>
#include <QtCore/QRect>

class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CustomTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addMapping(QString color, QRect area);
    void clearMapping() { m_mapping.clear(); }

private:
    QList<QVector<qreal> * > m_data;
    QHash<QString, QRect> m_mapping;
    int m_columnCount;
    int m_rowCount;
};

#endif // RB_DATABASETABLEWIDGET_H
