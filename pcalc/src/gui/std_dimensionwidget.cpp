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
#include "rb_cmbdelegate.h"
#include "std.h"

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
    ui->tvStandard->setModel(mStandardModel);
    ui->tbbStandard->initEdit(false, true, true, false);
    ui->tvStandard->setToolButtonBar(ui->tbbStandard);

    int col = mStandardModel->fieldIndex("comptype_id");
    QStringList strL = STD2::getCompTypeStringList();
    mStandardModel->setTextList(col, strL);
    RB_CmbDelegate* delegate = new RB_CmbDelegate(this, strL);
    ui->tvStandard->setItemDelegateForColumn(col, delegate);

    int hiddenCount = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvStandard->hideColumn(i);
    }

    // switch between EN and ASME tables
    connect(mStandardModel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotDetailTables(QModelIndex,QModelIndex)));

    // parent is Dimension Standard, flange rating
    mRatingModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompRating, false);
    formatTableView(ui->tvRating, mRatingModel);
    ui->tbbRating->initSlimTable();
    ui->tvRating->setToolButtonBar(ui->tbbRating);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvRating->hideColumn(i);
    }

    // parent is Dimension Standard, flange types
    mTypeModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompType, false);
    formatTableView(ui->tvType, mTypeModel);
    ui->tbbType->initSlimTable();
    ui->tvType->setToolButtonBar(ui->tbbType);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvType->hideColumn(i);
    }

    // parent is Dimension Standard, flange facing type
    mSerieModel = PCALC_MODELFACTORY->getModel(
                PCALC_ModelFactory::ModelCompSerie, false);
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
        if (mComponentModel) {
            mComponentModel->submitAllAndSelect();
        }
        if (mEndModel) {
            mEndModel->submitAllAndSelect();
        }
        if (mLimitModel) {
            mLimitModel->submitAllAndSelect();
        }
        if  (mRatingModel) {
            mRatingModel->submitAllAndSelect();
        }
        if (mSerieModel) {
            mSerieModel->submitAllAndSelect();
        }
        if (mTypeModel) {
            mTypeModel->submitAllAndSelect();
        }
        if (mStandardModel) {
            mStandardModel->submitAllAndSelect();
        }

        setWindowModified(false);
        return true;
    } else {
        if (mComponentModel) {
            mComponentModel->submitAll();
        }
        if (mEndModel) {
            mEndModel->submitAll();
        }
        if (mLimitModel) {
            mLimitModel->submitAll();
        }
        if  (mRatingModel) {
            mRatingModel->submitAll();
        }
        if (mSerieModel) {
            mSerieModel->submitAll();
        }
        if (mTypeModel) {
            mTypeModel->submitAll();
        }
        if (mStandardModel) {
            mStandardModel->submitAll();
        }

        setWindowModified(false);
        return true;
    }

    RB_DEBUG->error("STD_DimensionWidget fileSave() ERROR");
    return false;
}

void STD_DimensionWidget::fileRevert() {
    if (mComponentModel) {
        mComponentModel->revertAll();
    }
    if (mEndModel) {
        mEndModel->revertAll();
    }
    if (mLimitModel) {
        mLimitModel->revertAll();
    }
    if  (mRatingModel) {
        mRatingModel->revertAll();
    }
    if (mSerieModel) {
        mSerieModel->revertAll();
    }
    if (mTypeModel) {
        mTypeModel->revertAll();
    }
    if (mStandardModel) {
        mStandardModel->revertAll();
    }
    setWindowModified(false);
}

void STD_DimensionWidget::slotDetailTables(const QModelIndex& current,
                                           const QModelIndex& previous) {
    int row = current.row();
    int colCompType = mStandardModel->fieldIndex("comptype_id");
    int componentType = mStandardModel->data(
                mStandardModel->index(row, colCompType)).toInt();

    switch(componentType) {
    case (int)STD2::CompBolt:
        STD_DimensionTableHelper::setBoltDetailTables(current, previous);
        break;
    case (int)STD2::CompFlange:
        STD_DimensionTableHelper::setFlangeDetailTables(current, previous);
        break;
    case (int)STD2::CompGasket:
        STD_DimensionTableHelper::setGasketDetailTables(current, previous);
        break;
    case (int)STD2::CompNut:
        STD_DimensionTableHelper::setNutDetailTables(current, previous);
        break;
    default:
        break;
    }

    formatTableView(ui->tvComponent, mComponentModel);
    int hiddenCount = PCALC_MODELFACTORY->hiddenColumnCount();

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvComponent->hideColumn(i);
    }

    formatTableView(ui->tvLimit, mLimitModel);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvLimit->hideColumn(i);
    }

    formatTableView(ui->tvEnd, mEndModel);

    for (int i = 0; i < hiddenCount; ++i) {
        ui->tvEnd->hideColumn(i);
    }
}

