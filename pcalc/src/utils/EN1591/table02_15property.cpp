#include "table02_15property.h"
#include "pcalc_utilityfactory.h"
NAMESPACE_REDBAG_CALC_EN1591

QminLQsminLProperty::QminLQsminLProperty(double leakageRate,
                                         const QString& materialCode,
                                         double testPressure,
                                         double QA,
                                         double QminL,
                                         double QsminL) {
    mLeakageRate = leakageRate;
    mMaterialCode = materialCode;
    mTestPressure = testPressure;
    mQA = QA;
    mQminL = QminL;
    mQsminL = QsminL;
}

Table02_15Property* Table02_15Property::mActiveUtility = 0;

Table02_15Property::Table02_15Property() : RB_TableMath(), RB_Utility() {
    RB_DEBUG->print("Table02_15Property::Table02_15Property()");
    mTargetQA = 0.0;
    mLeft = NULL;
    mRight = NULL;
    createList();
    PCALC_UTILITYFACTORY->addUtility(this);
}

Table02_15Property::~Table02_15Property() {
    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        delete (*it);
    }

    PCALC_UTILITYFACTORY->removeUtility(this);
    mActiveUtility = NULL;
    RB_DEBUG->print("Table02_15Property::~Table02_15Property()");
}

Table02_15Property* Table02_15Property::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new Table02_15Property();
        mActiveUtility->refresh();
    }
    return mActiveUtility;
}

double Table02_15Property::getTableQA(double leakageRate,
                                      const QString& materialCode,
                                      double testPressure) {
    QminLQsminLProperty* minQAobj = NULL;

    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        QminLQsminLProperty* tmpObj = (*it);

        if (tmpObj->mLeakageRate == leakageRate
                && tmpObj->mMaterialCode == materialCode
                && tmpObj->mTestPressure == testPressure) {
            if (!minQAobj || (minQAobj->mQA > tmpObj->mQA)) {
                minQAobj = tmpObj;
            }
        }
    }

    if (minQAobj) {
        return minQAobj->mQA;
    }

    return -1.0;
}

double Table02_15Property::getTableQminL(double leakageRate,
                                         const RB_String& materialCode,
                                         double testPressure) {
    bool existing = false;

    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
                it != mList.end() && !existing; it++) {
        QminLQsminLProperty* tmpObj = (*it);

        if (tmpObj->mLeakageRate == leakageRate
                && tmpObj->mMaterialCode == materialCode
                && tmpObj->mTestPressure == testPressure) {
            return tmpObj->mQminL;
        }
    }

    return -1.0;
}

double Table02_15Property::getTableQsminL(double leakageRate,
                                          const QString& materialCode,
                                          double QA,
                                          double testPressure) {
    mTargetQA = QA;

    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        QminLQsminLProperty* tmpObj = (*it);

        if (tmpObj->mLeakageRate == leakageRate
                && tmpObj->mMaterialCode == materialCode
                && tmpObj->mTestPressure == testPressure) {
            updateLeft(tmpObj);
            updateRight(tmpObj);
        }
    }

    // TODO Amtec presentation of QsminL:
    double value = mRight->mQA - mLeft->mQA;



    return value;
}

