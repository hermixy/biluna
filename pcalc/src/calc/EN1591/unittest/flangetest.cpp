#include "flangetest.h"

#include "flange_integral.h"
NAMESPACE_REDBAG_CALC_EN1591

FlangeTest::FlangeTest() : RB_UnitTest() {
    target = NULL;
}

FlangeTest::~FlangeTest() {
    // these are normally deleted by Assembly
    delete target->mBolt;
    target->mBolt = NULL;
    delete target->mLoadCaseList;
    target->mLoadCaseList = NULL;
    // target itself
    delete target;
    target = NULL;
}

void FlangeTest::exec() {
    Calc_d3eTest();
    Calc_d5eTest();
    Calc_pBTest();
    Calc_hPTest();
    Calc_eQTest();
    Calc_ABTest();
    Calc_XBTest();
    Calc_etanminusTest();
    Calc_etanplusTest();
    Calc_deltaQTest();
    Calc_deltaRTest();
    Calc_cMTest();
    Is_cM_validTest();
    Calc_cSTest();
    Is_cS_validTest();
    Calc_jMTest();
    Calc_PsiOptTest();
    Calc_Psi0Test();
    Calc_PsiMaxTest();
    Is_PsiMaxMin_ValidTest();
    Calc_PsiZTest();
    Calc_WFTest();
    Calc_PhiFTest();
    Is_PhiF_validTest();
    Calc_dK1Test();
    Calc_dK2Test();
    Calc_XWTest();
}

void FlangeTest::setupTarget() {
    if (!target) {
        // Flange itself is abstract class
        target = new Flange_Integral(1); // includes shell and washer
        target->mBolt = new Bolt();
        target->mLoadCaseList = new LoadCaseList();
        target->mLoadCaseList->createLoadCase(); // includes force creation
    }

    target->eP = 0.8;
    target->eF = 15.6;
    target->nB = 16;
    target->d3 = 410.45;

    target->mBolt->mBoltHole->d5 = 43.7;
    target->mBolt->mBoltHole->isBlindHole = false; // threaded
    target->mBolt->mBoltHole->d5t = 24.33;
    target->mBolt->l5t = 42.7;
    target->mBolt->mBoltHole->eFb = 0.31;
}

void FlangeTest::Calc_d3eTest() {
    setupTarget();
    target->Calc_d3e();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_d3eTest()", 407.243359375, target->d3e);
}

void FlangeTest::Calc_d5eTest() {
    setupTarget();
    target->pB = 87.3;
    target->Calc_d5e();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_d5eTest()", 30.918259, target->d5e);
    target->mBolt->mBoltHole->isBlindHole = true;
    target->Calc_d5e();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_d5eTest()", 20763.727150987565, target->d5e);
}

void FlangeTest::Calc_pBTest() {
    setupTarget();
    target->Calc_pB();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_pBTest()", 80.591669, target->pB);
}

void FlangeTest::Calc_hPTest() {
    setupTarget();
    target->eP = 17.8;
    target->dE = 16.5;
    target->dF = 103.2;
    target->mLoadCaseList->at(0)->dGe = 150.14;
    target->Calc_hP(0);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_hPTest()", 44.730940732332016, target->hP);
}

void FlangeTest::Calc_eQTest() {
    setupTarget();
    target->Calc_eQ();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_eQTest()", 14.8, target->eQ);
}

void FlangeTest::Calc_ABTest() {
    setupTarget();
    target->nB = 17;
    target->mBolt->dBe = 18.7;
    target->mBolt->dBS = 21.3;
    target->Calc_AB();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_ABTest()", 4668.9800238937132,
             target->mBolt->AB);
}

void FlangeTest::Calc_XBTest() {
    setupTarget();
    target->nB = 12;
    target->mBolt->dB0 = 7.8;
    target->mBolt->dBe = 18.7;
    target->mBolt->dBS = 21.3;
    target->mBolt->lB = 9.1;
    target->mBolt->lS = 19.1;
    target->Calc_XB();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_XBTest()", 0.012315046218782398,
             target->mBolt->XB);
}

