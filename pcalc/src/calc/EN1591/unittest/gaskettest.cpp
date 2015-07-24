#include "gaskettest.h"
NAMESPACE_REDBAG_CALC_EN1591


GasketTest::GasketTest(RB_ObjectContainer *inputOutput) {
    target = NULL;
    mInputOutput = inputOutput;
}

GasketTest::~GasketTest() {
    // these are normally deleted by Assembly
    delete target->mLoadCaseList;
    target->mLoadCaseList = NULL;
    // target itself
    delete target;
    target = NULL;
}

void GasketTest::exec() {
    Calc_AGtTest();
    Calc_bGtTest();
    Calc_dGtTest();
    Calc_AGeTest();
    Calc_AQTest();
    Calc_bGeTest();
    Calc_XGTest();
}

void GasketTest::setupTarget() {
    if (!target) {
        target = new Gasket(mInputOutput);
        target->mLoadCaseList = new LoadCaseList(mInputOutput);
        target->mLoadCaseList->createLoadCase(); // includes force creation
    }
    target->dG1 = 70.8;
    target->dG2 = 100.3;
}

void GasketTest::Calc_AGtTest() {
    setupTarget();
    target->dGt = 6.7;
    target->bGt = 3.03;
    target->Calc_AGt();
    areEqual(target->getLastOutput(), "GasketTest::Calc_AGtTest()", 63.77747246052639, target->AGt);
}

void GasketTest::Calc_bGtTest() {
    setupTarget();
    target->Calc_bGt();
    areEqual(target->getLastOutput(), "GasketTest::Calc_bGtTest()", 14.75, target->bGt);
}

void GasketTest::Calc_dGtTest() {
    setupTarget();
    target->Calc_dGt();
    areEqual(target->getLastOutput(), "GasketTest::Calc_dGtTest()", 85.55, target->dGt);
}

void GasketTest::Calc_AGeTest() {
    setupTarget();
    int i = 0;
    target->mLoadCaseList->at(i)->dGe = 36.2;
    target->mLoadCaseList->at(i)->bGe = 5.78;
    target->Calc_AGe(i);
    areEqual(target->getLastOutput(), "GasketTest::Calc_AGeTest()", 657.334280466514,
                     target->mLoadCaseList->at(i)->AGe);
}

void GasketTest::Calc_AQTest() {
    setupTarget();
    int i = 0;
    target->mLoadCaseList->at(i)->dGe = 78.56;
    target->Calc_AQ(i);
    areEqual(target->getLastOutput(), "GasketTest::Calc_AQTest()", 4847.2211105285178,
             target->mLoadCaseList->at(i)->AQ);
}

void GasketTest::Calc_bGeTest() {
    setupTarget();
    int i = 0;
    target->bGi = 2.1;
    target->bGt = -9.1;
    target->Calc_bGe(i);
    areEqual(target->getLastOutput(), "GasketTest::Calc_bGeTest()", -9.1,
                     target->mLoadCaseList->at(i)->bGe);
    target->bGi = -2.2;
    target->bGt = 9.1;
    target->Calc_bGe(i);
    areEqual(target->getLastOutput(), "GasketTest::Calc_bGeTest()", -2.2,
                     target->mLoadCaseList->at(i)->bGe);
}

void GasketTest::Calc_XGTest() {
    setupTarget();
    int i = 0;
    target->mLoadCaseList->at(i)->eG = 5.6;
    target->bGt = 12.3;
    target->AGt = 8.3;
    target->mLoadCaseList->at(i)->bGe = 7.7;
    target->Calc_XG(i);
    areEqual(target->getLastOutput(), "GasketTest::Calc_XGTest()", 0.970281124497992, target->XG);
}


END_NAMESPACE_REDBAG_CALC_EN1591
