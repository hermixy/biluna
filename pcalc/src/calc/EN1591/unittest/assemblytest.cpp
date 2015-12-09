#include "assemblytest.h"
#include "bolt.h"
#include "flange.h"
#include "flange_blind.h"
#include "flange_integral.h"
#include "flange_loose.h"
#include "gasket.h"
#include "loadcase.h"
#include "loadcaselist.h"
#include "shell.h"
#include "washer.h"
NAMESPACE_BILUNA_CALC_EN1591

AssemblyTest::AssemblyTest() : RB_UnitTest() {
    target = NULL;
}

AssemblyTest::~AssemblyTest() {
    // nothing refer to deleteTarget
}

void AssemblyTest::exec() {
    Calc_dGeTest();
    Calc_F_GInitial_1Test();
    Calc_F_GInitialTest();
    Calc_F_QTest();
    Calc_F_RTest();
    Calc_lBTest();
    Calc_YBTest();
    Calc_YGTest();
    Calc_YQTest();
    Calc_YRTest();
    Calc_bGiTest();
    Calc_dUTTest();
    Calc_F_GminTest();
    Calc_F_G0deltaTest();
    Calc_F_G0reqTest();
    Is_F_G0act_larger_F_G0reqTest();
    Is_F_G0act_acceptable_to_F_G0reqTest();
    Calc_F_B0reqTest();
    Calc_F_B0nomTest();
    Is_F_B0nom_ValidTest();
    Calc_F_G0maxTest();
    Calc_F_G0d_2Test();
    Calc_F_G0dTest();
    Calc_F_GTest();
    Calc_F_BTest();
    Calc_cATest();
    Calc_cBTest();
    Calc_PhiBTest();
    Calc_PhiGTest();
    Is_PhiB_ValidTest();
    Calc_fETest();
    Is_PhiG_ValidTest();
    Calc_MtnomTest();
    Calc_MtBnomTest();
    Calc_F_BImaxminTest();
    Calc_F_GImaxminTest();
    Calc_ThetaFmaxminTest();
    Calc_ThetaLmaxminTest();
    Calc_Q_GTest();
    Calc_delta_eGcTest();
    Calc_F_B0avTest();
}

void AssemblyTest::SetupIntegralTarget() {
    if (target) return;
    target = new Assembly();

    Flange* flange1 = new Flange_Integral(1);
    Flange* flange2 = new Flange_Integral(2);
    target->mFlange1 = flange1;
    target->mFlange2 = flange2;

    SetupRemainingTarget();
}

void AssemblyTest::SetupLooseTarget() {
    if (target) return;
    target = new Assembly();

    // Create components
    Flange* flange1 = new Flange_Loose(1);
    Flange* flange2 = new Flange_Loose(2);
    target->mFlange1 = flange1;
    target->mFlange2 = flange2;

    SetupRemainingTarget();
}

void AssemblyTest::SetupBlindTarget() {
    if (target) return;
    target = new Assembly();

    // Create components
    target->mFlange1 = new Flange_Blind(1); // includes shell, washer
    target->mFlange2 = new Flange_Blind(2);

    SetupRemainingTarget();
}

void AssemblyTest::SetupRemainingTarget() {
    target->mLoadCaseList = new LoadCaseList();
    target->mLoadCaseList->createLoadCase(); // includes Force creation
    target->mLoadCaseList->createLoadCase();

    target->mGasket = new Gasket();
    target->mGasket->frmType = Gasket::Flat;

    target->mBolt = new Bolt();  // includes BoltHole creation
    target->mBolt->bType = Bolt::Stud;
    target->mBolt->mBoltHole->isBlindHole = false;

    // Share object
    target->mFlange1->mBolt = target->mBolt;
    target->mFlange2->mBolt = target->mBolt;
    target->mFlange1->mGasket = target->mGasket;
    target->mFlange2->mGasket = target->mGasket;
    target->mGasket->mLoadCaseList = target->mLoadCaseList;
    target->mFlange1->mLoadCaseList = target->mLoadCaseList;
    target->mFlange2->mLoadCaseList = target->mLoadCaseList;
}

void AssemblyTest::deleteTarget() {
    delete target->mBolt;  // deletes BoltHole
    target->mBolt = NULL;
    target->mFlange1->mBolt = NULL;
    target->mFlange2->mBolt = NULL;
    delete target->mLoadCaseList;
    target->mLoadCaseList = NULL;
    target->mGasket->mLoadCaseList = NULL;
    target->mFlange1->mLoadCaseList = NULL;
    target->mFlange2->mLoadCaseList = NULL;
    delete target->mGasket;
    target->mGasket = NULL;
    target->mFlange1->mGasket = NULL;
    target->mFlange2->mGasket = NULL;
    delete target->mFlange1;
    target->mFlange1 = NULL;
    delete target->mFlange2;
    target->mFlange2 = NULL;
    delete target;
    target = NULL;
}