void FlangeTest::Calc_etanminusTest() {
    setupTarget();
    target->mBolt->eta1minus = 0.13;
    target->nB = 19;
    target->Calc_etanminus();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_etanminusTest()", 0.05486803405237977,
             target->mBolt->etanminus);
}

void FlangeTest::Calc_etanplusTest() {
    setupTarget();
    target->mBolt->eta1minus = 0.27;
    target->nB = 21;
    target->Calc_etanminus();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_etanplusTest()", 0.11168912277278846,
             target->mBolt->etanminus);
}

void FlangeTest::Calc_deltaQTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->mLoadCaseList->at(loadCaseNo)->P = 1.1;
    target->dE = 2.2;
    target->mLoadCaseList->at(loadCaseNo)->fE1 = 3.3;
    target->eD = 4.4;
    target->mShell->phiS = 0.2;
    target->Calc_deltaQ(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_deltaQTest()", 0.085028237078432725,
             target->mLoadCaseList->at(loadCaseNo)->deltaQ1);
}

void FlangeTest::Calc_deltaRTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->mLoadCaseList->at(loadCaseNo)->F_R = 12.1;
    target->mLoadCaseList->at(loadCaseNo)->fE1 = 13.1;
    target->dE = 14.1;
    target->eD = 15.1;
    target->mShell->phiS = 0.45;
    target->Calc_deltaR(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_deltaRTest()", 0.0015335923629893277,
             target->mLoadCaseList->at(loadCaseNo)->deltaR1);
}

void FlangeTest::Calc_cMTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->mShell->sType = target->mShell->Cylindrical;
    target->mLoadCaseList->at(loadCaseNo)->deltaQ1 = 1.1;
    target->mLoadCaseList->at(loadCaseNo)->deltaR1 = 2.2;
    target->Calc_cM(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_cMTest()", 5.437423609377193,
             target->mLoadCaseList->at(loadCaseNo)->cM1);
    target->mShell->sType = target->mShell->Spherical;
    target->Calc_cM(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_cMTest()", 9.2779917323706158,
             target->mLoadCaseList->at(loadCaseNo)->cM1);
}

void FlangeTest::Is_cM_validTest() {
    setupTarget();
    int loadCaseNo = 0;
    bool expected = false;
    bool actual = false;
    target->mLoadCaseList->at(loadCaseNo)->cM1 = -1.1;
    actual = target->Is_cM_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_cM_validTest()", expected, actual);
}

void FlangeTest::Calc_cSTest() {
    setupTarget();
    int loadCaseNo = 0; // TODO: Initialize to an appropriate value
    target->mLoadCaseList->at(loadCaseNo)->deltaQ1 = 1.1;
    target->mLoadCaseList->at(loadCaseNo)->deltaR1 = 2.2;
    target->mShell->sType = target->mShell->Spherical;
    target->Calc_cS(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_cSTest()", -4.671875,
             target->mLoadCaseList->at(loadCaseNo)->cS_plus1);
    target->mLoadCaseList->at(loadCaseNo)->deltaQ1 = 0.1;
    target->mLoadCaseList->at(loadCaseNo)->deltaR1 = 0.2;
    target->Calc_cS(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_cSTest()", 0.98275396382587354,
             target->mLoadCaseList->at(loadCaseNo)->cS_plus1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_cSTest()", 0.550784973957277,
             target->mLoadCaseList->at(loadCaseNo)->cS_minus1);
    target->mShell->sType = target->mShell->Conical;
    target->Calc_cS(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_cSTest()", 0.78640442297651147,
             target->mLoadCaseList->at(loadCaseNo)->cS_plus1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_cSTest()", 0.74713451480663906,
             target->mLoadCaseList->at(loadCaseNo)->cS_minus1);
}

