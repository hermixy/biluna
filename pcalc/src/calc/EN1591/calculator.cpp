#include "calculator.h"
NAMESPACE_REDBAG_CALC_EN1591


Calculator::Calculator(FlangeType flange1Type, FlangeType flange2Type)
            : RB_Object() {
    setName("PCALC EN1591 Calculator");

    mAssembly = new Assembly();
    mWriteCalcFromNumber = 0;
    mIterNo = 0;
    mIsFirstApproximation = false;
    mLoadCaseCount = 0;

    createAssembly(flange1Type, flange2Type);
}

Calculator::~Calculator() {
    delete mAssembly;
    mAssembly = NULL;
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
    CreateAssemblyHelper(mAssembly);
}

void Calculator::CreateAssemblyHelper(Assembly* assembly) {
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

void Calculator::SetInputData_Ohmtech(Assembly* assembly)
{
    assembly->mFlange1->eP = 18.5; // inside head or shell
    assembly->mFlange2->eP = 18.5;

    assembly->mFlange1->eF = 18.5;
    assembly->mFlange2->eF = 18.5;
    assembly->mFlange1->eFt = 18.5;
    assembly->mFlange2->eFt = 18.5;

    // GA_5_54_to_5_56
    assembly->mGasket->dG1 = 407.0; // max of d0 and dGinterndiam
    assembly->mGasket->dG2 = 490.0; // min of raised face and dGouterdiam
    //assembly.Gasket.eGt = 3.0
    //assembly.LoadCaseList(0).eG = assembly.Gasket.eGt ' for initial GA.5-63
    //assembly.Gasket.eGA = assembly.Gasket.eGt ' for initial GA.5-64 required?
    assembly->mGasket->eGt = 3.0;

    // Tightness GA.6.5.1
    //assembly.Gasket.QA0 = 25
    //assembly.Gasket.QA1 = 40.4 ' Table GA.9.X
    //assembly.Gasket.QA2 = 161.1 ' Table GA.9.X
    //assembly.Gasket.M1 = 4.4 ' Table GA.9.X
    //assembly.Gasket.N1 = 2.1 ' Table GA.9.X
    //assembly.Gasket.M2 = 16.1 ' Table GA.9.X
    //assembly.Gasket.N2 = 2.3 ' Table GA.9.X

    // GA_5_1_to_5_22
    assembly->mFlange1->d3 = 525;
    assembly->mFlange2->d3 = 525;
    assembly->mFlange1->nB = 16;
    assembly->mFlange2->nB = 16;

    assembly->mBolt->mBoltHole->d5 = 30;

    assembly->mFlange1->d0 = 384.52; // to be calc. not greater inside diameter gasket
    assembly->mFlange2->d0 = 384.52; // to be calc. not greater inside diameter gasket
    assembly->mFlange1->d4 = 580.0; // direct usable
    assembly->mFlange2->d4 = 580.0; // direct usable

    assembly->mFlange1->eL = 36.5;
    assembly->mFlange2->eL = assembly->mFlange1->eL;

    assembly->mFlange1->d6 = 416;
    assembly->mFlange2->d6 = assembly->mFlange1->d6;
    assembly->mFlange1->d8 = 490;
    assembly->mFlange2->d8 = assembly->mFlange1->d8;
    assembly->mFlange1->d7min = 432;
    assembly->mFlange2->d7min = assembly->mFlange1->d7min;

    //assembly.Flange1.AF = 975.69 ' DONE: should be calculated based on eF and eF to be input
    //assembly.Flange2.AF = 975.69

    assembly->mFlange1->e1 = 4.38;
    assembly->mFlange1->mShell->eS = 4.38; // required for calculation if not hub
    assembly->mFlange1->mShell->dS = assembly->mFlange1->d0 + assembly->mFlange1->mShell->eS; // average
    assembly->mFlange1->e2 = 10.94; // with hub

    assembly->mFlange2->e1 = assembly->mFlange1->e1;
    assembly->mFlange2->mShell->eS = assembly->mFlange1->mShell->eS;
    assembly->mFlange2->mShell->dS = assembly->mFlange1->mShell->dS;
    assembly->mFlange2->e2 = assembly->mFlange1->e2;

    assembly->mFlange1->lH = 85;
    assembly->mFlange1->d1 = 402.02; // to be calculate based on flange dimensions
    assembly->mFlange1->d2 = 395.46; // to be calculate based on flange dimensions, integral only
    assembly->mFlange2->lH = 85;
    assembly->mFlange2->d1 = 402.02; // to be calculate based on flange dimensions
    assembly->mFlange2->d2 = 395.46; // to be calculate based on flange dimensions, integral only

    // GA_5_23_to_5_53
    assembly->mFlange1->mShell->phiS = 0.0;
    assembly->mFlange2->mShell->phiS = 0.0;

    assembly->mBolt->dBe = 24.19;
    assembly->mBolt->dBS = 27;
    assembly->mBolt->dB0 = 27;
    assembly->mBolt->lB = 113;
    assembly->mBolt->lS = 0.0;
    //assembly->mBolt->dB4 = 41.0 'Table GA.8-1
    // GA_7
    assembly->mBolt->tType = Bolt::TorqueWrench;
    assembly->mBolt->ruptureElongationA = 0.02;
    //assembly->mBolt->eN = 27.0 ' height of nut estimated
    //assembly->mBolt->l5t = -1 ' not blind

    // GA_5_57_to_5_84
    // FG0
    assembly->mFlange1->mBolt->etanplus = 0.31;
    // cE
    // Q
    //assembly.LoadCaseList(0).AGe = Math.PI * (assembly.Gasket.dG2 + assembly.Gasket.dG1) / 2 _
    //    * (assembly.Gasket.dG2 - assembly.Gasket.dG1) / 2
    // bGp
    //assembly.Gasket.QR0 = 280 ' Table GA.9.X TODO: bring to LoadCase


    //assembly.fldType = Assembly.FluidType.Gas

    assembly->mNR = 20;
    //assembly.IncreaseOfNominalBoltForce = 1.075 ' 7.5% refer GA.6-29
    assembly->mBolt->mut = 0.2; // 8.2.3

}

void Calculator::SetLoadCases_Ohmtech(Assembly* assembly)
{

    //
    // Load case 0
    //
    int loadCaseNo = 0;
    LoadCase *loadCase = assembly->mLoadCaseList->at(loadCaseNo);
    // Initial data Unit Tests
    loadCase->P = 0.0; // N/mm2
    // FG0
    loadCase->mForce->F_Z = 0;
    loadCase->mForce->M_AI = 0;

    // DGm values from table: GA.9.6, low alloy stainless steel jacket, temp. 300
    //loadCase.C0 = 190.1
    //loadCase.C1 = 30.7
    // EG
    //loadCase.K0 = 340.1
    //loadCase.K1 = 36.7
    //loadCase.cG = 1.0 ' from Table GA.9.X cG > 1 than calculated
    loadCase->muG = 0.18; // from Table GA.9.X

    // EN1591
    //loadCase.E0 = 8000
    //loadCase.gC = 1
    //loadCase.K1 = 20
    //loadCase.mi = 1.6
    //loadCase.Qmin = 25
    //loadCase.Qsmax = 80

    loadCase->alphaB = 0.000015; // bolt thermal expansion at temperature
    loadCase->alphaG = 0.000015; // thermal gasket expansion at temperature
    loadCase->alphaF1 = 0.000015;
    loadCase->alphaF2 = 0.000015;
    loadCase->alphaL1 = 0.000015;
    loadCase->alphaL2 = 0.000015;

    loadCase->EB = 212000.0; // Elasticity moduly except gasket, steel is 200 GPa
    loadCase->EF1 = 212000.0;
    loadCase->EF2 = 212000.0;
    loadCase->EL1 = 212000.0;
    loadCase->EL2 = 212000.0;

    loadCase->fB = 285.0;
    loadCase->fF1 = 285.0;
    loadCase->fF2 = 285.0;
    loadCase->fL1 = 285.0;
    loadCase->fL2 = 285.0;
    loadCase->fN = 285.0;
    loadCase->fS1 = 285.0;
    loadCase->fS2 = 285.0;

    loadCase->T0 = 20; // Celsius
    loadCase->TB = 20; // Celsius
    loadCase->TG = 20;
    loadCase->TF1 = 20;
    loadCase->TF2 = 20;
    loadCase->TL1 = 20;
    loadCase->TL2 = 20;

    //loadCase.Q_R = 320
    //
    // Load case 1, test pressure only
    //
    loadCaseNo = 1;
    LoadCase *loadCase1 = assembly->mLoadCaseList->at(loadCaseNo);
    loadCase1->setInputData(loadCase);
    loadCase1->P = 2.29; // N/mm2
    loadCase1->mForce->F_Z = 0.0;
    loadCase1->mForce->M_AI = 0.0;

    // EN1591
    //loadCase1.E0 = 8000
    //loadCase1.gC = 1
    //loadCase1.K1 = 20
    //loadCase1.mi = 1.6
    //loadCase1.Qmin = 25
    //loadCase1.Qsmax = 80

    loadCase1->EF1 = 200000.1; // Elasticity moduly except gasket
    loadCase1->EF2 = 200000.2;
    loadCase1->EL1 = 200000.0;
    loadCase1->EL2 = 200000.0;

    loadCase1->fB = 285.0;
    loadCase1->fF1 = 285.0;
    loadCase1->fF2 = 285.0;
    loadCase1->fL1 = 285.0;
    loadCase1->fL2 = 285.0;
    loadCase1->fN = 285.0;
    loadCase1->fS1 = 285.0;
    loadCase1->fS2 = 285.0;

    //loadCase1.Q_R = 280

    //
    // Load case 2, operating
    //
    loadCaseNo = 2;
    LoadCase* loadCase2 = assembly->mLoadCaseList->createLoadCase();
    loadCase2->setInputData(loadCase1);
    loadCase2->P = 1.6; // N/mm2
    loadCase2->mForce->F_Z = 154700;
    loadCase2->mForce->M_AI = 0.0;

    loadCase2->fB = 193.0;
    loadCase2->fF1 = 197.0;
    loadCase2->fF2 = 197.0;
    loadCase2->fL1 = 198.0;
    loadCase2->fL2 = 198.0;
    loadCase2->fN = 190.0;
    loadCase2->fS1 = 192.0;
    loadCase2->fS2 = 192.0;

    loadCase2->T0 = 20; // ambient
    loadCase2->TB = 100; // Celsius
    loadCase2->TG = 200;
    loadCase2->TF1 = 200;
    loadCase2->TF2 = 200;
    loadCase2->TL1 = 200;
    loadCase2->TL2 = 200;

    // EN1591
    //loadCase2.E0 = 8000
    //loadCase2.gC = 0.9
    //loadCase2.K1 = 20
    //loadCase2.mi = 1.6
    //loadCase2.Qmin = 0
    //loadCase2.Qsmax = 80

}

void Calculator::SetInputData_CR13642_B4(Assembly* assembly)
{
    // flanges
    assembly->mFlange1->d0 = 1000;
    assembly->mFlange2->d0 = assembly->mFlange1->d0;
    assembly->mFlange1->d1 = 1024;
    assembly->mFlange2->d1 = assembly->mFlange1->d1;
    assembly->mFlange1->d2 = 1062;
    assembly->mFlange2->d2 = assembly->mFlange1->d2;
    assembly->mFlange1->d4 = 1160;
    assembly->mFlange2->d4 = assembly->mFlange1->d4;

    assembly->mFlange1->lH = 85;
    assembly->mFlange2->lH = 85;

    assembly->mFlange1->eP = 55; // inside head or shell
    assembly->mFlange2->eP = 55;

    assembly->mFlange1->eF = 55;
    assembly->mFlange2->eF = 55;
    assembly->mFlange1->eFt = 55;
    assembly->mFlange2->eFt = 55;

    assembly->mFlange1->e1 = 10;
    assembly->mFlange2->e1 = assembly->mFlange1->e1;
    assembly->mFlange1->mShell->eS = 10; // thickness shell, required for calculation if not hub
    assembly->mFlange2->mShell->eS = assembly->mFlange1->mShell->eS;
    assembly->mFlange1->mShell->dS = assembly->mFlange1->d0 + assembly->mFlange1->mShell->eS; // average
    assembly->mFlange2->mShell->dS = assembly->mFlange1->mShell->dS;
    assembly->mFlange1->e2 = 15; // hub thickness
    assembly->mFlange2->e2 = assembly->mFlange1->e2;

    assembly->mFlange1->mShell->phiS = 0.0;
    assembly->mFlange2->mShell->phiS = 0.0;

    // gasket
    assembly->mGasket->dG1 = 1060.0; // max of d0 and dGinterndiam
    assembly->mGasket->dG2 = 1090.0; // min of raised face and dGouterdiam
    assembly->mGasket->eGt = 2.0;

    // bolt
    assembly->mFlange1->d3 = 1120;
    assembly->mFlange2->d3 = 1120;
    assembly->mFlange1->nB = 52;
    assembly->mFlange2->nB = 52;
    assembly->mBolt->mBoltHole->d5 = 23;

    assembly->mBolt->dBe = 17.65;
    assembly->mBolt->dBS = 18.3;
    assembly->mBolt->dB0 = 20;
    assembly->mBolt->lB = 113;
    assembly->mBolt->lS = 80.0;
    //assembly->mBolt->dB4 = 41.0 'Table GA.8-1

    assembly->mBolt->tType = Bolt::TorqueWrench;
    assembly->mBolt->ruptureElongationA = 0.02;
    //assembly->mBolt->eN = 27.0 ' height of nut estimated
    assembly->mBolt->l5t = -1; // not blind
    //assembly.IncreaseOfNominalBoltForce = 1.075 ' 7.5% refer GA.6-29
    assembly->mBolt->mut = 0.2; // 8.2.3

    //assembly.Flange1.Washer.dW1 = 0.0
    //assembly.Flange1.Washer.dW2 = 0.0
    //assembly.Flange2.Washer.dW1 = 0.0
    //assembly.Flange2.Washer.dW2 = 0.0
    //assembly.Flange1.Washer.eW = 0.0
    //assembly.Flange2.Washer.eW = 0.0

    // GA_5_57_to_5_84
    // FG0
    //assembly.Flange1->mBolt->etanplus = 0.31
    // cE
    // Q
    //assembly.LoadCaseList(0).AGe = Math.PI * (assembly.Gasket.dG2 + assembly.Gasket.dG1) / 2 _
    //    * (assembly.Gasket.dG2 - assembly.Gasket.dG1) / 2
    // bGp
    //assembly.Gasket.QR0 = 280 ' Table GA.9.X TODO: bring to LoadCase

    //assembly.fldType = Assembly.FluidType.Liquid

    assembly->mNR = 20;

}

void Calculator::SetLoadCases_CR13642_B4(Assembly* assembly)
{
    //
    // Load case 0
    //
    int loadCaseNo = 0;
    LoadCase *loadCase = assembly->mLoadCaseList->at(loadCaseNo);
    // Initial data Unit Tests
    loadCase->P = 0.0; // N/mm2
    // FG0
    loadCase->mForce->F_Z = 0;
    loadCase->mForce->M_AI = 0;

    // DGm values from table: GA.9.6, low alloy stainless steel jacket, temp. 300
    //loadCase.C0 = 190.1
    //loadCase.C1 = 30.7
    // EG
    //loadCase.K0 = 340.1
    //loadCase.K1 = 36.7
    //loadCase.cG = 1.0 ' from Table GA.9.X cG > 1 than calculated
    loadCase->muG = 0.18; // from Table GA.9.X

    // EN1591
    //loadCase.E0 = 8000
    //loadCase.gC = 1
    //loadCase.K1 = 20
    //loadCase.mi = 1.6
    //loadCase.Qmin = 25
    //loadCase.Qsmax = 80

    loadCase->alphaB = 0.000015; // bolt thermal expansion at temperature
    loadCase->alphaG = 0.000015; // thermal gasket expansion at temperature
    loadCase->alphaF1 = 0.000015;
    loadCase->alphaF2 = 0.000015;
    //loadCase.alphaL1 = 0.000015
    //loadCase.alphaL2 = 0.000015

    loadCase->EB = 212000.0; // Elasticity moduly except gasket, steel is 200 GPa
    loadCase->EF1 = 212000.0;
    loadCase->EF2 = 212000.0;
    loadCase->EL1 = 212000.0; // for Y values
    loadCase->EL2 = 212000.0; // for Y values

    loadCase->fB = 300.0;
    loadCase->fF1 = 240.0;
    loadCase->fF2 = 240.0;
    //loadCase.fL1 = 285.0
    //loadCase.fL2 = 285.0
    loadCase->fN = 285.0;
    loadCase->fS1 = 285.0;
    loadCase->fS2 = 285.0;

    loadCase->T0 = 20; // Celsius
    loadCase->TB = 20; // Celsius
    loadCase->TG = 20;
    loadCase->TF1 = 20;
    loadCase->TF2 = 20;
    loadCase->TL1 = 20;
    loadCase->TL2 = 20;

    //loadCase.Q_R = 320

    //
    // Load case 1, test pressure only
    //
    loadCaseNo = 1;
    LoadCase *loadCase1 = assembly->mLoadCaseList->at(loadCaseNo);
    loadCase1->setInputData(loadCase);
    loadCase1->P = 2.0; // N/mm2
    loadCase1->mForce->F_Z = 0.0;
    loadCase1->mForce->M_AI = 0.0;

    // EN1591
    //loadCase1.E0 = 8000
    //loadCase1.gC = 1
    //loadCase1.K1 = 20
    //loadCase1.mi = 1.6
    //loadCase1.Qmin = 25
    //loadCase1.Qsmax = 80

    loadCase1->EF1 = 200000.1; // Elasticity moduly except gasket
    loadCase1->EF2 = 200000.2;
    loadCase1->EL1 = 200000.0;
    loadCase1->EL2 = 200000.0;

    loadCase1->fB = 300.0;
    loadCase1->fF1 = 240.0;
    loadCase1->fF2 = 240.0;
    loadCase1->fL1 = 285.0;
    loadCase1->fL2 = 285.0;
    loadCase1->fN = 285.0;
    loadCase1->fS1 = 285.0;
    loadCase1->fS2 = 285.0;

    //loadCase1.Q_R = 280

}

void Calculator::SetInputData_Ambrosio(Assembly* assembly)
{
    // flanges Ambrosio
    assembly->mFlange1->d0 = 1500;
    assembly->mFlange2->d0 = assembly->mFlange1->d0;
    assembly->mFlange1->d1 = 1532;
    assembly->mFlange2->d1 = assembly->mFlange1->d1;
    assembly->mFlange1->d2 = 1522;
    assembly->mFlange2->d2 = assembly->mFlange1->d2;
    assembly->mFlange1->d4 = 1717;
    assembly->mFlange2->d4 = assembly->mFlange1->d4;

    assembly->mFlange1->lH = 44;
    assembly->mFlange2->lH = 44;

    assembly->mFlange1->eP = 143; // inside head or shell
    assembly->mFlange2->eP = 143;

    assembly->mFlange1->eF = 143;
    assembly->mFlange2->eF = 143;
    assembly->mFlange1->eFt = 143;
    assembly->mFlange2->eFt = 143;

    assembly->mFlange1->e1 = 14;
    assembly->mFlange2->e1 = assembly->mFlange1->e1;
    assembly->mFlange1->e2 = 22; // hub thickness
    assembly->mFlange2->e2 = assembly->mFlange1->e2;
    assembly->mFlange1->mShell->eS = 14; // thickness shell, required for calculation if not hub
    assembly->mFlange2->mShell->eS = assembly->mFlange1->mShell->eS;
    assembly->mFlange1->mShell->dS = assembly->mFlange1->d0 + assembly->mFlange1->mShell->eS; // average
    assembly->mFlange2->mShell->dS = assembly->mFlange1->mShell->dS;

    assembly->mFlange1->mShell->phiS = 0.0;
    assembly->mFlange2->mShell->phiS = 0.0;

    // gasket
    assembly->mGasket->dG1 = 1500.0; // max of d0 and dGinterndiam
    assembly->mGasket->dG2 = 1532.0; // min of raised face and dGouterdiam
    assembly->mGasket->eGt = 3.0;
    assembly->mGasket->frmType = Gasket::Flat;
    // assembly->mGasket->matType = Gasket::SpiralWound;
    assembly->mGasket->insType = Gasket::SpiralGraphFillOuterOnly;

    // bolt
    assembly->mFlange1->d3 = 1644;
    assembly->mFlange2->d3 = assembly->mFlange1->d3;
    assembly->mFlange1->nB = 60;
    assembly->mFlange2->nB = assembly->mFlange1->nB;
    assembly->mBolt->mBoltHole->d5 = 36;

    assembly->mBolt->dBe = 29.72;
    assembly->mBolt->dBS = 25.5;
    assembly->mBolt->dB0 = 33.0;
    assembly->mBolt->dB2 = 29;
    assembly->mBolt->lB = 290.0;
    assembly->mBolt->lS = 0.0;
    //assembly->mBolt->dB4 = 41.0 'Table GA.8-1

    assembly->mBolt->tType = Bolt::TorqueWrench;
    assembly->mBolt->ruptureElongationA = 0.02;
    assembly->mBolt->eN = 27.0; // height of nut estimated
    assembly->mBolt->l5t = -1; // not blind
    //assembly.IncreaseOfNominalBoltForce = 1.075 ' 7.5% refer GA.6-29
    assembly->mBolt->mut = 0.2; // 8.2.3

    //assembly.Flange1.Washer.dW1 = 0.0
    //assembly.Flange1.Washer.dW2 = 0.0
    //assembly.Flange2.Washer.dW1 = 0.0
    //assembly.Flange2.Washer.dW2 = 0.0
    //assembly.Flange1.Washer.eW = 0.0
    //assembly.Flange2.Washer.eW = 0.0

    // GA_5_57_to_5_84
    // FG0
    //assembly.Flange1->mBolt->etanplus = 0.31
    // cE
    // Q
    //assembly.LoadCaseList(0).AGe = Math.PI * (assembly.Gasket.dG2 + assembly.Gasket.dG1) / 2 _
    //    * (assembly.Gasket.dG2 - assembly.Gasket.dG1) / 2
    // bGp
    //assembly.Gasket.QR0 = 280 ' Table GA.9.X TODO: bring to LoadCase

    //assembly.fldType = Assembly.FluidType.Liquid

    assembly->mNR = 10;

}

void Calculator::SetLoadCases_Ambrosio(Assembly* assembly)
{
    //
    // Load case 0
    //
    int loadCaseNo = 0;
    LoadCase *loadCase = assembly->mLoadCaseList->at(loadCaseNo);

    loadCase->P = 0.0; // N/mm2
    loadCase->mForce->F_Z = 0;
    loadCase->mForce->M_AI = 0;

    //loadCase.E0 = 1
    //loadCase.gC = 1
    //loadCase.K1 = 48
    //loadCase.mi = 0.0
    //loadCase.Qmin = 20
    loadCase->Q_smax = 300; // depends only on temperature refer gasketdata.org

    loadCase->alphaB = 0.0000111; // bolt thermal expansion at temperature
    loadCase->alphaG = 0.0000103; // thermal gasket expansion at temperature
    loadCase->alphaF1 = 0.0000115;
    loadCase->alphaF2 = loadCase->alphaF1;

    loadCase->EB = 211000.0; // Elasticity moduly except gasket, steel is 200 GPa
    loadCase->EF1 = 200021.0;
    loadCase->EF2 = loadCase->EF1;
    //loadCase.EL1 = 212000.0 ' for Y values
    //loadCase.EL2 = loadCase.EL1
    loadCase->EW1 = 212000.0; // for Y values
    loadCase->EW2 = loadCase->EW1;

    loadCase->fB = 419.048;
    loadCase->fF1 = 266.667;
    loadCase->fF2 = loadCase->fF1;
    //loadCase.fL1 = 285.0
    //loadCase.fL2 = 285.0
    loadCase->fN = 285.0;
    loadCase->fS1 = 338.095;
    loadCase->fS2 = loadCase->fS1;
    //loadCase.fW1 = 285.0
    //loadCase.fW2 = 285.0

    loadCase->T0 = 20; // Celsius
    loadCase->TB = 20; // Celsius
    loadCase->TG = 20;
    loadCase->TF1 = 20;
    loadCase->TF2 = 20;
    //loadCase.TL1 = 20
    //loadCase.TL2 = 20

    //loadCase.Q_R = 320

    //
    // Load case 1, test pressure only
    //
    loadCaseNo = 1;
    LoadCase *loadCase1 = assembly->mLoadCaseList->at(loadCaseNo);
    loadCase1->setInputData(loadCase);
    loadCase1->P = 3.804; // N/mm2
    loadCase1->mForce->F_Z = 0;
    loadCase1->mForce->M_AI = 0;

    //loadCase1.E0 = 1
    //loadCase1.gC = 1
    //loadCase1.K1 = 48
    //loadCase1.mi = 1.3
    //loadCase1.Qmin = 0
    loadCase1->Q_smax = 300;

    loadCase1->alphaB = 0.0000111; // bolt thermal expansion at temperature
    loadCase1->alphaG = 0.0000103; // thermal gasket expansion at temperature
    loadCase1->alphaF1 = 0.0000115;
    loadCase1->alphaF2 = loadCase1->alphaF1;

    loadCase1->EB = 211000.0; // Elasticity moduly except gasket, steel is 200 GPa
    loadCase1->EF1 = 200021.0;
    loadCase1->EF2 = loadCase1->EF1;
    loadCase1->EW1 = 212000.0; // for Y values
    loadCase1->EW2 = loadCase->EW1;

    loadCase1->fB = 419.048;
    loadCase1->fF1 = 266.667;
    loadCase1->fF2 = loadCase1->fF1;
    loadCase1->fN = 285.0;
    loadCase1->fS1 = 338.095;
    loadCase1->fS2 = loadCase1->fS1;

    loadCase1->T0 = 20; // Celsius
    loadCase1->TB = 20; // Celsius
    loadCase1->TG = 20;
    loadCase1->TF1 = 20;
    loadCase1->TF2 = 20;

    //
    // Load case 2, test pressure only
    //
    loadCaseNo = 2;
    LoadCase* loadCase2 = assembly->mLoadCaseList->createLoadCase();
    loadCase2->setInputData(loadCase1);
    loadCase2->P = 2.5; // N/mm2
    loadCase2->mForce->F_Z = 0;
    loadCase2->mForce->M_AI = 0;

    //loadCase2.E0 = 1
    //loadCase2.gC = 1
    //loadCase2.K1 = 48
    //loadCase2.mi = 1.3
    //loadCase2.Qmin = 0
    loadCase2->Q_smax = 300;

    loadCase2->alphaB = 0.0000121; // bolt thermal expansion at temperature
    loadCase2->alphaG = 0.0000121; // thermal gasket expansion at temperature
    loadCase2->alphaF1 = 0.0000128;
    loadCase2->alphaF2 = loadCase2->alphaF1;

    loadCase2->EB = 196000.0; // Elasticity moduly except gasket, steel is 200 GPa
    loadCase2->EF1 = 190496.0;
    loadCase2->EF2 = loadCase2->EF1;
    loadCase2->EW1 = 212000.0; // for Y values
    loadCase2->EW2 = loadCase->EW1;

    loadCase2->fB = 250.048;
    loadCase2->fF1 = 153.333;
    loadCase2->fF2 = loadCase2->fF1;
    loadCase2->fN = 285.0;
    loadCase2->fS1 = 183.333;
    loadCase2->fS2 = loadCase2->fS1;

    loadCase2->T0 = 20; // Celsius
    loadCase2->TB = 200; // Celsius
    loadCase2->TG = 200;
    loadCase2->TF1 = 200;
    loadCase2->TF2 = 200;

}

void Calculator::SetInputData_Plantware(Assembly* assembly)
{
    assembly->mGasket->frmType = Gasket::CurvedSimpleContact;

    // flanges
    assembly->mFlange1->d0 = 24.0;
    assembly->mFlange2->d0 = assembly->mFlange1->d0;
    assembly->mFlange1->d1 = 32.4;
    assembly->mFlange2->d1 = assembly->mFlange1->d1;
    assembly->mFlange1->d2 = 32.4;
    assembly->mFlange2->d2 = assembly->mFlange1->d2;
    assembly->mFlange1->d4 = 115;
    assembly->mFlange2->d4 = assembly->mFlange1->d4;

    assembly->mFlange1->d6 = 42.4;
    assembly->mFlange2->d6 = assembly->mFlange1->d6;
    assembly->mFlange1->d7min = 42.4;
    assembly->mFlange2->d7min = assembly->mFlange1->d7min;
    assembly->mFlange1->d8 = 42.4;
    assembly->mFlange2->d8 = assembly->mFlange1->d8;

    assembly->mFlange1->lH = 0;
    assembly->mFlange2->lH = 0;

    assembly->mFlange1->eP = 25.0; // inside head or shell
    assembly->mFlange2->eP = 25.0;

    assembly->mFlange1->eF = 25.0;
    assembly->mFlange2->eF = assembly->mFlange1->eF;
    assembly->mFlange1->eFt = 25.0;
    assembly->mFlange2->eFt = assembly->mFlange1->eFt;

    assembly->mFlange1->eL = 25.0; // dummy because not available
    assembly->mFlange2->eL = assembly->mFlange1->eL;

    assembly->mFlange1->e1 = 9.5;
    assembly->mFlange2->e1 = assembly->mFlange1->e1;
    assembly->mFlange1->e2 = 9.5; // hub thickness
    assembly->mFlange2->e2 = assembly->mFlange1->e2;
    assembly->mFlange1->mShell->eS = 9.5; // thickness shell, required for calculation if not hub
    assembly->mFlange2->mShell->eS = assembly->mFlange1->mShell->eS;
    assembly->mFlange1->mShell->dS = 32.4; // assembly.Flange1.d0 + assembly.Flange1->mShell->eS ' average
    assembly->mFlange2->mShell->dS = assembly->mFlange1->mShell->dS;

    assembly->mFlange1->mShell->phiS = 0.0;
    assembly->mFlange2->mShell->phiS = 0.0;

    // gasket
    assembly->mGasket->dG1 = 24.0; // max of d0 and dGinterndiam
    assembly->mGasket->dG2 = 37.0; // min of raised face and dGouterdiam
    assembly->mGasket->eGt = 11.0;
    assembly->mGasket->phiG = 20 / 180 * M_PI;
    assembly->mGasket->r2 = 43.0;

    // bolt
    assembly->mFlange1->d3 = 80;
    assembly->mFlange2->d3 = assembly->mFlange1->d3;
    assembly->mFlange1->nB = 8;
    assembly->mFlange2->nB = assembly->mFlange1->nB;
    assembly->mBolt->mBoltHole->d5 = 24;

    assembly->mBolt->dBe = 21.2;
    assembly->mBolt->dBS = 22.1;
    assembly->mBolt->dB0 = 24.0;
    assembly->mBolt->lB = 576.2;
    assembly->mBolt->lS = 100;
    //assembly->mBolt->dB4 = 41.0 'Table GA.8-1

    assembly->mBolt->tType = Bolt::TorqueWrench;
    assembly->mBolt->ruptureElongationA = 0.2;
    assembly->mBolt->eN = 27.0; // height of nut estimated
    //assembly->mBolt->l5t = -1 ' not blind
    //assembly.IncreaseOfNominalBoltForce = 1.075 ' 7.5% refer GA.6-29
    assembly->mBolt->mut = 0.15; // 8.2.3

    //assembly.Flange1.Washer.dW1 = 0.0
    //assembly.Flange1.Washer.dW2 = 0.0
    //assembly.Flange2.Washer.dW1 = 0.0
    //assembly.Flange2.Washer.dW2 = 0.0
    //assembly.Flange1.Washer.eW = 0.0
    //assembly.Flange2.Washer.eW = 0.0

    // GA_5_57_to_5_84
    // FG0
    //assembly.Flange1->mBolt->etanplus = 0.31
    // cE
    // Q
    //assembly.LoadCaseList(0).AGe = Math.PI * (assembly.Gasket.dG2 + assembly.Gasket.dG1) / 2 _
    //    * (assembly.Gasket.dG2 - assembly.Gasket.dG1) / 2
    // bGp
    //assembly.Gasket.QR0 = 280 ' Table GA.9.X TODO: bring to LoadCase

    //assembly.TPmax = 1.4
    //assembly.fldType = Assembly.FluidType.Liquid

    assembly->mNR = 10;

}

void Calculator::SetLoadCases_Plantware(Assembly* assembly)
{
    //
    // Load case 0
    //
    int loadCaseNo = 0;
    LoadCase *loadCase = assembly->mLoadCaseList->at(loadCaseNo);

    loadCase->P = 0.0; // N/mm2
    loadCase->mForce->F_Z = 0;
    loadCase->mForce->M_AI = 0;

    //loadCase.E0 = 200000
    //loadCase.gC = 1
    //loadCase.K1 = 0
    //loadCase.mi = 2.0
    //loadCase.Qmin = 200
    //loadCase.Qsmax = 550

    loadCase->alphaB = 0.0000111; // bolt thermal expansion at temperature
    loadCase->alphaG = 0.0000103; // thermal gasket expansion at temperature
    loadCase->alphaF1 = 0.0000115;
    loadCase->alphaF2 = loadCase->alphaF1;

    loadCase->EB = 211000.0; // Elasticity moduly except gasket, steel is 200 GPa
    loadCase->EF1 = 200021.0;
    loadCase->EF2 = loadCase->EF1;
    loadCase->EL1 = 212000.0; // for Y values
    loadCase->EL2 = loadCase->EL1;
    //loadCase.EW1 = 212000.0 ' for Y values
    //loadCase.EW2 = loadCase.EW1

    loadCase->fB = 300;
    loadCase->fF1 = 266.667;
    loadCase->fF2 = loadCase->fF1;
    loadCase->fL1 = 285.0;
    loadCase->fL2 = 285.0;
    loadCase->fN = 300.0;
    loadCase->fS1 = 250.0;
    loadCase->fS2 = loadCase->fS1;
    //loadCase.fW1 = 285.0
    //loadCase.fW2 = 285.0

    loadCase->T0 = 20; // Celsius
    loadCase->TB = 20; // Celsius
    loadCase->TG = 20;
    loadCase->TF1 = 20;
    loadCase->TF2 = 20;
    //loadCase.TL1 = 20
    //loadCase.TL2 = 20

    //loadCase.Q_R = 320

    //
    // Load case 1, test pressure only
    //
    loadCaseNo = 1;
    LoadCase *loadCase1 = assembly->mLoadCaseList->at(loadCaseNo);
    loadCase1->setInputData(loadCase);
    loadCase1->P = 30.0; // N/mm2
    loadCase1->mForce->F_Z = 0;
    loadCase1->mForce->M_AI = 0;

    loadCase1->T0 = 250; // Celsius
    loadCase1->TB = 250; // Celsius
    loadCase1->TG = 250;
    loadCase1->TF1 = 250;
    loadCase1->TF2 = 250;

}

void Calculator::exec() {

    Assembly* assembly = this->mAssembly;
//    mReport->ToBeCreated = true;


    //CreateTargetLoose(assembly)
    //SetInputData_Ohmtech(assembly)
    //SetLoadCases_Ohmtech(assembly)

    //CreateTargetIntegral(assembly)
    //SetInputData_CR13642_B4(assembly)
    //SetLoadCases_CR13642_B4(assembly)

    //CreateTargetIntegral(assembly) ' last used
    //SetInputData_Ambrosio(assembly)
    //SetLoadCases_Ambrosio(assembly)

    //CreateTargetLoose(assembly) ' screwed
    //SetInputData_Plantware(assembly)
    //SetLoadCases_Plantware(assembly)

    mWriteCalcFromNumber = 4; // -1 is all
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

    while (! (isFG0largerFG0req && isFG0approximateFG0req) && counter < 10) {
        // Outside loop
        Loop_F55_to_108(assembly);

        // Formulae 109 and 110
        isFG0largerFG0req = assembly->Is_F_G0_larger_F_G0req();
        isFG0approximateFG0req = assembly->Is_F_G0act_within_0_1_percent_of_F_G0req();

        if (! (isFG0largerFG0req && isFG0approximateFG0req))
        {

            // TODO: is this setting of higher load required for convergence?
            if (isFG0largerFG0req)             {
                assembly->mLoadCaseList->at(0)->F_G
                        = assembly->mLoadCaseList->at(0)->F_Greq;
            } else {
                assembly->mLoadCaseList->at(0)->F_G
                        = assembly->mLoadCaseList->at(0)->F_Greq * 1.1;
            }

            PR->addDetail("After F.108", "F_G", "F_Greq + F_R",
                          assembly->mLoadCaseList->at(0)->F_G, "N");
        }

        mIsFirstApproximation = true;
        counter += 1;
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
        F90_to_104(assembly, loadCaseNo);
    }

    for (loadCaseNo = 1; loadCaseNo < mLoadCaseCount; loadCaseNo++) {
        F105_to_105(assembly, loadCaseNo);
    }

    F106_to_108(assembly);
}

void Calculator::F3_to_24(Assembly* assembly) {
    assembly->mFlange1->Calc_eQ();
    assembly->mFlange2->Calc_eQ();

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
    assembly->mFlange1->Calc_AF();
    assembly->mFlange2->Calc_AF();
    assembly->mFlange1->Calc_bL();
    assembly->mFlange2->Calc_bL();
    assembly->mFlange1->Calc_dL();
    assembly->mFlange2->Calc_dL();
    assembly->mFlange1->Calc_AL();
    assembly->mFlange2->Calc_AL();
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
    assembly->Calc_F_GInitial();
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
        assembly->Calc_Q_G();
        assembly->mGasket->Calc_E_G(0);
    }
    // HACK: End

    assembly->mGasket->Calc_Q_smax(loadCaseNo);

    assembly->Calc_bGi(loadCaseNo, mIsFirstApproximation);
    assembly->mGasket->Calc_bGe(loadCaseNo);
    assembly->Calc_dGe();
    assembly->mGasket->Calc_AGe();
    assembly->Calc_Q_G();
    assembly->mGasket->Calc_E_G(loadCaseNo);

    assembly->mFlange1->Calc_d7min(); // loose flange only
    assembly->mFlange2->Calc_d7min(); // loose flange only
    assembly->mFlange1->Calc_d7max(); // loose flange only
    assembly->mFlange2->Calc_d7max(); // loose flange only
    assembly->mFlange1->Calc_chi(loadCaseNo); // loose flange only
    assembly->mFlange2->Calc_chi(loadCaseNo); // loose flange only
    assembly->mFlange1->Calc_d70(loadCaseNo); // loose flange only
    assembly->mFlange2->Calc_d70(loadCaseNo); // loose flange only
    assembly->mFlange1->Calc_hG();
    assembly->mFlange2->Calc_hG();
}

