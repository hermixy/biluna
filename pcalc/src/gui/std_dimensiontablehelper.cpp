/*****************************************************************
 * $Id: std_dimensiontablehelper.cpp 2016-07-22T11:39:19 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna STD project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "std_dimensiontablehelper.h"

#include "pcalc_modelfactory.h"
#include "std.h"

STD_DimensionTableHelper::STD_DimensionTableHelper() {
    mComponentModel = nullptr;
    mEndModel = nullptr;
    mLimitModel = nullptr;
    mRatingModel = nullptr;
    mSerieModel = nullptr;
    mStandardModel = nullptr;
    mTypeModel = nullptr;
}

STD_DimensionTableHelper::~STD_DimensionTableHelper() {
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
}

void STD_DimensionTableHelper::setBoltDetailTableNames(
        const QModelIndex& current, const QModelIndex& /*previous*/) {
    if (!current.isValid()) {
        return;
    } else if (current.model() != mStandardModel) {
        RB_DEBUG->error("STD_DimensionTableHelper::setBoltDetailTableNames() "
                        "model ERROR");
        return;
    }

    int row = current.row();
    int colCompType = mStandardModel->fieldIndex("comptype_id");
    int componentType = mStandardModel->data(mStandardModel->index(row, colCompType)).toInt();
    int colCode = mStandardModel->fieldIndex("code");
    QString code = mStandardModel->data(mStandardModel->index(row, colCode)).toString();

    if (componentType == (int)STD2::CompBolt && code.startsWith("ASME")) {
        delete mEndModel;
        mEndModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelBoltAsme, false);
    } else if (componentType == (int)STD2::CompBolt && code.startsWith("EN")) {
//        delete mEndModel;
//        mEndModel = PCALC_MODELFACTORY->getModel(
//                    PCALC_ModelFactory::ModelBolt, false);
    }
}

void STD_DimensionTableHelper::setFlangeDetailTableNames(
                const QModelIndex& current, const QModelIndex& /*previous*/) {
    if (!current.isValid()) {
        return;
    } else if (current.model() != mStandardModel) {
        RB_DEBUG->error("STD_DimensionTableHelper::setFlangeDetailTableNames() "
                        "model ERROR");
        return;
    }

    int row = current.row();
    int colCompType = mStandardModel->fieldIndex("comptype_id");
    int componentType = mStandardModel->data(mStandardModel->index(row, colCompType)).toInt();
    int colCode = mStandardModel->fieldIndex("code");
    QString code = mStandardModel->data(mStandardModel->index(row, colCode)).toString();

    if (componentType == (int)STD2::CompFlange && code.startsWith("ASME")) {
        delete mEndModel;
        mEndModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeFacingDimAsme, false);
        delete mComponentModel;
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeAsme, false);

    } else if (componentType == (int)STD2::CompFlange && code.startsWith("EN")) {
        delete mEndModel;
        mEndModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeFacingDimEn, false);
        delete mComponentModel;
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeEn, false);
    }
}
