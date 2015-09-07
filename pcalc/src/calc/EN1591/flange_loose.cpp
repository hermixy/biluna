﻿#include "flange_loose.h"
#include <algorithm>
NAMESPACE_BILUNA_CALC_EN1591


Flange_Loose::Flange_Loose(int flangeNo) : Flange(flangeNo) {
    setName("PCALC EN1591 Flange Loose");
}

Flange_Loose::~Flange_Loose() {
    // shell, washer deleted in Flange
}

/**
 * @brief according para 4.2: eF/bF, eL/bL and cos(phiS)
 * @return true if flange is valid
 */
bool Flange_Loose::Is_flange_Valid() {
    QString str = "0.2 <= bF / eF AND 0.2 <= bL / eL AND bL / eL <= 5.0";
    QString strVal = "0.2 <= " + QN(bF) + " / " + QN(eF) + " AND 0.2 <= "
            + QN(bL) + " / " + QN(eL) + " AND "
            + QN(bL) + " / " + QN(eL) + " <= 5.0";

    if (0.2 <= bF / eF && 0.2 <= bL / eL && bL / eL <= 5.0) {

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
            PR->addDetail("Para 4.2", "result", str, 1, "-", strVal, 0);
            return true;
        }
    }

    PR->addDetail("Para 4.2", "result", str, 0, "-", strVal, 0);
    return false;
}

/**
 * @brief Formula 11: Effective width of flange
 */
void Flange_Loose::Calc_bF() {
    bF = (d8 - d0) / 2;
    PR->addDetail("Formula 11", "bF", "(d8 - d0) / 2", bF, "mm",
                  "(" + QN(d8) + " - " + QN(d0) + ") / 2", 0);
}

/**
 * @brief Formula 12: Average diameter of flange
 */
void Flange_Loose::Calc_dF() {
    dF = (d8 + d0) / 2;
    PR->addDetail("Formula 12", "bF", "(d8 + d0) / 2", dF, "mm",
                  "(" + QN(d8) + " + " + QN(d0) + ") / 2", 0);
}

/**
 * @brief Formula 13 alternative: Area for effective axial thickness of flange
 */
void Flange_Loose::Calc_AF() {
    AF = (d8 - d0) * eF / 2;
    PR->addDetail("Formula 13", "AF", "(d8 - d0) * eF / 2", AF, "mm^2",
                  "(" + QN(d8) + " - " + QN(d0) + ") * " + QN(eF) + " / 2", 0);
}

/**
 * @brief Formula 14: Effective width loose flange
 */
void Flange_Loose::Calc_bL() {
    bL = (d4 - d6) / 2 - d5e;
    PR->addDetail("Formula 14", "bL", "(d4 - d6) / 2 - d5e", bL, "mm",
                  "(" + QN(d4) + " - " + QN(d6) + ") / 2 - " + QN(d5e), 0);
}

/**
 * @brief Formula 15: Average diameter loose flange, default is 0.0
 */
void Flange_Loose::Calc_dL() {
    dL = (d4 + d6) / 2;
    PR->addDetail("Formula 15", "dL", "(d4 + d6) / 2", dL, "mm",
                  "(" + QN(d4) + " + " + QN(d6) + ") / 2", 0);
}

/**
 * @brief Formula 16: Effective axial thickness loose flange
 */
void Flange_Loose::Calc_AL() {
    AL = (d4 - d6) * eL / 2;
    PR->addDetail("Formula 16", "AL", "(d4 - d6) * eL / 2", AL, "mm^2",
                  "(" + QN(d4) + " - " + QN(d6) + ") * " + QN(eL) + " / 2", 0);
}

/**
 * @brief Formula 19: Intermediate working variable for calulation
 * of the wall thickness  of equivalent cylinder for load limit and flexibility
 */
void Flange_Loose::Calc_beta() {
    beta = e2 / e1;
    PR->addDetail("Formula 19", "beta", "e2 / e1", beta, "-",
                  QN(e2) + " / " + QN(e1), 0);
}

