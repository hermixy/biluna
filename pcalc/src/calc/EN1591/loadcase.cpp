#include "loadcase.h"
NAMESPACE_BILUNA_CALC_EN1591


LoadCase_IN::LoadCase_IN() : RB_Object() {
    setName("PCALC EN1591 LoadCase");

    mForce = NULL; // created from outside
    alphaB = 0;
    alphaG = 0;
    alphaF1 = 0;
    alphaF2 = 0;
    alphaL1 = 0;
    alphaL2 = 0;
    alphaW1 = 0;
    alphaW2 = 0;
    EB = 0;
    EF1 = 0;
    EF2 = 0;
    EL1 = 0;
    EL2 = 0;
    EW1 = 0;
    EW2 = 0;

    // input by user, bolt load specified by user
    F_Bspec = 0.0;

    fB = 0;
    fF1 = 0;
    fF2 = 0;
    fL1 = 0;
    fL2 = 0;
    fN = 0;
    fS1 = 0;
    fS2 = 0;
    fW1 = 0;
    fW2 = 0;

    muG = 0;
    P = 0;

    T0 = 0;
    TB = 0;
    TG = 0;
    TF1 = 0;
    TF2 = 0;
    TL1 = 0;
    TL2 = 0;
    TW1 = 0;
    TW2 = 0;
}

LoadCase_IN::~LoadCase_IN() {
    delete mForce;
    mForce = NULL;
}

LoadCase_OUT::LoadCase_OUT() : LoadCase_IN() {
    EGm = 0.0;
    E_G = 0.0;

    x = 0.0;
    F_R = 0.0;
    F_Rnegative = 0.0;
    F_Rpositive = 0.0;
    F_Q = 0.0;
    dUI = 0.0;

    P_QR = 0.0;

    // gasket creep delta ecG in Fomula 105, 106 and 120, 121 and C.8, C.9
    delta_eGc = 0.0;
    delta_eGc_EN13555 = 0.0;

    eG = 0.0;
    F_B = 0.0;
    F_Bav = 0.0;
    F_Bcalc = 0.0;
    F_Bmax = 0.0;
    F_BmaxA = 0.0;
    F_Bmin = 0.0;
    F_BminA = 0.0;
    F_Bnom = 0.0;
    F_Breq = 0.0;

    F_G = 0.0;
    F_Gd = 0.0; // design
    F_Gdelta = 0.0;
    F_Gmax = 0.0;
    F_GmaxA = 0.0;
    F_Gmin = 0.0;
    F_GminA = 0.0;
    F_Greq = 0.0;

    DGm = 0.0;
    Y_B = 0.0;
    Y_G = 0.0;
    Y_Q = 0.0;
    Y_R = 0.0;

    // Bolt and washer
    cA = 0.0;
    cB = 0.0;
    PhiB = 0.0;
    fCF1 = 0.0;
    fCF2 = 0.0;
    PhiC1 = 0.0;
    PhiC2 = 0.0;
    fCB1 = 0.0;
    fCB2 = 0.0;
    Sw1 = 0.0;
    Sw2 = 0.0;
    PhiCF1 = 0.0;
    PhiCF2 = 0.0;
    PhiCB1 = 0.0;
    PhiCB2 = 0.0;

    // Gasket
    PhiG = 0.0;
    cG = 0.0;

    // Flange
    fE1 = 0.0;
    fE2 = 0.0;
    deltaQ1 = 0.0;
    deltaQ2 = 0.0;
    deltaR1 = 0.0;
    deltaR2 = 0.0;
    Psi01 = 0.0;
    Psi02 = 0.0;
    PsiMax1 = 0.0;
    PsiMax2 = 0.0;
    PsiMin1 = 0.0;
    PsiMin2 = 0.0;
    PsiOpt1 = 0.0;
    PsiOpt2 = 0.0;
    Psi_jkk1 = 0.0;
    Psi_jkk2 = 0.0;
    PsiZ1 = 0.0;
    PsiZ2 = 0.0;
    cM1 = 0.0;
    cM2 = 0.0;
    cS_minus1 = 0.0;
    cS_minus2 = 0.0;
    cS_plus1 = 0.0;
    cS_plus2 = 0.0;
    jM1 = 0;
    jM2 = 0;
    jS = 0;
    kM1 = 0.0;
    kM2 = 0.0;
    kS = 0;
    WF1 = 0.0;
    WF2 = 0.0;
    PhiF1 = 0.0;
    PhiF2 = 0.0;
    WX1 = 0.0;
    WX2 = 0.0;
    PhiX1 = 0.0;
    PhiX2 = 0.0;
    WL1 = 0.0;
    WL2 = 0.0;
    PhiL1 = 0.0;
    PhiL2 = 0.0;
    WQ1 = 0.0;
    WQ2 = 0.0;
    MtBnom = 0.0;
    Mtnom = 0.0;
    Q_G = 0.0;
    Q_GmaxA = 0.0; // Annex C
    Q_GminA = 0.0; // Annex C
    Q_A = 0.0;
    Q_0min = 0.0;
    Q_minL = 0.0;
    Q_sminL = 0.0;
    Q_smax = 0.0;
    ThetaF1min = 0.0;
    ThetaF1max = 0.0;
    ThetaF2min = 0.0;
    ThetaF2max = 0.0;
    ThetaL1min = 0.0;
    ThetaL1max = 0.0;
    ThetaL2min = 0.0;
    ThetaL2max = 0.0;

}