void Table02_15Property::createList() {
    // leakageRate, materialCode, testPressure, QA, QminL, QsminL
    // Table 02, includes interpolated values
    cl(1.0, "3-3-100-1", 10.0, 160.0, 10.0, 10.0);
    cl(1.0, "3-3-100-1", 40.0, 40.0, 30.0, 10.0);
    cl(1.0, "3-3-100-1", 40.0, 60.0, 30.0, 10.0);
    cl(1.0, "3-3-100-1", 40.0, 80.0, 30.0, 10.0);
    cl(1.0, "3-3-100-1", 40.0, 100.0, 30.0, 10.0);
    cl(1.0, "3-3-100-1", 40.0, 160.0, 30.0, 10.0);
    cl(1.0, "3-3-100-1", 80.0, 40.0, 35.0, 10.0);
    cl(1.0, "3-3-100-1", 80.0, 60.0, 35.0, 10.0);
    cl(1.0, "3-3-100-1", 80.0, 80.0, 35.0, 10.0);
    cl(1.0, "3-3-100-1", 80.0, 100.0, 35.0, 10.0);
    cl(1.0, "3-3-100-1", 80.0, 160.0, 35.0, 11.0);
    cl(0.1, "3-3-100-1", 10.0, 160.0, 49.0, 10.0);
    cl(0.1, "3-3-100-1", 40.0, 60.0, 37.0, 10.0);
    cl(0.1, "3-3-100-1", 40.0, 80.0, 37.0, 10.0);
    cl(0.1, "3-3-100-1", 40.0, 100.0, 37.0, 10.0);
    cl(0.1, "3-3-100-1", 40.0, 160.0, 37.0, 10.0);
    cl(0.1, "3-3-100-1", 80.0, 60.0, 57.0, 43.0);
    cl(0.1, "3-3-100-1", 80.0, 80.0, 57.0, 11.0);
    cl(0.1, "3-3-100-1", 80.0, 100.0, 57.0, 10.0);
    cl(0.1, "3-3-100-1", 80.0, 160.0, 57.0, 12.0);
    cl(0.01, "3-3-100-1", 10.0, 160.0, 63.0, 10.0);
    cl(0.01, "3-3-100-1", 40.0, 80.0, 62.0, 19.0);
    cl(0.01, "3-3-100-1", 40.0, 100.0, 62.0, 10.0);
    cl(0.01, "3-3-100-1", 40.0, 160.0, 62.0, 10.0);
    cl(0.01, "3-3-100-1", 80.0, 80.0, 71.0, 25.0);
    cl(0.01, "3-3-100-1", 80.0, 100.0, 71.0, 17.0);
    cl(0.01, "3-3-100-1", 80.0, 160.0, 71.0, 15.0);
    cl(0.001, "3-3-100-1", 10.0, 160.0, 74.0, 10.0);
    cl(0.001, "3-3-100-1", 40.0, 80.0, 80.0, 80.0);
    cl(0.001, "3-3-100-1", 40.0, 100.0, 80.0, 28.0);
    cl(0.001, "3-3-100-1", 40.0, 160.0, 80.0, 19.0);
    cl(0.001, "3-3-100-1", 80.0, 100.0, 87.0, 32.0);
    cl(0.001, "3-3-100-1", 80.0, 160.0, 87.0, 28.0);
    cl(0.001, "3-3-100-1", 10.0, 160.0, 98.0, 24.0);
    cl(0.001, "3-3-100-1", 40.0, 160.0, 126.0, 71.0);
    cl(0.001, "3-3-100-1", 80.0, 160.0, 104.0, 40.0);
    // Table 03
    cl(1.0, "3-4-104-1", 40.0, 20.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 40.0, 40.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 40.0, 60.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 40.0, 80.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 40.0, 100.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 40.0, 160.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 40.0, 320.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 80.0, 320.0, 10.0, 10.0);
    cl(1.0, "3-4-104-1", 160.0, 1600.0, 10.0, 10.0);
    cl(0.1, "3-4-104-1", 40.0, 40.0, 16.0, 10.0);
    cl(0.1, "3-4-104-1", 40.0, 60.0, 16.0, 10.0);
    cl(0.1, "3-4-104-1", 40.0, 80.0, 16.0, 10.0);
    cl(0.1, "3-4-104-1", 40.0, 100.0, 16.0, 10.0);
    cl(0.1, "3-4-104-1", 40.0, 160.0, 16.0, 10.0);
    cl(0.1, "3-4-104-1", 40.0, 320.0, 16.0, 10.0);
    cl(0.1, "3-4-104-1", 80.0, 320.0, 32.0, 10.0);
    cl(0.1, "3-4-104-1", 160.0, 160.0, 20.0, 10.0);
    cl(0.01, "3-4-104-1", 40.0, 40.0, 25.0, 10.0);
    cl(0.01, "3-4-104-1", 40.0, 60.0, 25.0, 10.0);
    cl(0.01, "3-4-104-1", 40.0, 80.0, 25.0, 10.0);
    cl(0.01, "3-4-104-1", 40.0, 100.0, 25.0, 10.0);
    cl(0.01, "3-4-104-1", 40.0, 160.0, 25.0, 10.0);
    cl(0.01, "3-4-104-1", 40.0, 320.0, 25.0, 10.0);
    cl(0.01, "3-4-104-1", 80.0, 320.0, 48.0, 10.0);
    cl(0.01, "3-4-104-1", 160.0, 160.0, 45.0, 10.0);
    cl(0.001, "3-4-104-1", 40.0, 60.0, 42.0, 16.0);
    cl(0.001, "3-4-104-1", 40.0, 80.0, 42.0, 18.0);
    cl(0.001, "3-4-104-1", 40.0, 100.0, 42.0, 17.0);
    cl(0.001, "3-4-104-1", 40.0, 160.0, 42.0, 10.0);
    cl(0.001, "3-4-104-1", 40.0, 320.0, 42.0, 10.0);
    cl(0.001, "3-4-104-1", 80.0, 320.0, 81.0, 15.0);
    cl(0.001, "3-4-104-1", 160.0, 160.0, 83.0, 31.0);
    cl(0.0001, "3-4-104-1", 40.0, 100.0, 81.0, 62.0);
    cl(0.0001, "3-4-104-1", 40.0, 160.0, 81.0, 35.0);
    cl(0.0001, "3-4-104-1", 40.0, 320.0, 81.0, 26.0);
    cl(0.0001, "3-4-104-1", 80.0, 320.0, 143.0, 51.0);
    cl(0.0001, "3-4-104-1", 160.0, 160.0, 159.0, 157.0);
    cl(0.00001, "3-4-104-1", 40.0, 320.0, 181.0, 83.0);
    cl(0.00001, "3-4-104-1", 80.0, 320.0, 281.0, 198.0);
    cl(0.000001, "3-4-104-1", 40.0, 320.0, 314.0, 298.0);
    // Table 04


    // TODO: complete
}

bool Table02_15Property::isGasketMaterialCodeExisting(
        const RB_String& materialCode) {
    bool existing = false;

    for (std::vector<QminLQsminLProperty*>::iterator it = mList.begin();
                it != mList.end() && !existing; it++) {
        QminLQsminLProperty* tmpObj = (*it);

        if (tmpObj->mMaterialCode == materialCode) {
            existing = true;
        }
    }

    return existing;
}

void Table02_15Property::cl(double leakageRate,
                            const QString& materialCode,
                            double testPressure,
                            double QA,
                            double QminL,
                            double QsminL) {
    mList.push_back(new QminLQsminLProperty(leakageRate,
                                            materialCode,
                                            testPressure,
                                            QA,
                                            QminL,
                                            QsminL));
}

void Table02_15Property::updateLeft(QminLQsminLProperty* obj) {
    if (obj->mQA < mTargetQA) {
        if (mLeft && obj->mQA > mLeft->mQA) {
            mLeft = obj;
        }
    }
}

void Table02_15Property::updateRight(QminLQsminLProperty* obj) {
    if (obj->mQA > mTargetQA) {
        if (mRight && obj->mQA < mRight->mQA) {
            mRight = obj;
        }
    }
}

END_NAMESPACE_REDBAG_CALC_EN1591
