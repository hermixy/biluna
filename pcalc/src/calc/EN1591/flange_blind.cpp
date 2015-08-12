#include "flange_blind.h"
NAMESPACE_REDBAG_CALC_EN1591


Flange_Blind::Flange_Blind(int flangeNo) : Flange(flangeNo) {
    setName("PCALC EN1591 Flange Blind");
}

Flange_Blind::~Flange_Blind() {
    // no shell
    delete mWasher;
    mWasher = NULL;
}

/**
 * @brief according para 4.2: eF/bF, eL/bL
 * @return
 */
bool Flange_Blind::Is_flange_Valid() {
    if (0.2 <= bF / eF && bF / eF <= 5.0) {
        PR->addDetail("Para 4.2", "result", "Is flange valid?", 1, "-");
        return true;
    }

    PR->addDetail("Para 4.2", "result", "Is flange valid?", 0, "-");
    return false;
}

/**
 * @brief Formula 7: Effective width of flange
 */
void Flange_Blind::Calc_bF() {
    bF = (d4 - d0) / 2 - d5e;
    PR->addDetail("Formula 7", "bF", "(d4 - d0) / 2 - d5e", bF, "mm");
}

/**
 * @brief Formula 9: Average diameter of flange
 */
void Flange_Blind::Calc_dF() {
    dF = (d4 + d0) / 2;
    PR->addDetail("Formula 9", "dF", "(d4 + d0) / 2", dF, "mm");
}

/**
 * @brief Formula 10: Area for effective axial thickness of flange
 */
void Flange_Blind::Calc_AF() {
    AF = eF * (d4 - d0) / 2;
    PR->addDetail("Formula 10", "AF", "(d4 - d0) * eF / 2", AF, "mm^2");
}

/**
 * @brief Formula 8: Effective width loose flange, default is 0.0
 */
void Flange_Blind::Calc_bL() {
    bL = 0;
    PR->addDetail("Formula 8", "bL", "0", bL, "mm");
}

/**
 * @brief Formula 8: Average diameter loose flange, default is 0.0
 */
void Flange_Blind::Calc_dL() {
    dL = 0;
    PR->addDetail("Formula 8", "dL", "0", dL, "mm");
}

/**
 * @brief Formula 8: Effective axial thickness loose flange, default is 0.0
 */
void Flange_Blind::Calc_AL() {
    AL = 0;
    PR->addDetail("Formula 8", "AL", "0", AL, "mm");
}

/**
 * @brief Formula 23: Wall thickness of equivalent cylinder
 * for load limit and flexibility calculations
 */
void Flange_Blind::Calc_eE() {
    eE = 0;
    PR->addDetail("Formula 23", "eE", "0", eE, "mm");
}

/**
 * @brief Formula 24: Average diameter of equivalent
 * or effective cylinder, gasket pressure
 */
void Flange_Blind::Calc_dE() {
    dE = d0;
    PR->addDetail("Formula 24", "dE", "d0", dE, "mm");
}

/**
 * @brief Formula 36: Intermediate working variable rho
 */
void Flange_Blind::Calc_rho() {
    rho = d9 / dE;
    PR->addDetail("Formula 36", "rho", "d9 / dE", rho, "-");
}

/**
 * @brief Formula 80: Lever arm correction hQ
 * @param loadCaseNo
 */
void Flange_Blind::Calc_hQ(int loadCaseNo) {
    double tmpdGe = mLoadCaseList->at(loadCaseNo)->dGe;
    hQ = (dE * (1 - pow(rho, 2)) / 8) * ((0.7 + 3.3 * pow(rho, 2))
                                         / (0.7 + 1.3 * pow(rho, 2)))
            * pow(dE / tmpdGe, 2);
    PR->addDetail("Formula 80",
              "hQ", "(dE * (1 - rho ^ 2) / 8) * ((0.7 + 3.3 * rho ^ 2) "
              "/ (0.7 + 1.3 * rho ^ 2)) * (dE / dGe) ^ 2", hQ, "mm");
}

/**
 * @brief Formula 37: Lever arm correction hR
 */
void Flange_Blind::Calc_hR() {
    hR = (dE * (1 - pow(rho, 2)) / (4 * (1 + pow(rho, 2))))
            * ((0.7 + 3.3 * pow(rho, 2)) / (0.7 + 1.3 * pow(rho, 2)));
    PR->addDetail("Formula 37",
              "hR", "(dE * (1 - rho ^ 2) / (4 * (1 + rho ^ 2))) "
              "* ((0.7 + 3.3 * rho ^ 2) / (0.7 + 1.3 * rho ^ 2))", hR, "mm");
}

/**
 * @brief Formula 38: Rotation flexibility modulus flange
 */