void AssemblyTest::Calc_dGeTest() {
    SetupIntegralTarget();
    target->mGasket->frmType = Gasket::Flat;
    target->mGasket->dG2 = 7.3;
    target->mGasket->bGe = 2.6;
    target->Calc_dGe();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_dGeTest()", 4.7,
             target->mGasket->dGe);

    target->mGasket->frmType = Gasket::CurvedSimpleContact;
    target->mGasket->dG0 = 9.78;
    target->Calc_dGe();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_dGeTest()", 9.78,
             target->mGasket->dGe);

    target->mGasket->frmType = Gasket::CurvedDoubleContact;
    target->mGasket->dGt = 25.8;
    target->Calc_dGe();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_dGeTest()", 25.8,
             target->mGasket->dGe);

    target->mGasket->frmType = Gasket::OctagonalDoubleContact;
    target->Calc_dGe();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_dGeTest()", 25.8,
             target->mGasket->dGe);
    deleteTarget();
}

void AssemblyTest::Calc_F_GInitial_1Test() {
    SetupIntegralTarget();
    int loadCaseNo = 0;
    target->mLoadCaseList->at(loadCaseNo)->F_Bspec = 103.2;
    target->mBolt->eta1minus = 1.3;
    target->mLoadCaseList->at(loadCaseNo)->F_R = 6.7;
    target->Calc_F_GInitial_1();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GInitialTest_1()",
             -37.66, target->mLoadCaseList->at(loadCaseNo)->F_G);
    deleteTarget();
}

void AssemblyTest::Calc_F_GInitialTest() {
    SetupIntegralTarget();
    target->mFlange1->mBolt->AB = 7.3;
    target->mLoadCaseList->at(0)->fB = 103.2;
    target->mLoadCaseList->at(0)->F_R = 12.1;
    target->Calc_F_GInitial();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GInitialTest()",
             239.02, target->mLoadCaseList->at(0)->F_G);
    deleteTarget();
}

void AssemblyTest::Calc_F_QTest() {
    SetupIntegralTarget();
    target->mGasket->AQ = 5.6;
    target->mLoadCaseList->at(0)->P = 9.2;
    target->Calc_F_Q(0);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_QTest()", 51.52,
             target->mLoadCaseList->at(0)->F_Q);
    deleteTarget();
}

void AssemblyTest::Calc_F_RTest() {
    SetupIntegralTarget();
    int i = 0; // loadcase
    target->mLoadCaseList->at(i)->mForce->F_Z = 12.1; // F_Z = F_A
    target->mLoadCaseList->at(i)->mForce->M_AI = 832.7;
    target->mFlange1->d3e = 1643.2;
    target->Calc_F_R(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_RTest()",
             14.127020447906524,
             target->mLoadCaseList->at(i)->F_Rpositive);
    deleteTarget();
}

void AssemblyTest::Calc_lBTest() {
    SetupIntegralTarget();
    target->mGasket->eGt = 3.2;
    target->mFlange1->eFt = 30.9;
    target->mFlange2->eFt = 30.7;
    target->mFlange1->eL = 33.8;
    target->mFlange2->eL = 33.7;
    target->mFlange1->mWasher->eW = 1.2;
    target->mFlange2->mWasher->eW = 1.3;
    target->Calc_lB();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_lBTest()", 134.8,
             target->mBolt->lB);
    deleteTarget();
}

void AssemblyTest::Calc_YBTest() {
    SetupLooseTarget();
    int i = 0;
    target->mFlange1->ZL = 1.1;
    target->mFlange1->hL = 2.1;
    target->mLoadCaseList->at(i)->EL1 = 3.1;
    target->mFlange1->mWasher->XW = 4.1;
    target->mLoadCaseList->at(i)->EW1 = 5.1;

    target->mFlange2->ZL = 1.2;
    target->mFlange2->hL = 2.2;
    target->mLoadCaseList->at(i)->EL2 = 3.2;
    target->mFlange2->mWasher->XW = 4.2;
    target->mLoadCaseList->at(i)->EW2 = 5.2;

    target->mBolt->XB = 6.4;
    target->mLoadCaseList->at(i)->EB = 7.5;

    target->Calc_YB(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_YBTest()", 5.8447859193305112,
             target->mLoadCaseList->at(i)->Y_B);
    deleteTarget();
}

void AssemblyTest::Calc_YGTest() {
    SetupIntegralTarget();
    int i = 0;
    target->mFlange1->ZF = 1.1;
    target->mFlange1->hG = 2.1;
    target->mLoadCaseList->at(i)->EF1 = 3.1;
    target->mFlange2->ZF = 1.2;
    target->mFlange2->hG = 2.2;
    target->mLoadCaseList->at(i)->EF2 = 3.2;
    target->mLoadCaseList->at(i)->Y_B = 0.123;
    target->mGasket->XG = 4.1;
    target->mLoadCaseList->at(i)->E_G = 5.1;
    target->Calc_YG(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_YGTest()",
             4.30676027830487,
             target->mLoadCaseList->at(i)->Y_G);
    i = 1;
    target->mLoadCaseList->at(i)->EF1 = 3.1;
    target->mLoadCaseList->at(i)->EF2 = 3.2;
    target->mLoadCaseList->at(i)->Y_B = 0.123;
    // determination of E_G should use QG0 and Temp. from loadCase
    target->mLoadCaseList->at(i)->E_G = 6.1;
    target->Calc_YG(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_YGTest()",
             4.1749698572184029614,
             target->mLoadCaseList->at(i)->Y_G);
    deleteTarget();
}

