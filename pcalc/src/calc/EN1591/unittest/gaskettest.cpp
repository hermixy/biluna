#include "gaskettest.h"
NAMESPACE_BILUNA_CALC_EN1591


GasketTest::GasketTest() : RB_UnitTest() {
    target = NULL;
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
        target = new Gasket();
        target->mLoadCaseList = new LoadCaseList();
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
    areEqual(PR->getLastOutput(), "GasketTest::Calc_AGtTest()", 63.77747246052639, target->AGt);
}

void GasketTest::Calc_bGtTest() {
    setupTarget();
    target->Calc_bGt();
    areEqual(PR->getLastOutput(), "GasketTest::Calc_bGtTest()", 14.75, target->bGt);
}

void GasketTest::Calc_dGtTest() {
    setupTarget();
    target->Calc_dGt();
    areEqual(PR->getLastOutput(), "GasketTest::Calc_dGtTest()", 85.55, target->dGt);
}

void GasketTest::Calc_AGeTest() {
    setupTarget();
    target->dGe = 36.2;
    target->bGe = 5.78;
    target->Calc_AGe();
    areEqual(PR->getLastOutput(), "GasketTest::Calc_AGeTest()",
             657.334280466514, target->AGe);
}

void GasketTest::Calc_AQTest() {
    setupTarget();
    target->dGe = 78.56;
    target->Calc_AQ();
    areEqual(PR->getLastOutput(), "GasketTest::Calc_AQTest()",
             4847.2211105285178, target->AQ);
}

void GasketTest::Calc_bGeTest() {
    setupTarget();
    target->bGi = 2.1;
    target->bGt = -9.1;
    target->Calc_bGe();
    areEqual(PR->getLastOutput(), "GasketTest::Calc_bGeTest()", -9.1,
                     target->bGe);
    target->bGi = -2.2;
    target->bGt = 9.1;
    target->Calc_bGe();
    areEqual(PR->getLastOutput(), "GasketTest::Calc_bGeTest()", -2.2,
                     target->bGe);
}

void GasketTest::Calc_XGTest() {
    setupTarget();
    int i = 0;
    target->mLoadCaseList->at(i)->eG = 5.6;
    target->bGt = 12.3;
    target->AGt = 8.3;
    target->bGe = 7.7;
    target->Calc_XG();
    areEqual(PR->getLastOutput(), "GasketTest::Calc_XGTest()", 0.970281124497992, target->XG);
}


END_NAMESPACE_BILUNA_CALC_EN1591
