/*****************************************************************
 * $Id: std_matthermexpwidget.cpp 0 2016-06-01 08:23:20Z rutger $
 * Created: June 1, 2016 8:23:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "std_matthermexpwidget.h"
#include "ui_std_matthermexpwidget.h"

#include "pcalc_modelfactory.h"

STD_MatThermExpWidget::STD_MatThermExpWidget(QWidget *parent)
        : RB_Widget(parent), ui(new Ui::STD_MatThermExpWidget) {
    ui->setupUi(this);
}

STD_MatThermExpWidget::~STD_MatThermExpWidget() {
    delete mChildModel;
    delete mModel;
    delete ui;
    RB_DEBUG->debug("STD_MatThermExpWidget::~STD_MatThermExpWidget()");
}

QString STD_MatThermExpWidget::getName() const {
    return "Material Thermal Expansion Tables";
}

RB2::PerspectiveType STD_MatThermExpWidget::getPerspectiveType() const {
    return RB2::PerspectivePCALC;
}

void STD_MatThermExpWidget::init() {
    // Set model, root="" is already set by modelFactory, however change to system root
    mModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelMatThermExpTable, false);
    mModel->setRoot("");
    mModel->setWhere("id <> '0'");
    mModel->select();

    formatTableView(ui->tvThermExpTable, mModel);
    ui->tbbThermExpTable->initEdit(false, true, true, false);
    ui->tvThermExpTable->setToolButtonBar(ui->tbbThermExpTable);

    mChildModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelMatThermExpValue, false);

    formatTableView(ui->tvThermExpValue, mChildModel);
    ui->tbbThermExpValue->initEdit(false, false, false, false);
    ui->tvThermExpValue->setToolButtonBar(ui->tbbThermExpValue);

    int count = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < count; i++) {
        ui->tvThermExpTable->hideColumn(i);
    }

    for (int i = 0; i < count; i++) {
        ui->tvThermExpValue->hideColumn(i);
    }

    readSettings();
}

bool STD_MatThermExpWidget::fileSave(bool withSelect) {
    beforeFileSave();

    if (withSelect) {
        if (mChildModel->submitAllAndSelect() && mModel->submitAllAndSelect()) {
            setWindowModified(false);
            return true;
        }
    } else {
        if (mChildModel->submitAll() && mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }

    RB_DEBUG->error("STD_MatThermExpWidget ERROR");
    return false;
}

void STD_MatThermExpWidget::fileRevert() {
    mChildModel->revertAll();
    mModel->revertAll();
    setWindowModified(false);

}
