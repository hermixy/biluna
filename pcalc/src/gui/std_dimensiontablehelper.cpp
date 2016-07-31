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

void STD_DimensionTableHelper::setBoltDetailTables(
                const QModelIndex& current, const QModelIndex& /*previous*/) {

    if (isCodeStartWith("ASME", current)) {
        delete mComponentModel;
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelBoltAsme, false);
        delete mEndModel;
        mEndModel = nullptr;
        delete mLimitModel;
        mLimitModel = nullptr;
    } else if (isCodeStartWith("EN", current)) {
//        delete mEndModel;
//        mEndModel = PCALC_MODELFACTORY->getModel(
//                    PCALC_ModelFactory::ModelBolt, false);
    }
}

void STD_DimensionTableHelper::setFlangeDetailTables(
                const QModelIndex& current, const QModelIndex& /*previous*/) {

    if (isCodeStartWith("ASME", current)) {
        delete mComponentModel;
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeAsme, false);
        delete mEndModel;
        mEndModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeFacingDimAsme, false);
        delete mLimitModel;
        mLimitModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeTypeLimit, false);
    } else if (isCodeStartWith("EN", current)) {
        delete mComponentModel;
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeEn, false);
        delete mEndModel;
        mEndModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeFacingDimEn, false);
        delete mLimitModel;
        mLimitModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelFlangeTypeLimit, false);
    }
}

void STD_DimensionTableHelper::setNutDetailTables(
                const QModelIndex& current, const QModelIndex& /*previous*/) {

    if (isCodeStartWith("ASME", current)) {
        delete mComponentModel;
        mComponentModel = PCALC_MODELFACTORY->getModel(
                    PCALC_ModelFactory::ModelNutAsme, false);
        delete mEndModel;
        mEndModel = nullptr;
        delete mLimitModel;
        mLimitModel = nullptr;
    } else if (isCodeStartWith("EN", current)) {
//        delete mComponentModel;
//        mComponentModel = PCALC_MODELFACTORY->getModel(
//                    PCALC_ModelFactory::ModelFlangeEn, false);
//        delete mEndModel;
//        mEndModel = PCALC_MODELFACTORY->getModel(
//                    PCALC_ModelFactory::ModelFlangeFacingDimEn, false);
//        delete mLimitModel;
//        mLimitModel = PCALC_MODELFACTORY->getModel(
//                    PCALC_ModelFactory::ModelFlangeTypeLimit, false);
    }
}

bool STD_DimensionTableHelper::isCodeStartWith(const QString& startString,
                                               const QModelIndex& current) {
    if (!isCurrentStandardIndexValid(current)) {
        return false;
    }

    int row = current.row();
    int colCode = mStandardModel->fieldIndex("code");
    QString code = mStandardModel->data(mStandardModel->index(row, colCode)).toString();
    return code.startsWith(startString);
}

bool STD_DimensionTableHelper::isCurrentStandardIndexValid(const QModelIndex& current) {
    if (!current.isValid()) {
        return false;
    } else if (current.model() != mStandardModel) {
        RB_DEBUG->error("STD_DimensionTableHelper::isCurrentStandardIndexValid() "
                        "model ERROR");
        return false;
    }

    return true;
}

