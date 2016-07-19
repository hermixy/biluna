/*****************************************************************
 * $Id: std_selectflangewidget.cpp 0001 2016-07-13T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_selectflangewidget.h"

#include "pcalc.h"
#include "pcalc_modelfactory.h"
#include "ui_std_selectcomponentwidget.h"

STD_SelectFlangeWidget::STD_SelectFlangeWidget(QWidget *parent)
        : STD_SelectComponentWidget(parent) {
    // nothing
}

STD_SelectFlangeWidget::~STD_SelectFlangeWidget() {
    // nothing
}

void STD_SelectFlangeWidget::init() {
    mDimensionModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelDimension, false);
    mDimensionModel->setRoot("");
    mDimensionModel->setWhere("id <> '0' AND comptype_id = "
                              + QString::number((int)PCALC2::CompFlange));
    ui->cbStandard->setModel(mDimensionModel);
    ui->cbStandard->setModelColumn(mDimensionModel->fieldIndex("code"));
    mDimensionModel->select();

    mTypeModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeType, false);
    ui->cbType->setModel(mTypeModel);
    ui->cbType->setModelColumn(mTypeModel->fieldIndex("type"));
    connect(ui->cbStandard, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotDimensionRowChanged(int)));

    mSerieModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeFacing, false);
    ui->cbSerie->setModel(mSerieModel);
    ui->cbSerie->setModelColumn(mSerieModel->fieldIndex("type"));
    ui->lblSerie->setText("Facing");

    mRatingModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeRating, false);
    ui->cbRating->setModel(mRatingModel);
    ui->cbRating->setModelColumn(mRatingModel->fieldIndex("rating"));
    connect(ui->cbType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotTypeRowChanged(int)));

    // Continue here: fill flange model in database and create tableView
    mComponentModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlange, false);
    formatTableView(ui->tvComponent, mComponentModel);

    // Hide columns
    int colCount = mComponentModel->columnCount();

    for (int i = 0; i < colCount; ++i) {
        if (i < RB2::HIDDENCOLUMNS || i > RB2::HIDDENCOLUMNS + 4) {
            ui->tvComponent->hideColumn(i);
        } else {
            ui->tvComponent->showColumn(i);

            if (ui->tvComponent->columnWidth(i) < 5) {
                ui->tvComponent->setColumnWidth(i, 100);
            }
        }
    }

    connect(ui->cbRating, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotRatingRowChanged(int)));

    initWidgets();
    readSettings();
}

void STD_SelectFlangeWidget::slotDimensionRowChanged(int row) {
    if (!mDimensionModel || !mTypeModel || !mSerieModel) return;
    QModelIndex index = mDimensionModel->index(row, 0);
    mTypeModel->slotParentCurrentRowChanged(index, QModelIndex());
    mSerieModel->slotParentCurrentRowChanged(index, QModelIndex());
}

void STD_SelectFlangeWidget::slotTypeRowChanged(int row) {
    if (!mTypeModel || !mRatingModel) return;
    QModelIndex index = mTypeModel->index(row, 0);
    mRatingModel->slotParentCurrentRowChanged(index, QModelIndex());
}

void STD_SelectFlangeWidget::slotRatingRowChanged(int row) {
    if (!mRatingModel || !mComponentModel) return;
    QModelIndex index = mRatingModel->index(row, 0);
    mComponentModel->slotParentCurrentRowChanged(index, QModelIndex());
}