/**
 * @brief Formula 18: Wall thickness of equivalent cylinder for load limit
 * and flexibility calculations
 */
void Flange_Loose::Calc_eD() {
    eD = e1 * (1 + (beta - 1) * lH
               / pow(pow(beta / 3, 4)
                     * pow(d1 * e1, 2) + pow(lH, 4), 0.25));
    PR->addDetail("Formula 18", "eD",
                  "e1 * (1 + (beta - 1) * lH / ((beta / 3) ^ 4 * "
                  "(d1 * e1) ^ 2 + lH ^ 4) ^ 0.25)", eD, "mm",
                  QN(e1) + " * (1 + (" + QN(beta) + " - 1) * " + QN(lH)
                  + " / ((" + QN(beta) + " / 3) ^ 4 * (" + QN(d1) + " * "
                  + QN(e1) + ") ^ 2 + " + QN(lH) + " ^ 4) ^ 0.25)", 0);
}

/**
 * @brief Formula 17 and 21: Wall thickness of equivalent cylinder
 * for load limit and flexibility calculations
 */
void Flange_Loose::Calc_eE() {
//    if (fabs(e2 - e1) < ACCURACY)     {
        // No hub because wallthickness hub is same as connecting pipe
        eE = mShell->eS;
        PR->addDetail("Formula 21", "eE", "eS", eE, "mm", QN(mShell->eS), 0);
//    } else {
//        // Tapered hub
//        eE = e1 * (1 + ((beta - 1) * lH / ((beta / 3) * sqrt(e1 * d1) + lH)));
//        PR->addDetail("Formula 17",
//                  "eE", "e1 * (1 + ((beta - 1) * lH "
//                  "/ ((beta / 3) * Math.Sqrt(e1 * d1) + lH)))",
//                  eE, "mm");
//    }
}

/**
 * @brief Formula 20 and 22: Average diameter of equivalent
 * or effective cylinder, gasket pressure
 */
