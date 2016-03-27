#include "flange_blindtest.h"
NAMESPACE_BILUNA_CALC_EN1591


Flange_BlindTest::Flange_BlindTest() : RB_UnitTest() {
    target = NULL;
}

Flange_BlindTest::~Flange_BlindTest() {
    // these are normally deleted by Assembly
    delete target->mGasket;
    target->mGasket = NULL;
    delete target->mLoadCaseList;
    target->mLoadCaseList = NULL;
    // target itself
    delete target;
    target = NULL;
}

void Flange_BlindTest::exec() {
    Calc_d0Test();
    Calc_ZFTest();
    Calc_ZLTest();
    Calc_bFTest();
    Calc_dETest();
    Calc_dFTest();
    Calc_dLTest();
    Calc_eETest();
    Calc_eLTest();
    Calc_hGTest();
    Calc_hHTest();
    Calc_hLTest();
    Calc_hPTest();
    Calc_hQTest();
    Calc_hRTest();
    Calc_rhoTest();
    Is_d0_ValidTest();
    Calc_WFTest();
    Calc_PhiFTest();
    Is_flange_ValidTest();
    Calc_WXTest();
    Calc_PhiXTest();
}

void Flange_BlindTest::setupTarget() {
    if (!target) {
        //IN similar to integral flange
        target = new Flange_Blind(1);
        target->mGasket = new Gasket();
        target->mLoadCaseList = new LoadCaseList();
        target->mLoadCaseList->createLoadCase(); // includes force creation
        target->mLoadCaseList->createLoadCase();
    }

    target->d0 = 320.1; //inside diameter
    target->d4 = 383.2; //outside diameter
    target->eF = 21.7; //flange thickness
    target->AF = 3300.7; //cross section area
}

void Flange_BlindTest::Calc_d0Test() {
    setupTarget();
    target->mGasket->dG1 = 12.1;
    target->Calc_d0();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_d0Test()",
             12.1, target->d0);
}

void Flange_BlindTest::Calc_ZFTest() {
    setupTarget();
    target->dF = 12.1;
    target->bF = 1.65;
    target->eF = 7.3;
    target->e0 = 6.9;
    target->rho = 1.2;
    target->Calc_ZF();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_ZFTest()", 0.038276423029374368,
             target->ZF);
}

void Flange_BlindTest::Calc_ZLTest() {
    setupTarget();
    target->Calc_ZL();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_ZLTest()", 0.0, target->ZL);
}

void Flange_BlindTest::Calc_bFTest() {
    setupTarget();
    target->d5e = 15.3;
    target->Calc_bF();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_bFTest()", 16.25, target->bF);
}

void Flange_BlindTest::Calc_dETest() {
    setupTarget();
    target->d0 = 17.3;
    target->Calc_dE();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_dETest()", 17.3, target->dE);
}

void Flange_BlindTest::Calc_dFTest() {
    setupTarget();
    target->Calc_dF();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_dFTest()", 351.65, target->dF);
}

void Flange_BlindTest::Calc_dLTest() {
    setupTarget();
    target->Calc_dL();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_dLTest()", 0.0, target->dL);
}

void Flange_BlindTest::Calc_eETest() {
    setupTarget();
    target->Calc_eE();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_eETest()", 0.0, target->eE);
}

void Flange_BlindTest::Calc_eLTest() {
    setupTarget();
    target->Calc_AL();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_eLTest()", 0.0, target->AL);
}

void Flange_BlindTest::Calc_hGTest() {
    setupTarget();
    target->d3e = 418.23;
    target->mGasket->dGe = 348.6;
    target->Calc_hG();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_hGTest()", 34.815,
             target->hG);
}

void Flange_BlindTest::Calc_hHTest() {
    setupTarget();
    target->d3e = 418.23;
    target->dE = 397.2;
    target->Calc_hH();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_hHTest()", 10.515,
             target->hH);
}

void Flange_BlindTest::Calc_hLTest() {
    setupTarget();
    target->Calc_hL();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_hLTest()", 0.0,
             target->hL);
}

void Flange_BlindTest::Calc_hPTest() {
    setupTarget();
    target->eP = 17.8; // just to test whether eP is set to zero
    target->dE = 16.5;
    target->dF = 103.2;
    target->mGasket->dGe = 150.14;
    target->Calc_hP();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_hPTest()",
             41.829879640183911, target->hP, 0.000001);
}

