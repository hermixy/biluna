#include "flange_loosetest.h"
NAMESPACE_BILUNA_CALC_EN1591


Flange_LooseTest::Flange_LooseTest()
            : RB_UnitTest() {
    target = NULL;
}

Flange_LooseTest::~Flange_LooseTest() {
    // these are normally deleted by Assembly (Shell deleted with flange)
    delete target->mBolt;
    target->mBolt = NULL;
    delete target->mGasket;
    target->mGasket = NULL;
    delete target->mLoadCaseList;
    target->mLoadCaseList = NULL;
    // target itself
    delete target;
    target = NULL;
}

void Flange_LooseTest::exec() {
    Calc_ZFTest();
    Calc_ZLTest();
    Calc_bFTest();
    Calc_bLTest();
    Calc_cFTest();
    Calc_d7Test();
    Calc_d7maxTest();
    Calc_d7minTest();
    Calc_betaTest();
    Calc_dETest();
    Calc_dFTest();
    Calc_dLTest();
    Calc_eETest();
    Calc_AFTest();
    Calc_eFTest();
    Calc_ALTest();
    Calc_eLTest();
    Calc_gammaTest();
    Calc_hGTest();
    Calc_hHTest();
    Calc_hLTest();
    Calc_hQTest();
    Calc_hRTest();
    Calc_hSTest();
    Calc_hTTest();
    Calc_kQTest();
    Calc_kRTest();
    Calc_chiTest();
    Calc_lambdaTest();
    Calc_thetaTest();
    Calc_WLTest();
    Calc_PhiLTest();
    Is_PhiL_validTest();
    Calc_WQTest();
    Calc_WFTest();
    Calc_PhiFTest();
    Is_flange_ValidTest();

}

void Flange_LooseTest::setupTarget() {
    if (!target) {
        //IN similar to integral flange
        target = new Flange_Loose(1); // includes shell and washer
        target->mBolt = new Bolt();
        target->mGasket = new Gasket();
        target->mLoadCaseList = new LoadCaseList();
        target->mLoadCaseList->createLoadCase(); // includes force creation
        target->mLoadCaseList->createLoadCase();
    }

    target->d0 = 320.1; //inside diameter
    target->d4 = 383.2; //outside diameter loose
    target->d6 = 325.4; //inside diameter loose
    target->d8 = 344.3; //outside diameter ring/collar
    target->eF = 21.7; //flange thickness
    target->eFb = 7.3; //flange thickness
    target->eL = 15.3;
}

void Flange_LooseTest::Calc_ZFTest() {
    setupTarget();
    target->dF = 4.3;
    target->cF = 12.3;
    target->bF = 0.8;
    target->eF = 1.7;
    target->Calc_ZF();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_ZFTest()", 12.850150020553141, target->ZF);
}

void Flange_LooseTest::Calc_ZLTest() {
    setupTarget();
    target->dL = 4.3;
    target->bL = 0.8;
    target->eL = 1.7;
    target->Calc_ZL();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_ZLTest()", 1.0447276439474098, target->ZL);
}

void Flange_LooseTest::Calc_bFTest() {
    setupTarget();
    target->Calc_bF();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_bFTest()", 12.1, target->bF);
}

void Flange_LooseTest::Calc_bLTest() {
    setupTarget();
    target->d5e = 15.3;
    target->Calc_bL();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_bLTest()", 13.6, target->bL);
}

void Flange_LooseTest::Calc_cFTest() {
    setupTarget();
    target->gamma = 1.23;
    target->theta = 2.34;
    target->lambda = 3.45;
    target->Calc_cF();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_cFTest()",
             0.013072460636480212, target->cF);
}

void Flange_LooseTest::Calc_d7Test() {
    setupTarget();
    target->d7min = 203.8;
    target->d7max = 217.3;
    target->mGasket->dGe = 212.7;
    target->chi = 0.1;
    target->d3e = 225.7;
    target->Calc_d70();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_d7Test()",
             213.88181818181818, target->d7);
}

void Flange_LooseTest::Calc_d7maxTest() {
    setupTarget();
    target->d8 = 412.7;
    target->Calc_d7max();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_d7maxTest()",
             412.7, target->d7max);
}

void Flange_LooseTest::Calc_d7minTest() {
    setupTarget();
    target->d6 = 312.1;
    target->b0 = 8.7;
    target->Calc_d7min();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_d7minTest()", 329.5, target->d7min);
}

void Flange_LooseTest::Calc_betaTest() {
    setupTarget();
    target->e1 = 7.3;
    target->e2 = 10.4;
    target->Calc_beta();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_betaTest()", 1.4246575342, target->beta);
}

void Flange_LooseTest::Calc_dETest() {
    setupTarget();
    // d1 - e1 + eE, d2 + e2 - eE
    target->d1 = 17.4;
    target->d2 = 17.5;
    target->e1 = 7.3;
    target->e2 = 10.4;
    target->eE = 3.7;
    target->Calc_dE();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_dETest()", 17.4, target->dE);
    target->e1 = 0.0;
    target->e2 = 0.0;
    target->mShell->dS = 21.17;
    target->Calc_dE();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_dETest()", 21.17, target->dE);
}

