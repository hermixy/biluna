#include "calculator.h"
NAMESPACE_BILUNA_CALC_EN1591


Calculator::Calculator(FlangeType flange1Type, FlangeType flange2Type)
            : RB_Object() {
    setName("PCALC EN1591 Calculator");

    mAssembly = new Assembly();
    mIterNo = 0;
    mIsFirstApproximation = false;
    mLoadCaseCount = 0;

    createAssembly(flange1Type, flange2Type);
}

Calculator::~Calculator() {
    deleteAssembly();
}

void Calculator::createAssembly(FlangeType flange1Type,
                                FlangeType flange2Type) {
    Flange* flange1;
    Flange* flange2;

    // Flange creation also creates washer and shell
    switch (flange1Type) {
    case Blind:
        flange1 = new Flange_Blind(1);
        break;
    case Integral:
        flange1 = new Flange_Integral(1);
        break;
    case Loose:
        flange1 = new Flange_Loose(1);
        break;
    default:
        break;
    }

    switch (flange2Type) {
    case Blind:
        flange2 = new Flange_Blind(2);
        break;
    case Integral:
        flange2 = new Flange_Integral(2);
        break;
    case Loose:
        flange2 = new Flange_Loose(2);
        break;
    default:
        break;
    }

    mAssembly->mFlange1 = flange1;
    mAssembly->mFlange2 = flange2;
    createAssemblyHelper(mAssembly);
}

void Calculator::createAssemblyHelper(Assembly* assembly) {
    // LoadCaseList
    assembly->mLoadCaseList = new LoadCaseList();
    //    assembly->mLoadCaseList->createLoadCase(); // includes Force creation

    assembly->mGasket = new Gasket();
    assembly->mGasket->frmType = Gasket::Flat;

    assembly->mBolt = new Bolt();  // includes BoltHole creation
    assembly->mBolt->bType = Bolt::Stud;
    assembly->mBolt->mBoltHole->isBlindHole = false;

    // Share object
    assembly->mFlange1->mBolt = assembly->mBolt;
    assembly->mFlange2->mBolt = assembly->mBolt;
    assembly->mFlange1->mGasket = assembly->mGasket;
    assembly->mFlange2->mGasket = assembly->mGasket;
    assembly->mGasket->mLoadCaseList = assembly->mLoadCaseList;
    assembly->mFlange1->mLoadCaseList = assembly->mLoadCaseList;
    assembly->mFlange2->mLoadCaseList = assembly->mLoadCaseList;
}

void Calculator::deleteAssembly() {
    delete mAssembly->mBolt;
    mAssembly->mBolt = NULL;
    mAssembly->mFlange1->mBolt = NULL;
    mAssembly->mFlange2->mBolt = NULL;
    delete mAssembly->mLoadCaseList;
    mAssembly->mLoadCaseList = NULL;
    mAssembly->mGasket->mLoadCaseList = NULL;
    mAssembly->mFlange1->mLoadCaseList = NULL;
    mAssembly->mFlange2->mLoadCaseList = NULL;
    delete mAssembly->mGasket;
    mAssembly->mGasket = NULL;
    mAssembly->mFlange1->mGasket = NULL;
    mAssembly->mFlange2->mGasket = NULL;
    delete mAssembly->mFlange1;
    mAssembly->mFlange1 = NULL;
    delete mAssembly->mFlange2;
    mAssembly->mFlange2 = NULL;
    delete mAssembly;
    mAssembly = NULL;
}