void Flange_BlindTest::Calc_hQTest() {
    setupTarget();
    target->dE = 12.1;
    target->rho = 1.65;
    target->mGasket->dGe = 7.3;
    target->Calc_hQ();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_hQTest()",
             -16.35146231289276, target->hQ);
}

void Flange_BlindTest::Calc_hRTest() {
    setupTarget();
    target->dE = 12.1;
    target->rho = 1.65;
    target->Calc_hR();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_hRTest()", -3.1976201101327963,
             target->hR);
}

void Flange_BlindTest::Calc_rhoTest() {
    setupTarget();
    target->d9 = 12.1;
    target->dE = 7.3;
    target->Calc_rho();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_rhoTest()", 1.6575342465753424,
             target->rho);
}

void Flange_BlindTest::Is_d0_ValidTest() {
    setupTarget();
    target->d0 = 23.5;
    target->mGasket->dG1 = 24.5;
    bool ret = target->Is_d0_Valid();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_Is_d0_ValidTest()", 1,
             static_cast<int>(ret));
}

void Flange_BlindTest::Calc_WFTest() {
    setupTarget();
    int loadCaseNo = 0;

    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->fF1 = 1.1;
    target->bF = 2.3;
    target->eF = 3.4;
    target->d0 = 5.4;
    target->rho = 8.1;
    target->e0 = 2.0;

    target->Calc_WF(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_WFTest()", -86.55276256199096,
             target->mLoadCaseList->at(loadCaseNo)->WF1);
}

void Flange_BlindTest::Calc_PhiFTest() {
    setupTarget();
    int loadCaseNo = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->F_Bmax = 1.1;
    target->hG = 2.3;
    loadCase->F_Q = 3.4;
    target->rho = 4.5;
    target->mGasket->dGe = 5.6;
    loadCase->F_R = 6.7;
    loadCase->WF1 = 4.3;

    target->Calc_PhiF(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_PhiFTest",
             81.192248062015508,
             target->mLoadCaseList->at(loadCaseNo)->PhiF1);

    loadCaseNo = 1;
    loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->F_B = 1.1;
    target->hG = 2.3;
    loadCase->F_Q = 3.4;
    target->rho = 4.5;
    target->mGasket->dGe = 5.6;
    loadCase->F_R = 6.7;
    loadCase->WF1 = 8.6;

    target->Calc_PhiF(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_PhiFTest",
             40.596124031007754,
             target->mLoadCaseList->at(loadCaseNo)->PhiF1);
}

void Flange_BlindTest::Is_flange_ValidTest() {
    setupTarget();
    bool actual = false;
    target->bF = 1.2;
    target->eF = 2.3;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_Is_flange_ValidTest()", 1,
             static_cast<int>(actual));
    target->bF = 0.12;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_Is_flange_ValidTest()", 0,
             static_cast<int>(actual));
    target->bF = 120.0;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_Is_flange_ValidTest()", 0,
             static_cast<int>(actual));
}

void Flange_BlindTest::Calc_WXTest() {
    setupTarget();
    int loadCaseNo = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->fF1 = 5.6;
    target->d4 = 2.3;
    target->d5e = 4.5;
    target->dX = 4.9;
    target->eF = 9.5;
    target->eX = 8.3;
    target->Calc_WX(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_WXTest()", -3119.8358678266227,
             target->mLoadCaseList->at(loadCaseNo)->WX1);
}

void Flange_BlindTest::Calc_PhiXTest() {
    setupTarget();
    int loadCaseNo = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->F_Bmax = 9.3;
    target->d3 = 6.4;
    target->dX = 4.6;
    loadCase->WX1 = 3.2;

    target->Calc_PhiX(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_Calc_PhiXTest()",
             2.615625,
             target->mLoadCaseList->at(loadCaseNo)->PhiX1);

    loadCaseNo = 1;
    loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->F_B = 9.3;
    target->d3 = 6.4;
    target->dX = 4.6;
    loadCase->WX1 = 6.4;

    target->Calc_PhiX(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_BlindTest::Calc_Calc_PhiXTest()",
             1.3078125,
             target->mLoadCaseList->at(loadCaseNo)->PhiX1);
}

END_NAMESPACE_BILUNA_CALC_EN1591