void AssemblyTest::Calc_YQTest() {
    SetupIntegralTarget();
    int i = 0;
    target->mFlange1->ZF = 1.1;
    target->mFlange1->hG = 2.1;
    target->mFlange1->hH = 3.1;
    target->mFlange1->hP = 4.1;
    target->mFlange1->hQ = 5.1;
    target->mLoadCaseList->at(i)->EF1 = 6.1;
    target->mFlange2->ZF = 1.2;
    target->mFlange2->hG = 2.2;
    target->mFlange2->hH = 3.2;
    target->mFlange2->hP = 4.2;
    target->mFlange2->hQ = 5.2;
    target->mLoadCaseList->at(i)->EF2 = 6.2;
    target->mLoadCaseList->at(i)->Y_B = 0.923;
    target->Calc_YQ(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_YQTest()", 4.2640100475938656,
             target->mLoadCaseList->at(i)->Y_Q);
    deleteTarget();
}

void AssemblyTest::Calc_YRTest() {
    SetupIntegralTarget();
    int i = 0;
    target->mFlange1->ZF = 1.1;
    target->mFlange1->hG = 2.1;
    target->mFlange1->hH = 3.1;
    target->mFlange1->hR = 4.1;
    target->mLoadCaseList->at(i)->EF1 = 5.1;
    target->mFlange2->ZF = 1.3;
    target->mFlange2->hG = 2.3;
    target->mFlange2->hH = 3.3;
    target->mFlange2->hR = 4.3;
    target->mLoadCaseList->at(i)->EF2 = 5.3;
    target->mLoadCaseList->at(i)->Y_B = 0.525;
    target->Calc_YR(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_YRTest()", 8.0737236403995567,
             target->mLoadCaseList->at(i)->Y_R);
    deleteTarget();
}

void AssemblyTest::Calc_bGiTest() {
    SetupIntegralTarget();
    int i = 0;
    bool isFirstApproximation = true;

    target->mGasket->frmType = Gasket::Flat;
    target->mGasket->bGt = 2.3;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 2.3, target->mGasket->bGi);

    //target.LoadCaseList(0).Qsmax = 100.3
    //target.LoadCaseList(0).E0 = 7.1
    //target.LoadCaseList(0).K1 = 1.9
    //Assert.AreEqual(197.67, target.LoadCaseList(i).E_G, ACCURACY)
    target->mGasket->frmType = Gasket::CurvedSimpleContact;
    target->mGasket->r2 = 155.3;
    target->mGasket->phiG = 0.1;
    target->mGasket->bGt = 0.3;
    target->mLoadCaseList->at(i)->Q_smax = 4561.23;
    target->mLoadCaseList->at(i)->E_G = 6000.4;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 14.5407000127705715317,
             target->mGasket->bGi);

    target->mGasket->frmType = Gasket::CurvedDoubleContact;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 20.5636551644587786355,
             target->mGasket->bGi);

    target->mGasket->frmType = Gasket::OctagonalDoubleContact;
    target->mGasket->bGiOct = 3.2;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 3.2, target->mGasket->bGi);

    isFirstApproximation = false;

    //target.LoadCaseList(0).E0 = 6.4
    //target.LoadCaseList(0).K1 = 3.1
    //target.LoadCaseList(i).Qsmax = 1.5
    target->mGasket->frmType = Gasket::Flat;
    target->mGasket->insType = Gasket::SteelSoft; // Metalic
    target->mLoadCaseList->at(i)->F_G = 7.3;
    target->mGasket->dGe = 2.3;
    target->mLoadCaseList->at(i)->Q_smax = 71.3;
    target->mLoadCaseList->at(i)->eG = 9.1;
    target->mLoadCaseList->at(i)->E_G = 1.1;
    target->mFlange1->hG = 3.3;
    target->mFlange1->ZF = 6.3;
    target->mLoadCaseList->at(i)->EF1 = 8.7;
    target->mFlange2->hG = 4.3;
    target->mFlange2->ZF = 7.3;
    target->mLoadCaseList->at(i)->EF2 = 9.7;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 0.45134629619661641586,
             target->mGasket->bGi);

    target->mGasket->insType = Gasket::SpiralGraphFillOuterInner; // Non metalic
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 0.638143,
             target->mGasket->bGi);

    target->mGasket->frmType = Gasket::CurvedSimpleContact;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 29.18099436801514326863,
             target->mGasket->bGi);

    target->mGasket->frmType = Gasket::CurvedDoubleContact;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 41.26815556620527463859,
             target->mGasket->bGi);

    target->mGasket->frmType = Gasket::OctagonalDoubleContact;
    target->mGasket->bGiOct = 7.2;
    target->Calc_bGi(isFirstApproximation);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_bGiTest()", 7.2, target->mGasket->bGi);
    deleteTarget();
}