void Calculator::exec() {

    Assembly* assembly = this->mAssembly;

    mIterNo = 0;
    mLoadCaseCount = assembly->mLoadCaseList->size();

    F3_to_24(assembly);
    F25_to_40(assembly);
    F41_to_53(assembly);
    F54_to_54(assembly); // is first approximation

    mIsFirstApproximation = true;
    bool isFG0largerFG0req = false;
    bool isFG0approximateFG0req = false;
    int counter = 0;

    while (!(isFG0largerFG0req && isFG0approximateFG0req)
           && counter < 10
           && (counter < 1 || assembly->mF_Bspec <= 0.0)) {
        int loadCaseNo = 0;

        // Outside loop
        Loop_F55_to_108(assembly);

        // Formulae 109 and 110
        isFG0largerFG0req = assembly->Is_F_G0_larger_F_G0req();
        isFG0approximateFG0req = assembly->Is_F_G0act_within_0_1_percent_of_F_G0req();

        if (!(isFG0largerFG0req && isFG0approximateFG0req)
                && (assembly->mF_Bspec <= 0.0)) {
            LoadCase* loadCase0 = assembly->mLoadCaseList->at(loadCaseNo);
            loadCase0->F_G = loadCase0->F_Greq;
            PR->addDetail("After_F. 108", "F_G",
                          "F_Greq (new initial force)",
                          loadCase0->F_G, "N", QN(loadCase0->F_Greq),
                          loadCaseNo);

            // Also reset initial bolt load
            loadCase0->F_B = loadCase0->F_G + loadCase0->F_R;
            PR->addDetail("After_F. 108", "F_B",
                          "F_B (new initial force)",
                          loadCase0->F_B, "N", QN(loadCase0->F_G)
                          + " + " + QN(loadCase0->F_R),
                          loadCaseNo);
        }

        mIsFirstApproximation = true;
        ++counter;
    }

    F111_to_118(assembly);
    F119_to_119(assembly);

    for (int loadCaseNo = 1; loadCaseNo < mLoadCaseCount; loadCaseNo++) {
        F120_to_122(assembly, loadCaseNo);
    }

    for (int loadCaseNo = 0; loadCaseNo < mLoadCaseCount; loadCaseNo++) {
        F123_to_151(assembly, loadCaseNo);
        FC1_to_C10(assembly, loadCaseNo);
    }
}

void Calculator::Loop_F55_to_108(Assembly* assembly) {
    for (this->mIterNo = 0; this->mIterNo <= 5; this->mIterNo++) {
        F55_to_62_table1(assembly);
        mIsFirstApproximation = false;
    }

    int loadCaseNo = 0;
    F63_to_63(assembly, loadCaseNo);
    F77_to_89(assembly, loadCaseNo);

    for (loadCaseNo = 0; loadCaseNo < mLoadCaseCount; loadCaseNo++) {
        F90_to_102(assembly, loadCaseNo);
    }

    for (loadCaseNo = 0; loadCaseNo < mLoadCaseCount; loadCaseNo++) {
        F103_to_104(assembly, loadCaseNo);
    }

    for (loadCaseNo = 1; loadCaseNo < mLoadCaseCount; loadCaseNo++) {
        F105_to_105(assembly, loadCaseNo);
    }

    F106_to_108(assembly);
}

void Calculator::F3_to_24(Assembly* assembly) {
    assembly->mFlange1->Calc_pB();
    assembly->mFlange2->Calc_pB();
    assembly->mFlange1->Calc_d5e();
    assembly->mFlange2->Calc_d5e();
    assembly->mFlange1->Calc_d3e();
    assembly->mFlange2->Calc_d3e();

    assembly->mFlange1->Calc_bF();
    assembly->mFlange2->Calc_bF();
    assembly->mFlange1->Calc_dF();
    assembly->mFlange2->Calc_dF();
    assembly->mFlange1->Calc_bL();
    assembly->mFlange2->Calc_bL();
    assembly->mFlange1->Calc_dL();
    assembly->mFlange2->Calc_dL();

    assembly->mFlange1->Calc_AF();
    assembly->mFlange2->Calc_AF();
    assembly->mFlange1->Calc_AL();
    assembly->mFlange2->Calc_AL();

    assembly->mFlange1->Calc_eFt();
    assembly->mFlange2->Calc_eFt();
    assembly->mFlange1->Calc_eF();
    assembly->mFlange2->Calc_eF();
    assembly->mFlange1->Calc_eL();
    assembly->mFlange2->Calc_eL();
    assembly->mFlange1->Calc_eP();
    assembly->mFlange2->Calc_eP();

    bool res = assembly->mFlange1->Is_flange_Valid();
    res = assembly->mFlange2->Is_flange_Valid(); // moved to before beta calculation

    assembly->mFlange1->Calc_beta();
    assembly->mFlange2->Calc_beta();
    assembly->mFlange1->Calc_eE();
    assembly->mFlange2->Calc_eE();
    assembly->mFlange1->Calc_eD();
    assembly->mFlange2->Calc_eD();
    assembly->mFlange1->Calc_dE();
    assembly->mFlange2->Calc_dE();
}

