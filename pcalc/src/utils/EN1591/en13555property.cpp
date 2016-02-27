/*****************************************************************
 * $Id: pcalc_en13555property.cpp 0001 2016-02-20T16:56:54 rutger $
 *
 * Copyright (C) 2016 Red-Bag. All rights reserved.
 * This file is part of the Biluna PCALC project.
 *
 * See http://www.biluna.com for further details.
 *****************************************************************/

#include "en13555property.h"
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

bool EN13555Property::getGasket(const QString& gasketIdx) {
    bool success = false;
    QString gasketId = RB2::IdxId(gasketIdx);
    RB_ObjectBase* obj = mGasketList->getObject(gasketId);

    if (obj) {
        success = true;
    } else {
        success = loadGasket(gasketId);
    }

    return success;
}

double EN13555Property::getQA(const QString& gasketIdx, double leakageRate,
                              double testPressure) {
//    QminLQsminLProperty* minQAobj = NULL;

//    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
//                it != mList.end(); it++) {
//        QminLQsminLProperty* tmpObj = (*it);

//        if (tmpObj->mLeakageRate == leakageRate
//                && tmpObj->mMaterialCode == materialCode
//                && tmpObj->mTestPressure == testPressure) {
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
                                 double testPressure) {
//    bool existing = false;

//    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
//                it != mList.end() && !existing; it++) {
//        QminLQsminLProperty* tmpObj = (*it);

//        if (tmpObj->mLeakageRate == leakageRate
//                && tmpObj->mMaterialCode == materialCode
//                && tmpObj->mTestPressure == testPressure) {
//            return tmpObj->mQminL;
//        }
//    }

    return -1.0;
}

double EN13555Property::getQsminL(const QString& gasketIdx, double leakageRate,
                                  double QA, double testPressure) {
//    mLeft = NULL;
//    mRight = NULL;
//    mTargetQA = QA;

//    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
//                it != mList.end(); it++) {
//        QminLQsminLProperty* tmpObj = (*it);

//        if (tmpObj->mLeakageRate == leakageRate
//                && tmpObj->mMaterialCode == materialCode
//                && tmpObj->mTestPressure == testPressure) {
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
    RB_ObjectBase* gasket = mGasketList->newObject(gasketId);
    return gasket->dbRead(db, RB2::ResolveAll);
}


//void EN13555Property::createList() {
//    // leakageRate, materialCode, testPressure, QA, QminL, QsminL
//    // Table 02, includes interpolated values
//    cl(1.0, "3-3-100-1", 10.0, 160.0, 10.0, 10.0);
//    cl(1.0, "3-3-100-1", 40.0, 40.0, 30.0, 10.0);
//    cl(1.0, "3-3-100-1", 40.0, 60.0, 30.0, 10.0);

//    // TODO: complete
//}

//RB_String EN13555Property::gasketId(const RB_String& manufacturer,
//                                              const RB_String &gasketIdx) {
//    return QString();
//    bool existing = false;

//    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
//                it != mList.end() && !existing; it++) {
//        QminLQsminLProperty* tmpObj = (*it);

//        if (tmpObj->mMaterialCode == materialCode) {
//            existing = true;
//        }
//    }

//    return existing;
//}

//void EN13555Property::cl(double leakageRate,
//                            const QString& materialCode,
//                            double testPressure,
//                            double QA,
//                            double QminL,
//                            double QsminL) {
//    mList.push_back(new QminLQsminLProperty(leakageRate,
//                                            materialCode,
//                                            testPressure,
//                                            QA,
//                                            QminL,
//                                            QsminL));
//}

//void EN13555Property::updateLeft(QminLQsminLProperty* obj) {
//    if (obj->mQA <= mTargetQA) {
//        if (!mLeft || obj->mQA > mLeft->mQA) {
//            mLeft = obj;
//        }
//    }
//}

//void EN13555Property::updateRight(QminLQsminLProperty* obj) {
//    if (obj->mQA >= mTargetQA) {
//        if (!mRight || obj->mQA < mRight->mQA) {
//            mRight = obj;
//        }
//    }
//}

END_NAMESPACE_BILUNA_CALC_EN1591
