/*****************************************************************
 * $Id: std_selectflangewidget.cpp 0001 2016-07-13T17:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_selectflangewidget.h"

#include "pcalc_modelfactory.h"
#include "std.h"
#include "ui_std_selectcomponentwidget.h"

STD_SelectFlangeWidget::STD_SelectFlangeWidget(QWidget *parent)
        : STD_SelectComponentWidget(parent) {
    setObjectName("STD_SelectFlangeWidget");
    mFlangeObject = nullptr;
}

STD_SelectFlangeWidget::~STD_SelectFlangeWidget() {
    if (mFlangeObject) {
        delete mFlangeObject;
        mFlangeObject = nullptr;
    }
}

/**
 * @brief STD_SelectFlangeWidget::getCurrentObject
 * @return Flange object with additional displayname
 */
RB_ObjectBase* STD_SelectFlangeWidget::getCurrentObject() {
    if (!ui->tvComponent->currentIndex().isValid()) {
        return nullptr;
    }

    RB_ObjectBase* obj = mComponentModel->getCurrentObject();
    QString displayName = ui->cbStandard->currentText() + " "
            + obj->getValue("nomsize").toString() + " "
            + ui->cbType->currentText() + " "
            + ui->cbRating->currentText() + " "
            + ui->cbSerie->currentText();

    if (!mFlangeObject) {
        mFlangeObject = new RB_ObjectAtomic(obj);
        mFlangeObject->addMember("displayname", "-", displayName);

    } else {
        // Required? set only once
        *mFlangeObject = *obj;
        mFlangeObject->setValue("displayname", displayName);
    }

    return mFlangeObject;
}

/**
 * @brief STD_SelectFlangeWidget::getCurrentChild1Object
 * @return facing object
 */
RB_ObjectBase* STD_SelectFlangeWidget::getCurrentChild1Object() {
    // continue here other relevant dim data of facing
    if (!ui->tvComponent->currentIndex().isValid()) {
        return nullptr;
    }

    int row = ui->tvComponent->currentIndex().row();
    int col = mComponentModel->fieldIndex("nomsize");
    QModelIndex index = mComponentModel->index(row, col);
    double nomSize = index.data().toDouble();

    col = mEndModel->fieldIndex("nomsize");
    int rowCount = mEndModel->rowCount();

    for (row = 0; row < rowCount; row++) {
        index = mEndModel->index(row,col);

        if (index.data().toDouble() == nomSize) {
            return mEndModel->getObject(index, RB2::ResolveNone);
        }
    }

    return nullptr;
}

/**
 * @brief STD_SelectFlangeWidget::getCurrentChild2Object
 * @return flange type object
 */
RB_ObjectBase *STD_SelectFlangeWidget::getCurrentChild2Object() {
    if (!ui->tvComponent->currentIndex().isValid()) {
        return nullptr;
    }

    return mTypeModel->getCurrentObject();
}

void STD_SelectFlangeWidget::init() {
    mStandardModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelDimension, false);
    mStandardModel->setRoot("");
    mStandardModel->setWhere("id <> '0' AND comptype_id = "
                              + QString::number((int)STD2::CompFlange));
    ui->cbStandard->setModel(mStandardModel);
    ui->cbStandard->setModelColumn(mStandardModel->fieldIndex("code"));
    mStandardModel->select();

    connect(ui->cbStandard, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotStandardRowChanged(int)));

    mTypeModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompType, false);
    ui->cbType->setModel(mTypeModel);
    ui->cbType->setModelColumn(mTypeModel->fieldIndex("type"));

    connect(ui->cbType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotTypeRowChanged(int)));

    mSerieModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompSerie, false);
    ui->cbSerie->setModel(mSerieModel);
    ui->cbSerie->setModelColumn(mSerieModel->fieldIndex("type"));
    ui->lblSerie->setText("Facing");

    mRatingModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompRating, false);
    ui->cbRating->setModel(mRatingModel);
    ui->cbRating->setModelColumn(mRatingModel->fieldIndex("rating"));

    connect(ui->cbRating, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotRatingRowChanged(int)));

    // flange model
    mComponentModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeEn, false);
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

    // facing model not shown but for data only
    mEndModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeFacingDimEn, false);


    for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
        ui->tvComponent->hideColumn(i);
    }

    initWidgets();
    readSettings();
}

void STD_SelectFlangeWidget::slotStandardRowChanged(int row) {
    if (!mStandardModel || !mTypeModel || !mSerieModel) return;
    QModelIndex index = mStandardModel->index(row, 0);
    STD_DimensionTableHelper::setFlangeDetailTables(index, QModelIndex());
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
    mTypeModel->slotChangeCurrentRow(index, QModelIndex());
    // TODO: set limits in tvComponent
}

void STD_SelectFlangeWidget::slotRatingRowChanged(int row) {
    if (!mRatingModel || !mComponentModel) return;
    QModelIndex index = mRatingModel->index(row, 0);
    mComponentModel->slotParentCurrentRowChanged(index, QModelIndex());
    mEndModel->slotParentCurrentRowChanged(index, QModelIndex());
}
