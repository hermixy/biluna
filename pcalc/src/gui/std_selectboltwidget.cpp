/*****************************************************************
 * $Id: std_selectboltwidget.cpp 0001 2016-07-31T08:39:51 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_selectboltwidget.h"

#include "pcalc_modelfactory.h"
#include "pcalc_sqlcommonfunctions.h"
#include "std.h"
#include "ui_std_selectcomponentwidget.h"

STD_SelectBoltWidget::STD_SelectBoltWidget(QWidget *parent)
        : STD_SelectComponentWidget(parent) {
    setObjectName("STD_SelectBoltWidget");
    mBoltObject = nullptr;
}

STD_SelectBoltWidget::~STD_SelectBoltWidget() {
    if (mBoltObject) {
        delete mBoltObject;
        mBoltObject = nullptr;
    }
}

/**
 * @brief STD_SelectBoltWidget::getCurrentObject
 * @return Bolt object with additional displayname
 */
RB_ObjectBase* STD_SelectBoltWidget::getCurrentObject() {
    if (!ui->tvComponent->currentIndex().isValid()) {
        return nullptr;
    }

    RB_ObjectBase* obj = mComponentModel->getCurrentObject();
    QString nomSize = obj->getValue("nomsize").toString();

    QVariant var = PCALC_SQLCOMMONFUNCTIONS->selectFromWhere(
                "hbasic", "std_nutasme",
                "parent='{b9354340-290f-4d40-a53a-13f7b88861ea}' "
                "AND nomsize=" + nomSize);
    double hBasic = var.toDouble();
    var = PCALC_SQLCOMMONFUNCTIONS->selectFromWhere(
                "fbasic", "std_nutasme",
                "parent='{b9354340-290f-4d40-a53a-13f7b88861ea}' "
                "AND nomsize=" + nomSize);
    double fBasic = var.toDouble();
    QString displayName = ui->cbStandard->currentText() + " "
            + nomSize + " "
            + ui->cbType->currentText();

    if (!mBoltObject) {
        mBoltObject = new RB_ObjectAtomic(obj);
        mBoltObject->addMember("displayname", "-", displayName);
        mBoltObject->addMember("hbasic", "mm", hBasic);
        mBoltObject->addMember("fbasic", "mm", fBasic);
    } else {
        // Required? set only once
        *mBoltObject = *obj;
        mBoltObject->setValue("displayname", displayName);
    }

    return mBoltObject;
}

void STD_SelectBoltWidget::init() {
    mStandardModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelDimension, false);
    mStandardModel->setRoot("");
    mStandardModel->setWhere("id <> '0' AND comptype_id = "
                              + QString::number((int)STD2::CompBolt));
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

    // bolt model
    mComponentModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelBoltEn, false);
    formatTableView(ui->tvComponent, mComponentModel);

    for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
        ui->tvComponent->hideColumn(i);
    }

    ui->cbRating->setEnabled(false);
    ui->cbSerie->setEnabled(false);

    readSettings();
}

void STD_SelectBoltWidget::slotStandardRowChanged(int row) {
    if (!mStandardModel || !mTypeModel) return;
    QModelIndex index = mStandardModel->index(row, 0);
    STD_DimensionTableHelper::setBoltDetailTables(index, QModelIndex());
    mTypeModel->slotParentCurrentRowChanged(index, QModelIndex());
}

void STD_SelectBoltWidget::slotTypeRowChanged(int row) {
    if (!mTypeModel || !mComponentModel) return;
    QModelIndex index = mTypeModel->index(row, 0);
    mComponentModel->slotParentCurrentRowChanged(index, QModelIndex());
    formatTableView(ui->tvComponent, mComponentModel);

    for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
        ui->tvComponent->hideColumn(i);
    }

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