void Flange_LooseTest::Calc_dFTest() {
    setupTarget();
    target->Calc_dF();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_dFTest()", 332.2, target->dF);
}

void Flange_LooseTest::Calc_dLTest() {
    setupTarget();
    target->Calc_dL();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_dLTest()", 354.3, target->dL);
}

void Flange_LooseTest::Calc_eETest() {
    setupTarget();
    target->mShell->eS = 2.7;
    target->Calc_eE();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_eETest(", 2.7, target->eE);
}

void Flange_LooseTest::Calc_AFTest() {
    setupTarget();
    target->d0 = 320.1;
    target->d8 = 344.3;
    target->eFb = 7.3;
    target->Calc_AF();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_AFTest()",
             88.33, target->AF);
}

void Flange_LooseTest::Calc_eFTest() {
    setupTarget();
    target->AF = 51.3;
    target->d8 = 52.6;
    target->d0 = 78.1;
    target->Calc_eF();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_eFTest()",
             -4.02352941176470588235, target->eF);
}

void Flange_LooseTest::Calc_ALTest() {
    setupTarget();
    target->Calc_AL();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_ALTest()", 442.17, target->AL);
}

void Flange_LooseTest::Calc_eLTest() {
    setupTarget();
    target->AL = -31.9;
    target->d4 = 16.4;
    target->d6 = 3.1;
    target->Calc_eL();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_eLTest()",
             -4.7969924812030075188, target->eL);
}

void Flange_LooseTest::Calc_gammaTest() {
    setupTarget();
    target->eE = 3.1;
    target->dF = 5.8;
    target->bF = 2.3;
    target->dE = 108.6;
    target->mShell->phiS = 0.7; // radians
    target->Calc_gamma();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_gammaTest()", 0.094115291416486585,
             target->gamma);
}

void Flange_LooseTest::Calc_hGTest() {
    setupTarget();
    target->setFlangeNumber(1);
    target->d7 = 234.9;
    target->mGasket->dGe = 222.1;
    target->Calc_hG();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_hGTest()", 6.4,
             target->hG);
}

void Flange_LooseTest::Calc_hHTest() {
    setupTarget();
    target->setFlangeNumber(1);
    target->d7 = 34.1;
    target->dE = 27.1;
    target->Calc_hH();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_hHTest()", 3.5,
             target->hH);
}

void Flange_LooseTest::Calc_hLTest() {
    setupTarget();
    target->setFlangeNumber(1);
    target->d7 = 131.4;
    target->d3e = 27.3;
    target->Calc_hL();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_hLTest()", -52.05,
             target->hL);
}

void Flange_LooseTest::Calc_hQTest() {
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
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_hQTest()",
             2.4619527610220837, target->hQ);
}

void Flange_LooseTest::Calc_hRTest() {
    setupTarget();
    target->hS = 1.23;
    target->kR = 2.34;
    target->hT = 7.3;
    target->mShell->phiS = 0.31;
    target->Calc_hR();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_hRTest()",
             1.7090046064655768, target->hR);
}

void Flange_LooseTest::Calc_hSTest() {
    setupTarget();
    target->dE = 7.2;
    target->eE = 3.3;
    target->gamma = 1.23;
    target->theta = 2.34;
    target->lambda = 3.45;

    target->Calc_hS();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_hSTest()",
             -14.834159238788418, target->hS);
}

void Flange_LooseTest::Calc_hTTest() {
    setupTarget();
    target->gamma = 1.23;
    target->theta = 2.34;
    target->lambda = 3.45;

    target->Calc_hT();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_hTTest()", -70.697550306843382,
             target->hT);
}

void Flange_LooseTest::Calc_kQTest() {
    setupTarget();
    target->mShell->phiS = 0.2967; // ~17 degree
    target->mShell->sType = Shell::Conical;
    target->Calc_kQ();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_kQTest()", 0.8888363699434797,
             target->kQ);
    target->mShell->sType = Shell::Spherical;
    target->Calc_kQ();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_kQTest()", 0.36599144644731518,
             target->kQ);
    target->mShell->sType = Shell::Cylindrical;
    target->Calc_kQ();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_kQTest()", 0.8888363699434797,
             target->kQ);
}

void Flange_LooseTest::Calc_kRTest() {
    setupTarget();
    target->mShell->phiS = 0.2967; // ~17 degree
    target->mShell->sType = Shell::Conical;
    target->Calc_kR();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_kRTest()", -0.15685347704884936,
             target->kR);
    target->mShell->sType = Shell::Spherical;
    target->Calc_kR();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_kRTest()", -0.679698400545014,
             target->kR);
    target->mShell->sType = Shell::Cylindrical;
    target->Calc_kR();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_kRTest()", -0.15685347704884936,
             target->kR);
}

void Flange_LooseTest::Calc_chiTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->setFlangeNumber(1);
    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    target->ZF = 2.3;
    target->ZL = 3.4;
    loadCase->EF1 = 4.5;
    loadCase->EL1 = 5.6;
    target->Calc_chi(/*loadCaseNo*/);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_chiTest()", 1.1878881987577641,
             target->chi);
}