void Calculator::F25_to_40(Assembly* assembly) {
    assembly->mFlange1->Calc_gamma();
    assembly->mFlange2->Calc_gamma();
    assembly->mFlange1->Calc_theta();
    assembly->mFlange2->Calc_theta();
    assembly->mFlange1->Calc_lambda();
    assembly->mFlange2->Calc_lambda();
    assembly->mFlange1->Calc_cF();
    assembly->mFlange2->Calc_cF();
    assembly->mFlange1->Calc_hS();
    assembly->mFlange2->Calc_hS();
    assembly->mFlange1->Calc_hT();
    assembly->mFlange2->Calc_hT();

    assembly->mFlange1->Calc_kQ();
    assembly->mFlange2->Calc_kQ();
    assembly->mFlange1->Calc_kR();
    assembly->mFlange2->Calc_kR();

    assembly->mFlange1->Calc_rho();
    assembly->mFlange2->Calc_rho();
    assembly->mFlange1->Calc_hR();
    assembly->mFlange2->Calc_hR();
    assembly->mFlange1->Calc_ZF();
    assembly->mFlange2->Calc_ZF();
    assembly->mFlange1->Calc_ZL();
    assembly->mFlange2->Calc_ZL();
}

void Calculator::F41_to_53(Assembly* assembly) 	{
    assembly->Calc_lB(); // Formula 98 here

    assembly->mFlange1->Calc_AB(); // or assembly.Flange2.Calc_AB() sets Bolt.AB due to number of bolts
    assembly->mFlange1->Calc_XB(); // or assembly.Flange2.Calc_XB() sets Bolt.XB due to number of bolts

    assembly->mFlange1->mWasher->Calc_bW();
    assembly->mFlange2->mWasher->Calc_bW();
    assembly->mFlange1->mWasher->Calc_dW();
    assembly->mFlange2->mWasher->Calc_dW();
    assembly->mFlange1->Calc_dK1();
    assembly->mFlange2->Calc_dK1();
    assembly->mFlange1->Calc_dK2();
    assembly->mFlange2->Calc_dK2();
    assembly->mFlange1->mWasher->Calc_bKB();
    assembly->mFlange2->mWasher->Calc_bKB();
    assembly->mFlange1->Calc_XW();
    assembly->mFlange2->Calc_XW();

    // mIsFirstApproximation And loadCaseNo = 0
    // call this function only once in the beginning
    assembly->Calc_dG1();
    assembly->Calc_dG2();
    assembly->mGasket->Calc_bGt();
    assembly->mGasket->Calc_dGt();
    assembly->mGasket->Calc_AGt();
}

void Calculator::F54_to_54(Assembly* assembly) 	{
    assembly->mBolt->Calc_eta1plus();
    assembly->mBolt->Calc_eta1minus();
    assembly->mFlange1->Calc_etanplus(); // stored in bolt
    // assembly.Flange2.Calc_etanplus() ' was already stored in bolt
    assembly->mFlange1->Calc_etanminus(); // stored in bolt
    // assembly.Flange2.Calc_etanminus() ' was already stored in bolt

    int loadCaseNo = 0;
    LoadCase* loadCase = assembly->mLoadCaseList->at(loadCaseNo);
    loadCase->mForce->Calc_M_AI();
    assembly->Calc_F_R(loadCaseNo);

    if (loadCase->F_Bspec > 0.0) {
        // use Formula 1 (and 2)
        assembly->Calc_F_GInitial_1();
    } else {
        assembly->Calc_F_GInitial();
    }
}

