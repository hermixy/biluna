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
    setObjectName("STD_SelectFlangeWidget");
    mCurrentObject = nullptr;
}

STD_SelectFlangeWidget::~STD_SelectFlangeWidget() {
    if (mCurrentObject) {
        delete mCurrentObject;
        mCurrentObject = nullptr;
    }
}

RB_ObjectBase* STD_SelectFlangeWidget::getCurrentObject() {
    if (!ui->tvComponent->currentIndex().isValid()) {
        return nullptr;
    }

    if (!mCurrentObject) {
        mCurrentObject = new RB_ObjectAtomic(mComponentModel->getCurrentObject());
        mCurrentObject->addMember("displayname", "-", ui->cbStandard->currentText() + " "
                                 + ui->cbType->currentText() + " "
                                 + ui->cbRating->currentText() + " "
                                 + ui->cbSerie->currentText());
        // continue here other relevant dim data of facing

    } else {
        // Required? set only once
        mCurrentObject = mComponentModel->getCurrentObject();
        // continue here: set displayname
        // continue here other relevant dim data of facing
    }

    return mCurrentObject;
}

void STD_SelectFlangeWidget::init() {
    mStandardModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelDimension, false);
    mStandardModel->setRoot("");
    mStandardModel->setWhere("id <> '0' AND comptype_id = "
                              + QString::number((int)PCALC2::CompFlange));
    ui->cbStandard->setModel(mStandardModel);
    ui->cbStandard->setModelColumn(mStandardModel->fieldIndex("code"));
    mStandardModel->select();

    connect(ui->cbStandard, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotStandardRowChanged(int)));

    mTypeModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeType, false);
    ui->cbType->setModel(mTypeModel);
    ui->cbType->setModelColumn(mTypeModel->fieldIndex("type"));

    connect(ui->cbType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotTypeRowChanged(int)));

    mSerieModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeFacingType, false);
    ui->cbSerie->setModel(mSerieModel);
    ui->cbSerie->setModelColumn(mSerieModel->fieldIndex("type"));
    ui->lblSerie->setText("Facing");

    mRatingModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRating, false);
    ui->cbRating->setModel(mRatingModel);
    ui->cbRating->setModelColumn(mRatingModel->fieldIndex("rating"));

    connect(ui->cbRating, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotRatingRowChanged(int)));

    // flange model
    mComponentModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlange, false);
    formatTableView(ui->tvComponent, mComponentModel);

    // TODO: Hide columns, EN has the largest number of columns
//    int colCount = mComponentModel->columnCount();

//    for (int i = 0; i < colCount; ++i) {
//        if (i < RB2::HIDDENCOLUMNS || i > RB2::HIDDENCOLUMNS + 4) {
//            ui->tvComponent->hideColumn(i);
//        } else {
//            ui->tvComponent->showColumn(i);

//            if (ui->tvComponent->columnWidth(i) < 5) {
//                ui->tvComponent->setColumnWidth(i, 100);
//            }
//        }
//    }

    for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
        ui->tvComponent->hideColumn(i);
    }

    initWidgets();
    readSettings();
}

void STD_SelectFlangeWidget::slotStandardRowChanged(int row) {
    if (!mStandardModel || !mTypeModel || !mSerieModel) return;
    QModelIndex index = mStandardModel->index(row, 0);
    STD_DimensionTableHelper::setFlangeDetailTableNames(index, QModelIndex());
    mRatingModel->slotParentCurrentRowChanged(index, QModelIndex());
    mTypeModel->slotParentCurrentRowChanged(index, QModelIndex());
    mSerieModel->slotParentCurrentRowChanged(index, QModelIndex());
    formatTableView(ui->tvComponent, mComponentModel);

    // TODO: Hide columns
//    int colCount = mComponentModel->columnCount();

//    for (int i = 0; i < colCount; ++i) {
//        if (i < RB2::HIDDENCOLUMNS || i > RB2::HIDDENCOLUMNS + 4) {
//            ui->tvComponent->hideColumn(i);
//        } else {
//            ui->tvComponent->showColumn(i);

//            if (ui->tvComponent->columnWidth(i) < 5) {
//                ui->tvComponent->setColumnWidth(i, 100);
//            }
//        }
//    }
}

void STD_SelectFlangeWidget::slotTypeRowChanged(int row) {
    if (!mTypeModel || !mRatingModel) return;
    QModelIndex index = mTypeModel->index(row, 0);
    // limits
}

void STD_SelectFlangeWidget::slotRatingRowChanged(int row) {
    if (!mRatingModel || !mComponentModel) return;
    QModelIndex index = mRatingModel->index(row, 0);
    mComponentModel->slotParentCurrentRowChanged(index, QModelIndex());
}