void AssemblyTest::Calc_dUTTest() {
    SetupIntegralTarget();
    int i = 0;

    target->mLoadCaseList->at(i)->alphaB = 1.1;
    target->mLoadCaseList->at(i)->alphaG = 2.2;
    target->mLoadCaseList->at(i)->alphaF1 = 3.3;
    target->mLoadCaseList->at(i)->alphaF2 = 4.4;
    target->mLoadCaseList->at(i)->alphaL1 = 5.5;
    target->mLoadCaseList->at(i)->alphaL2 = 6.6;
    target->mLoadCaseList->at(i)->alphaW1 = 7.7;
    target->mLoadCaseList->at(i)->alphaW2 = 8.8;

    target->mLoadCaseList->at(i)->T0 = 20.0;
    target->mLoadCaseList->at(i)->TB = 20.1;
    target->mLoadCaseList->at(i)->TG = 20.2;
    target->mLoadCaseList->at(i)->TF1 = 20.3;
    target->mLoadCaseList->at(i)->TF2 = 20.4;
    target->mLoadCaseList->at(i)->TL1 = 20.5;
    target->mLoadCaseList->at(i)->TL2 = 20.6;
    target->mLoadCaseList->at(i)->TW1 = 20.7;
    target->mLoadCaseList->at(i)->TW2 = 20.8;

    target->mLoadCaseList->at(i)->eG = 3.2;

    target->mBolt->lB = 2014.3;
    target->mFlange1->eFt = 30.9;
    target->mFlange2->eFt = 30.7;
    target->mFlange1->eL = 33.8;
    target->mFlange2->eL = 33.7;
    target->mFlange1->mWasher->eW = 3.6;
    target->mFlange2->mWasher->eW = 3.5;
    target->Calc_dUI(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_dUTTest()", -134.904,
             target->mLoadCaseList->at(i)->dUI);
    deleteTarget();
}

void AssemblyTest::Calc_F_GminTest() {
    SetupIntegralTarget();
    int i = 0;
    target->mGasket->AGe = 7.6;
    target->mLoadCaseList->at(i)->Q_A = 3.7;
    target->Calc_F_Gmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GminTest()", 28.12,
             target->mLoadCaseList->at(i)->F_Gmin);
    i = 1;
    target->mGasket->frmType = Gasket::Flat;
    target->mGasket->AGe = 12.9;
    target->mLoadCaseList->at(i)->Q_sminL = 0.458;
    // tmpVal1 = 5.9082
    target->mLoadCaseList->at(i)->mForce->M_AI = 7.8;
    target->mGasket->dGt = 20.1;
    target->mLoadCaseList->at(i)->muG = 0.1;
    target->mLoadCaseList->at(i)->mForce->F_LI = 4.1;
    target->mLoadCaseList->at(i)->mForce->M_AI = 13.2;
    target->mLoadCaseList->at(i)->mForce->M_Z = 3.9;
    target->mLoadCaseList->at(i)->F_Q = 0.0;
    target->mLoadCaseList->at(i)->F_R = 0.0;
    // tmpVal2 = 43.567164179104481
    target->Calc_F_Gmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GminTest()", 43.567164179104481,
             target->mLoadCaseList->at(i)->F_Gmin);
    target->mLoadCaseList->at(i)->F_Q = -17.3;
    target->mLoadCaseList->at(i)->F_R = -89.23;
    // tmpVal3 = 106.53
    target->Calc_F_Gmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GminTest()", 106.53,
             target->mLoadCaseList->at(i)->F_Gmin);
    deleteTarget();
}

void AssemblyTest::Calc_F_G0deltaTest() {
    SetupIntegralTarget();

    int i = 1;
    target->mLoadCaseList->at(i)->F_Gmin = 1.3;
    target->mLoadCaseList->at(i)->Y_G = 6.2;
    target->mLoadCaseList->at(0)->Y_G = 1.4;
    target->mLoadCaseList->at(i)->F_Q = 3.9;
    target->mLoadCaseList->at(i)->Y_Q = 7.3;
    target->mLoadCaseList->at(i)->F_R = 4.9;
    target->mLoadCaseList->at(i)->Y_R = 8.3;
    target->mLoadCaseList->at(0)->F_R = 5.9;
    target->mLoadCaseList->at(0)->Y_R = 9.3;
    target->mLoadCaseList->at(i)->dUI = 2.8;
    target->mLoadCaseList->at(i)->delta_eGc = 3.8;
    target->Calc_F_Gdelta(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_G0deltaTest()", 20.664285714285715,
             target->mLoadCaseList->at(0)->F_Gdelta);
    target->mLoadCaseList->at(i)->F_Gmin = -52.3;
    target->Calc_F_Gdelta(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_G0deltaTest()", 0.0,
             target->mLoadCaseList->at(0)->F_Gdelta);
    deleteTarget();
}

void AssemblyTest::Calc_F_G0reqTest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->F_Gmin = 3.4;
    target->mLoadCaseList->at(0)->F_Gdelta = 8.9;
    target->Calc_F_G0req();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_G0reqTest()", 8.9,
             target->mLoadCaseList->at(0)->F_Greq);
    deleteTarget();
}

