/*****************************************************************
 * $Id: std_selectgasketwidget.cpp 0001 2016-08-03 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_selectgasketwidget.h"

#include "pcalc_modelfactory.h"
#include "pcalc_sqlcommonfunctions.h"
#include "std.h"
#include "ui_std_selectcomponentwidget.h"

STD_SelectGasketWidget::STD_SelectGasketWidget(QWidget *parent)
        : STD_SelectComponentWidget(parent) {
    setObjectName("STD_SelectGasketWidget");
    mGasketObject = nullptr;
}

STD_SelectGasketWidget::~STD_SelectGasketWidget() {
    if (mGasketObject) {
        delete mGasketObject;
        mGasketObject = nullptr;
    }
}

/**
 * @brief STD_SelectGasketWidget::getCurrentObject
 * @return Gasket object with additional displayname
 */
RB_ObjectBase* STD_SelectGasketWidget::getCurrentObject() {
    if (!ui->tvComponent->currentIndex().isValid()) {
        return nullptr;
    }

    RB_ObjectBase* obj = mComponentModel->getCurrentObject();
    QString nomSize = obj->getValue("nomsize").toString();

    QString displayName = ui->cbStandard->currentText() + " "
            + ui->cbRating->currentText() + " "
            + nomSize + " "
            + ui->cbType->currentText();

    if (!mGasketObject) {
        mGasketObject = new RB_ObjectAtomic(obj);
        mGasketObject->addMember("displayname", "-", displayName);
    } else {
        // Required? set only once
        *mGasketObject = *obj;
        mGasketObject->setValue("displayname", displayName);
    }

    return mGasketObject;
}

void STD_SelectGasketWidget::init() {
    mStandardModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelDimension, false);
    mStandardModel->setRoot("");
    mStandardModel->setWhere("id <> '0' AND comptype_id = "
                              + QString::number((int)STD2::CompGasket));
    ui->cbStandard->setModel(mStandardModel);
    ui->cbStandard->setModelColumn(mStandardModel->fieldIndex("code"));
    mStandardModel->select();

    connect(ui->cbStandard, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotStandardRowChanged(int)));

    mTypeModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompType, false);
    ui->cbType->setModel(mTypeModel);
    ui->cbType->setModelColumn(mTypeModel->fieldIndex("type"));

    mRatingModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompRating, false);
    ui->cbRating->setModel(mRatingModel);
    ui->cbRating->setModelColumn(mRatingModel->fieldIndex("rating"));

    connect(ui->cbRating, SIGNAL(currentIndexChanged(int)),
            this, SLOT(slotRatingRowChanged(int)));

    // gasket model
    mComponentModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelGasket, false);
    formatTableView(ui->tvComponent, mComponentModel);

    for (int i = 0; i < RB2::HIDDENCOLUMNS + 2; ++i) {
        ui->tvComponent->hideColumn(i);
    }

    ui->cbSerie->setEnabled(false);

    readSettings();
}

void STD_SelectGasketWidget::slotStandardRowChanged(int row) {
    if (!mStandardModel || !mTypeModel) return;
    QModelIndex index = mStandardModel->index(row, 0);
    STD_DimensionTableHelper::setGasketDetailTables(index, QModelIndex());
    mTypeModel->slotParentCurrentRowChanged(index, QModelIndex());
    mRatingModel->slotParentCurrentRowChanged(index, QModelIndex());
}

void STD_SelectGasketWidget::slotRatingRowChanged(int row) {
    if (!mRatingModel || !mComponentModel) return;
    QModelIndex index = mRatingModel->index(row, 0);
    mComponentModel->setWhere("typename='" + ui->cbType->currentText() + "'", false);
    mComponentModel->slotParentCurrentRowChanged(index, QModelIndex());

    formatTableView(ui->tvComponent, mComponentModel);

    for (int i = 0; i < RB2::HIDDENCOLUMNS + 2; ++i) {
        ui->tvComponent->hideColumn(i);
    }

}
