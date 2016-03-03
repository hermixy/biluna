/*****************************************************************
 * $Id: pcalc_en13555property.cpp 0001 2016-02-20T16:56:54 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "en13555property.h"
#include "pcalc.h"
#include "pcalc_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "pcalc_utilityfactory.h"
NAMESPACE_BILUNA_CALC_EN1591


EN13555Property* EN13555Property::mActiveUtility = 0;

EN13555Property::EN13555Property() : RB_TableMath(), RB_Utility() {
    RB_DEBUG->print("EN13555Property::EN13555Property()");
    mGasketList = new RB_ObjectContainer("", nullptr,
                                         "PCALC_EN13555GasketList",
                                         PCALC_OBJECTFACTORY);
    mCurrentGasket = nullptr;
//    mTargetQA = 0.0;
//    mLeft = NULL;
//    mRight = NULL;
//    createList();
    PCALC_UTILITYFACTORY->addUtility(this);
}

EN13555Property::~EN13555Property() {
    delete mGasketList;
    mGasketList = nullptr;

    PCALC_UTILITYFACTORY->removeUtility(this);
    mActiveUtility = NULL;
    RB_DEBUG->print("EN13555Property::~EN13555Property()");
}

EN13555Property* EN13555Property::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new EN13555Property();
        mActiveUtility->refresh();
    }
    return mActiveUtility;
}

bool EN13555Property::setCurrentGasket(const QString& gasketIdx) {
    bool success = false;
    QString gasketId = RB2::IdxId(gasketIdx);

    mCurrentGasket = mGasketList->getObject(gasketId);

    if (mCurrentGasket) {
        success = true;
    } else {
        // will also set mCurrentGasket
        success = loadGasket(gasketId);
    }

    return success;
}

/**
 * Get delta_eGc
 * @param gasketPressure[N/mm2]
 * @param designtemp [Celsius]
 * @return delta eGc from EN13555 test tables
 */
double EN13555Property::get_deltaeGc(double gasketPressure,
                                    double designTemp) {
    if (!mCurrentGasket) {
        RB_DEBUG->error("EN13555Property::get_deltaeGc() "
                        "mCurrentGasket NULL ERROR");
        return 0.0;
    }

    double deltaeGc = getBilInterpValue(
                mCurrentGasket->getContainer("PCALC_EN13555PqrDeltaeGCList"),
                "qa", "temp", "deltaegc",
                gasketPressure, designTemp);

    return deltaeGc;
}

/**
 * @brief EN13555Property::get_PQR Get P_QR
 * @param gasketPressure
 * @param designTemp
 * @return P_QR from EN13555 test tables
 */
double EN13555Property::get_PQR(double gasketPressure, double designTemp) {
    if (!mCurrentGasket) {
        RB_DEBUG->error("EN13555Property::get_PQR() "
                        "mCurrentGasket NULL ERROR");
        return 0.0;
    }

    double P_QR = getBilInterpValue(
                mCurrentGasket->getContainer("PCALC_EN13555PqrDeltaeGCList"),
                "qa", "temp", "pqr",
                gasketPressure, designTemp);

    return P_QR;
}

/**
 * @brief EN13555Property::get_eG get gasket compressed thickness eG
 * @param gasketPressure
 * @param designTemp
 * @return eG compressed thickness
 */
double EN13555Property::get_eG(double gasketPressure, double designTemp) {
    if (!mCurrentGasket) {
        RB_DEBUG->error("EN13555Property::get_eG() "
                        "mCurrentGasket NULL ERROR");
        return 0.0;
    }

    double eG = getBilInterpValue(
                mCurrentGasket->getContainer("PCALC_EN13555EGeGList"),
                "qa", "temp", "eg",
                gasketPressure, designTemp);

    return eG;
}

/**
 * @brief EN13555Property::get_EG get gasket elasticity
 * @param gasketPressure
 * @param designTemp
 * @return EG elasticity
 */
double EN13555Property::get_EG(double gasketPressure, double designTemp) {
    if (!mCurrentGasket) {
        RB_DEBUG->error("EN13555Property::get_EG() "
                        "mCurrentGasket NULL ERROR");
        return 0.0;
    }

    double EG = getBilInterpValue(
                mCurrentGasket->getContainer("PCALC_EN13555EGeGList"),
                "qa", "temp", "capitaleg",
                gasketPressure, designTemp);

    return EG;
}