void AssemblyTest::Is_F_G0act_larger_F_G0reqTest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->F_G = 1.2;
    target->mLoadCaseList->at(0)->F_Greq = 2.3;
    bool actual = target->Is_F_G0_larger_F_G0req();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_G0act_larger_F_G0reqTest()",
             false, actual);
    target->mLoadCaseList->at(0)->F_G = 1.5;
    target->mLoadCaseList->at(0)->F_Greq = -0.7;
    actual = target->Is_F_G0_larger_F_G0req();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_G0act_larger_F_G0reqTest()",
             true, actual);
    deleteTarget();
}

void AssemblyTest::Is_F_G0act_acceptable_to_F_G0reqTest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->F_G = 1.2;
    target->mLoadCaseList->at(0)->F_Greq = 2.3;
    bool actual = target->Is_F_G0act_within_0_1_percent_of_F_G0req();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_G0act_acceptable_to_F_G0reqTest()",
             false, actual);
    target->mLoadCaseList->at(0)->F_G = 1.5;
    target->mLoadCaseList->at(0)->F_Greq = -0.7;
    actual = target->Is_F_G0act_within_0_1_percent_of_F_G0req();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_G0act_acceptable_to_F_G0reqTest()",
             false, actual);
    target->mLoadCaseList->at(0)->F_G = 1.500001;
    target->mLoadCaseList->at(0)->F_Greq = 1.5;
    actual = target->Is_F_G0act_within_0_1_percent_of_F_G0req();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_G0act_acceptable_to_F_G0reqTest()",
             true, actual);
    deleteTarget();
}

void AssemblyTest::Calc_F_B0reqTest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->F_Greq = 5.6;
    target->mLoadCaseList->at(0)->F_R = 4.05;
    target->Calc_F_B0req();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0reqTest()", 9.65,
             target->mLoadCaseList->at(0)->F_Breq);
    deleteTarget();
}

void AssemblyTest::Calc_F_B0nomTest() {
    SetupIntegralTarget();
    LoadCase* loadCase = target->mLoadCaseList->at(0);
//    loadCase->F_Bspec = 12.3;
    target->mBolt->etanminus = 7.1;
    target->mBolt->etanplus = -5.4;
//    target->Calc_F_B0nom();
//    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()", 12.3,
//             loadCase->F_Bnom);
//    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()", -75.03,
//             loadCase->F_Bmin);
//    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()", -54.12,
//             loadCase->F_Bmax);
    target->mBolt->tType = Bolt::TorqueWrench;
    loadCase->F_Bspec = 0;
    loadCase->F_Breq = 6.2;
    target->Calc_F_B0nom();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()",
             -1.0163934426229508, loadCase->F_Bnom);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()",
             6.2, loadCase->F_Bmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()",
             4.472131147540984, loadCase->F_Bmax);
    loadCase->F_Bav = 0.73;
    target->mBolt->tType = Bolt::ManualStandardRing;
    target->mFlange1->nB = 32;
    target->Calc_F_B0nom();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()",
             0.73, loadCase->F_Bnom);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()",
             0.5903573796625456538, loadCase->F_Bmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0nomTest()",
             -3.212, loadCase->F_Bmax);
    deleteTarget();
}

void AssemblyTest::Is_F_B0nom_ValidTest() {
    SetupIntegralTarget();
    LoadCase* loadCase = target->mLoadCaseList->at(0);
    target->mBolt->tType = Bolt::TorqueWrench;
    loadCase->F_Breq = 23.1;
    loadCase->F_Bmin = 23.3;
    loadCase->F_Bnom = 23.5;
    loadCase->F_Bmax = 30.1;
    target->mBolt->etanminus = 0.2;
    target->mBolt->tType = Bolt::TorqueWrench;
    bool actual = false;
    actual = target->Is_F_B0nom_Valid();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_B0nom_ValidTest()",
             false, actual);
    target->mBolt->etanminus = 0.00000001;
    actual = target->Is_F_B0nom_Valid();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_B0nom_ValidTest()",
             true, actual);
    target->mBolt->tType = Bolt::ManualStandardRing;
    loadCase->F_Bspec = 0.0;
    target->mFlange1->nB = 3;
    actual = target->Is_F_B0nom_Valid();
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_F_B0nom_ValidTest()",
             false, actual);
    deleteTarget();
}

void AssemblyTest::Calc_F_G0maxTest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->F_Bmax = 4.8;
    target->mLoadCaseList->at(0)->F_R = 9.7;
    target->Calc_F_G0max();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_G0maxTest()", -4.9,
             target->mLoadCaseList->at(0)->F_Gmax);
    deleteTarget();
}

void AssemblyTest::Calc_F_G0d_2Test() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->F_Bspec = 7.6;
    target->mLoadCaseList->at(0)->F_Bmin = 0.7;
    target->mLoadCaseList->at(0)->F_Bmax = 700;
    target->mNR = 300;
    target->mLoadCaseList->at(0)->F_R = -2.3;
    target->Calc_F_G0d_2();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_G0d_2Test()", 453.4111111111111,
             target->mLoadCaseList->at(0)->F_Gd);
    deleteTarget();
}

