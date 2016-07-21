/*****************************************************************
 * $Id: std_dimensionwidget.cpp 0001 2016-07-29T10:39:19 rutger $
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
            RB_Widget(parent), ui(new Ui::STD_DimensionWidget) {
    ui->setupUi(this);
    mComponentModel = nullptr;
    mEndModel = nullptr;
    mLimitModel = nullptr;
    mRatingModel = nullptr;
    mSerieModel = nullptr;
    mStandardModel = nullptr;
    mTypeModel = nullptr;
}

STD_DimensionWidget::~STD_DimensionWidget() {
    delete mComponentModel;
    mComponentModel = nullptr;
    delete mEndModel;
    mEndModel = nullptr;
    delete mRatingModel;
    mRatingModel = nullptr;
    delete mSerieModel;
    mSerieModel = nullptr;
    delete mLimitModel;
    mLimitModel = nullptr;
    delete mTypeModel;
    mTypeModel = nullptr;
    delete mStandardModel;
    mStandardModel = nullptr;
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
            this, SLOT(slotUpdateDetailTableNames(QModelIndex,QModelIndex)));

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
                PCALC_ModelFactory::ModelFlangeFacingDim, false);
    formatTableView(ui->tvEnd, mEndModel);
    ui->tbbEnd->initSlimTable();
    ui->tvEnd->setToolButtonBar(ui->tbbEnd);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvEnd->hideColumn(i);
    }

    // parent is Rating
    mComponentModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlange, false);
    formatTableView(ui->tvComponent, mComponentModel);
    ui->tbbComponent->initSlimTable();
    ui->tvComponent->setToolButtonBar(ui->tbbComponent);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvComponent->hideColumn(i);
    }

    // parent is Type
    mLimitModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelFlangeRating, false);
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
    mStandardModel->revertAll()  ;
    setWindowModified(false);
}

void STD_DimensionWidget::slotUpdateDetailTableNames(const QModelIndex& current,
                                                     const QModelIndex& /*previous*/) {
    if (current.model() != mStandardModel) {
        RB_DEBUG->error("STD_DimensionWidget::slotUpdateDetailTableNames() "
                        "model ERROR");
        return;
    }

    int row = current.row();
    int colCompType = mStandardModel->fieldIndex("comptype_id");
    int componentType = mStandardModel->data(mStandardModel->index(row, colCompType)).toInt();
    int colCode = mStandardModel->fieldIndex("code");
    QString code = mStandardModel->data(mStandardModel->index(row, colCode)).toString();

    if (componentType == (int)PCALC2::CompFlange && code.startsWith("ASME")) {
        delete mEndModel;
        mEndModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeFacingDimAsme, false);
        formatTableView(ui->tvEnd, mEndModel);
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeAsme, false);
        formatTableView(ui->tvComponent, mComponentModel);

    } else if (componentType == (int)PCALC2::CompFlange && code.startsWith("EN")) {
        delete mEndModel;
        mEndModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeFacingDim, false);
        formatTableView(ui->tvEnd, mEndModel);
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlange, false);
        formatTableView(ui->tvComponent, mComponentModel);
    }
}

