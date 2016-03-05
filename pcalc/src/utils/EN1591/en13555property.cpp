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
                "qg", "temp", "deltaegc",
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
                "qg", "temp", "pqr",
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
                "qg", "temp", "eg",
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
                "qg", "temp", "capitaleg",
                gasketPressure, designTemp);

    return EG;
}

/**
 * @brief EN13555Property::get_Qsmax get Q_smax
 * @param designTemp
 * @return Q_smax maximum allowable gasket pressure at temperature
 */
double EN13555Property::get_Qsmax(double designTemp) {
    if (!mCurrentGasket) {
        RB_DEBUG->error("EN13555Property::get_Qsmax() "
                        "mCurrentGasket NULL ERROR");
        return 0.0;
    }

    double Qsmax = getMaxLinInterpValue(
                mCurrentGasket->getContainer("PCALC_EN13555PqrDeltaeGCList"),
                "temp", "qg", designTemp);

    return Qsmax;
}

double EN13555Property::get_QsminL(double leakageRate, double QA,
                                   double designPressure) {
    if (!mCurrentGasket) {
        RB_DEBUG->error("EN13555Property::get_QsminL() "
                        "mCurrentGasket NULL ERROR");
        return 0.0;
    }

    double testPressure =
            EN13555PROPERTY->closestInnerPressureBar(designPressure);
    double QsminL = getBilInterpValue(
                mCurrentGasket->getContainer("PCALC_EN13555QsminLList"),
                "leakrate", "qa", "qsminl",
                leakageRate, QA, "testpress", testPressure);

    return QsminL;
}

double EN13555Property::get_QminL(const RB_String& gasketIdx, double leakageRate,
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

/**
 * @brief EN13555Property::getMaxLinInterpValue special for Qsmax from
 * P_QR and delta_eGc table
 * @param fromObjC
 * @param xField
 * @param yField
 * @param xValue
 * @param extraField
 * @param extraValue
 * @return value
 */
double EN13555Property::getMaxLinInterpValue(RB_ObjectContainer *fromObjC,
                                             const QString &xField,
                                             const QString &yField,
                                             double xValue,
                                             const QString &extraField,
                                             double extraValue) {

    mXfield = xField;
    mYfield = yField;
    mXvalue = xValue;
    mYvalue = 0.0; // now to be set to determine maximum
    mExtraField = extraField;
    mExtraValue = extraValue;
    mTopLeft = nullptr;
    mTopRight = nullptr;
    mBottomLeft = nullptr;
    mBottomRight = nullptr;

    RB_ObjectIterator* iter = fromObjC->createIterator();

    for(iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        updateMaxLeftRightObjects(obj);
    }

    delete iter;
    double value = 0.0;

    if (!mTopLeft || !mTopRight) {
        // mZfield is returned, set to qg to retrieve the correct value qsmax
        QString temp = mZfield;
        mZfield = "qg";
        value = getOutOfBoundValue();
        mZfield = temp;
    } else {
        value = getLinearValue(
                mXvalue,
                mTopLeft->getValue(mXfield).toDouble(),
                mTopLeft->getValue(mYfield).toDouble(),
                mTopRight->getValue(mXfield).toDouble(),
                mTopRight->getValue(mYfield).toDouble());
    }

    return value;
}

/**
 * @brief EN13555Property::updateMaxLeftRightObjects special for Qsmax from
 * P_QR and delta_eGc table
 * @param obj
 */
void EN13555Property::updateMaxLeftRightObjects(RB_ObjectBase* obj) {
    if (!mExtraField.isEmpty()
            && obj->getValue(mExtraField).toDouble() != mExtraValue) {
        return;
    }

    double xValue = obj->getValue(mXfield).toDouble();
    double yValue = obj->getValue(mYfield).toDouble();

    // Top left
    if (!mTopLeft && xValue <= mXvalue && yValue >= mYvalue) {
        mTopLeft = obj;
        mYvalue = yValue;
    } else if (mTopLeft
               && xValue >= mTopLeft->getValue(mXfield).toDouble()
               && xValue <= mXvalue && yValue >= mYvalue) {
        mTopLeft = obj;
        mYvalue = yValue;
    }

    // Top right
    if (!mTopRight && xValue >= mXvalue && yValue >= mYvalue) {
        mTopRight = obj;
        mYvalue = yValue;
    } else if (mTopRight
               && xValue <= mTopRight->getValue(mXfield).toDouble()
               && xValue >= mXvalue && yValue >= mYvalue) {
        mTopRight = obj;
        mYvalue = yValue;
    }

}

END_NAMESPACE_BILUNA_CALC_EN1591