void AssemblyTest::Calc_F_G0dTest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->F_Bspec = 0.0;
    target->mLoadCaseList->at(0)->F_Gdelta = 5.6;
    target->mLoadCaseList->at(0)->F_Bmax = 0.7;
    target->mNR = 3;
    target->mLoadCaseList->at(0)->F_R = 2.3;
    target->Calc_F_G0d();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_G0dTest()", 5.6,
             target->mLoadCaseList->at(0)->F_Gd);
    deleteTarget();
}

void AssemblyTest::Calc_F_GTest() {
    SetupIntegralTarget();
    int i = 1;
    target->mLoadCaseList->at(0)->F_Gd = 1.1;
    target->mLoadCaseList->at(0)->Y_G = 1.2;
    target->mLoadCaseList->at(i)->F_Q = 2.1;
    target->mLoadCaseList->at(i)->Y_Q = 2.2;
    target->mLoadCaseList->at(i)->F_R = 3.1;
    target->mLoadCaseList->at(i)->Y_R = 3.2;
    target->mLoadCaseList->at(0)->F_R = 4.1;
    target->mLoadCaseList->at(0)->Y_R = 4.2;
    target->mLoadCaseList->at(i)->dUI = 5.1;
    target->mLoadCaseList->at(i)->Y_G = 5.2;
    target->mLoadCaseList->at(i)->delta_eGc = 0.0;
    target->Calc_F_G(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GTest()", -0.21153846153846154,
             target->mLoadCaseList->at(i)->F_G);
    deleteTarget();
}

void AssemblyTest::Calc_F_BTest() {
    SetupIntegralTarget();
    int i = 1;
    target->mLoadCaseList->at(i)->F_G = 3.5;
    target->mLoadCaseList->at(i)->F_Q = 3.9;
    target->mLoadCaseList->at(i)->F_R = 9.4;
    target->Calc_F_B(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_BTest()", 16.8,
             target->mLoadCaseList->at(i)->F_B);
    deleteTarget();
}

void AssemblyTest::Calc_cATest() {
    SetupIntegralTarget();
    target->mBolt->tType = Bolt::TorqueWrench;
    target->mBolt->ruptureElongationA = 20.0; // percent
    int i = 0;
    target->Calc_cA(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_cATest()", 1,
             target->mLoadCaseList->at(i)->cA);
    target->mBolt->ruptureElongationA = 2.0;
    target->Calc_cA(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_cATest()", 4 / 3.0,
             target->mLoadCaseList->at(i)->cA);
    target->mBolt->tType = Bolt::ManualOperatorFeel;
    target->mBolt->ruptureElongationA = 20.0;
    i = 1;
    target->Calc_cA(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_cATest()", 1.0,
             target->mLoadCaseList->at(i)->cA);
    target->mBolt->ruptureElongationA = 2.0;
    target->Calc_cA(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_cATest()", 4 / 3.0,
             target->mLoadCaseList->at(i)->cA);
    target->mBolt->tType = Bolt::TensionerMeasureBoltElongation;
    i = 0;
    target->Calc_cA(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_cATest()", 0.0,
             target->mLoadCaseList->at(i)->cA);
    target->mBolt->tType = Bolt::TensionerMeasureHydraulicPressure;
    i = 0;
    target->Calc_cA(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_cATest()", 0.0,
             target->mLoadCaseList->at(i)->cA);
    deleteTarget();
}

void AssemblyTest::Calc_cBTest() {
    SetupBlindTarget();
    int i = 0;
    target->mBolt->eN = 15.3;
    target->mLoadCaseList->at(i)->fN = 200000;
    target->mBolt->dB0 = 700.3;
    target->mLoadCaseList->at(i)->fB = 203000;
    target->mBolt->l5t = 13.5;
    target->mLoadCaseList->at(i)->fF1 = 207000;
    target->mLoadCaseList->at(i)->fF2 = 207000; // takes minimum of both flanges
    target->Calc_cB(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_cBTest()", 0.024571629751921942,
             target->mLoadCaseList->at(i)->cB);
    deleteTarget();
}

void AssemblyTest::Calc_PhiBTest() {
    SetupIntegralTarget();
    int i = 0;
    target->mLoadCaseList->at(i)->F_B = 12.3;
    target->mBolt->AB = 2.7;
    target->mLoadCaseList->at(i)->fB = 12.3;
    target->mLoadCaseList->at(i)->Mtnom = 8.36;
    target->mBolt->IB = 13.78;
    target->mLoadCaseList->at(i)->cA = 5.8;
    target->mLoadCaseList->at(i)->cB = 1.0;
    target->mBolt->mut = 7.8;
    target->mBolt->etanplus = 0.7;
    target->Calc_PhiB(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_Calc_PhiBTest()",
             0.92017261349202768449, target->mLoadCaseList->at(i)->PhiB);
    i = 1;
    target->mLoadCaseList->at(i)->F_B = 12.3;
    target->mBolt->AB = 2.7;
    target->mLoadCaseList->at(i)->fB = 12.3;
    target->mLoadCaseList->at(i)->cB = 1.0;
    target->Calc_PhiB(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_Calc_PhiBTest()",
             0.37037037037037037037, target->mLoadCaseList->at(i)->PhiB);
    deleteTarget();
}