void Flange_Blind::Calc_ZF() {
    ZF = 3 * dF / (M_PI * (bF * pow(eF, 3) + dF * pow(e0, 3) * (1 - rho * rho)
                                            / (1.4 + 2.6 * pow(rho, 2))));
    PR->addDetail("Formula 38",
              "ZF", "3 * dF / (Math.PI * (bF * eF ^ 3 + dF * e0 ^ 3 "
              "* (1 - rho * rho) / (1.4 + 2.6 * rho ^ 2)))", ZF, "1/mm^2");
}

/**
 * @brief Formula 39: Rotation flexibility modulus loose flange, default is 0.0
 */
void Flange_Blind::Calc_ZL() {
    ZL = 0;
    PR->addDetail("Formula 39", "ZL", "0", ZL, "1/mm^2");
}

/**
 * @brief Formula 78 and 77: Lever arm correction pressure (P), default = 0.0
 * @param loadCaseNo
 */
void Flange_Blind::Calc_hP(int loadCaseNo) {
    eP = 0.0;
    PR->addDetail("Formula 78", "eP", "0", eP, "1/mm^2");
    Flange::Calc_hP(loadCaseNo);
}

/**
 * @brief Formula 59 and 81: Gasket lever arm for all load cases
 * @param loadCaseNo
 */
void Flange_Blind::Calc_hG(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (getFlangeNumber() == 1) {
        loadCase->hG1 = (d3e - loadCase->dGe) / 2;
        PR->addDetail("Formula 59, 81",
                  "hG1", "(d3e - dGe) / 2", loadCase->hG1, "mm");
    } else {
        loadCase->hG2 = (d3e - loadCase->dGe) / 2;
        PR->addDetail("Formula 59, 81",
                  "hG2", "(d3e - dGe) / 2", loadCase->hG2, "mm");
    }
}

/**
 * @brief Formula 82: Hub lever arm for all load case
 * @param loadCaseNo
 */
void Flange_Blind::Calc_hH(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (getFlangeNumber() == 1) {
        loadCase->hH1 = (d3e - dE) / 2;
        PR->addDetail("Formula 82", "hH1", "(d3e - dE) / 2", loadCase->hH1, "mm");
    } else {
        loadCase->hH2 = (d3e - dE) / 2;
        PR->addDetail("Formula 82", "hH2", "(d3e - dE) / 2", loadCase->hH2, "mm");
    }
}

/**
 * @brief Formula 83: Loose flange lever arm for all load cases sets hL1 = 0.0
 * @param loadCaseNo
 */
void Flange_Blind::Calc_hL(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (getFlangeNumber() == 1) {
        loadCase->hL1 = 0.0;
        PR->addDetail("Formula 83", "hL1", "0.0", loadCase->hL1, "mm");
    } else {
        loadCase->hL2 = 0.0;
        PR->addDetail("Formula 83", "hL2", "0.0", loadCase->hL2, "mm");
    }
}

/**
 * @brief Legend with d0: Inside diameter of gasket larger then
 * outside central part of blind flange
 * @return
 */
bool Flange_Blind::Is_d0_Valid() {
    bool result = d0 <= this->mGasket->dG1;
    PR->addDetail("Legend with d0", "result", "d0 <= mGasket->dG1",
              static_cast<int>(result), "-");
    return result;
}

/**
 * @brief Formula 146: Resistance flange
 * @param loadCaseNo
 */
void Flange_Blind::Calc_WF(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_fF = loadCase->fF1;

    if (getFlangeNumber() == 2)     {
        tmp_fF = loadCase->fF2;
    }

    double tmp_WF = 0;
    tmp_WF = M_PI / 4 * tmp_fF * (2 * bF * pow(eF, 2)
                                  + d0 * (1 - rho) * pow(e0, 2));

    if (getFlangeNumber() == 1) {
        loadCase->WF1 = tmp_WF;
        PR->addDetail("Formula 146",
                  "WF1", "PI / 4 * fF1 * (2 * bF * eF ^ 2 "
                  "+ d0 * (1 - rho) * e0 ^ 2)", loadCase->WF1, "Nmm");
    } else if (getFlangeNumber() == 2) {
        loadCase->WF2 = tmp_WF;
        PR->addDetail("Formula 146",
                  "WF2", "PI / 4 * fF2 * (2 * bF * eF ^ 2 "
                  "+ d0 * (1 - rho) * e0 ^ 2)", loadCase->WF2, "Nmm");
    }
}

/**
 * @brief Formula 145: Load ratio flange
 * @param loadCaseNo
 */
