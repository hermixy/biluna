#include "flange_integraltest.h"
NAMESPACE_REDBAG_CALC_EN1591

Flange_IntegralTest::Flange_IntegralTest() : RB_UnitTest() {
    target = NULL;
}

Flange_IntegralTest::~Flange_IntegralTest() {
    // these are normally deleted by Assembly (Shell deleted with flange)
    delete target->mGasket;
    target->mGasket = NULL;
    delete target->mLoadCaseList;
    target->mLoadCaseList = NULL;
    // target itself
    delete target;
    target = NULL;
}

void Flange_IntegralTest::exec() {
    Calc_thetaTest();
    Calc_lambdaTest();
    Calc_kRTest();
    Calc_kQTest();
    Calc_hTTest();
    Calc_hSTest();
    Calc_hRTest();
    Calc_hQTest();
    Calc_hLTest();
    Calc_hHTest();
    Calc_hGTest();
    Calc_gammaTest();
    Calc_ALTest();
    Calc_AFTest();
    Calc_eETest();
    Calc_eDTest();
    Calc_dLTest();
    Calc_dFTest();
    Calc_betaTest();
    Calc_dETest();
    Calc_cFTest();
    Calc_bLTest();
    Calc_bFTest();
    Calc_ZLTest();
    Calc_ZFTest();
    Is_flange_ValidTest();
}

void Flange_IntegralTest::setupTarget() {
    if (!target) {
        //IN
        target = new Flange_Integral(1); // included shell and washer
        target->mGasket = new Gasket();
        target->mLoadCaseList = new LoadCaseList();
        target->mLoadCaseList->createLoadCase(); // includes force creation
    }

    target->AF = 3300.7; //cross section area
    target->d0 = 320.1; //inside diameter
    target->d4 = 383.2; //outside diameter
    target->e1 = 7.3;
    target->e2 = 10.4;
    target->eF = 21.7; //flange thickness
    target->eP = 17.8;
    target->lH = 12.2;
}

void Flange_IntegralTest::Calc_thetaTest() {
    setupTarget();
    target->mShell->phiS = 0.34; // radians
    target->dE = 1.6;
    target->eE = 3.1;
    target->eF = 5.8;
    target->Calc_theta();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_thetaTest()",
             0.1991013590545693, target->theta);
}

void Flange_IntegralTest::Calc_lambdaTest() {
    setupTarget();
    target->eP = 60.1;
    target->eF = 80.2;
    target->Calc_lambda();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_lambdaTest()",
             0.25062344139650872, target->lambda);
}

void Flange_IntegralTest::Calc_kRTest() {
    setupTarget();
    target->mShell->phiS = 0.2967; // ~17 degree
    target->mShell->sType = Shell::Conical;
    target->Calc_kR();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_kRTest()",
             -0.15685347704884936, target->kR);
    target->mShell->sType = Shell::Spherical;
    target->Calc_kR();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_kRTest()",
             -0.679698400545014, target->kR);
    target->mShell->sType = Shell::Cylindrical;
    target->Calc_kR();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_kRTest()",
             -0.15685347704884936, target->kR);
}

void Flange_IntegralTest::Calc_kQTest() {
    setupTarget();
    target->mShell->phiS = 0.2967; // ~17 degree
    target->mShell->sType = Shell::Conical;
    target->Calc_kQ();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_kQTest()",
             0.8888363699434797, target->kQ);
    target->mShell->sType = Shell::Spherical;
    target->Calc_kQ();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_kQTest()",
             0.36599144644731518, target->kQ);
    target->mShell->sType = Shell::Cylindrical;
    target->Calc_kQ();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_kQTest()",
             0.8888363699434797, target->kQ);
}

void Flange_IntegralTest::Calc_hTTest() {
    setupTarget();
    target->gamma = 1.23;
    target->theta = 2.34;
    target->lambda = 3.45;
    target->Calc_hT();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_hTTest()",
             -70.697550306843382, target->hT);
}

void Flange_IntegralTest::Calc_hSTest() {
    setupTarget();
    target->dE = 7.2;
    target->eE = 3.3;
    target->gamma = 1.23;
    target->theta = 2.34;
    target->lambda = 3.45;

    target->Calc_hS();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_hSTest()",
             -14.834159238788418, target->hS);
}

void Flange_IntegralTest::Calc_hRTest() {
    setupTarget();
    target->hS = 1.23;
    target->kR = 2.34;
    target->hT = 7.3;
    target->mShell->phiS = 0.31;
    target->Calc_hR();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_hRTest()",
             1.7090046064655768, target->hR);
}

void Flange_IntegralTest::Calc_hQTest() {
    setupTarget();
    target->hS = 1.23;
    target->kQ = 2.34;
    target->hT = 3.45;
    target->dF = 384.5;
    target->eP = 7.32;
    target->dE = 654.3;
    target->mShell->phiS = 0.31;
    target->mGasket->dGe = 642.1;
    target->Calc_hQ();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_hQTest()",
             2.4619527610220837, target->hQ);
}