/**
 * @brief EN13555Property::get_Qsmax get Q_smax
 * @param designTemp
 * @return Q_smax maximum allowable gasket pressure at temperature
 */
double EN13555Property::get_Qsmax(double designTemp) {


    // continue here ...
    // with linear interpolation, already in RB_TableMath






}

double EN13555Property::getQA(const QString& gasketIdx, double leakageRate,
                              double designPressure) {
//    QminLQsminLProperty* minQAobj = NULL;

//    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
//                it != mList.end(); it++) {
//        QminLQsminLProperty* tmpObj = (*it);

//        if (tmpObj->mLeakageRate == leakageRate
//                && tmpObj->mMaterialCode == materialCode
//                && tmpObj->mdesignPressure == designPressure) {
//            if (!minQAobj || (minQAobj->mQA > tmpObj->mQA)) {
//                minQAobj = tmpObj;
//            }
//        }
//    }

//    if (minQAobj) {
//        return minQAobj->mQA;
//    }

    return -1.0;
}

double EN13555Property::getQminL(const RB_String& gasketIdx, double leakageRate,
                                 double designPressure) {
//    bool existing = false;

//    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
//                it != mList.end() && !existing; it++) {
//        QminLQsminLProperty* tmpObj = (*it);

//        if (tmpObj->mLeakageRate == leakageRate
//                && tmpObj->mMaterialCode == materialCode
//                && tmpObj->mdesignPressure == designPressure) {
//            return tmpObj->mQminL;
//        }
//    }

    return -1.0;
}

double EN13555Property::getQsminL(const QString& gasketIdx, double leakageRate,
                                  double QA, double designPressure) {
//    mLeft = NULL;
//    mRight = NULL;
//    mTargetQA = QA;

//    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
//                it != mList.end(); it++) {
//        QminLQsminLProperty* tmpObj = (*it);

//        if (tmpObj->mLeakageRate == leakageRate
//                && tmpObj->mMaterialCode == materialCode
//                && tmpObj->mdesignPressure == designPressure) {
//            updateLeft(tmpObj);
//            updateRight(tmpObj);
//        }
//    }

    double value = -1.0;

//    if (mLeft && mRight) {
//        double denominator = mRight->mQA - mLeft->mQA;

//        if (denominator != 0) {
//            value = mLeft->mQsminL + (QA - mLeft->mQA)
//                    * (mRight->mQsminL - mLeft->mQsminL)
//                    / (mRight->mQA - mLeft->mQA);
//        } else {
//            value = mRight->mQsminL;
//        }
//    } else if (mRight) {
//        value = mRight->mQsminL;
//    } else if (mLeft) {
//        value = mLeft->mQsminL;
//    }

    return value;
}

bool EN13555Property::loadGasket(const QString& gasketId) {
    QSqlDatabase db = PCALC_MODELFACTORY->getStandardDatabase();
    mCurrentGasket = mGasketList->newObject(gasketId);
    return mCurrentGasket->dbRead(db, RB2::ResolveAll);
}

/**
 * Determine closest inner test pressure from EN13555 data. Test pressures
 * usually are 10 or 40 bar
 * @param designPressure the assembly design pressure [N/mm2]
 * @return closest inner test pressure [N/mm2]
 */
double EN13555Property::closestInnerPressureBar(double designPressure) {
    if (!mCurrentGasket) {
        RB_DEBUG->error("EN13555Property::closestInnerPressureBar() "
                        "mCurrentGasket NULL ERROR");
        return 0.0;
    }

    RB_ObjectContainer* objC =
            mCurrentGasket->getContainer("PCALC_EN13555QminLList");
    RB_ObjectIterator* iter = objC->createIterator();
    double designPressureBar = PCALC2::MPaToBar(designPressure);
    double closestPressureBar = 0.0;

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        double pressureBar = obj->getValue("testpress").toDouble();
        if (std::abs(designPressureBar - pressureBar)
                < std::abs(designPressureBar - closestPressureBar)) {
            closestPressureBar = pressureBar;
        }
    }

    delete iter;

    return closestPressureBar;
}

END_NAMESPACE_BILUNA_CALC_EN1591