void AssemblyTest::Calc_PhiGTest() {
    SetupIntegralTarget();
    int i = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(i);
    loadCase->F_G = 1.1;
    target->mGasket->AGt = 2.2;
    loadCase->Q_smax = 3.3;
    target->Calc_PhiG(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_Calc_PhiGTest()", 0.15151515151515152,
             target->mLoadCaseList->at(i)->PhiG);
    deleteTarget();
}

void AssemblyTest::Is_PhiB_ValidTest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->PhiB = 1.1;
    bool actual = false;
    actual = target->Is_PhiB_Valid(0);
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_PhiB_ValidTest()",
             false, actual);
    target->mLoadCaseList->at(0)->PhiB = 0.8;
    actual = target->Is_PhiB_Valid(0);
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_PhiB_ValidTest()",
             true, actual);
    deleteTarget();
}

void AssemblyTest::Calc_fETest() {
    SetupIntegralTarget();
    target->mLoadCaseList->at(0)->fF1 = 3.2;
    target->mLoadCaseList->at(0)->fS1 = 15.3;
    target->Calc_fE(target->mFlange1, 0);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_fETest()", 3.2,
             target->mLoadCaseList->at(0)->fE1);
    deleteTarget();
}

void AssemblyTest::Is_PhiG_ValidTest() {
    SetupIntegralTarget();
    int i = 0;
    bool expected = true;
    bool actual = false;
    target->mLoadCaseList->at(i)->PhiG = 0.9;
    actual = target->Is_PhiG_Valid(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_PhiG_ValidTest()", expected, actual);
    expected = false;
    target->mLoadCaseList->at(i)->PhiG = 1.2;
    actual = target->Is_PhiG_Valid(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Is_PhiG_ValidTest()", expected, actual);
    deleteTarget();
}

void AssemblyTest::Calc_MtnomTest() {
    SetupIntegralTarget();
    LoadCase* loadCase = target->mLoadCaseList->at(0);
    target->mBolt->kB = 24.3;
    loadCase->F_Bnom = 17.9;
    target->mFlange1->nB = 17;
    target->Calc_Mtnom();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_Mtnom()",
             25.58647058823529411765, loadCase->Mtnom);
    deleteTarget();
}

void AssemblyTest::Calc_MtBnomTest() {
    SetupIntegralTarget();
    LoadCase* loadCase = target->mLoadCaseList->at(0);
    target->mBolt->kB9 = 4.3;
    loadCase->F_Bnom = 7.9;
    target->mFlange1->nB = 18;
    target->Calc_MtBnom();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_MtBnom()",
             1.88722222222222222222, loadCase->MtBnom);
    deleteTarget();
}

void AssemblyTest::Calc_F_BImaxminTest() {
    SetupIntegralTarget();
    int i = 0;
    target->mLoadCaseList->at(0)->F_Bnom = 1.1;
    target->mBolt->etanminus = 1.2;
    target->mBolt->etanplus = 1.3;
    target->Calc_F_BImaxmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_BImaxminTest()", -0.22,
             target->mLoadCaseList->at(i)->F_Bmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_BImaxminTest()", 2.53,
             target->mLoadCaseList->at(i)->F_Bmax);
    i = 1;
    target->mLoadCaseList->at(i)->F_Gmin = 2.1;
    target->mLoadCaseList->at(i)->F_Q = 2.2;
    target->mLoadCaseList->at(i)->F_R = 2.3;
    target->mLoadCaseList->at(i)->F_Gmax = 3.1;
    target->Calc_F_BImaxmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_BImaxminTest()", 6.6,
             target->mLoadCaseList->at(i)->F_Bmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_BImaxminTest()", 7.6,
             target->mLoadCaseList->at(i)->F_Bmax);
    deleteTarget();
}

void AssemblyTest::Calc_F_GImaxminTest() {
    SetupIntegralTarget();
    int i = 0;
    target->mLoadCaseList->at(i)->F_Bmin = 1.1;
    target->mLoadCaseList->at(i)->F_Bmax = 1.2;
    target->mLoadCaseList->at(i)->F_R = 1.3;
    target->mGasket->AGe = 15.3;
    target->Calc_F_GImaxmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()", -0.2,
             target->mLoadCaseList->at(i)->F_Gmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()", -0.1,
             target->mLoadCaseList->at(i)->F_Gmax);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()",
             -0.01307189542483660131, target->mLoadCaseList->at(i)->Q_Gmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()",
             -0.00653594771241830065, target->mLoadCaseList->at(i)->Q_Gmax);
    i = 1;
    target->mLoadCaseList->at(0)->F_Gmin = 2.1;
    target->mLoadCaseList->at(0)->Y_G = 3.1;
    target->mLoadCaseList->at(0)->F_Gmax = 2.2;
    target->mLoadCaseList->at(i)->F_Q = 2.2;
    target->mLoadCaseList->at(i)->Y_Q = 2.3;
    target->mLoadCaseList->at(i)->F_R = 2.4;
    target->mLoadCaseList->at(i)->Y_R = 2.5;
    target->mLoadCaseList->at(0)->F_R = 2.6;
    target->mLoadCaseList->at(0)->Y_R = 2.7;
    target->mLoadCaseList->at(i)->dUI = 2.8;
    target->mLoadCaseList->at(i)->Y_G = 2.9;
    target->mLoadCaseList->at(i)->delta_eGc = 0.0;
    target->mGasket->AGe = 3.5;
    target->Calc_F_GImaxmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()",
             -0.11379310344827587, target->mLoadCaseList->at(i)->F_Gmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()",
             -0.0068965517241379309, target->mLoadCaseList->at(i)->F_Gmax);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()",
             -0.03251231527093596286, target->mLoadCaseList->at(i)->Q_Gmin);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_GImaxminTest()",
             -0.00197044334975369454, target->mLoadCaseList->at(i)->Q_Gmax);
    deleteTarget();
}

