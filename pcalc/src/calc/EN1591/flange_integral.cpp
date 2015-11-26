#include "flange_integral.h"
NAMESPACE_BILUNA_CALC_EN1591


Flange_Integral::Flange_Integral(int flangeNo) : Flange(flangeNo) {
    setName("PCALC EN1591 Flange Integral");
}

Flange_Integral::~Flange_Integral() {
    // shell, washer deleted in Flange
}

/**
 * @brief according para 4.2 eF/bF, eL/bL and cos(phiS)
 * @return true if flange is valid
 */
bool Flange_Integral::Is_flange_Valid() {
    bool result = 0.2 <= bF / eF && bF / eF <= 5.0;
    QString str = "0.2 <= bF / eF AND bF / eF <= 5.0";
    QString strVal = "0.2 <= " + QN(bF) + " / " + QN(eF) + " AND "
            + QN(bF) + " / " + QN(eF) + " <= 5.0";

    if (result) {
        if (mShell != NULL) {
            if (cos(mShell->phiS) >= 1 / (1 + 0.01 * mShell->dS / mShell->eS)) {
                PR->addDetail("Para 4.2", "result", str
                              + " AND cos(phiS) >= 1 / (1 + 0.01 * dS / eS)",
                              1, "-", strVal + " AND cos(" + QN(mShell->phiS)
                              + ") >= 1 / (1 + 0.01 * " + QN(mShell->dS)
                              + " / " + QN(mShell->eS) + ")", 0);
                return true;
            }
        } else {
            PR->addDetail("Para 4.2", "result(" + QN(mFlangeNumber) + ")",
                          str, 1, "-", strVal, 0);
            return true;
        }
    }

    PR->addDetail("Para 4.2", "result(" + QN(mFlangeNumber) + ")",
                  str, 0, "-", strVal, 0);
    return false;
}

/**
 * @brief Formula 7: Effective width of flange
 */
void Flange_Integral::Calc_bF() {
    bF = (d4 - d0) / 2 - d5e;
    PR->addDetail("Formula 7", "bF(" + QN(mFlangeNumber) + ")",
                  "(d4 - d0) / 2 - d5e", bF, "mm",
                  "(" + QN(d4) + " - " + QN(d0) + ") / 2 - " + QN(d5e), 0);
}

/**
 * @brief Formula 9: Average diameter of flange
 */
void Flange_Integral::Calc_dF() {
    dF = (d4 + d0) / 2;
    PR->addDetail("Formula 9", "dF(" + QN(mFlangeNumber) + ")",
                  "(d4 + d0) / 2", dF, "mm",
                  "(" + QN(d4) + " + " + QN(d0) + ") / 2", 0);
}

/**
 * @brief Formula 10: Area for effective axial thickness of flange
 */
void Flange_Integral::Calc_AF() {
    AF = (d4 - d0) * eF / 2;
    PR->addDetail("Formula 10", "AF(" + QN(mFlangeNumber) + ")",
                  "(d4 - d0) * eF / 2", AF, "mm^2",
                  "(" + QN(d4) + " - " + QN(d0) + ") * " + QN(eF) + " / 2", 0);
}

/**
 * @brief Formula 8: Effective width loose flange, default is 0.0
 */
void Flange_Integral::Calc_bL() {
    bL = 0;
    PR->addDetail("Formula 8", "bL(" + QN(mFlangeNumber) + ")",
                  "0", bL, "mm", "0", 0);
}

/**
 * @brief Formula 8: Average diameter loose flange, default is 0.0
 */
void Flange_Integral::Calc_dL() {
    dL = 0;
    PR->addDetail("Formula 8", "dL(" + QN(mFlangeNumber) + ")",
                  "0", dL, "mm", "0", 0);
}

/**
 * @brief Formula 8: Effective axial thickness loose flange, default is 0.0
 */
void Flange_Integral::Calc_AL() {
    AL = 0;
    PR->addDetail("Formula 8", "AL(" + QN(mFlangeNumber) + ")",
                  "0", AL, "mm", "0", 0);
}

/**
 * @brief Formula 19: Intermediate working variable for calulation
 * of the wall thickness  of equivalent cylinder for load limit and flexibility
 */
void Flange_Integral::Calc_beta() {
    beta = e2 / e1;
    PR->addDetail("Formula 19", "beta(" + QN(mFlangeNumber) + ")",
                  "e2 / e1", beta, "-", QN(e2) + " / " + QN(e1), 0);
}

/**
 * @brief Formula 18: Wall thickness of equivalent cylinder for load limit
 * and flexibility calculations
 */