void LoadCase::init() {
    // TODO set based on bolt material
    // and load case temperature, required?
    fB = 0.0;
}

void LoadCase::setInputData(LoadCase *loadCaseIn) {
    if (mForce) {
        mForce->F_X = loadCaseIn->mForce->F_X;
        mForce->F_Y = loadCaseIn->mForce->F_Y;
        mForce->F_Z = loadCaseIn->mForce->F_Z;

        mForce->M_X = loadCaseIn->mForce->M_X;
        mForce->M_Y = loadCaseIn->mForce->M_Y;
        mForce->M_Z = loadCaseIn->mForce->M_Z;
    }

    alphaB = loadCaseIn->alphaB;
    alphaG = loadCaseIn->alphaG;
    alphaF1 = loadCaseIn->alphaF1;
    alphaF2 = loadCaseIn->alphaF2;
    alphaL1 = loadCaseIn->alphaL1;
    alphaL2 = loadCaseIn->alphaL2;
    alphaW1 = loadCaseIn->alphaW1;
    alphaW2 = loadCaseIn->alphaW2;

    EB = loadCaseIn->EB;
    EF1 = loadCaseIn->EF1;
    EF2 = loadCaseIn->EF2;
    EL1 = loadCaseIn->EL1;
    EL2 = loadCaseIn->EL2;
    EW1 = loadCaseIn->EW1;
    EW2 = loadCaseIn->EW2;

    F_Bspec = loadCaseIn->F_Bspec;

    fB = loadCaseIn->fB;
    fF1 = loadCaseIn->fF1;
    fF2 = loadCaseIn->fF2;
    fL1 = loadCaseIn->fL1;
    fL2 = loadCaseIn->fL2;
    fN = loadCaseIn->fN;
    fS1 = loadCaseIn->fS1;
    fS2 = loadCaseIn->fS2;
    fW1 = loadCaseIn->fW1;
    fW2 = loadCaseIn->fW2;

    muG = loadCaseIn->muG;
    P = loadCaseIn->P;

    // Temperatures
    T0 = loadCaseIn->T0;
    TB = loadCaseIn->TB;
    TG = loadCaseIn->TG;
    TF1 = loadCaseIn->TF1;
    TF2 = loadCaseIn->TF2;
    TL1 = loadCaseIn->TL1;
    TL2 = loadCaseIn->TL2;
    TW1 = loadCaseIn->TW1;
    TW2 = loadCaseIn->TW2;
}

END_NAMESPACE_BILUNA_CALC_EN1591
