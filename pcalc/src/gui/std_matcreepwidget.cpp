/*****************************************************************
 * $Id: std_matelasmodulwidget.cpp 0 2016-06-01 10:23:20Z rutger $
 * Created: June 1, 2016 10:23:42 AM - rutger
 *
 * Copyright (C) 2015 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "std_matcreepwidget.h"
#include "ui_std_matcreepwidget.h"

#include "pcalc_modelfactory.h"

STD_MatCreepWidget::STD_MatCreepWidget(QWidget *parent)
        : RB_Widget(parent), ui(new Ui::STD_MatCreepWidget) {
    ui->setupUi(this);
}

STD_MatCreepWidget::~STD_MatCreepWidget() {
    delete mRp10T100khModel;
    delete mRp10T10khModel;
    delete mRpT100khModel;
    delete mRpT10khModel;
    delete mRpT200khModel;
    delete mModel;
    delete ui;
    RB_DEBUG->debug("STD_MatCreepWidget::~STD_MatCreepWidget()");
}

QString STD_MatCreepWidget::getName() const {
    return "Material Elasticity Modulus Tables";
}

RB2::PerspectiveType STD_MatCreepWidget::getPerspectiveType() const {
    return RB2::PerspectivePCALC;
}

void STD_MatCreepWidget::init() {
    // Set model, root="" is already set by modelFactory, however change to system root
    mModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelMatCreepTable, false);
    mModel->setRoot("");
    mModel->setWhere("id <> '0'");
    mModel->select();

    formatTableView(ui->tvCreepTable, mModel);
    ui->tbbCreepTable->initEdit(false, true, true, false);
    ui->tvCreepTable->setToolButtonBar(ui->tbbCreepTable);

    int count = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < count; i++) {
        ui->tvCreepTable->hideColumn(i);
    }

    mRp10T100khModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRp10T100kh, false);

    formatTableView(ui->tvRp10T100kh, mRp10T100khModel);
    ui->tbbRp10T100kh->initSlimTable(); // initEdit(false, false, false, false);
    ui->tvRp10T100kh->setToolButtonBar(ui->tbbRp10T100kh);

    for (int i = 0; i < count; i++) {
        ui->tvRp10T100kh->hideColumn(i);
    }

    mRp10T10khModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRp10T10kh, false);

    formatTableView(ui->tvRp10T10kh, mRp10T10khModel);
    ui->tbbRp10T10kh->initSlimTable();
    ui->tvRp10T10kh->setToolButtonBar(ui->tbbRp10T10kh);

    for (int i = 0; i < count; i++) {
        ui->tvRp10T10kh->hideColumn(i);
    }

    mRpT100khModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRpT100kh, false);

    formatTableView(ui->tvRpT100kh, mRpT100khModel);
    ui->tbbRpT100kh->initSlimTable();
    ui->tvRpT100kh->setToolButtonBar(ui->tbbRpT100kh);

    for (int i = 0; i < count; i++) {
        ui->tvRpT100kh->hideColumn(i);
    }

    mRpT10khModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRpT10kh, false);

    formatTableView(ui->tvRpT10kh, mRpT10khModel);
    ui->tbbRpT10kh->initSlimTable();
    ui->tvRpT10kh->setToolButtonBar(ui->tbbRpT10kh);

    for (int i = 0; i < count; i++) {
        ui->tvRpT10kh->hideColumn(i);
    }

    mRpT200khModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRpT200kh, false);

    formatTableView(ui->tvRpT200kh, mRpT200khModel);
    ui->tbbRpT200kh->initSlimTable();
    ui->tvRpT200kh->setToolButtonBar(ui->tbbRpT200kh);

    for (int i = 0; i < count; i++) {
        ui->tvRpT200kh->hideColumn(i);
    }

    readSettings();
}

bool STD_MatCreepWidget::fileSave(bool withSelect) {
    beforeFileSave();

    if (withSelect) {
        if (mRp10T100khModel->submitAllAndSelect()
                && mRp10T10khModel->submitAllAndSelect()
                && mRpT100khModel->submitAllAndSelect()
                && mRpT10khModel->submitAllAndSelect()
                && mRpT200khModel->submitAllAndSelect()
                && mModel->submitAllAndSelect()) {
            setWindowModified(false);
            return true;
        }
    } else {
        if (mRp10T100khModel->submitAll()
                && mRp10T10khModel->submitAll()
                && mRpT100khModel->submitAll()
                && mRpT10khModel->submitAll()
                && mRpT200khModel->submitAll()
                && mModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }

    RB_DEBUG->error("STD_MatCreepWidget ERROR");
    return false;
}

void STD_MatCreepWidget::fileRevert() {
    mRp10T100khModel->revertAll();
    mRp10T10khModel->revertAll();
    mRpT100khModel->revertAll();
    mRpT10khModel->revertAll();
    mRpT200khModel->revertAll();
    mModel->revertAll();
    setWindowModified(false);
}