void Flange_Integral::Calc_eD() {
    eD = e1 * (1 + (beta - 1) * lH / pow((pow(beta / 3, 4))
                                         * pow(d1 * e1, 2) + pow(lH, 4), 0.25));
    PR->addDetail("Formula 18", "eD(" + QN(mFlangeNumber) + ")",
                  "e1 * (1 + (beta - 1) * lH / ((beta / 3) ^ 4 *"
                  " ((d1 * e1) ^ 2) + lH ^ 4, 0.25))", eD, "mm",
                  QN(e1) + " * (1 + (" + QN(beta) + " - 1) * " + QN(lH)
                  + " / (((" + QN(beta) + " / 3) ^ 4) * (" + QN(d1) + " * "
                  + QN(e1) + ") ^ 2 + " + QN(lH)  + " ^ 4) ^ 0.25)", 0);
}

/**
 * @brief Formula 17 and 21: Wall thickness of equivalent cylinder
 * for load limit and flexibility calculations
 */
void Flange_Integral::Calc_eE() {
    if (fabs(e2 - e1) < ACCURACY) {
        // No hub because wallthickness hub is same as connecting pipe
        eE = mShell->eS;
        PR->addDetail("Formula 21", "eE(" + QN(mFlangeNumber) + ")",
                      "eS", eE, "mm", QN(mShell->eS), 0);
    } else {
        // Tapered hub
        eE = e1 * (1 + ((beta - 1) * lH / ((beta / 3) * sqrt(e1 * d1) + lH)));
        PR->addDetail("Formula 17", "eE(" + QN(mFlangeNumber) + ")",
                      "e1 * (1 + ((beta - 1) * lH / ((beta / 3) "
                      "* Math.Sqrt(e1 * d1) + lH)))", eE, "mm",
                      QN(e1) + " * (1 + ((" + QN(beta) + " - 1) * " + QN(lH)
                      + " / ((" + QN(beta) + " / 3) * (" + QN(e1) + " * "
                      + QN(d1) + ") ^ 0.5 + " + QN(lH) + ")))", 0);
    }
}

/**
 * @brief Formula 20 and 22: Average diameter of equivalent
 * or effective cylinder, gasket pressure
 */
void Flange_Integral::Calc_dE() {
    if (fabs(e2 - e1) < ACCURACY) {
        // No hub because wallthickness hub is same as connecting pipe
        dE = mShell->dS;
        PR->addDetail("Formula 22", "dE(" + QN(mFlangeNumber) + ")",
                      "dS", dE, "-", QN(mShell->dS), 0);
    } else {
        // Tapered hub
        dE = 0.5 * (std::min(d1 - e1 + eE, d2 + e2 - eE)
                    + std::max(d1 + e1 - eE, d2 - e2 + eE));
        PR->addDetail("Formula 20", "dE(" + QN(mFlangeNumber) + ")",
                      "0.5 * (Min(d1 - e1 + eE, d2 + e2 - eE) "
                      "+ Math.Max(d1 + e1 - eE, d2 - e2 + eE))", dE, "-",
                      "0.5 * (min(" + QN(d1) + " - " + QN(e1) + " + "
                      + QN(eE) + ", " + QN(d2) + " + " + QN(e2) + " - "
                      + QN(eE) + ") + max(" + QN(d1) + " + " + QN(e1) + " - "
                      + QN(eE) + ", " + QN(d2) + " - " + QN(e2) + " + "
                      + QN(eE) + "))", 0);
    }
}

/**
 * @brief Formula 25: Intermediate working variable gamma
 */
void Flange_Integral::Calc_gamma() {
    gamma = eE * dF / (bF * dE * cos(mShell->phiS));
    PR->addDetail("Formula 25", "gamma(" + QN(mFlangeNumber) + ")",
                  "eE * dF / (bF * dE * cos(phiS))", gamma, "-",
                  QN(eE) + " * " + QN(dF) + " / (" + QN(bF) + " * "
                  + QN(dE) + " * cos(" + QN(mShell->phiS) + "))", 0);
}

/**
 * @brief Formula 26: Intermediate working variable theta
 */
void Flange_Integral::Calc_theta() {
    theta = 0.55 * cos(mShell->phiS) * sqrt(dE * eE) / eF;
    PR->addDetail("Formula 26", "theta(" + QN(mFlangeNumber) + ")",
                  "0.55 * cos(phiS) * (dE * eE) ^ 0.5 / eF",
                  theta, "-",
                  "0.55 * cos(" + QN(mShell->phiS) + ") * ("
                  + QN(dE) + " * " + QN(eE) + ") ^ 0.5 / " + QN(eF), 0);
}