void Flange_LooseTest::Calc_lambdaTest() {
    setupTarget();
    target->eP = 60.1;
    target->eF = 87.2;
    target->Calc_lambda();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_lambdaTest()", 0.31077981651376146,
             target->lambda);
}

void Flange_LooseTest::Calc_thetaTest() {
    setupTarget();
    target->mShell->phiS = 0.7; // radians
    target->dE = 1.6;
    target->eE = 3.1;
    target->eF = 5.8;
    target->Calc_theta();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_thetaTest()", 0.16152783382436875,
             target->theta);
}

void Flange_LooseTest::Calc_WLTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->setFlangeNumber(1);
    target->mLoadCaseList->at(loadCaseNo)->fL1 = 5.6;
    target->bL = 5.6;
    target->eL = 21.5;

    target->Calc_WL(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_WLTest()", 22770.514880631108,
             target->mLoadCaseList->at(loadCaseNo)->WL1);
}

void Flange_LooseTest::Calc_PhiLTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->setFlangeNumber(1);
    target->mLoadCaseList->at(loadCaseNo)->F_Bmax = 4.5;
    target->hL = 6.3;
    target->mLoadCaseList->at(loadCaseNo)->WL1 = 5.1;
    target->Calc_PhiL(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_PhiLTest()",
             5.5588235294117645,
             target->mLoadCaseList->at(loadCaseNo)->PhiL1);

    loadCaseNo = 1;
    target->mLoadCaseList->at(loadCaseNo)->F_B = 4.5;
    target->hL = 6.3;
    target->mLoadCaseList->at(loadCaseNo)->WL1 = 10.2;
    target->Calc_PhiL(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_PhiLTest()",
             2.77941176470588225,
             target->mLoadCaseList->at(loadCaseNo)->PhiL1);
}

void Flange_LooseTest::Is_PhiL_validTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->setFlangeNumber(1);
    bool expected = false;
    bool actual = false;
    target->mLoadCaseList->at(loadCaseNo)->PhiL1 = 1.1;
    actual = target->Is_PhiL_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Is_PhiL_validTest()", expected, actual);
    expected = true;
    target->mLoadCaseList->at(loadCaseNo)->PhiL1 = 0.9;
    actual = target->Is_PhiL_valid(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Is_PhiL_validTest()", expected, actual);
}

void Flange_LooseTest::Calc_WQTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->setFlangeNumber(1);

    LoadCase* loadCase = target->mLoadCaseList->at(loadCaseNo);
    loadCase->fS1 = 0.011;
    target->mShell->eS = 1.4;
    loadCase->fF1 = 1.2;
    target->eF = 1.3;
    loadCase->Q_smax = 0.015;
    target->mGasket->dG2 = 1.6;
    target->d7 = 1.7;
    target->mShell->dS = 1.8;
    target->Calc_WQ(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_WQTest()", 0.0305327463011575,
             loadCase->WQ1);
    loadCase->fS1 = 1.1;
    loadCase->fF1 = 0.012;
    loadCase->Q_smax = 1.5;
    target->d7 = 7.7;
    target->Calc_WQ(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_WQTest()", 0.057340349113320907,
             loadCase->WQ1);

}

void Flange_LooseTest::Calc_WFTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->setFlangeNumber(1);
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
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_WFTest()",
             368.15244856183011, target->mLoadCaseList->at(loadCaseNo)->WF1);
}

void Flange_LooseTest::Calc_PhiFTest() {
    setupTarget();
    int loadCaseNo = 0;
    target->setFlangeNumber(1);

    LoadCase* loadCase =
            target->mLoadCaseList->at(loadCaseNo);
    target->mGasket->dG2 = 2.9;
    target->d7 = 2.7;
    loadCase->F_Q = -102.3;
    loadCase->F_R = 2.4;
    target->hH = 6.2;
    loadCase->WQ1 = 3.2;
    target->Calc_PhiF(loadCaseNo);
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Calc_PhiFTest()",
             193.55625, loadCase->PhiF1);
}

void Flange_LooseTest::Is_flange_ValidTest() {
    setupTarget();
    bool actual = false;
    target->bF = 1.2;
    target->eF = 2.3;
    target->bL = 3.4;
    target->eL = 4.5;
    target->mShell->dS = 5.6;
    target->mShell->eS = 6.7;
    target->mShell->phiS = 0.0;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Is_flange_ValidTest()", true, actual);
    target->bF = 0.12;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Is_flange_ValidTest()", false, actual);
    target->bF = 1.2;
    target->bL = 340.0;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Is_flange_ValidTest()", false, actual);
    target->bL = 0.034;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Is_flange_ValidTest()", false, actual);
    target->bL = 3.4;
    target->mShell->phiS = 0.3;
    actual = target->Is_flange_Valid();
    areEqual(PR->getLastOutput(), "Flange_LooseTest::Is_flange_ValidTest()", false, actual);
}


END_NAMESPACE_BILUNA_CALC_EN1591