void Calculator::F55_to_62_table1(Assembly* assembly) {
    int loadCaseNo = 0;

    if (!mIsFirstApproximation) {
        assembly->mGasket->Calc_eG(loadCaseNo);
    }

    // HACK: start
    if (mIsFirstApproximation
            && (assembly->mGasket->frmType == Gasket::CurvedSimpleContact
            || assembly->mGasket->frmType == Gasket::CurvedDoubleContact)) {
        // E_G is unknown at this stage, refer Assembly::Calc_bGi()
        assembly->mGasket->dGe = assembly->mGasket->dGt;
        assembly->mGasket->bGe = assembly->mGasket->bGt;
        assembly->mGasket->Calc_AGe();
        assembly->Calc_Q_G(0);
        assembly->mGasket->Calc_E_G(0);
    }
    // HACK: End

    assembly->mGasket->Calc_Q_smax(loadCaseNo);

    assembly->Calc_bGi(mIsFirstApproximation);
    assembly->mGasket->Calc_bGe();
    assembly->Calc_dGe();
    assembly->mGasket->Calc_AGe();
    assembly->Calc_Q_G(0);
    assembly->mGasket->Calc_E_G(loadCaseNo);

    assembly->mFlange1->Calc_d7min(); // loose flange only
    assembly->mFlange2->Calc_d7min(); // loose flange only
    assembly->mFlange1->Calc_d7max(); // loose flange only
    assembly->mFlange2->Calc_d7max(); // loose flange only
    assembly->mFlange1->Calc_chi(); // loose flange only
    assembly->mFlange2->Calc_chi(); // loose flange only
    assembly->mFlange1->Calc_d70(loadCaseNo); // loose flange only
    assembly->mFlange2->Calc_d70(loadCaseNo); // loose flange only
    assembly->mFlange1->Calc_hG();
    assembly->mFlange2->Calc_hG();
}

void Calculator::F63_to_63(Assembly* assembly, int loadCaseNo) {
    assembly->mGasket->Calc_eG(loadCaseNo);
    assembly->mGasket->Calc_XG();
}

void Calculator::F77_to_89(Assembly* assembly, int loadCaseNo) {
    assembly->mFlange1->Calc_hP();
    assembly->mFlange2->Calc_hP();
    assembly->mFlange1->Calc_hQ();
    assembly->mFlange2->Calc_hQ();
    assembly->mFlange1->Calc_d7minMax(loadCaseNo);
    assembly->mFlange2->Calc_d7minMax(loadCaseNo);
    assembly->mFlange1->Calc_hG();
    assembly->mFlange2->Calc_hG();
    assembly->mFlange1->Calc_hH();
    assembly->mFlange2->Calc_hH();
    assembly->mFlange1->Calc_hL();
    assembly->mFlange2->Calc_hL();
}

void Calculator::F90_to_102(Assembly* assembly, int loadCaseNo) {
    assembly->mGasket->Calc_AQ();
    assembly->Calc_F_Q(loadCaseNo);
    assembly->mLoadCaseList->at(loadCaseNo)->mForce->Calc_F_LI();
    assembly->mLoadCaseList->at(loadCaseNo)->mForce->Calc_M_AI();
    assembly->Calc_F_R(loadCaseNo);
//    assembly->Calc_lB(); already before Formula 42
    assembly->Calc_dUI(loadCaseNo);
    assembly->Calc_Q_G(loadCaseNo); // for determining EG Formula 100
    assembly->mGasket->Calc_E_G(loadCaseNo);
    assembly->Calc_YB(loadCaseNo);
    assembly->Calc_YG(loadCaseNo);
    assembly->Calc_YQ(loadCaseNo);
    assembly->Calc_YR(loadCaseNo);
}