void FlangeTest::Is_cS_validTest() {
    setupTarget();
    int loadCaseNo = 0;
    bool expected = false;
    bool actual = false;
    target->mLoadCaseList->at(loadCaseNo)->cS_minus1 = 0.1;
    target->mLoadCaseList->at(loadCaseNo)->cS_plus1 = -0.1;
    actual = target->Is_cS_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_cS_validTest()", expected, actual);
    target->mLoadCaseList->at(loadCaseNo)->cS_minus1 = -0.2;
    target->mLoadCaseList->at(loadCaseNo)->cS_plus1 = 0.2;
    actual = target->Is_cS_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_cS_validTest()", expected, actual);
    target->mLoadCaseList->at(loadCaseNo)->cS_minus1 = -0.3;
    target->mLoadCaseList->at(loadCaseNo)->cS_plus1 = -0.3;
    actual = target->Is_cS_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_cS_validTest()", expected, actual);
    expected = true;
    target->mLoadCaseList->at(loadCaseNo)->cS_minus1 = 0.4;
    target->mLoadCaseList->at(loadCaseNo)->cS_plus1 = 0.5;
    actual = target->Is_cS_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_cS_validTest()", expected, actual);
}

void FlangeTest::Calc_jMTest() {
    setupTarget();
    int loadCaseNo = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->F_G = 10.1;
    loadCase->hG1 = 2.4;
    loadCase->F_Q = 5.1;
    loadCase->hH1 = 4.9;
    target->hP = 8.1;
    loadCase->F_R = 9.3;
    loadCase->hH1 = 2.3;

    target->Calc_jM(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_jMTest()", 1,
             target->mLoadCaseList->at(loadCaseNo)->jM1);
    target->mLoadCaseList->at(loadCaseNo)->F_G = -10.1;
    target->Calc_jM(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_jMTest()", -1,
             target->mLoadCaseList->at(loadCaseNo)->jM1);
}

void FlangeTest::Calc_PsiOptTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->mLoadCaseList->at(loadCaseNo)->jM1 = 1;
    target->eP = 2.2;
    target->eF = 3.9;
    target->Calc_PsiOpt(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiOptTest()", 0.12820512820512819,
             target->mLoadCaseList->at(loadCaseNo)->PsiOpt1);
    target->mLoadCaseList->at(loadCaseNo)->jM1 = -1;
    target->Calc_PsiOpt(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiOptTest()", -0.12820512820512819,
             target->mLoadCaseList->at(loadCaseNo)->PsiOpt1);
}

void FlangeTest::Calc_Psi0Test() {
    setupTarget();
    int loadCaseNo = 0;
    target->Calc_Psi0(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_Psi0Test()", 0,
             target->mLoadCaseList->at(loadCaseNo)->jS);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_Psi0Test()", 0,
             target->mLoadCaseList->at(loadCaseNo)->kM1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_Psi0Test()", 0,
             target->mLoadCaseList->at(loadCaseNo)->kS);
}

void FlangeTest::Calc_PsiMaxTest() {
    setupTarget();
    int loadCaseNo = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->fE1 = 1.1;
    target->dE = 2.2;
    target->eD = 3.3;
    target->mShell->phiS = 0.45;
    loadCase->fF1 = 5.5;
    target->bF = 6.6;
    target->eF = 7.7;
    loadCase->deltaQ1 = 0.1;
    loadCase->deltaR1 = 0.2;
    target->eP = 8.8;
    loadCase->jS = 1;
    loadCase->kS = 1;
    loadCase->cM1 = 0.3;
    loadCase->cS_plus1 = 0.4;
    loadCase->kM1 = 1;

    target->Calc_PsiMax(loadCaseNo);
    // 1.1*2.2*3.3*cos(0.45)/(5.5*2*6.6*7.7)*((0.5*0.1+0.2)
    // *tan(0.45)-0.1*2*8.8/2.2+1*1*((3.3*0.3*0.4*(1+1*1))
    // /(2.2*(cos(0.45))^3))^0.5)
    //   0.00029545994904527013
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiMaxTest()", 0.00029545994904527014,
             target->mLoadCaseList->at(loadCaseNo)->Psi_jkk1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiMaxTest()", 0.00029545994904527014,
             target->mLoadCaseList->at(loadCaseNo)->PsiMax1);
}

