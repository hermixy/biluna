/*****************************************************************
 * $Id: sail_polardiagramwidget.cpp 2166 2014-09-25 13:23:18Z rutger $
 * Created: Aug 22, 2013 13:00:25 AM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna SAIL project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "sail_polardiagramwidget.h"

//#include "qareaseries.h"
//#include "qlineseries.h"
//#include "qpolarchart.h"
//#include "qscatterseries.h"
//#include "qsplineseries.h"
//#include <QValueAxis>

//QTCOMMERCIALCHART_USE_NAMESPACE // is QtCommercialChart:: namespace

SAIL_PolarDiagramWidget::SAIL_PolarDiagramWidget(QWidget* parent)
    : RB_Widget(parent) {


}

SAIL_PolarDiagramWidget::~SAIL_PolarDiagramWidget() {

}

RB_String SAIL_PolarDiagramWidget::getSaveAsFileName() {
    return QString();
}

void SAIL_PolarDiagramWidget::init() {
    /*
    const qreal angularMin = 0;
    const qreal angularMax = 360;

    const qreal radialMin = 0;
    const qreal radialMax = 10;

    QScatterSeries *series1 = new QScatterSeries();
    series1->setName("scatter");
//    for (int i = angularMin; i <= angularMax; i += 10)
//        series1->append(i, (i / radialMax) * radialMax + 8.0);
    series1->append(30.0, 4.0); // apparent wind direction
    series1->append(45.0, 5.5);
    series1->append(90.0, 6.5);
    series1->append(135.0, 7.0);
    series1->append(180.0, 6.0);

    QSplineSeries *series2 = new QSplineSeries();
    series2->setName("spline");
//    for (int i = angularMin; i <= angularMax; i += 10)
//        series2->append(i, (i / radialMax) * radialMax);
    series2->append(30.0, 4.0); // apparent wind direction
    series2->append(45.0, 5.5);
    series2->append(90.0, 6.5);
    series2->append(135.0, 7.0);
    series2->append(180.0, 6.0);

    QLineSeries *series3 = new QLineSeries();
    series3->setName("star outer");
    qreal ad = (angularMax - angularMin) / 8;
    qreal rd = (radialMax - radialMin) / 3 * 1.3;
    series3->append(angularMin, radialMax);
    series3->append(angularMin + ad*1, radialMin + rd);
    series3->append(angularMin + ad*2, radialMax);
    series3->append(angularMin + ad*3, radialMin + rd);
    series3->append(angularMin + ad*4, radialMax);
    series3->append(angularMin + ad*5, radialMin + rd);
    series3->append(angularMin + ad*6, radialMax);
    series3->append(angularMin + ad*7, radialMin + rd);
    series3->append(angularMin + ad*8, radialMax);

    QLineSeries *series4 = new QLineSeries();
    series4->setName("star inner");
    ad = (angularMax - angularMin) / 8;
    rd = (radialMax - radialMin) / 3;
    series4->append(angularMin, radialMax);
    series4->append(angularMin + ad*1, radialMin + rd);
    series4->append(angularMin + ad*2, radialMax);
    series4->append(angularMin + ad*3, radialMin + rd);
    series4->append(angularMin + ad*4, radialMax);
    series4->append(angularMin + ad*5, radialMin + rd);
    series4->append(angularMin + ad*6, radialMax);
    series4->append(angularMin + ad*7, radialMin + rd);
    series4->append(angularMin + ad*8, radialMax);

    QAreaSeries *series5 = new QAreaSeries();
    series5->setName("star area");
    series5->setUpperSeries(series3);
    series5->setLowerSeries(series4);
    series5->setOpacity(0.5);

    mChart = new QPolarChart();
    mChart->addSeries(series1);
    mChart->addSeries(series2);
    // mChart->addSeries(series3);
    // mChart->addSeries(series4);
    // mChart->addSeries(series5);

    mChart->setTitle("Boat speed in knots at apparent wind");

    QValueAxis *angularAxis = new QValueAxis();
    angularAxis->setTickCount(25); // First and last ticks are co-located on 0/360 angle.
    angularAxis->setLabelFormat("%.1f");
    angularAxis->setShadesVisible(true);
    angularAxis->setShadesBrush(QBrush(QColor(249, 249, 255)));
    mChart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setTickCount(10);
    radialAxis->setLabelFormat("%d");
    mChart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);

    series1->attachAxis(radialAxis);
    series1->attachAxis(angularAxis);
    series2->attachAxis(radialAxis);
    series2->attachAxis(angularAxis);
    series3->attachAxis(radialAxis);
    series3->attachAxis(angularAxis);
    series4->attachAxis(radialAxis);
    series4->attachAxis(angularAxis);
    series5->attachAxis(radialAxis);
    series5->attachAxis(angularAxis);

    radialAxis->setRange(radialMin, radialMax);
    angularAxis->setRange(angularMin, angularMax);

    mChartView = new QChartView();
    mChartView->setChart(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(mChartView);
    this->setLayout(layout);
    */
}

void SAIL_PolarDiagramWidget::keyPressEvent(QKeyEvent *event)
{
    return;
    /*
    switch (event->key()) {
    case Qt::Key_Plus:
        mChart->zoomIn();
        break;
    case Qt::Key_Minus:
        mChart->zoomOut();
        break;
    case Qt::Key_Left:
        mChart->scroll(-1.0, 0);
        break;
    case Qt::Key_Right:
        mChart->scroll(1.0, 0);
        break;
    case Qt::Key_Up:
        mChart->scroll(0, 1.0);
        break;
    case Qt::Key_Down:
        mChart->scroll(0, -1.0);
        break;
    case Qt::Key_Space:
        switchChartType();
        break;
    default:
        break;
    }
    */
}

void SAIL_PolarDiagramWidget::switchChartType()
{
    /*
    QChart *newChart;
    QChart *oldChart = mChart;

    if (oldChart->chartType() == QChart::ChartTypeCartesian)
        newChart = new QPolarChart();
    else
        newChart = new QChart();

    // Move series and axes from old chart to new one
    QList<QAbstractSeries *> seriesList = oldChart->series();
    QList<QAbstractAxis *> axisList = oldChart->axes();
    QList<QPair<qreal, qreal> > axisRanges;

    foreach (QAbstractAxis *axis, axisList) {
        QValueAxis *valueAxis = static_cast<QValueAxis *>(axis);
        axisRanges.append(QPair<qreal, qreal>(valueAxis->min(), valueAxis->max()));
    }

    foreach (QAbstractSeries *series, seriesList)
        oldChart->removeSeries(series);

    foreach (QAbstractAxis *axis, axisList) {
        oldChart->removeAxis(axis);
        newChart->addAxis(axis, axis->alignment());
    }

    foreach (QAbstractSeries *series, seriesList) {
        newChart->addSeries(series);
        foreach (QAbstractAxis *axis, axisList)
            series->attachAxis(axis);
    }

    int count = 0;
    foreach (QAbstractAxis *axis, axisList) {
        axis->setRange(axisRanges[count].first, axisRanges[count].second);
        count++;
    }

    newChart->setTitle(oldChart->title());
    mChartView->setChart(newChart);
    delete oldChart;
    */
}

/**
 * Get help subject
 * @return help subject (name of widget)
 */
QString SAIL_PolarDiagramWidget::getHelpSubject() const {
    return objectName();
}

bool SAIL_PolarDiagramWidget::closeWidget() {
    RB_Widget::closeWidget();
    if (maybeSave()) {
        return true;
    }
    return false;
}