void Calculator::F103_to_104(Assembly* assembly, int loadCaseNo) {
    assembly->Calc_Q_A_Qsmin(loadCaseNo);
    assembly->Calc_F_Gmin(loadCaseNo);
}

void Calculator::F105_to_105(Assembly* assembly, int loadCaseNo) {
    if (loadCaseNo == 1) {
        assembly->mGasket->setLoadCaseValues(0);
    }
    assembly->mGasket->setLoadCaseValues(loadCaseNo);
    // includes direct delta_eGc, uses P_QR or set delta_eGc to zero
    assembly->mGasket->Calc_delta_eGc(loadCaseNo);
    assembly->Calc_F_Gdelta(loadCaseNo);
}

void Calculator::F106_to_108(Assembly* assembly) {
    assembly->Calc_F_G0req();
    assembly->Calc_F_B0req();
}

void Calculator::F111_to_118(Assembly* assembly) {
    assembly->Calc_F_B0av();
    assembly->Calc_F_B0nom();
    assembly->Is_F_B0nom_Valid();
    assembly->Calc_F_G0max();
}

void Calculator::F119_to_119(Assembly* assembly) {
    int loadCaseNo = 0;
    LoadCase* loadCase = assembly->mLoadCaseList->at(loadCaseNo);

    if (loadCase->F_Bspec > 0.0) {
        // use Formula 2 (and 1)
        assembly->Calc_F_G0d_2();
    } else {
        assembly->Calc_F_G0d();
    }
}

void Calculator::F120_to_122(Assembly* assembly, int loadCaseNo) {
    assembly->Calc_F_G(loadCaseNo);
    assembly->Calc_F_B(loadCaseNo);
    assembly->Calc_Q_G(loadCaseNo); // 3rd time?
//    assembly->Calc_F_Bmax(loadCaseNo);
//    assembly->Calc_F_Gmax(loadCaseNo);
}

