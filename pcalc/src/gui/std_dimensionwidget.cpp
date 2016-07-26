/*****************************************************************
 * $Id: std_dimensionwidget.cpp 0001 2016-06-29T10:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_dimensionwidget.h"
#include "ui_std_dimensionwidget.h"

#include "pcalc.h"
#include "pcalc_modelfactory.h"

STD_DimensionWidget::STD_DimensionWidget(QWidget *parent) :
            RB_Widget(parent), ui(new Ui::STD_DimensionWidget),
            STD_DimensionTableHelper() {
    ui->setupUi(this);
}

STD_DimensionWidget::~STD_DimensionWidget() {
    delete ui;
}

void STD_DimensionWidget::init() {
    // Dimension or Standard list
    mStandardModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelDimension, false);
    mStandardModel->setRoot("");
    mStandardModel->setWhere("id <> '0'");
    mStandardModel->select();
    formatTableView(ui->tvStandard, mStandardModel);
    ui->tbbStandard->initEdit(false, true, true, false);
    ui->tvStandard->setToolButtonBar(ui->tbbStandard);

    int hiddenCount = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvStandard->hideColumn(i);
    }

    // switch between EN and ASME tables
    connect(mStandardModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotFlangeDetailTableNames(QModelIndex,QModelIndex)));

    // parent is Dimension Standard
    mRatingModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelRating, false);
    formatTableView(ui->tvRating, mRatingModel);
    ui->tbbRating->initSlimTable();
    ui->tvRating->setToolButtonBar(ui->tbbRating);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvRating->hideColumn(i);
    }

    // parent is Dimension Standard
    mTypeModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeType, false);
    formatTableView(ui->tvType, mTypeModel);
    ui->tbbType->initSlimTable();
    ui->tvType->setToolButtonBar(ui->tbbType);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvType->hideColumn(i);
    }

    // parent is Dimension Standard
    mSerieModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeFacingType, false);
    formatTableView(ui->tvSerie, mSerieModel);
    ui->tbbSerie->initSlimTable();
    ui->tvSerie->setToolButtonBar(ui->tbbSerie);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvSerie->hideColumn(i);
    }

    // parent is Rating
    mEndModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeFacingDimEn, false);
    formatTableView(ui->tvEnd, mEndModel);
    ui->tbbEnd->initSlimTable();
    ui->tvEnd->setToolButtonBar(ui->tbbEnd);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvEnd->hideColumn(i);
    }

    // parent is Rating
    mComponentModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeEn, false);
    formatTableView(ui->tvComponent, mComponentModel);
    ui->tbbComponent->initSlimTable();
    ui->tvComponent->setToolButtonBar(ui->tbbComponent);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvComponent->hideColumn(i);
    }

    // parent is Type
    mLimitModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeTypeLimit, false);
    formatTableView(ui->tvLimit, mLimitModel);
    ui->tbbLimit->initSlimTable();
    ui->tvLimit->setToolButtonBar(ui->tbbLimit);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvLimit->hideColumn(i);
    }

    readSettings();
}

bool STD_DimensionWidget::fileSave(bool withSelect) {
    beforeFileSave();

    if (withSelect) {
        if (mComponentModel->submitAllAndSelect()
                && mEndModel->submitAllAndSelect()
                && mLimitModel->submitAllAndSelect()
                && mRatingModel->submitAllAndSelect()
                && mSerieModel->submitAllAndSelect()
                && mTypeModel->submitAllAndSelect()
                && mStandardModel->submitAllAndSelect()) {
            setWindowModified(false);
            return true;
        }
    } else {
        if (mComponentModel->submitAll()
                && mEndModel->submitAll()
                && mLimitModel->submitAll()
                && mRatingModel->submitAll()
                && mSerieModel->submitAll()
                && mTypeModel->submitAll()
                && mStandardModel->submitAll()) {
            setWindowModified(false);
            return true;
        }
    }

    RB_DEBUG->error("STD_DimensionWidget fileSave() ERROR");
    return false;
}

void STD_DimensionWidget::fileRevert() {
    mComponentModel->revertAll();
    mEndModel->revertAll();
    mLimitModel->revertAll();
    mRatingModel->revertAll();
    mSerieModel->revertAll();
    mTypeModel->revertAll();
    mStandardModel->revertAll();
    setWindowModified(false);
}

void STD_DimensionWidget::slotFlangeDetailTableNames(const QModelIndex& current,
                                                     const QModelIndex& previous) {
    STD_DimensionTableHelper::setFlangeDetailTableNames(current, previous);
    formatTableView(ui->tvEnd, mEndModel);
    formatTableView(ui->tvComponent, mComponentModel);
}