/**
 * @brief Formula 27: Intermediate working variable lambda
 */
void Flange_Integral::Calc_lambda() {
    lambda = 1 - eP / eF;
    PR->addDetail("Formula 27", "lambda(" + QN(mFlangeNumber) + ")",
                  "1 - eP / eF", lambda, "-",
                  "1 - " + QN(eP) + " / " + QN(eF), 0);
}

/**
 * @brief Formula 28: Correction factor flange
 */
void Flange_Integral::Calc_cF() {
    cF = (1 + gamma * theta)
            / (1 + gamma * theta
               * (4 * (1 - 3 * lambda + 3 * pow(lambda, 2))
                  + 6 * (1 - 2 * lambda) * theta + 6 * pow(theta, 2))
               + 3 * pow(gamma, 2) * pow(theta, 4));
    PR->addDetail("Formula 28", "cF(" + QN(mFlangeNumber) + ")",
                  "(1 + gamma * theta) / (1 + gamma * theta * "
                  "(4 * (1 - 3 * lambda + 3 * (lambda ^ 2)) "
                  "+ 6 * (1 - 2 * lambda) * theta + 6 * theta ^ 2) "
                  "+ 3 * (gamma ^ 2) * (theta ^ 4))", cF, "-",
                  "(1 + " + QN(gamma) + " * " + QN(theta) + ") / (1 + "
                  + QN(gamma) + " * " + QN(theta) + " * (4 * (1 - 3 * "
                  + QN(lambda) + " + 3 * " + QN(lambda)
                  + " ^ 2) + 6 * (1 - 2 * " + QN(lambda) + ") * " + QN(theta)
                  + " + 6 * " + QN(theta) + " ^ 2) + 3 * (" + QN(gamma)
                  + " ^ 2) * (" + QN(theta) + " ^ 4))", 0);
}

/**
 * @brief Formula 29: Lever arm correction hS
 */
void Flange_Integral::Calc_hS() {
    hS = 1.1 * eF * sqrt(eE / dE) * (1 - 2 * lambda + theta)
            / (1 + gamma * theta);
    PR->addDetail("Formula 29", "hS(" + QN(mFlangeNumber) + ")",
                  "1.1 * eF * (eE / dE) ^ 0.5 * (1 - 2 * lambda + theta) "
                  "/ (1 + gamma * theta)", hS, "mm",
                  "1.1 * " + QN(eF) + " * (" + QN(eE) + " / " + QN(dE)
                  + ") ^ 0.5 * (1 - 2 * " + QN(lambda) + " + " + QN(theta)
                  + ") / (1 + " + QN(gamma) + " * " + QN(theta) + ")", 0);
}

/**
 * @brief Formula 30: Lever arm correction hT
 */
void Flange_Integral::Calc_hT() {
    hT = eF * (1 - 2 * lambda - gamma * theta * theta) / (1 + gamma * theta);
    PR->addDetail("Formula 30", "hT(" + QN(mFlangeNumber) + ")",
                  "eF * (1 - 2 * lambda - gamma * theta * theta)"
                  " / (1 + gamma * theta)", hT, "mm",
                  QN(eF) + " * (1 - 2 * " + QN(lambda) + " - " + QN(gamma)
                  + " * " + QN(theta) + " * " + QN(theta) + ") / (1 + "
                  + QN(gamma) + " * " + QN(theta) + ")", 0);
}

/**
 * @brief Formula 79: Lever arm correction hQ
 */
void Flange_Integral::Calc_hQ() {
    hQ = (hS * kQ + hT * (2 * dF * eP / (dE * dE)
                          - 0.5 * tan(mShell->phiS)))
            * (pow((dE / mGasket->dGe), 2));
    PR->addDetail("Formula 79", "hQ(" + QN(mFlangeNumber) + ")",
                  "(hS * kQ + hT * (2 * dF * eP / (dE * dE) "
                  "- 0.5 * tan(phiS))) * ((dE / dGe) ^ 2)",
                  hQ, "mm",
                  "(" + QN(hS) + " * " + QN(kQ) + " + " + QN(hT) + " * (2 * "
                  + QN(dF) + " * " + QN(eP) + " / (" + QN(dE) + " * "
                  + QN(dE) + ") - 0.5 * tan(" + QN(mShell->phiS) + "))) * (("
                  + QN(dE) + " / " + QN(mGasket->dGe) + ") ^ 2)", 0);
}