void Calculator::F63_to_63(Assembly* assembly, int loadCaseNo) {
    assembly->mGasket->Calc_eG(loadCaseNo);
    assembly->mGasket->Calc_XG(loadCaseNo);
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

void Calculator::F90_to_104(Assembly* assembly, int loadCaseNo) {
    assembly->mGasket->Calc_AQ();
    assembly->Calc_F_Q(loadCaseNo);
    assembly->mLoadCaseList->at(loadCaseNo)->mForce->Calc_F_LI();
    assembly->mLoadCaseList->at(loadCaseNo)->mForce->Calc_M_AI();
    assembly->Calc_F_R(loadCaseNo);
//    assembly->Calc_lB(); already before Formula 42
    assembly->Calc_dUI(loadCaseNo);
    assembly->Calc_Q_G(); // for determining EG Formula 100
    assembly->mGasket->Calc_E_G(loadCaseNo);
    assembly->Calc_YB(loadCaseNo);
    assembly->Calc_YG(loadCaseNo);
    assembly->Calc_YQ(loadCaseNo);
    assembly->Calc_YR(loadCaseNo);
    assembly->Calc_Q_A_Qsmin(loadCaseNo);
    assembly->Calc_F_Gmin(loadCaseNo);
}

void Calculator::F105_to_105(Assembly* assembly,
                             int loadCaseNo) {
    assembly->mGasket->Calc_P_QR(loadCaseNo);
    assembly->Calc_delta_eGc(loadCaseNo);
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
    assembly->Calc_F_G0d();
}

void Calculator::F120_to_122(Assembly* assembly,
                             int loadCaseNo) {
    assembly->Calc_F_G(loadCaseNo);
    assembly->Calc_F_B(loadCaseNo);
}

void Calculator::F123_to_151(Assembly* assembly,
                             int loadCaseNo) {
    // bolt load
    assembly->Calc_MtB();
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

void Calculator::FC1_to_C10(Assembly* assembly,
                            int loadCaseNo) {

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

END_NAMESPACE_REDBAG_CALC_EN1591