void Calculator::F123_to_151(Assembly* assembly, int loadCaseNo) {
    // bolt load
    assembly->mBolt->Calc_kB();
    assembly->Calc_Mtnom();
    assembly->mBolt->Calc_kB9();
    assembly->Calc_MtBnom();
    assembly->mBolt->Calc_IB();
    assembly->Calc_cA(loadCaseNo);
    assembly->Calc_cB(loadCaseNo);
    assembly->Calc_PhiB(loadCaseNo);
    bool res = assembly->Is_PhiB_Valid(loadCaseNo);

    assembly->mGasket->Calc_Q_smax(loadCaseNo);
    assembly->Calc_PhiG(loadCaseNo);
    res = assembly->Is_PhiG_Valid(loadCaseNo);

    assembly->Calc_fE(assembly->mFlange1, loadCaseNo);
    assembly->Calc_fE(assembly->mFlange2, loadCaseNo);
    assembly->mFlange1->Calc_deltaQ(loadCaseNo);
    assembly->mFlange2->Calc_deltaQ(loadCaseNo);
    assembly->mFlange1->Calc_deltaR(loadCaseNo);
    assembly->mFlange2->Calc_deltaR(loadCaseNo);

    assembly->mFlange1->Calc_cM(loadCaseNo);
    res = assembly->mFlange1->Is_cM_valid(loadCaseNo);
    assembly->mFlange2->Calc_cM(loadCaseNo);
    res = assembly->mFlange2->Is_cM_valid(loadCaseNo);
    assembly->mFlange1->Calc_cS(loadCaseNo);
    res = assembly->mFlange1->Is_cS_valid(loadCaseNo);
    assembly->mFlange2->Calc_cS(loadCaseNo);
    res = assembly->mFlange2->Is_cS_valid(loadCaseNo);
    assembly->mFlange1->Calc_jM(loadCaseNo);
    assembly->mFlange2->Calc_jM(loadCaseNo);

    assembly->mFlange1->Calc_PsiOpt(loadCaseNo);
    assembly->mFlange2->Calc_PsiOpt(loadCaseNo);
    assembly->mFlange1->Calc_Psi0(loadCaseNo);
    assembly->mFlange2->Calc_Psi0(loadCaseNo);
    assembly->mFlange1->Calc_PsiMax(loadCaseNo);
    assembly->mFlange2->Calc_PsiMax(loadCaseNo);
    assembly->mFlange1->Calc_PsiMin(loadCaseNo);
    assembly->mFlange2->Calc_PsiMin(loadCaseNo);
    assembly->mFlange1->Calc_PsiZ(loadCaseNo);
    assembly->mFlange2->Calc_PsiZ(loadCaseNo);
    //assembly.Flange1.Calc_WF(loadCaseNo) ' already in PsiZ or PhiF
    //assembly.Flange2.Calc_WF(loadCaseNo)
    assembly->mFlange1->Calc_WQ(loadCaseNo); // here denominator of formula 151, in alternative for PhiF
    assembly->mFlange2->Calc_WQ(loadCaseNo);
    assembly->mFlange1->Calc_PhiF(loadCaseNo);
    assembly->mFlange2->Calc_PhiF(loadCaseNo);
    // only relevant if groove and eX has been set
    assembly->mFlange1->Calc_WX(loadCaseNo);
    assembly->mFlange2->Calc_WX(loadCaseNo);
    assembly->mFlange1->Calc_PhiX(loadCaseNo);
    assembly->mFlange2->Calc_PhiX(loadCaseNo);
    // only for loose flange
    assembly->mFlange1->Calc_WL(loadCaseNo);
    assembly->mFlange2->Calc_WL(loadCaseNo);
    assembly->mFlange1->Calc_PhiL(loadCaseNo);
    assembly->mFlange2->Calc_PhiL(loadCaseNo);

    res = assembly->Is_PhiB_Valid(loadCaseNo);
    res = assembly->Is_PhiG_Valid(loadCaseNo);
    res = assembly->mFlange1->Is_cM_valid(loadCaseNo);
    res = assembly->mFlange2->Is_cM_valid(loadCaseNo);
    res = assembly->mFlange1->Is_cS_valid(loadCaseNo);
    res = assembly->mFlange2->Is_cS_valid(loadCaseNo);
    res = assembly->mFlange1->Is_PsiMaxMin_Valid(loadCaseNo);
    res = assembly->mFlange2->Is_PsiMaxMin_Valid(loadCaseNo);
    res = assembly->mFlange1->Is_PhiF_valid(loadCaseNo);
    res = assembly->mFlange2->Is_PhiF_valid(loadCaseNo);
    res = assembly->mFlange1->Is_PhiX_Valid(loadCaseNo);
    res = assembly->mFlange2->Is_PhiX_Valid(loadCaseNo);
    res = assembly->mFlange1->Is_PhiL_valid(loadCaseNo);
    res = assembly->mFlange2->Is_PhiL_valid(loadCaseNo);
}

void Calculator::FC1_to_C10(Assembly* assembly, int loadCaseNo) {

    // Note: in below output: Dim radToDeg As Double = 180 / Math.PI

    if (loadCaseNo == 0) {
        assembly->Calc_F_BImaxmin(loadCaseNo);
        assembly->Calc_F_GImaxmin(loadCaseNo);
    } else {
        // Note different order
        assembly->Calc_F_GImaxmin(loadCaseNo);
        assembly->Calc_F_BImaxmin(loadCaseNo);
    }

    assembly->Calc_ThetaFmaxmin(loadCaseNo);
    assembly->Calc_ThetaLmaxmin(loadCaseNo);
}

END_NAMESPACE_BILUNA_CALC_EN1591
