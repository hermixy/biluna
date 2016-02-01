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
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QVXYModelMapper>
#include <QtWidgets/QHeaderView>


using namespace QtCharts;

RB_DatabaseTableWidget::RB_DatabaseTableWidget(QWidget *parent)
    : RB_Widget(parent), ui(new Ui::RB_DatabaseTableWidget) {

    ui->setupUi(this);

    mCodeManufId = "";
    mTypeId = "";

    init();
}

RB_DatabaseTableWidget::~RB_DatabaseTableWidget() {
    delete ui;
}

void RB_DatabaseTableWidget::init() {
    ui->tbbData->initSlimTable(true, false, true);

//    ui->wgtChart = nullptr;



    // create simple model for storing data
    // user's table data model
    CustomTableModel *model = new CustomTableModel;

    // create table view and add model to it
    QTableView *tableView = new QTableView;
    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QChart *chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);

    // series 1
    QLineSeries *series = new QLineSeries;
    series->setName("Line 1");
    QVXYModelMapper *mapper = new QVXYModelMapper(this);
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setSeries(series);
    mapper->setModel(model);
    chart->addSeries(series);

    // for storing color hex from the series
    QString seriesColorHex = "#000000";

    // get the color of the series and use it for showing the mapped area
    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    model->addMapping(seriesColorHex, QRect(0, 0, 2, model->rowCount()));

    // series 2
    series = new QLineSeries;
    series->setName("Line 2");

    mapper = new QVXYModelMapper(this);
    mapper->setXColumn(2);
    mapper->setYColumn(3);
    mapper->setSeries(series);
    mapper->setModel(model);
    chart->addSeries(series);
//    chart->legend()->hide();
    chart->setTitle("Logarithmic axis example");

    // get the color of the series and use it for showing the mapped area
    seriesColorHex = "#" + QString::number(series->pen().color().rgb(), 16).right(6).toUpper();
    model->addMapping(seriesColorHex, QRect(2, 0, 2, model->rowCount()));

    // Axis
//    chart->createDefaultAxes();
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Data point");
//    axisX->setTickCount(6);
    axisX->setLabelFormat("%i");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QLogValueAxis *axisY = new QLogValueAxis;
    axisY->setLabelFormat("%g");
    axisY->setTitleText("Values");
//    axisY->setBase(8);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
//    chartView->setMinimumSize(640, 480);

    // chart layout
    QGridLayout* chartLayout = new QGridLayout;
    chartLayout->addWidget(chartView, 0, 0);
    chartLayout->setMargin(0);
    ui->chartFrame->setLayout(chartLayout);
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


#include <QtCore/QVector>
#include <QtCore/QTime>
#include <QtCore/QRect>
#include <QtGui/QColor>

CustomTableModel::CustomTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    m_columnCount = 4;
    m_rowCount = 15;

    // m_data
    for (int i = 0; i < m_rowCount; i++) {
        QVector<qreal>* dataVec = new QVector<qreal>(m_columnCount);
        for (int k = 0; k < dataVec->size(); k++) {
            if (k % 2 == 0)
                dataVec->replace(k, i * 50 + qrand() % 20);
            else
                dataVec->replace(k, qrand() % 100);
        }
        m_data.append(dataVec);
    }
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.count();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section % 2 == 0)
            return "x";
        else
            return "y";
    } else {
        return QString("%1").arg(section + 1);
    }
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::EditRole) {
        return m_data[index.row()]->at(index.column());
    } else if (role == Qt::BackgroundRole) {
        foreach (QRect rect, m_mapping) {
            if (rect.contains(index.column(), index.row()))
                return QColor(m_mapping.key(rect));
        }
        // cell not mapped return white color
        return QColor(Qt::white);
    }
    return QVariant();
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_data[index.row()]->replace(index.column(), value.toDouble());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void CustomTableModel::addMapping(QString color, QRect area)
{
    m_mapping.insertMulti(color, area);
}