void FlangeTest::Is_PsiMaxMin_ValidTest() {
    setupTarget();
    int loadCaseNo = 0;
    bool expected = false;
    bool actual = false;
    target->mLoadCaseList->at(loadCaseNo)->PsiMax1 = -1.1;
    target->mLoadCaseList->at(loadCaseNo)->PsiMin1 = 0.9;
    actual = target->Is_PsiMaxMin_Valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_PsiMaxMin_ValidTest()", expected, actual);
    target->mLoadCaseList->at(loadCaseNo)->PsiMax1 = -0.9;
    target->mLoadCaseList->at(loadCaseNo)->PsiMin1 = 1.1;
    actual = target->Is_PsiMaxMin_Valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_PsiMaxMin_ValidTest()", expected, actual);
    target->mLoadCaseList->at(loadCaseNo)->PsiMax1 = -2.1;
    target->mLoadCaseList->at(loadCaseNo)->PsiMin1 = 2.1;
    actual = target->Is_PsiMaxMin_Valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_PsiMaxMin_ValidTest()", expected, actual);
    expected = true;
    target->mLoadCaseList->at(loadCaseNo)->PsiMax1 = -0.8;
    target->mLoadCaseList->at(loadCaseNo)->PsiMin1 = 0.8;
    actual = target->Is_PsiMaxMin_Valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_PsiMaxMin_ValidTest()", expected, actual);
}

void FlangeTest::Calc_PsiZTest() {
    setupTarget();
    int loadCaseNo = 0;

    // No optimatization of kM required
    target->mLoadCaseList->at(loadCaseNo)->jM1 = 1;
    target->mLoadCaseList->at(loadCaseNo)->PsiOpt1 = 0.95;
    target->mLoadCaseList->at(loadCaseNo)->PsiMax1 = 0.9;
    target->mLoadCaseList->at(loadCaseNo)->PsiMin1 = 0.5;
    target->mLoadCaseList->at(loadCaseNo)->Psi01 = 0.7;
    target->Calc_PsiZ(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", 0.9,
             target->mLoadCaseList->at(loadCaseNo)->PsiZ1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", 1.0,
             target->mLoadCaseList->at(loadCaseNo)->kM1);
    target->mLoadCaseList->at(loadCaseNo)->PsiOpt1 = 0.8;
    target->Calc_PsiZ(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", 0.8,
             target->mLoadCaseList->at(loadCaseNo)->PsiZ1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", 1.0,
             target->mLoadCaseList->at(loadCaseNo)->kM1);
    target->mLoadCaseList->at(loadCaseNo)->jM1 = -1;
    target->mLoadCaseList->at(loadCaseNo)->PsiOpt1 = 0.3;
    target->Calc_PsiZ(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", 0.5,
             target->mLoadCaseList->at(loadCaseNo)->PsiZ1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", -1.0,
             target->mLoadCaseList->at(loadCaseNo)->kM1);
    target->mLoadCaseList->at(loadCaseNo)->jM1 = -1;
    target->mLoadCaseList->at(loadCaseNo)->PsiOpt1 = 0.6;
    target->Calc_PsiZ(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", 0.6,
             target->mLoadCaseList->at(loadCaseNo)->PsiZ1);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PsiZTest()", -1.0,
             target->mLoadCaseList->at(loadCaseNo)->kM1);

    // Optimatization of kM required, TODO
    //target.LoadCaseList(loadCaseNo).jM1 = 1
    //target.LoadCaseList(loadCaseNo).PsiOpt1 = 0.6
    //target.Calc_PsiZ(loadCaseNo)
    //Assert.AreEqual(0.0, target.LoadCaseList(loadCaseNo).PsiZ1)
    //Assert.AreEqual(0.0, target.LoadCaseList(loadCaseNo).kM1)
    //target.LoadCaseList(loadCaseNo).jM1 = -1
    //target.LoadCaseList(loadCaseNo).PsiOpt1 = 0.8
    //target.Calc_PsiZ(loadCaseNo)
    //Assert.AreEqual(0.0, target.LoadCaseList(loadCaseNo).PsiZ1)
    //Assert.AreEqual(0.0, target.LoadCaseList(loadCaseNo).kM1)
}