void Flange_Blind::Calc_PhiF(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_hG = loadCase->hG1;
    double tmp_WF = loadCase->WF1;

    if (getFlangeNumber() == 2)
    {
        tmp_hG = loadCase->hG2;
        tmp_WF = loadCase->WF2;
    }

    double tmpVal1 = loadCase->F_B * tmp_hG + loadCase->F_Q *(1 - pow(rho, 3))
            * loadCase->dGe / 6;
    double tmpVal2 = loadCase->F_R *(1 - rho) * loadCase->dGe / 2;
    double tmp_PhiF = std::max(fabs(tmpVal1 + tmpVal2),
                        std::max(fabs(tmpVal1), fabs(tmpVal2))) / tmp_WF;
    if (getFlangeNumber() == 1) {
        loadCase->PhiF1 = tmp_PhiF;
        // PR->addDetail("Formula 146", "WF1", "...", WF1, "-");
        PR->addDetail("Formula 145", "PhiF1", "...", loadCase->PhiF1, "-");
    } else if (getFlangeNumber() == 2) {
        loadCase->PhiF2 = tmp_PhiF;
        // PR->addDetail("Formula 146", ".WF2 = ...", loadCase->WF2, "-");
        PR->addDetail("Formula 145", "PhiF2", "...", loadCase->PhiF2, "-");
    }
}

/**
 * @brief Formula 147: Load ratio at reduced flange thickness due to groove
 * @param loadCaseNo
 */
void Flange_Blind::Calc_PhiX(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (getFlangeNumber() == 1) {
        loadCase->PhiX1 = loadCase->F_B * (d3 - dX) / (2 * loadCase->WX1);
        PR->addDetail("Formula 147", "PhiX1", "F_B * (d3 - dX) / (2 * WX1)",
                  loadCase->PhiX1, "-");
    } else if (getFlangeNumber() == 2) {
        loadCase->PhiX2 = loadCase->F_B * (d3 - dX) / (2 * loadCase->WX2);
        PR->addDetail("Formula 147", "PhiX2", "F_B * (d3 - dX) / (2 * WX2)",
                  loadCase->PhiX2, "-");
    }
}

/**
 * @brief With Formula 147: PhiX assessment
 * @param loadCaseNo
 * @return
 */
bool Flange_Blind::Is_PhiX_Valid(int loadCaseNo) {
    bool result = false;
    if (getFlangeNumber() == 1) {
        result = mLoadCaseList->at(loadCaseNo)->PhiX1 <= 1.0;
        PR->addDetail("With Formula 147", "result", "PhiX1 <= 1.0",
                  static_cast<int>(result), "-");
        return result;
    }

    result = mLoadCaseList->at(loadCaseNo)->PhiX2 <= 1.0;
    PR->addDetail("With Formula 147", "result", "PhiX2 <= 1.0",
              static_cast<int>(result), "-");
    return result;
}

/**
 * @brief Formula 148: Resistance at reduced flange thickness due to groove
 * @param loadCaseNo
 */
void Flange_Blind::Calc_WX(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_fF = loadCase->fF1;

    if (getFlangeNumber() == 2) {
        tmp_fF = loadCase->fF2;
    }

    double tmp_WX = M_PI / 4 * tmp_fF *((d4 - 2 *d5e - dX)
                                        * pow(eF, 2) + dX * pow(eX, 2));

    if (getFlangeNumber() == 1)     {
        loadCase->WX1 = tmp_WX;
        PR->addDetail("Formula 148",
                  "WX1", "PI / 4 * fF1 * ((d4 - 2 * d5e - dX) "
                  "* eF ^ 2 + dX * eX ^ 2)",
                  loadCase->WX1, "Nmm");
    }
    else if (getFlangeNumber() == 2)
    {
        loadCase->WX2 = tmp_WX;
        PR->addDetail("Formula 148",
                  "WX2", "PI / 4 * fF2 * ((d4 - 2 * d5e - dX) "
                  "* eF ^ 2 + dX * eX ^ 2)",
                  loadCase->WX2, "Nmm");
    }
}

void Flange_Blind::Calc_cM(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

bool Flange_Blind::Is_cM_valid(int /*loadCaseNo*/) {
    return false;
}

void Flange_Blind::Calc_cS(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

bool Flange_Blind::Is_cS_valid(int /*loadCaseNo*/) {
    return false;
}

void Flange_Blind::Calc_jM(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

void Flange_Blind::Calc_PsiOpt(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

void Flange_Blind::Calc_Psi0(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

void Flange_Blind::Calc_PsiMax(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

void Flange_Blind::Calc_PsiMin(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

void Flange_Blind::Calc_PsiZ(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

bool Flange_Blind::Is_PsiMaxMin_Valid(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
    return false;
}

void Flange_Blind::Calc_kM_for_WFmax(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

void Flange_Blind::Calc_deltaQ(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only Formula GA.7-28
}

void Flange_Blind::Calc_deltaR(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only Formula GA.7-29
}

END_NAMESPACE_REDBAG_CALC_EN1591