void Flange_IntegralTest::Calc_hLTest() {
    setupTarget();
    target->Calc_hL();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_hLTest()",
             0.0, target->hL);
}

void Flange_IntegralTest::Calc_hHTest() {
    setupTarget();
    target->d3e = 418.23;
    target->dE = 397.2;
    target->Calc_hH();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_hHTest()",
             10.515, target->hH);
}

void Flange_IntegralTest::Calc_hGTest() {
    setupTarget();
    target->d3e = 418.23;
    target->mGasket->dGe = 348.6;
    target->Calc_hG();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_hGTest()",
             34.815, target->hG);
}

void Flange_IntegralTest::Calc_gammaTest() {
    setupTarget();
    target->eE = 3.1;
    target->dF = 5.8;
    target->bF = 2.3;
    target->dE = 108.6;
    target->mShell->phiS = 0.7; // radians
    target->Calc_gamma();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_gammaTest()",
             0.094115291416486585, target->gamma);
}

void Flange_IntegralTest::Calc_ALTest() {
    setupTarget();
    target->Calc_AL();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_ALTest()",
             0.0, target->eL);
}

void Flange_IntegralTest::Calc_AFTest() {
    setupTarget();
    target->Calc_AF();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_AFTest()",
             684.635, target->AF);
}

void Flange_IntegralTest::Calc_eETest() {
    setupTarget();
    target->beta = 1.7;
    target->d1 = 327.4; //or (D3+D1)/2 from PCC model
    target->Calc_eE();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_eETest()",
             8.86233459, target->eE);
    target->e1 = 17.8;
    target->e2 = 17.8;
    target->mShell->eS = 21.17;
    target->Calc_eE();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_eETest()",
             21.17, target->eE);
}

void Flange_IntegralTest::Calc_eDTest() {
    setupTarget();
    target->beta = 1.7;
    target->d1 = 327.4; //or (D2+D8)/2 from PCC model
    target->Calc_eD();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_eDTest()",
             9.5296852339, target->eD);
}

void Flange_IntegralTest::Calc_dLTest() {
    setupTarget();
    target->Calc_dL();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_dLTest()",
             0.0, target->dL);
}

void Flange_IntegralTest::Calc_dFTest() {
    setupTarget();
    target->Calc_dF();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_dFTest()",
             351.65, target->dF);
}

void Flange_IntegralTest::Calc_betaTest() {
    setupTarget();
    target->Calc_beta();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_betaTest()",
             1.4246575342, target->beta);
}

void Flange_IntegralTest::Calc_dETest() {
    setupTarget();
    target->d1 = 327.4;
    target->d2 = 327.5;
    target->e1 = 2.1;
    target->e2 = 17.1;
    target->eE = 323.7;
    target->Calc_dE();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_dETest()",
             327.5, target->dE);
    target->e1 = 15.3;
    target->e2 = 15.3;
    target->mShell->dS = 22.7;
    target->Calc_dE();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_dETest()",
             22.7, target->dE);
}

void Flange_IntegralTest::Calc_cFTest() {
    setupTarget();
    target->gamma = 1.23;
    target->theta = 2.34;
    target->lambda = 3.45;
    target->Calc_cF();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_cFTest()",
             0.013072460636480212, target->cF);
}

void Flange_IntegralTest::Calc_bLTest() {
    setupTarget();
    target->Calc_bL();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_bLTest()",
             0.0, target->bL);
}

void Flange_IntegralTest::Calc_bFTest() {
    setupTarget();
    target->d5e = 15.3;
    target->Calc_bF();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_bFTest()",
             16.25, target->bF);
}

void Flange_IntegralTest::Calc_ZLTest() {
    setupTarget();
    target->Calc_ZL();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_ZLTest()",
             0.0, target->ZL);
}

void Flange_IntegralTest::Calc_ZFTest() {
    setupTarget();
    target->dF = 4.3;
    target->cF = 12.3;
    target->bF = 0.8;
    target->eF = 1.7;
    target->Calc_ZF();
    areEqual(PR->getLastOutput(), "Flange_IntegralTest::Calc_ZFTest()",
             12.850150020553141, target->ZF);
}

void Flange_IntegralTest::Is_flange_ValidTest() {
    setupTarget();
    bool actual = false;
    target->bF = 1.2;
    target->eF = 2.3;
    target->mShell->dS = 5.6;
    target->mShell->eS = 6.7;
    target->mShell->phiS = 0.0;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(),
             "Flange_IntegralTest::Is_flange_ValidTest()", true, actual);
    target->bF = 0.12;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(),
             "Flange_IntegralTest::Is_flange_ValidTest()", false, actual);
    target->bF = 120.0;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(),
             "Flange_IntegralTestIs_flange_ValidTest()::", false, actual);
    target->bF = 1.2;
    target->mShell->phiS = 0.3;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(),
             "Flange_IntegralTest::Is_flange_ValidTest()", false, actual);
}
END_NAMESPACE_REDBAG_CALC_EN1591
