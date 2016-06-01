/*****************************************************************
 * $Id: std_matelasmodulwidget.cpp 0 2016-06-01 10:23:20Z rutger $
 * Created: June 1, 2016 10:23:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "std_matelasmodulwidget.h"
#include "ui_std_matelasmodulwidget.h"

#include "pcalc_modelfactory.h"

STD_MatElasModulWidget::STD_MatElasModulWidget(QWidget *parent)
        : RB_Widget(parent), ui(new Ui::STD_MatElasModulWidget) {
    ui->setupUi(this);
}

STD_MatElasModulWidget::~STD_MatElasModulWidget() {
    delete mChildModel;
    delete mModel;
    delete ui;
    RB_DEBUG->debug("STD_MatElasModulWidget::~STD_MatElasModulWidget()");
}

QString STD_MatElasModulWidget::getName() const {
    return "Material Elasticity Modulus Tables";
}

RB2::PerspectiveType STD_MatElasModulWidget::getPerspectiveType() const {
    return RB2::PerspectivePCALC;
}

void STD_MatElasModulWidget::init() {
    // Set model, root="" is already set by modelFactory, however change to system root
    mModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelMatElasModulTable, false);
    mModel->setRoot("");
    mModel->setWhere("id <> '0'");
    mModel->select();

    formatTableView(ui->tvElasModulTable, mModel);
    ui->tbbElasModulTable->initEdit(false, true, true, false);
    ui->tvElasModulTable->setToolButtonBar(ui->tbbElasModulTable);

    mChildModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelMatElasModulValue, false);

    formatTableView(ui->tvElasModulValue, mChildModel);
    ui->tbbElasModulValue->initEdit(false, false, false, false);
    ui->tvElasModulValue->setToolButtonBar(ui->tbbElasModulValue);

    int count = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < count; i++) {
        ui->tvElasModulTable->hideColumn(i);
    }

    for (int i = 0; i < count; i++) {
        ui->tvElasModulValue->hideColumn(i);
    }

    readSettings();
}

bool STD_MatElasModulWidget::fileSave(bool withSelect) {
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

    RB_DEBUG->error("STD_MatElasModulWidget ERROR");
    return false;
}

void STD_MatElasModulWidget::fileRevert() {
    mChildModel->revertAll();
    mModel->revertAll();
    setWindowModified(false);

}