/**
 * @brief Formula 31: Lever arm correction hR
 */
void Flange_Integral::Calc_hR() {
    hR = hS * kR - hT * 0.5 * tan(mShell->phiS);
    PR->addDetail("Formula 31", "hR(" + QN(mFlangeNumber) + ")",
                  "hS * kR - hT * 0.5 * tan(phiS)", hR, "mm",
                  QN(hS) + " * " + QN(kR) + " - " + QN(hT)
                  + " * 0.5 * tan(" + QN(mShell->phiS) + ")", 0);
}

/**
 * @brief Formula 32: Correction factor kQ
 */
void Flange_Integral::Calc_kQ() {
    if ((mShell->sType == mShell->Conical)
            || (mShell->sType == mShell->Cylindrical)) {
        kQ = 0.85 / cos(mShell->phiS);
        PR->addDetail("Formula 32", "kQ(" + QN(mFlangeNumber) + ")",
                      "0.85 / cos(phiS)", kQ, "-",
                      "0.85 / cos(" + QN(mShell->phiS) + ")", 0);
    } else if (mShell->sType == mShell->Spherical) {
        kQ = 0.35 / cos(mShell->phiS);
        PR->addDetail("Formula 32", "kQ(" + QN(mFlangeNumber) + ")",
                      "0.35 / cos(phiS)", kQ, "-",
                      "0.35 / cos(" + QN(mShell->phiS) + ")", 0);
    }
}

/**
 * @brief Formula 33: Correction factor kR
 */
void Flange_Integral::Calc_kR() {
    if ((mShell->sType == mShell->Conical)
            || (mShell->sType == mShell->Cylindrical)) {
        kR = -0.15 / cos(mShell->phiS);
        PR->addDetail("Formula 33", "kR(" + QN(mFlangeNumber) + ")",
                      "-0.15 / cos(phiS)", kR, "-",
                      "-0.15 / cos(" + QN(mShell->phiS) + ")", 0);
    } else if (mShell->sType == mShell->Spherical) {
        kR = -0.65 / cos(mShell->phiS);
        PR->addDetail("Formula 33", "kR(" + QN(mFlangeNumber) + ")",
                      "-0.65 / cos(phiS)", kR, "-",
                      "-0.65 / cos(" + QN(mShell->phiS) + ")", 0);
    }
}

/**
 * @brief Formula 34: Rotation flexibility of flange
 */
void Flange_Integral::Calc_ZF() {
    ZF = 3 * dF * cF / (M_PI * bF * pow(eF, 3));
    PR->addDetail("Formula 34", "ZF(" + QN(mFlangeNumber) + ")",
                  "3 * dF * cF / (PI * bF * eF ^ 3)", ZF, "1/mm^2",
                  "3 * " + QN(dF) + " * " + QN(cF) + " / (pi * " + QN(bF)
                  + " * " + QN(eF) + " ^ 3)", 0);
}

/**
 * @brief Formula 35: Rotation flexibility of loose flange, default is 0.0
 */
void Flange_Integral::Calc_ZL() {
    ZL = 0;
    PR->addDetail("Formula 35", "ZL(" + QN(mFlangeNumber) + ")",
                  "0", ZL, "1/mm^2", "0", 0);
}

/**
 * @brief Formula 59 and 81: Gasket lever arm for all load cases
 */
void Flange_Integral::Calc_hG() {
    hG = (d3e - mGasket->dGe) / 2;
    PR->addDetail("Formula 59, 81", "hG(" + QN(mFlangeNumber) + ")",
                  "(d3e - dGe) / 2", hG, "mm",
                  "(" + QN(d3e) + " - " + QN(mGasket->dGe) + ") / 2", 0);
}

/**
 * @brief Formula 82: Hub lever arm for all load case
 */
void Flange_Integral::Calc_hH() {
    hH = (d3e - dE) / 2;
    PR->addDetail("Formula 82", "hH(" + QN(mFlangeNumber) + ")",
                  "(d3e - dE) / 2", hH, "mm",
                  "(" + QN(d3e) + " - " + QN(dE) + ") / 2", 0);
}

/**
 * @brief Formula 83: Loose flange lever arm for all load cases, 0 for integral
 */
void Flange_Integral::Calc_hL() {
    hL = 0.0;
    PR->addDetail("Formula 83", "hL(" + QN(mFlangeNumber) + ")",
                  "0", hL, "mm", "0", 0);
}

END_NAMESPACE_BILUNA_CALC_EN1591