void FlangeTest::Calc_WFTest() {
    setupTarget();
    int loadCaseNo = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->fE1 = 1.1;
    loadCase->fF1 = 2.2;
    loadCase->cM1 = 0.4;
    loadCase->jM1 = 1;
    loadCase->kM1 = 0.5;
    loadCase->PsiOpt1 = 0.7;
    loadCase->PsiZ1 = 0.8;
    target->bF = 3.3;
    target->eF = 4.4;
    target->dE = 5.5;
    target->eD = 6.6;

    target->Calc_WF(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_WFTest()", 368.15244856183011,
             target->mLoadCaseList->at(loadCaseNo)->WF1);
}

void FlangeTest::Calc_PhiFTest() {
    setupTarget();
    int loadCaseNo = 0;

    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->F_G = 1.1;
    loadCase->hG1 = 2.2;
    loadCase->F_Q = 3.3;
    loadCase->hH1 = 4.4;
    target->hP = 5.5;
    loadCase->F_R = 5.9;
    //.hH1 = 6.6
    loadCase->WF1 = 7.7;

    target->Calc_PhiF(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_PhiFTest()", 3.2142857142857144,
             target->mLoadCaseList->at(loadCaseNo)->PhiF1);
}

void FlangeTest::Is_PhiF_validTest() {
    setupTarget();
    int loadCaseNo = 0;
    bool expected = false;
    bool actual = false;
    target->mLoadCaseList->at(loadCaseNo)->PhiF1 = 2.1;
    actual = target->Is_PhiF_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_PhiF_validTest()", expected, actual);
    expected = true;
    target->mLoadCaseList->at(loadCaseNo)->PhiF1 = 0.1;
    actual = target->Is_PhiF_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "FlangeTest::Is_PhiF_validTest()", expected, actual);
}

void FlangeTest::Calc_dK1Test() {
    setupTarget();
    target->mWasher->eW = 2;
    target->mWasher->dW1 = 3;
    target->mWasher->dW2 = 52.5;
    target->mBolt->mBoltHole->d5 = 2.1;
    target->mWasher->dW1 = 2.3;
    target->Calc_dK1();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_dK1Test()", 2.3, target->mWasher->dK1);
    target->mWasher->dW1 = 10.9;
    target->Calc_dK1();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_dK1Test()", 10.9, target->mWasher->dK1);
}

void FlangeTest::Calc_dK2Test() {
    setupTarget();
    target->mWasher->eW = 2;
    target->mWasher->dW1 = 0.1;
    target->mBolt->dB4 = 2.1;
    target->mWasher->dW2 = 2.3;
    target->Calc_dK2();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_dK2Test()", 2.1, target->mWasher->dK2);
    target->mWasher->dW2 = 0.9;
    target->Calc_dK2();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_dK2Test()", 0.9, target->mWasher->dK2);
}

void FlangeTest::Calc_XWTest() {
    setupTarget();
    target->mWasher->eW = 3.1;
    target->mWasher->dW1 = 42.5;
    target->mWasher->dW2 = 52.5;
    target->nB = 16;
    target->mWasher->dW = 32.5;
    target->mWasher->bW = 171.8;
    target->mWasher->bKB = 3.123;
    target->Calc_XW();
    areEqual(PR->getLastOutput(), "FlangeTest::Calc_XWTest()", 0.000021504377065960737,
             target->mWasher->XW);
}

END_NAMESPACE_REDBAG_CALC_EN1591