void AssemblyTest::Calc_ThetaFmaxminTest() {
    SetupIntegralTarget();
    int i = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(i);
    loadCase->F_Q = 1.1;
    target->mFlange1->hH = 1.2;
    target->mFlange2->hH = 2.2;
    target->mFlange1->hP = 1.3;
    target->mFlange2->hP = 2.3;
    target->mFlange1->hQ = 1.4;
    target->mFlange2->hQ = 2.4;
    loadCase->F_R = 1.5;
    target->mFlange1->hR = 1.7;
    target->mFlange2->hR = 2.7;
    target->mFlange1->ZF = 3.1;
    target->mFlange2->ZF = 4.1;
    loadCase->EF1 = 3.2;
    loadCase->EF2 = 4.2;
    loadCase->F_Gmin = 3.3;
    loadCase->F_Gmax = 4.3;
    target->mFlange1->hG = 3.4;
    target->mFlange2->hG = 4.4;
    target->Calc_ThetaFmaxmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaFmaxminTest()", 16.46875,
             loadCase->ThetaF1min);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaFmaxminTest()", 19.7625,
             loadCase->ThetaF1max);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaFmaxminTest()", 23.81904761904762,
             loadCase->ThetaF2min);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaFmaxminTest()", 28.114285714285714,
             loadCase->ThetaF2max);
    deleteTarget();
}

void AssemblyTest::Calc_ThetaLmaxminTest() {
    SetupLooseTarget();
    int i = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(i);
    target->mFlange1->ZL = 1.1;
    loadCase->EL1 = 1.2;
    loadCase->F_Bmin = 1.3;
    target->mFlange1->hL = 1.4;
    target->mFlange2->ZL = 2.1;
    loadCase->EL2 = 2.2;
    loadCase->F_Bmax = 2.3;
    target->mFlange2->hL = 2.4;
    target->Calc_ThetaLmaxmin(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaLmaxminTest()", 1.6683333333333332,
             loadCase->ThetaL1min);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaLmaxminTest()", 2.9516666666666667,
             loadCase->ThetaL1max);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaLmaxminTest()", 2.978181818181818,
             loadCase->ThetaL2min);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_ThetaLmaxminTest()", 5.2690909090909095,
             loadCase->ThetaL2max);
    deleteTarget();
}

void AssemblyTest::Calc_Q_GTest() {
    SetupIntegralTarget();
    int i = 0;
    LoadCase* loadCase0 = target->mLoadCaseList->at(i);
    loadCase0->F_G = 1.1;
    target->mGasket->AGe = 2.2;
    target->Calc_Q_G(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_Q_GTest()", 0.5,
             loadCase0->Q_G);
    deleteTarget();
}

void AssemblyTest::Calc_delta_eGcTest() {
    SetupIntegralTarget();
    LoadCase* loadCase0 = target->mLoadCaseList->at(0);
    int i = 1;
    LoadCase* loadCase = target->mLoadCaseList->at(i);
    loadCase->delta_eGc_EN13555 = 0.0;
    loadCase->Y_G = 1.3;
    target->mGasket->K = 500000.0;
    target->mGasket->dG2_EN13555 = 2.4;
    target->mGasket->dG1_EN13555 = 9.1;
    loadCase0->Q_A = 15.3;
    loadCase->P_QR = 0.0456;
    target->Calc_delta_eGc(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_delta_eGcTest()",
             -1148.7558557602242, loadCase->delta_eGc);
    target->mGasket->dG2_EN13555 = 0.0;
    target->mGasket->dG1_EN13555 = 0.0;
    loadCase->P_QR = 0.0;
    target->Calc_delta_eGc(i);
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_delta_eGcTest()", 0.0,
             loadCase->delta_eGc);
    deleteTarget();
}

void AssemblyTest::Calc_F_B0avTest() {
    SetupIntegralTarget();
    target->mBolt->tType = Bolt_IN::ManualStandardRing;
    int i = 0;
    LoadCase* loadCase = target->mLoadCaseList->at(i);
    target->mBolt->AB = 3.2;
    loadCase->fB = 123.45;
    target->mFlange1->nB = 3;
    target->Calc_F_B0av();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0avTest()", 395.04, loadCase->F_Bav);
    target->mBolt->AB = 320000;
    target->Calc_F_B0av();
    areEqual(PR->getLastOutput(), "AssemblyTest::Calc_F_B0avTest()", 600000, loadCase->F_Bav);
    deleteTarget();
}

END_NAMESPACE_BILUNA_CALC_EN1591
