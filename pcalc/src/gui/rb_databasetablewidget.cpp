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
    mSeries = nullptr;
    mMapper = nullptr;
    mAxisX = nullptr;
    mAxisY = nullptr;
    mChartView = nullptr;
    mChartLayout = nullptr;

    mModel = nullptr;
    mScaleType = ScaleLinear;
    mXfield = "";
    mYfield = "";

    init();
}

RB_DatabaseTableWidget::~RB_DatabaseTableWidget() {
    delete ui;
}

void RB_DatabaseTableWidget::init() {
    ui->tbbData->initSlimTable(true, false, true);
    ui->tableView->setToolButtonBar(ui->tbbData);

    connect(ui->cbProperty, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotSetPropertyTable(int)));
    connect(ui->cbXcoord, SIGNAL(currentTextChanged(QString)),
            this, SLOT(slotSetXcoord(QString)));
    connect(ui->cbYcoord, SIGNAL(currentTextChanged(QString)),
            this, SLOT(slotSetYcoord(QString)));
}

void RB_DatabaseTableWidget::setCodeManufacturer(RB_ObjectBase *obj) {
    if (!obj) {
        mCodeManufId = "";
        ui->leManuf->clear();
    } else {
        mCodeManufId = obj->getId();
        ui->leManuf->setText(obj->getValue("manufacturer").toString());
    }

    mModel = nullptr;
}

void RB_DatabaseTableWidget::setType(RB_ObjectBase *obj) {
    if (!obj) {
        mTypeId = "";
        ui->leType->clear();
    } else {
        mTypeId = obj->getId();
        ui->leType->setText(obj->getValue("type").toString());
    }

    mModel = nullptr;
}

void RB_DatabaseTableWidget::setChartModel(RB_MmProxy* model,
                                           const QString& xField,
                                           const QString& yField,
                                           ScaleType scale) {
    if (!mChart) {
        mChart = new QChart();
        mChart->setAnimationOptions(QChart::AllAnimations);
    }

    if (!model) {
        return;
    } else {
        mModel = model;
    }

    // data series
    if (mSeries && mMapper) {
        delete mSeries;
        delete mMapper;
    }
    mScaleType = scale;
    mXfield = xField;
    mYfield = yField;

    mSeries = new QLineSeries(mChart);
    mSeries->setName(yField + " " + tr("graph"));
    mMapper = new QVXYModelMapper(mChart);
    mMapper->setXColumn(model->fieldIndex(xField));
    mMapper->setYColumn(model->fieldIndex(yField));
    mMapper->setSeries(mSeries);
    mMapper->setModel(model);
    mChart->addSeries(mSeries);

    // for storing color hex from the series
    //    QString seriesColorHex = "#000000";

    // get the color of the series and use it for showing the mapped area
    //    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    //    model->addMapping(seriesColorHex, QRect(0, 0, 2, model->rowCount()));

    mChart->setTitle(yField + " " + tr("values"));


    // Axis
    if (mAxisX || mAxisY) {
        delete mAxisX;
        mAxisX = nullptr;
        delete mAxisY;
        mAxisY = nullptr;
    }

    if (scale == ScaleLinear || scale == ScaleYLog) {
        QValueAxis* axX = new QValueAxis(mChart);
        axX->setLabelFormat("%g");
        mAxisX = axX;
    } else {
        QLogValueAxis* axX = new QLogValueAxis(mChart);
        axX->setLabelFormat("%g");
        mAxisX = axX;
    }
    mAxisX->setTitleText(xField);
    mChart->addAxis(mAxisX, Qt::AlignBottom);
    mSeries->attachAxis(mAxisX);

    if (scale == ScaleLinear || scale == ScaleXLog) {
        QValueAxis* axY = new QValueAxis(mChart);
        axY->setLabelFormat("%g");
        mAxisY = axY;
    } else {
        QLogValueAxis* axY = new QLogValueAxis(mChart);
        axY->setLabelFormat("%g");
        mAxisY = axY;
    }

    mAxisY->setTitleText(yField);
    mChart->addAxis(mAxisY, Qt::AlignLeft);
    mSeries->attachAxis(mAxisY);

    // Chart viewer
    if (!mChartView) {
        mChartView = new QChartView(mChart);
        mChartView->setRenderHint(QPainter::Antialiasing);
    }

    // Layout
    if (!mChartLayout) {
        mChartLayout = new QGridLayout;
        mChartLayout->addWidget(mChartView, 0, 0);
        mChartLayout->setMargin(0);
        ui->chartFrame->setLayout(mChartLayout);
    }
}

void RB_DatabaseTableWidget::slotSetXcoord(const QString& currentText) {
    if (!mModel) {
        return;
    }
    setChartModel(mModel, currentText, mYfield, mScaleType);
}

void RB_DatabaseTableWidget::slotSetYcoord(const QString& currentText) {
    if (!mModel) {
        return;
    }
    setChartModel(mModel, mXfield, currentText, mScaleType);
}

