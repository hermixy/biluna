/*****************************************************************
 * $Id: rb_databasetablewidget.h 0001 2016-01-24T16:47:45 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "rb_databasetablewidget.h"
#include "rb_dialogfactory.h"
#include "rb_dialogwindow.h"
#include "rb_modelfactory.h"
#include "ui_rb_databasetablewidget.h"
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>



RB_DatabaseTableWidget::RB_DatabaseTableWidget(QWidget *parent)
    : RB_Widget(parent), ui(new Ui::RB_DatabaseTableWidget) {

    ui->setupUi(this);

    mCodeManufId = "";
    mTypeId = "";
    mChart = nullptr;

    init();
}

RB_DatabaseTableWidget::~RB_DatabaseTableWidget() {
    delete ui;
}

void RB_DatabaseTableWidget::init() {
    ui->tbbData->initSlimTable(true, false, true);
}

void RB_DatabaseTableWidget::setCodeManufacturer(RB_ObjectBase *obj) {
    if (!obj) {
        mCodeManufId = "";
        ui->leManuf->clear();
    } else {
        mCodeManufId = obj->getId();
        ui->leManuf->setText(obj->getValue("manufacturer").toString());
    }
}

void RB_DatabaseTableWidget::setType(RB_ObjectBase *obj) {
    if (!obj) {
        mTypeId = "";
        ui->leType->clear();
    } else {
        mTypeId = obj->getId();
        ui->leType->setText(obj->getValue("type").toString());
    }
}

void RB_DatabaseTableWidget::setChartModel(RB_MmProxy* model,
                                           const QString& xField,
                                           const QString& yField,
                                           ScaleType scale) {
    if (mChart) {
        delete mChart;
    }

    if (!model) {
        return;
    }

    mChart = new QChart;
    mChart->setAnimationOptions(QChart::AllAnimations);

    // series 1
    QLineSeries* series = new QLineSeries(mChart);
    series->setName("Line 1");
    QVXYModelMapper* mapper = new QVXYModelMapper(mChart);
    mapper->setXColumn(model->fieldIndex(xField));
    mapper->setYColumn(model->fieldIndex(yField));
    mapper->setSeries(series);
    mapper->setModel(model);
    mChart->addSeries(series);

    // for storing color hex from the series
//    QString seriesColorHex = "#000000";

    // get the color of the series and use it for showing the mapped area
//    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
//    model->addMapping(seriesColorHex, QRect(0, 0, 2, model->rowCount()));

    // series 2
//    series = new QLineSeries;
//    series->setName("Line 2");

//    mapper = new QVXYModelMapper(this);
//    mapper->setXColumn(2);
//    mapper->setYColumn(3);
//    mapper->setSeries(series);
//    mapper->setModel(model);
//    mChart->addSeries(series);

//    // get the color of the series and use it for showing the mapped area
//    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
//    model->addMapping(seriesColorHex, QRect(2, 0, 2, model->rowCount()));

    //    mChart->legend()->hide();
    mChart->setTitle(yField + "values");


    // Axis
    QAbstractAxis* axisX;

    if (scale == ScaleLinear || scale == ScaleYLog) {
        axisX = new QValueAxis(mChart);
    } else {
        axisX = new QLogValueAxis(mChart);
    }
//    axisX->setLabelFormat("%g");
    axisX->setTitleText(xField + "Data point");
    mChart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QAbstractAxis* axisY;

    if (scale == ScaleLinear || scale == ScaleXLog) {
        axisY = new QValueAxis(mChart);
    } else {
        axisY = new QLogValueAxis(mChart);
    }

//    axisY->setLabelFormat("%g");
    axisY->setTitleText(yField + "Values");
    mChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView* chartView = new QChartView(mChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // mChart layout
    QGridLayout* chartLayout = new QGridLayout;
    chartLayout->addWidget(chartView, 0, 0);
    chartLayout->setMargin(0);
    ui->chartFrame->setLayout(chartLayout);
}