void Flange_Loose::Calc_dE() {
    if (fabs(e2 - e1) < ACCURACY)     {
        // No hub because wallthickness hub is same as connecting pipe
        dE = mShell->dS;
        PR->addDetail("Formula 22", "dE", "dS", dE, "-", QN(mShell->dS), 0);
    } else {
        // Tapered hub
        dE = 0.5 * (std::min(d1 - e1 + eE, d2 + e2 - eE)
                    + std::max(d1 + e1 - eE, d2 - e2 + eE));
        PR->addDetail("Formula 20", "dE",
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
void Flange_Loose::Calc_gamma() {
    gamma = eE * dF / (bF * dE * cos(mShell->phiS));
    PR->addDetail("Formula 25", "gamma",
                  "eE * dF / (bF * dE * cos(phiS))", gamma, "-",
                  QN(eE) + " * " + QN(dF) + " / (" + QN(bF) + " * "
                  + QN(dE) + " * cos(" + QN(mShell->phiS) + "))", 0);
}

/**
 * @brief Formula 26: Intermediate working variable theta
 */
void Flange_Loose::Calc_theta() {
    theta = 0.55 * cos(mShell->phiS) * sqrt(dE * eE) / eF;
    PR->addDetail("Formula 26", "theta",
                  "0.55 * cos(phiS) * (dE * eE) ^ 0.5 / eF",
                  theta, "-",
                  "0.55 * cos(" + QN(mShell->phiS) + ") * ("
                  + QN(dE) + " * " + QN(eE) + ") ^ 0.5 / " + QN(eF), 0);
}

/**
 * @brief Formula 27: Intermediate working variable lambda
 */
void Flange_Loose::Calc_lambda() {
    lambda = 1 - eP / eF;
    PR->addDetail("Formula 27", "lambda", "1 - eP / eF", lambda, "-",
                  "1 - " + QN(eP) + " / " + QN(eF), 0);
}

/**
 * @brief Formula 28: Correction factor flange
 */
void Flange_Loose::Calc_cF() {
    cF = (1 + gamma * theta)
            / (1 + gamma * theta
               * (4 * (1 - 3 * lambda + 3 * pow(lambda, 2))
                  + 6 * (1 - 2 * lambda) * theta + 6 * pow(theta, 2))
               + 3 * pow(gamma, 2) * pow(theta, 4));
    PR->addDetail("Formula 28", "cF",
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
void Flange_Loose::Calc_hS() {
    hS = 1.1 * eF * sqrt(eE / dE) * (1 - 2 * lambda + theta)
            / (1 + gamma * theta);
    PR->addDetail("Formula 29", "hS",
                  "1.1 * eF * (eE / dE) ^ 0.5 * (1 - 2 * lambda + theta) "
                  "/ (1 + gamma * theta)", hS, "mm",
                  "1.1 * " + QN(eF) + " * (" + QN(eE) + " / " + QN(dE)
                  + ") ^ 0.5 * (1 - 2 * " + QN(lambda) + " + " + QN(theta)
                  + ") / (1 + " + QN(gamma) + " * " + QN(theta) + ")", 0);
}

/**
 * @brief Formula 30: Lever arm correction hT
 */
void Flange_Loose::Calc_hT() {
    hT = eF * (1 - 2 * lambda - gamma * theta * theta) / (1 + gamma * theta);
    PR->addDetail("Formula 30", "hT",
                  "eF * (1 - 2 * lambda - gamma * theta * theta)"
                  " / (1 + gamma * theta)", hT, "mm",
                  QN(eF) + " * (1 - 2 * " + QN(lambda) + " - " + QN(gamma)
                  + " * " + QN(theta) + " * " + QN(theta) + ") / (1 + "
                  + QN(gamma) + " * " + QN(theta) + ")", 0);
}

/**
 * @brief Formula 79: Lever arm correction hQ
 */
void Flange_Loose::Calc_hQ() {
    hQ = (hS * kQ + hT * (2 * dF * eP / (dE * dE)
                          - 0.5 * tan(mShell->phiS)))
            * (pow((dE / mGasket->dGe), 2));
    PR->addDetail("Formula 79", "hQ",
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
void Flange_Loose::Calc_hR() {
    hR = hS * kR - hT * 0.5 * tan(mShell->phiS);
    PR->addDetail("Formula 31", "hR",
                  "hS * kR - hT * 0.5 * tan(phiS)", hR, "mm",
                  QN(hS) + " * " + QN(kR) + " - " + QN(hT)
                  + " * 0.5 * tan(" + QN(mShell->phiS) + ")", 0);
}

/**
 * @brief Formula 32: Correction factor kQ
 */
void Flange_Loose::Calc_kQ() {
    if ((mShell->sType == Shell::Conical)
            || (mShell->sType == Shell::Cylindrical))     {
        kQ = 0.85 / cos(mShell->phiS);
        PR->addDetail("Formula 32", "kQ", "0.85 / cos(phiS)", kQ, "-",
                      "0.85 / cos(" + QN(mShell->phiS) + ")", 0);
    } else if (mShell->sType == Shell::Spherical) {
        kQ = 0.35 / cos(mShell->phiS);
        PR->addDetail("Formula 32", "kQ", "0.35 / cos(phiS)", kQ, "-",
                      "0.35 / cos(" + QN(mShell->phiS) + ")", 0);
    }
}

/**
 * @brief Formula 33: Correction factor kR
 */
void Flange_Loose::Calc_kR() {
    if ((mShell->sType == Shell::Conical)
            || (mShell->sType == Shell::Cylindrical)) {
        kR = -0.15 / cos(mShell->phiS);
        PR->addDetail("Formula 33", "kR", "-0.15 / cos(phiS)", kR, "-",
                      "-0.15 / cos(" + QN(mShell->phiS) + ")", 0);
    } else if (mShell->sType == Shell::Spherical) {
        kR = -0.65 / cos(mShell->phiS);
        PR->addDetail("Formula 33", "kR", "-0.65 / cos(phiS)", kR, "-",
                      "-0.65 / cos(" + QN(mShell->phiS) + ")", 0);
    }
}

/**
 * @brief Formula 34: Rotation flexibility of flange
 */
void Flange_Loose::Calc_ZF() {
    ZF = 3 * dF * cF / (M_PI * bF * pow(eF, 3));
    PR->addDetail("Formula 34", "ZF", "3 * dF * cF "
              "/ (PI * bF * eF ^ 3)", ZF, "1/mm^2",
              "3 * " + QN(dF) + " * " + QN(cF) + " / (pi * " + QN(bF)
              + " * " + QN(eF) + " ^ 3)", 0);
}

/**
 * @brief Formula 40: Rotation flexibility of loose flange
 */
void Flange_Loose::Calc_ZL() {
    ZL = 3 * dL / (M_PI * bL * pow(eL, 3));
    PR->addDetail("Formula 40", "ZL", "3 * dL / (PI * bL * eL ^ 3)",
                  ZL, "1/mm^2", "3 * " + QN(dL) + " / (pi * "
                  + QN(bL) + " * " + QN(eL) + " ^ 3)", 0);
}

/**
 * @brief Formula 85: Minimum load transfer diameter ring and collar
 */
void Flange_Loose::Calc_d7min() {
    d7min = d6 + 2 * b0;
    PR->addDetail("Formula 85", "d7min", "d6 + 2 * b0", d7min, "mm",
                  QN(d6) + " + 2 * " + QN(b0), 0);
}

/**
 * @brief Formula 86: Maximum load transfer diameter ring and collar
 */
void Flange_Loose::Calc_d7max() {
    d7max = d8;
    PR->addDetail("Formula 86", "d7max", "d8", d7max, "mm", QN(d8), 0);
}

/**
 * @brief Formula 61: Load transfer diameter at load case 0.
 * This includes the note after formula 89 and sets d70
 * as d7 for all loadcases
 * @param loadCaseNo
 */
void Flange_Loose::Calc_d70() {
    d70 = (mGasket->dGe + chi * d3e) / (1 + chi);

    if (d70 < d7min)     {
        d70 = d7min;
    } else if (d70 > d7max) {
        d70 = d7max;
    }

    d7 = d70;

    PR->addDetail("Formula 61", "d70",
                  "(dGe + chi * d3e) / (1 + chi) [d7min;d7max]",
                  d70, "mm",
                  "(" + QN(mGasket->dGe) + " + " + QN(chi) + " * "
                  + QN(d3e) + ") / (1 + " + QN(chi) + ")", 0);
}

/**
 * @brief Formula 84: Limit d7 to minimum and maximum
 * @param loadCaseNo
 */
void Flange_Loose::Calc_d7minMax() {
    d7 = std::min(std::max(d7min, d7), d7max);
    PR->addDetail("Formula 84", "result", "min(max(d7min, d7), d7max)",
                  d7, "mm",
                  "min(max(" + QN(d7min) + "; " + QN(d7) + "); "
                  + QN(d7max) + ")", 0);
}

/**
 * @brief Formula 62: Intermediate working variable loadCaseNo
 * is not always 0, although assumed for ZL in Formula 40
 * @param loadCaseNo
 */
void Flange_Loose::Calc_chi(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (getFlangeNumber() == 1) {
        chi = ZL * loadCase->EF1 / (ZF * loadCase->EL1);
        PR->addDetail("Formula 62", "chi",
                      "ZL * EF1 / (ZF * EL1)", chi, "-",
                      QN(ZL) + " * " + QN(loadCase->EF1) + " / (" + QN(ZF)
                      + " * " + QN(loadCase->EL1) + ")", 0);
    } else {
        chi = ZL * loadCase->EF2 / (ZF * loadCase->EL2);
        PR->addDetail("Formula 62", "chi",
                      "ZL * EF2 / (ZF * EL2)", chi, "-",
                      QN(ZL) + " * " + QN(loadCase->EF2) + " / (" + QN(ZF)
                      + " * " + QN(loadCase->EL2) + ")", 0);
    }
}

/**
 * @brief Formula 60 and 87: Lever arm gasket at load case
 * @param loadCaseNo
 */
void Flange_Loose::Calc_hG() {
    hG = (d7 - mGasket->dGe) / 2;
    PR->addDetail("Formula 60, 87", "hG" + QN(getFlangeNumber()),
                  "(d7 - dGe) / 2", hG, "-",
                  "(" + QN(d7) + " - " + QN(mGasket->dGe) + ") / 2", 0);
}

/**
 * @brief Formula 88: Hub lever arm for all load case
 * @param loadCaseNo
 */
void Flange_Loose::Calc_hH() {
    hH = (d7 - dE) / 2;
    PR->addDetail("Formula 88", "hH" + QN(getFlangeNumber()),
                  "(d7 - dE) / 2", hH, "mm",
                  "(" + QN(d7) + " - " + QN(dE) + ") / 2", 0);
}

/**
 * @brief Formula 89: Lever arm loose ring
 */
void Flange_Loose::Calc_hL() {
    hL = (d3e - d7) / 2;
    PR->addDetail("Formula 89", "hL" + QN(getFlangeNumber()), "(d3e - d7) / 2",
                  hL, "mm", "(" + QN(d3e) + " - " + QN(d7) + ") / 2", 0);
}

/**
 * @brief Formula 150: Resistance loose flange
 * @param loadCaseNo
 */
void Flange_Loose::Calc_WL(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_fL = loadCase->fL1;

    if (getFlangeNumber() == 2) {
        tmp_fL = loadCase->fF2;
    }

    double tmp_WL = M_PI / 2 * tmp_fL * bL * pow(eL, 2);

    if (getFlangeNumber() == 1) {
        loadCase->WL1 = tmp_WL;
    } else if (getFlangeNumber() == 2) {
        loadCase->WL2 = tmp_WL;
    }

    PR->addDetail("Formula 150", "WL" + QN(getFlangeNumber()),
                  "PI / 2 * fL2 * bL * eL ^ 2", loadCase->WL2, "Nmm",
                  "pi / 2 * " + QN(tmp_fL) + " * " + QN(bL)
                  + " * " + QN(eL) + " ^ 2", loadCaseNo);
}

/**
 * @brief Formula 149: Load ratio loose flange
 * @param loadCaseNo
 */
void Flange_Loose::Calc_PhiL(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_WL = loadCase->WL1;

    if (getFlangeNumber() == 2) {
        tmp_WL = loadCase->WL2;
    }

    double tmp_PhiL = loadCase->F_B * hL / tmp_WL;

    if (getFlangeNumber() == 1) {
        loadCase->PhiL1 = tmp_PhiL;
        PR->addDetail("Formula 149", "PhiL1", "F_B * hL1 / WL1",
                      loadCase->PhiL1, "-",
                      QN(loadCase->F_B) + " * " + QN(hL) + " / " + QN(tmp_WL),
                      loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        loadCase->PhiL2 = tmp_PhiL;
        PR->addDetail("Formula 149", "PhiL2", "F_B * hL2 / WL2",
                      loadCase->PhiL2, "-",
                      QN(loadCase->F_B) + " * " + QN(hL) + " / " + QN(tmp_WL),
                      loadCaseNo);
    }
}

/**
 * @brief After Formula 149: Assess PhiL
 * @param loadCaseNo
 * @return
 */
bool Flange_Loose::Is_PhiL_valid(int loadCaseNo) {
    bool result = false;

    if (getFlangeNumber() == 1) {
        result = mLoadCaseList->at(loadCaseNo)->PhiL1 <= 1.0;
        PR->addDetail("Formula 149", "result", "PhiL1 <= 1.0",
                      static_cast<int>(result), "-",
                      QN(mLoadCaseList->at(loadCaseNo)->PhiL1) + " <= 1.0",
                      loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        result = mLoadCaseList->at(loadCaseNo)->PhiL2 <= 1.0;
        PR->addDetail("Formula 149", "result", "PhiL2 <= 1.0",
                      static_cast<int>(result), "-",
                      QN(mLoadCaseList->at(loadCaseNo)->PhiL1) + " <= 1.0",
                      loadCaseNo);
    }

    return result;
}

/**
 * @brief With Formula 151: Resistance stub or collar of loose flange,
 * part of EN13445 appendix GA, here denominator of formula 151
 * @param loadCaseNo
 */
void Flange_Loose::Calc_WQ(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_fF = loadCase->fF1;
    double tmp_fS = loadCase->fS1;

    if (getFlangeNumber() == 2) {
        tmp_fF = loadCase->fF2;
        tmp_fS = loadCase->fS2;
    }

    double tmpVal1 = std::min(tmp_fS * pow(mShell->eS, 2), tmp_fF * pow(eF, 2));
    double tmpVal2 = std::min(tmp_fF * pow(eF, 2),
                              loadCase->Q_smax *
                                  pow(mGasket->dG2 - d7, 2) / 4.0);
    double tmp_WQ = M_PI / 4 * mShell->dS *(tmpVal1 + tmpVal2);

    QString str = "pi / 4 * dS *(min(fS * eS ^ 2, fF * eF ^ 2) "
                  "+ min(fF * eF ^ 2, Q_smax * (dG2 - d7) ^ 2 / 4.0))";

    QString strVal1 = "min(" + QN(tmp_fS) + " * " + QN(mShell->eS)
            + " ^ 2, " + QN(tmp_fF) + " * eF ^ 2)";
    QString strVal2 = "min(" + QN(tmp_fF) + " * " + QN(eF) + " ^ 2, "
            + QN(loadCase->Q_smax) + " * (" + QN(mGasket->dG2)
            + " - " + QN(d7) + ") ^ 2 / 4.0)";
    QString strWQ = "pi / 4 * " + QN(mShell->dS) + " *("
            + strVal1 + " + " + strVal2 + ")";

    if (getFlangeNumber() == 1)     {
        loadCase->WQ1 = tmp_WQ;
        PR->addDetail("With F. 151", "WQ1", str,
                      loadCase->WQ1, "Nmm",
                      strWQ, loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        loadCase->WQ2 = tmp_WQ;
        PR->addDetail("With F. 151", "WQ2", str,
                      loadCase->WQ2, "Nmm",
                      strWQ, loadCaseNo);
    }
}

/**
 * @brief Formula 151: Load ratio stub or collar of loose flange
 * @param loadCaseNo
 */
void Flange_Loose::Calc_PhiF(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (mGasket->dG2 - d7 > 0) {
        double tmp_WQ = loadCase->WQ1;

        if (getFlangeNumber() == 2) {
            tmp_WQ = loadCase->WQ2;
        }

        double tmp_PhiF = fabs(loadCase->F_Q + loadCase->F_R) * hH / tmp_WQ;

        if (getFlangeNumber() == 1) {
            loadCase->PhiF1 = tmp_PhiF;
        } else if (getFlangeNumber() == 2) {
            loadCase->PhiF2 = tmp_PhiF;
        }

        PR->addDetail("Formula 151", "PhiF" + QN(getFlangeNumber()),
                      "abs(F_Q + F_R) * hH / WQ", loadCase->PhiF1, "-",
                      "abs(" + QN(loadCase->F_Q) + " + " + QN(loadCase->F_R)
                      + ") * " + QN(hH) + " / " + QN(tmp_WQ), loadCaseNo);
    } else {
        Flange::Calc_PhiF(loadCaseNo);
    }
}

END_NAMESPACE_BILUNA_CALC_EN1591