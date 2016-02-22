/*****************************************************************
 * $Id: pcalc_en1591selectgasketwidget.h 2234 2016-02-22 19:23:20Z rutger $
 * Created: Feb 22, 2016 4:34:42 PM - rutger
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_en1591selectgasketwidget.h"
#include "ui_pcalc_en1591selectgasketwidget.h"

#include "pcalc_modelfactory.h"

PCALC_EN1591SelectGasketWidget::PCALC_EN1591SelectGasketWidget(
        QWidget *parent) : RB_Widget(parent),
        ui(new Ui::PCALC_EN1591SelectGasketWidget) {
    ui->setupUi(this);

    mManufModel = nullptr;
    mGasketModel = nullptr;
}

PCALC_EN1591SelectGasketWidget::~PCALC_EN1591SelectGasketWidget() {
    delete ui;
    delete mManufModel;
    mManufModel = nullptr;
    delete mGasketModel;
    mGasketModel = nullptr;
}

RB_ObjectBase* PCALC_EN1591SelectGasketWidget::getCurrentObject() {
    if (ui->tvManufacturer->currentIndex().isValid()) {
        return mGasketModel->getCurrentObject();
    }

    return nullptr;
}

void PCALC_EN1591SelectGasketWidget::init() {
    // Manufacturer list
    mManufModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN13555Manuf, false);
    mManufModel->setRoot("");
    mManufModel->setWhere("id <> '0'");
    mManufModel->select();
    formatTableView(ui->tvManufacturer, mManufModel);

    for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
        ui->tvManufacturer->hideColumn(i);
    }

    // Gasket type list
    mGasketModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelEN13555Gasket, false);
    formatTableView(ui->tvGasket, mGasketModel);

    for (int i = 0; i < RB2::HIDDENCOLUMNS; ++i) {
        ui->tvGasket->hideColumn(i);
    }
}

