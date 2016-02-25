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
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>


namespace Ui {
class RB_DatabaseTableWidget;
}

using namespace QtCharts;

class RB_DatabaseTableWidget : public RB_Widget {

    Q_OBJECT

public:
    enum ScaleType {
        ScaleLinear,
        ScaleXLog,
        ScaleYLog,
        ScaleBothLog
    };

    RB_DatabaseTableWidget(QWidget* parent = 0);
    virtual ~RB_DatabaseTableWidget();

    void init();
    void setCodeManufacturer(RB_ObjectBase* obj);
    void setType(RB_ObjectBase* obj);
    void setChartModel(RB_MmProxy* model, const QString& xField,
                       const QString& yField, ScaleType scale);


public slots:
    virtual void on_pbSelectManuf_clicked() = 0;
    virtual void on_pbClearManuf_clicked() = 0;
    virtual void on_pbSelectType_clicked() = 0;
    virtual void on_pbClearType_clicked() = 0;

protected:
    QString mCodeManufId;
    QString mTypeId;

// private:
    Ui::RB_DatabaseTableWidget* ui;

    QChart* mChart;
    QLineSeries* mSeries;
    QVXYModelMapper* mMapper;
    QAbstractAxis* mAxisX;
    QAbstractAxis* mAxisY;
    QChartView* mChartView;
    QGridLayout* mChartLayout;

};
#endif // RB_DATABASETABLEWIDGET_H
