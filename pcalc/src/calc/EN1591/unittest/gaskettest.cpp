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
    Calc_delta_eGcTest();
}

void GasketTest::setupTarget() {
    if (!target) {
        target = new Gasket();
        target->mLoadCaseList = new LoadCaseList();
        target->mLoadCaseList->createLoadCase(); // includes force creation
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

void GasketTest::Calc_delta_eGcTest() {
    setupTarget();
    LoadCase* loadCase0 = target->mLoadCaseList->at(0);
    int i = 1;
    LoadCase* loadCase = target->mLoadCaseList->at(i);
    loadCase->delta_eGc_EN13555 = 7.3;
    loadCase->Y_G = 3.8;
    target->K = 500000.0;
    target->Calc_delta_eGc(i);
    areEqual(PR->getLastOutput(), "GasketTest::Calc_delta_eGcTest()",
             13870000, loadCase->delta_eGc);
    loadCase->delta_eGc_EN13555 = 0.0;
    loadCase->Y_G = 1.3;
    target->dG2_EN13555 = 2.4;
    target->dG1_EN13555 = 9.1;
    loadCase0->Q_A = 15.3;
    loadCase->P_QR = 0.0456;
    target->Calc_delta_eGc(i);
    areEqual(PR->getLastOutput(), "GasketTest::Calc_delta_eGcTest()",
             -1148.7558557602242, loadCase->delta_eGc);
    target->dG2_EN13555 = 0.0;
    target->dG1_EN13555 = 0.0;
    loadCase->P_QR = 0.0;
    target->Calc_delta_eGc(i);
    areEqual(PR->getLastOutput(), "GasketTest::Calc_delta_eGcTest()", 0.0,
             loadCase->delta_eGc);
}



END_NAMESPACE_BILUNA_CALC_EN1591
