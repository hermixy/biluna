#include "flange.h"
#include "pcalc_report.h"
#include "shell.h"
#include "washer.h"

NAMESPACE_BILUNA_CALC_EN1591


double Flange_IN::ACCURACY = 0.000001; //1.0e-6 required for integral a.o.


Flange_IN::Flange_IN(int flangeNo) : RB_Object() {

    if (flangeNo != 1 && flangeNo != 2) {
        qDebug("Flange constructor: flange Number ERROR.");
    }

    mFlangeNumber = flangeNo;
    mShell = new Shell(); // also for blind!
    mWasher = new Washer();
    mShell->shellNumber = getFlangeNumber();
    mWasher->mWasherNumber = getFlangeNumber();

    // ratio factor for calculation of eP and eQ based on eF,
    // depends on type of flange

    nB = 0;
    d3 = 0;
    d0 = 0;
    d4 = 0;
    temp_d7 = 0;
    eF = 0;
    eFt = 0;
    eP = 1;
    dX = 0;
    eRF = 0;
    eX = 0;

    // Blind only
    d9 = 0;
    e0 = 0;

    // Integral only
    d2 = 0;

    // Integral loose only
    e1 = 0;
    e2 = 0;
    lH = 0;

    // Loose only
    b0 = 0;
    d6 = 0;
    eL = 0;
}

Flange_IN::~Flange_IN() {
    delete mShell;
    mShell = NULL;
    delete mWasher;
    mWasher = NULL;
}

/**
 * Override to add flange number to variable name
 * @param formulaNumber
 * @param variableName
 * @param formula
 * @param result
 * @param unit
 * @param loadCaseNo
 * @param note
 */
void Flange_IN::addDetail(const QString& formulaNumber,
                          const QString& variableName,
                          const QString& formula,
                          double result, const QString& unit,
                          const QString& formulaValues,
                          int loadCaseNo, const QString& note) {
    PR->addDetail(formulaNumber, variableName + "("
                  + QString::number(getFlangeNumber()) + ")",
                  formula, result, unit, formulaValues, loadCaseNo, note);
}

Flange_OUT::Flange_OUT(int flangeNo) : Flange_IN(flangeNo) {
    pB = 0.0;
    d5e = 0.0;
    d3e = 0.0;
    eQ = 0.0;
    bF = 0.0;
    dF = 0.0;
    AF = 0.0;
    bL = 0.0;
    dL = 0.0;
    AL = 0.0;
    beta = 0.0;
    eD = 0.0;
    dE = 0.0;
    eE = 0.0;
    rho = 0.0;
    gamma = 0.0;
    theta = 0.0;
    lambda = 0.0;
    cF = 0.0;

    d7 = 0.0;
    hG = 0.0;
    hH = 0.0;
    hL = 0.0;

    hS = 0.0;
    hT = 0.0;
    kQ = 0.0;
    kR = 0.0;
    hQ = 0.0;
    hR = 0.0;
    hP = 0.0;
    ZF = 0.0;
    ZL = 0.0;
    d7min = 0.0;
    d7max = 0.0;
    d70 = 0.0;
    chi = 0.0;
}

/**
 * @brief Figure 8-9: Portion of the flange thickness NOT subject
 * to radial loading.
 */
void Flange::Calc_eQ() {
    eQ = eF - eP;
    PR->addDetail("Before_F. 1 fig 4-10", "eQ(" + QN(mFlangeNumber) + ")",
                  "eF - eP", eQ, "mm", QN(eF) + " - " + QN(eP));
}

/**
 * @brief Formula 3: Pitch between bolts
 */
void Flange::Calc_pB() {
    pB = M_PI * d3 / nB;
    PR->addDetail("Formula 3", "pB",
                  "PI * d3 / nB", pB, "-", "pi * " + QN(d3) + " / " + QN(nB));
}

/**
 * @brief Formula 4 and 5: Effective diameter of the bolt hole
 */
void Flange::Calc_d5e() {
    if (mBolt->mBoltHole->isBlindHole) {
        mBolt->mBoltHole->d5 = mBolt->mBoltHole->d5t * mBolt->l5t
                / mBolt->mBoltHole->eFb;
        PR->addDetail("Formula 4, 5", "d5(" + QN(mFlangeNumber) + ")",
                      "d5t * l5t / eFb", mBolt->mBoltHole->d5, "mm",
                      QN(mBolt->mBoltHole->d5t) + " * " + QN(mBolt->l5t)
                      + " / " + QN(mBolt->mBoltHole->eFb));
    }

    d5e = mBolt->mBoltHole->d5 * sqrt(mBolt->mBoltHole->d5 / pB);
    PR->addDetail("Formula 4, 5", "d5e(" + QN(mFlangeNumber) + ")",
                  "d5 * (d5 / pB) ^ 0.5", d5e, "mm",
                  QN(mBolt->mBoltHole->d5) + " * (" + QN(mBolt->mBoltHole->d5)
                  + " / " + QN(pB) + ") ^ 0.5");
}

/**
 * @brief Formula 6: Effective bolt circle diameter
 */
void Flange::Calc_d3e() {
    d3e = d3 * (1.0 - 2.0 / static_cast<double>(nB * nB));
    PR->addDetail("Formula 6", "d3e", "d3 * (1 - 2 / (nB * nB))", d3e, "mm",
                  QN(d3) + " * (1.0 - 2.0 / (" + QN(nB)
                  + " * " + QN(nB) + "))");
}

/**
 * @brief Formula 41: Effective cross-section area of bolts
 */
void Flange::Calc_AB() {
    mBolt->AB = nB * M_PI / 4 * pow((std::min(mBolt->dBe, mBolt->dBS)), 2);
    PR->addDetail("Formula 41", "AB", "nB * PI / 4 * (Min(dBe, dBS)) ^ 2",
                  mBolt->AB, "mm^2", QN(nB) + " * pi / 4 * (min("
                  + QN(mBolt->dBe) + ", " + QN(mBolt->dBS) + ")) ^ 2");
}

/**
 * @brief Formula 42: Flexibility modulus of bolts
 */
void Flange::Calc_XB() {
    mBolt->XB = 4 * (mBolt->lS / (mBolt->dBS * mBolt->dBS) + (mBolt->lB - mBolt->lS)
                   / (mBolt->dBe * mBolt->dBe) + 0.8 / mBolt->dB0) / (nB * M_PI);
    PR->addDetail("Formula 42",
                  "XB", "4 * (lS / (dBS * dBS) + (lB - lS) "
                        "/ (dBe * dBe) + 0.8 / dB0) / (nB * PI)",
                  mBolt->XB, "1/mm",
                  "4 * (" + QN(mBolt->lS) + " / (" + QN(mBolt->dBS) + " * "
                  + QN(mBolt->dBS) + ") + (" + QN(mBolt->lB) + " - "
                  + QN(mBolt->lS) + ") / (" + QN(mBolt->dBe) + " * "
                  + QN(mBolt->dBe) + ") + 0.8 / " + QN(mBolt->dB0) + ") / ("
                  + QN(nB) + " * pi)");
}

/**
 * @brief Formula 46: Maximum inner contact diameter washer flange
 */
void Flange::Calc_dK1() {
    if (mWasher->isPresent()) {
        mWasher->dK1 = std::max(mBolt->mBoltHole->d5, mWasher->dW1);
        PR->addDetail("Formula 46", "dK1(" + QN(mFlangeNumber) + ")",
                      "max(d5, dW1)", mWasher->dK1, "mm",
                      "max(" + QN(mBolt->mBoltHole->d5) + ", "
                      + QN(mWasher->dW1) + ")");
    }
}

/**
 * @brief Formula 47: Minimum outer contact diameter nut washer
 */
void Flange::Calc_dK2() {
    if (mWasher->isPresent()) {
        mWasher->dK2 = std::min(mBolt->dB4, mWasher->dW2);
        PR->addDetail("Formula 47", "dK2(" + QN(mFlangeNumber) + ")",
                      "min(dB4, dW2)", mWasher->dK2, "mm",
                      "min(" + QN(mBolt->dB4) + ", "
                      + QN(mWasher->dW2) + ")");
    }
}

/**
 * @brief Formula 49 and 50: Flexibility modulus of washer
 */
void Flange::Calc_XW() {
    if (mWasher->isPresent()) {
        mWasher->XW = (mWasher->eW / (nB * M_PI * mWasher->dW * mWasher->bW))
                * ((2 * mWasher->bW / (mWasher->bW + mWasher->bKB) + mWasher->eW
                    / (mWasher->bW - mWasher->bKB))
                   / (1 + mWasher->eW / (mWasher->bW - mWasher->bKB)));
        PR->addDetail("Formula 49 50","XW(" + QN(mFlangeNumber) + ")",
                      "(eW / (nB * Math.PI * dW * bW)) "
                      "* ((2 * bW / (bW + bKB) + eW "
                      "/ (bW - bKB)) / (1 + eW "
                      "/ (bW - bKB)))",
                      mWasher->XW, "1/mm",
                      "(" + QN(mWasher->eW) + " / (nB * pi * " + QN(mWasher->dW)
                      + " * " + QN(mWasher->bW) + ")) * ((2 * " + QN(mWasher->bW)
                      + " / (" + QN(mWasher->bW) + " + " + QN(mWasher->bKB)
                      + ") + " + QN(mWasher->eW) + " / (" + QN(mWasher->bW)
                      + " - " + QN(mWasher->bKB) + ")) / (1 + " + QN(mWasher->eW)
                      + " / (" + QN(mWasher->bW) + " - " + QN(mWasher->bKB)
                      + ")))");
    } else {
        mWasher->XW = 0;
        PR->addDetail("Formula 49 50", "XW(" + QN(mFlangeNumber) + ")",
                      "0", mWasher->XW, "1/mm", "0");
    }
}

/**
 * @brief Formula 77: Lever arm correction pressure (P) uses dGe
 */
void Flange::Calc_hP() {
    double tmpdGe = mGasket->dGe;
    hP = (pow((tmpdGe - dE), 2) * (2 * tmpdGe + dE)
          / 6 + 2 * pow(eP, 2) * dF) / pow(tmpdGe, 2);
    PR->addDetail("Formula 77", "hP(" + QN(mFlangeNumber) + ")",
                  "(((dGe - dE) ^ 2) * (2 * dGe + dE) "
                  "/ 6 + 2 * (eP ^ 2) * dF) / (dGe ^ 2)", hP, "mm",
                  "(((" + QN(tmpdGe) + " - " + QN(dE) + ") ^ 2) * (2 * "
                  + QN(tmpdGe)+ " + " + QN(dE) + ") / 6 + 2 * (" + QN(eP)
                  + " ^ 2) * " + QN(dF) + ") / (" + QN(tmpdGe) + " ^ 2)");
}

/**
 * @brief Formula B.1: High scatter value of serie of bolts tensioning
 */
void Flange::Calc_etanplus() {
    mBolt->etanplus = mBolt->eta1plus * (1 + 3 / sqrt(nB)) / 4;
    PR->addDetail("Formula B.1", "etanplus",
                  "eta1plus * (1 + 3 / Sqrt(nB)) / 4",
                  mBolt->etanplus, "-", QN(mBolt->eta1plus) + " * (1 + 3 / "
                  + QN(nB) + " ^ 0.5) / 4");
}

/**
 * @brief Formula B.2: Low scatter value of serie of bolts tensioning
 */
void Flange::Calc_etanminus() {
    mBolt->etanminus = mBolt->eta1minus * (1 + 3 / sqrt(nB)) / 4;
    PR->addDetail("Formula B.2", "etanminus",
                  "eta1minus * (1 + 3 / Sqrt(nB)) / 4",
                  mBolt->etanminus, "-", QN(mBolt->eta1minus) + " * (1 + 3 / "
                  + QN(nB) + " ^ 0.5) / 4");
}

void Flange::Calc_beta() {
    // does nothing, Formula 19
}

void Flange::Calc_eD() {
    // does nothing, Formula 18
}

void Flange::Calc_gamma() {
    // does nothing, Formula 25
}

void Flange::Calc_theta() {
    // does nothing, Formula 26
}

void Flange::Calc_lambda() {
    // does nothing, Formula 27
}

void Flange::Calc_cF() {
    // does nothing, Formula 28
}

void Flange::Calc_hS() {
    // does nothing, Formula 29
}

void Flange::Calc_hT() {
    // does nothing, Formula 30
}

void Flange::Calc_kQ() {
    // does nothing, Formula 32
}

void Flange::Calc_kR() {
    // does nothing, Formula 33
}

void Flange::Calc_rho() {
    // does nothing, Formula 36
}

void Flange::Calc_d7min() {
    // does nothing, for loose flange Formula 85
}

void Flange::Calc_d7max() {
    // does nothing, for loose flange Formula 86
}

void Flange::Calc_d70(int /*loadCaseNo*/) {
    // does nothing, for loose flange Formula 62
}

void Flange::Calc_d7minMax(int /*loadCaseNo*/) {
    // does nothing, for loose flange Formula 84
}

void Flange::Calc_chi() {
    // does nothing, for loose flange Formula 63
}

/**
 * @brief After Formula 129, 145: PhiF assessment
 * @param loadCaseNo
 * @returns true if PhiF is valid
 */
bool Flange::Is_PhiF_valid(int loadCaseNo) {
    bool result = false;

    if (getFlangeNumber() == 1) {
        result = mLoadCaseList->at(loadCaseNo)->PhiF1 <= 1.0;
        PR->addDetail("Formula 129", "result(" + QN(mFlangeNumber) + ")",
                      "PhiF1 <= 1.0", static_cast<int>(result), "-",
                      QN(mLoadCaseList->at(loadCaseNo)->PhiF1)
                      + " &lt;= 1.0", loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        result = mLoadCaseList->at(loadCaseNo)->PhiF2 <= 1.0;
        PR->addDetail("Formula 129", "result(" + QN(mFlangeNumber) + ")",
                      "PhiF2 <= 1.0", static_cast<int>(result), "-",
                      QN(mLoadCaseList->at(loadCaseNo)->PhiF1)
                      + " &lt;= 1.0", loadCaseNo);
    }

    return result;
}

/**
 * @brief Formula 132: Intermediate working variable,
 * only for integral flange, stub or collar
 * @param loadCaseNo
 */
void Flange::Calc_deltaQ(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    if (getFlangeNumber() == 1) {
        loadCase->deltaQ1 = loadCase->P * dE / (loadCase->fE1 * 2 * eD
                                                * cos(mShell->phiS));
        PR->addDetail("Formula 132", "deltaQ(" + QN(mFlangeNumber) + ")",
                      "P * dE / (fE1 * 2 * eD * Cos(Shell.phiS))",
                      loadCase->deltaQ1, "-",
                      QN(loadCase->P) + " * " + QN(dE) + " / ("
                      + QN(loadCase->fE1) + " * 2 * " + QN(eD)
                      + " * cos(" + QN(mShell->phiS) + "))", loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        loadCase->deltaQ2 = loadCase->P * dE / (loadCase->fE2 * 2 * eD
                                                * cos(mShell->phiS));
        PR->addDetail("Formula 132", "deltaQ(" + QN(mFlangeNumber) + ")",
                      "P * dE / (fE2 * 2 * eD * Cos(Shell.phiS))",
                      loadCase->deltaQ2, "-",
                      QN(loadCase->P) + " * " + QN(dE) + " / ("
                      + QN(loadCase->fE2) + " * 2 * " + QN(eD)
                      + " * cos(" + QN(mShell->phiS) + "))", loadCaseNo);
    }
}

/**
 * @brief Formula 133: Intermediate working variable,
 * only for integral flange, stub or collar
 * @param loadCaseNo
 */
void Flange::Calc_deltaR(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    if (getFlangeNumber() == 1) {
        loadCase->deltaR1 = loadCase->F_R / (loadCase->fE1 * M_PI * dE
                                             * eD * cos(mShell->phiS));
        PR->addDetail("Formula 133", "deltaR(" + QN(mFlangeNumber) + ")",
                      "F_R / (fE1 * PI * dE * eD * Cos(Shell.phiS))",
                      loadCase->deltaR1, "-",
                      QN(loadCase->F_R) + " / (" + QN(loadCase->fE1)
                      + " * pi * " + QN(dE) + " * " + QN(eD)
                      + " * cos(" + QN(mShell->phiS) + "))", loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        loadCase->deltaR2 = loadCase->F_R / (loadCase->fE2 * M_PI * dE
                                             * eD * cos(mShell->phiS));
        PR->addDetail("Formula 133", "deltaR(" + QN(mFlangeNumber) + ")",
                      "F_R / (fE2 * PI * dE * eD * Cos(Shell.phiS))",
                      loadCase->deltaR2, "-",
                      QN(loadCase->F_R) + " / (" + QN(loadCase->fE2)
                      + " * pi * " + QN(dE) + " * " + QN(eD)
                      + " * cos(" + QN(mShell->phiS) + "))", loadCaseNo);
    }
}

/**
 * @brief Formula 134: Correction factor cM, if value is negative
 * then the hub is overstressed and the value can not be used!
 * @param loadCaseNo
 */
void Flange::Calc_cM(int loadCaseNo) {
    double val1 = 0.75;
    double val2 = 1.0;

    if (mShell->sType == mShell->Spherical) {
        val1 = 0.25;
        val2 = 3.0;
    }

    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_deltaQ = loadCase->deltaQ1;
    double tmp_deltaR = loadCase->deltaR1;

    if (getFlangeNumber() == 2)     {
        tmp_deltaQ = loadCase->deltaQ2;
        tmp_deltaR = loadCase->deltaR2;
    }

    double cM_root = 1.333 *(1 - 0.75 * pow(0.5 * tmp_deltaQ + tmp_deltaR, 2))
            * (1 - (val1 * pow(tmp_deltaQ, 2) + val2 * pow(tmp_deltaR, 2)));
    QString str = "(1.333 *(1 - 0.75 * (0.5 * " + QN(tmp_deltaQ) + " + "
            + QN(tmp_deltaR) + ") ^ 2 * (1 - (" + QN(val1) + " * "
            + QN(tmp_deltaQ) + " ^ 2 + " + QN(val2) + " * "
            + QN(tmp_deltaR) + " ^ 2))) ^ 0.5";
    if (cM_root >= 0) {
        if (getFlangeNumber() == 1) {
            loadCase->cM1 = pow(cM_root, 0.5);
            PR->addDetail("Formula 134", "cM(" + QN(mFlangeNumber) + ")",
                          "1.333 * (1 - 0.75 * (0.5 * deltaQ "
                          "+ deltaR) ^ 2) * (1 - (val * deltaQ ^ 2 "
                          "+ val2 * tmp_deltaR ^ 2))",
                          loadCase->cM1, "-", str, loadCaseNo);
        } else if (getFlangeNumber() == 2) {
            loadCase->cM2 = pow(cM_root, 0.5);
            PR->addDetail("Formula 134", "cM(" + QN(mFlangeNumber) + ")",
                          "1.333 * (1 - 0.75 * (0.5 * deltaQ "
                          "+ deltaR) ^ 2) * (1 - (val * deltaQ ^ 2 "
                          "+ val2 * deltaR ^ 2))",
                          loadCase->cM2, "-", str, loadCaseNo);
        }
    } else {
        if (getFlangeNumber() == 1) {
            loadCase->cM1 = cM_root;
            PR->addDetail("Formula 134", "cM(" + QN(mFlangeNumber) + ")",
                          "cM_root, be carefull, not a valid value!",
                          loadCase->cM1, "-", str, loadCaseNo);
        } else if (getFlangeNumber() == 2) {
            loadCase->cM2 = cM_root;
            PR->addDetail("Formula 134", "cM(" + QN(mFlangeNumber) + ")",
                          "cM_root, be carefull, not a valid value!",
                          loadCase->cM2, "-", str, loadCaseNo);
        }
    }
}

/**
 * @brief After Formula 134: Check whether hub is overloaded,
 * in that case cM is negative and this function returns False
 * @param loadCaseNo
 * @returns False in case of overloaded
 */
bool Flange::Is_cM_valid(int loadCaseNo) {
    bool result = false;

    if (getFlangeNumber() == 1) {
        result = mLoadCaseList->at(loadCaseNo)->cM1 >= 0.0;
        PR->addDetail("After_F. 134", "result(" + QN(mFlangeNumber) + ")",
                      "cM1 >= 0.0", static_cast<int>(result), "-",
                      QN(mLoadCaseList->at(loadCaseNo)->cM1) + " &gt;= 0.0",
                      loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        result = mLoadCaseList->at(loadCaseNo)->cM2 >= 0.0;
        PR->addDetail("After_F. 134", "result(" + QN(mFlangeNumber) + ")",
                      "cM2 >= 0.0", static_cast<int>(result), "-",
                      QN(mLoadCaseList->at(loadCaseNo)->cM2) + " &gt;= 0.0",
                      loadCaseNo);
    }

    return result;
}

/**
 * @brief Formula 135: Correction factor cS
 * @param loadCaseNo
 */
void Flange::Calc_cS(int loadCaseNo) {
    double val1 = 0.5;
    double val2 = 0.75;

    if (mShell->sType == mShell->Spherical) {
        val1 = 1.5;
        val2 = 0.25;
    }

    for (int jS = -1; jS <= 1; jS += 2) {
        LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
        double tmp_deltaQ = loadCase->deltaQ1;
        double tmp_deltaR = loadCase->deltaR1;

        if (getFlangeNumber() == 2) {
            tmp_deltaQ = loadCase->deltaQ2;
            tmp_deltaR = loadCase->deltaR2;
        }

        double cS_root = 1 - 0.75 *(pow((0.5 *tmp_deltaQ + tmp_deltaR), 2));
        double cS_tmp = 0;

        if (cS_root >= 0) {
            cS_tmp = M_PI / 4 * (pow(cS_root, 0.5) + jS
                                 * (val1 * tmp_deltaR - val2 * tmp_deltaQ));

            if (jS == -1) {
                if (getFlangeNumber() == 1) {
                    loadCase->cS_minus1 = cS_tmp;
                } else if (getFlangeNumber() == 2) {
                    loadCase->cS_minus2 = cS_tmp;
                }
            } else {
                if (getFlangeNumber() == 1) {
                    loadCase->cS_plus1 = cS_tmp;
                } else if (getFlangeNumber() == 2) {
                    loadCase->cS_plus2 = cS_tmp;
                }
            }
        } else {
            // be carefull, not  valid values!
            if (jS == -1) {
                if (getFlangeNumber() == 1) {
                    loadCase->cS_minus1 = cS_root;
                } else if (getFlangeNumber() == 2) {
                    loadCase->cS_minus2 = cS_root;
                }
            } else {
                if (getFlangeNumber() == 1) {
                    loadCase->cS_plus1 = cS_root;
                } else if (getFlangeNumber() == 2) {
                    loadCase->cS_plus2 = cS_root;
                }
            }
        }

        QString str = "pi / 4 * ((1 - 0.75 * (0.5 * " + QN(tmp_deltaQ) + " + "
                + QN(tmp_deltaR) + ") ^ 2) ^ 0.5" + QN(jS)
                + " * (" + QN(val1) + " * " + QN(tmp_deltaR) + " - "
                + QN(val2) + " * " + QN(tmp_deltaQ) + "))";

        if (getFlangeNumber() == 1) {
            PR->addDetail("Formula 135", "cS_minus(" + QN(mFlangeNumber) + ")",
                          "PI / 4 * ((1 - 0.75 *(0.5 * deltaQ + deltaR ^ 2) ^"
                          " 0.5 - (" + QN(val1) + " * deltaR - "
                          + QN(val2) + " * deltaQ))",
                          loadCase->cS_minus1, "-", str, loadCaseNo);
            PR->addDetail("Formula 135", "cS_plus(" + QN(mFlangeNumber) + ")",
                          "PI / 4 * ((1 - 0.75 *(0.5 * deltaQ + deltaR ^ 2) ^"
                          " 0.5 + (" + QN(val1) + " * deltaR - "
                          + QN(val2) + " * deltaQ))",
                          loadCase->cS_plus1, "-", str, loadCaseNo);
        } else if (getFlangeNumber() == 2) {
            PR->addDetail("Formula 135", "cS_minus(" + QN(mFlangeNumber) + ")",
                          "PI / 4 * ((1 - 0.75 *(0.5 * deltaQ + deltaR ^ 2) ^"
                          " 0.5 - (" + QN(val1) + " * deltaR - "
                          + QN(val2) + " * deltaQ))",
                          loadCase->cS_minus2, "-", str, loadCaseNo);
            PR->addDetail("Formula 135", "cS_plus(" + QN(mFlangeNumber) + ")",
                          "PI / 4 * ((1 - 0.75 *(0.5 * deltaQ + deltaR ^ 2) ^"
                          " 0.5 + (" + QN(val1) + " * deltaR - "
                          + QN(val2) + " * deltaQ))",
                          loadCase->cS_plus2, "-", str, loadCaseNo);
        }
    }
}

/**
 * @brief After Formula 135: Check validity of cS per flange,
 * \todo still valid to check?
 * @param loadCaseNo
 * @return True if cS is valid
 */
bool Flange::Is_cS_valid(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    bool result = false;
    if (getFlangeNumber() == 1) {
        result = loadCase->cS_minus1 >= 0.0 && loadCase->cS_plus1 >= 0.0;
        PR->addDetail("With_F. 135", "result(" + QN(mFlangeNumber) + ")",
                      "cS_minus1 >= 0.0 And cS_plus1 >= 0.0",
                      static_cast<int>(result), "-",
                      QN(loadCase->cS_minus1) + " &gt;= 0.0 AND "
                      + QN(loadCase->cS_plus1) + " &gt;= 0.0", loadCaseNo);
    } else {
        result = loadCase->cS_minus2 >= 0.0 && loadCase->cS_plus2 >= 0.0;
        PR->addDetail("With_F. 135", "result(" + QN(mFlangeNumber) + ")",
                      "cS_minus2 >= 0.0 And cS_plus2 >= 0.0",
                      static_cast<int>(result), "-",
                      QN(loadCase->cS_minus2) + " &gt;= 0.0 AND "
                      + QN(loadCase->cS_plus2) + " &gt;= 0.0", loadCaseNo);
    }

    return result;
}

/**
 * @brief Formula 136: Calculation of sign for moment
 * @param loadCaseNo
 */
void Flange::Calc_jM(int loadCaseNo) {
    double tmpVal = 0;

    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    tmpVal = loadCase->F_G * hG + loadCase->F_Q * (hH - hP) + loadCase->F_R * hH;
    tmpVal /= fabs(tmpVal);

    QString str = QN(loadCase->F_G) + " * " + QN(hG) + " + "
            + QN(loadCase->F_Q) + " * (" + QN(hH) + " - "
            + QN(hP) + ") + " + QN(loadCase->F_R) + " * " + QN(hH)
            + " / abs(" + QN(loadCase->F_G) + " * " + QN(hG) + " + "
            + QN(loadCase->F_Q) + " * (" + QN(hH) + " - "
            + QN(hP) + ") + " + QN(loadCase->F_R) + " * " + QN(hH) + ")";

    if (getFlangeNumber() == 1) {
        loadCase->jM1 = static_cast<int>(tmpVal);
        PR->addDetail("Formula 136", "jM(" + QN(mFlangeNumber) + ")",
                      "F_G * hG1 + F_Q * (hH1 - hP1) + F_R * hH1"
                      "/ abs (F_G * hG1 + F_Q * (hH1 - hP1) + F_R * hH1)",
                      loadCase->jM1, "-", str, loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        loadCase->jM2 = static_cast<int>(tmpVal);
        PR->addDetail("Formula 136", "jM(" + QN(mFlangeNumber) + ")",
                      "F_G * hG2 + F_Q * (hH2 - hP2) + F_R * hH2"
                      "/ abs (F_G * hG2 + F_Q * (hH2 - hP2) + F_R * hH2)",
                      loadCase->jM2, "-", str, loadCaseNo);
    }

}

/**
 * @brief Formula 140: Load ratio for different jS, kM and kS,
 * unit tested with Calc_PsiMax
 * @param loadCaseNo
 */
void Flange::Calc_Psi_jkk(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_fE = loadCase->fE1;
    double tmp_fF = loadCase->fF1;
    double tmp_deltaQ = loadCase->deltaQ1;
    double tmp_deltaR = loadCase->deltaR1;
    double tmp_cM = loadCase->cM1;
    double tmp_cS = 0.0;
    double tmp_kM = loadCase->kM1;

    if (loadCase->jS == 1){
        tmp_cS = loadCase->cS_plus1;
    } else if (loadCase->jS == -1) {
        tmp_cS = loadCase->cS_minus1;
    }

    if (getFlangeNumber() == 2) {
        tmp_fE = mLoadCaseList->at(loadCaseNo)->fE2;
        tmp_fF = mLoadCaseList->at(loadCaseNo)->fF2;
        tmp_deltaQ = loadCase->deltaQ2;
        tmp_deltaR = loadCase->deltaR2;
        tmp_cM = loadCase->cM2;
        tmp_kM = loadCase->kM2;

        if (loadCase->jS == 1) {
            tmp_cS = loadCase->cS_plus2;
        } else if (loadCase->jS == -1) {
            tmp_cS = loadCase->cS_minus2;
        }
    }

    double val1 = tmp_fE * dE * eD * cos(mShell->phiS)
            / (tmp_fF *2 * bF * eF);
    double val2 = (0.5 * tmp_deltaQ + tmp_deltaR)
            * tan(mShell->phiS) - (tmp_deltaQ * 2 * eP) / dE;
    double val3 = loadCase->jS * loadCase->kS
            * pow(eD * tmp_cM * tmp_cS * (1 + loadCase->jS * tmp_kM)
                   / (dE * pow(cos(mShell->phiS), 3)), 0.5);
    QString str = "fE * dE * eD * cos(phiS) / (fF *2 * bF * eF) "
                  "* ((0.5 * deltaQ + deltaR) * tan(phiS) - (deltaQ * 2 * eP) "
                  "/ dE + jS * kS * (eD * cM * cS * (1 + jS * kM) "
                  "/ (dE * cos(phiS) ^ 3)) ^ 0.5)";
    QString strVal = QN(tmp_fE) + " * " + QN(dE) + " * " + QN(eD) + " * cos("
            + QN(mShell->phiS) + ") / (" + QN(tmp_fF) + " * 2 * " + QN(bF)
            + " * " + QN(eF) + ") * ((0.5 * " + QN(tmp_deltaQ) + " + "
            + QN(tmp_deltaR) + ") * tan(" + QN(mShell->phiS) + ") - ("
            + QN(tmp_deltaQ) + " * 2 * " + QN(eP) + ") / " + QN(dE) + " + "
            + QN(loadCase->jS) + " * " + QN(loadCase->kS) + " * (" + QN(eD)
            + " * " + QN(tmp_cM) + " * " + QN(tmp_cS) + " * (1 + "
            + QN(loadCase->jS) + " * " + QN(tmp_kM) + ") / (" + QN(dE)
            + " * cos(" + QN(mShell->phiS) + ") ^ 3)) ^ 0.5)";

    double tmpPsi_jkk = val1 * (val2 + val3);

    if (getFlangeNumber() == 1) {
        loadCase->Psi_jkk1 = tmpPsi_jkk;
    } else if (getFlangeNumber() == 2) {
        loadCase->Psi_jkk2 = tmpPsi_jkk;
    }

    PR->addDetail("Formula 140", "Psi_jkk(" + QN(mFlangeNumber) + ")", str,
                  tmpPsi_jkk, "-", strVal, loadCaseNo);
}

/**
 * @brief Formula 141: Load ratio of flange ring due to radial force, optimum
 * @param loadCaseNo
 */
void Flange::Calc_PsiOpt(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmpPsiOpt = 0.0;
    double tmpjM = 0.0;

    if (getFlangeNumber() == 1) {
        tmpjM = loadCase->jM1;
        tmpPsiOpt = loadCase->jM1 * (2 * eP / eF - 1);
        loadCase->PsiOpt1 = tmpPsiOpt;
    } else if (getFlangeNumber() == 2) {
        tmpjM = loadCase->jM2;
        tmpPsiOpt = loadCase->jM2 * (2 * eP / eF - 1);
        loadCase->PsiOpt2 = tmpPsiOpt;
    }

    PR->addDetail("Formula 141", "PsiOpt(" + QN(mFlangeNumber) + ")",
                  "jM" + QN(getFlangeNumber()) + " * (2 * eP / eF - 1)",
                  tmpPsiOpt, "-", QN(tmpjM) + " * (2 * "
                  + QN(eP) + " / " + QN(eF) + " - 1)", loadCaseNo);
}

/**
 * @brief Formula 143: Load ratio of flange ring due to radial force, maximum
 * @param loadCaseNo
 */
void Flange::Calc_PsiMax(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->jS = 1;

    if (getFlangeNumber() == 1) {
        loadCase->kM1 = 1;
    } else if (getFlangeNumber() == 2) {
        loadCase->kM2 = 1;
    }

    loadCase->kS = 1;
    Calc_Psi_jkk(loadCaseNo);
    Calc_PsiMaxMin0(loadCaseNo);

    if (getFlangeNumber() == 1) {
        PR->addDetail("Formula 143", "PsiMax(" + QN(mFlangeNumber) + ")",
                      "Psi_jkk1(1,1,1)", loadCase->Psi_jkk1, "-",
                      QN(loadCase->Psi_jkk1), loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        PR->addDetail("Formula 143", "PsiMax(" + QN(mFlangeNumber) + ")",
                      "Psi_jkk2(1,1,1)", loadCase->Psi_jkk2, "-",
                      QN(loadCase->Psi_jkk2) , loadCaseNo);
    }
}

/**
 * @brief Formula 142: Load ratio of flange ring due to radial force,
 * jS, kM, kS all 0
 * @param loadCaseNo
 */
void Flange::Calc_Psi0(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->jS = 0;
    loadCase->kM1 = 0;
    loadCase->kM2 = 0;
    loadCase->kS = 0;
    Calc_Psi_jkk(loadCaseNo);
    Calc_PsiMaxMin0(loadCaseNo);

    if (getFlangeNumber() == 1) {
        PR->addDetail("Formula 142", "Psi0(" + QN(mFlangeNumber) + ")",
                      "Psi_jkk1(0,0,0)", loadCase->Psi_jkk1, "-",
                      QN(loadCase->Psi_jkk1), loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        PR->addDetail("Formula 142", "Psi0(" + QN(mFlangeNumber) + ")",
                      "Psi_jkk2(0,0,0)", loadCase->Psi_jkk2, "-",
                      QN(loadCase->Psi_jkk2) , loadCaseNo);
    }
}

/**
 * @brief Formula 144: Load ratio of flange ring due to radial force, minimum
 * @param loadCaseNo
 */
void Flange::Calc_PsiMin(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->jS = -1;

    if (getFlangeNumber() == 1) {
        loadCase->kM1 = -1;
    } else if (getFlangeNumber() == 2) {
        loadCase->kM2 = -1;
    }

    loadCase->kS = 1;
    Calc_Psi_jkk(loadCaseNo);
    Calc_PsiMaxMin0(loadCaseNo);

    if (getFlangeNumber() == 1) {
        PR->addDetail("Formula 144", "PsiMin(" + QN(mFlangeNumber) + ")",
                      "Psi_jkk1(-1,-1,+1)", loadCase->Psi_jkk1, "-",
                      QN(loadCase->Psi_jkk1), loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        PR->addDetail("Formula 144", "PsiMin(" + QN(mFlangeNumber) + ")",
                      "Psi_jkk2(-1,-1,+1)", loadCase->Psi_jkk2, "-",
                      QN(loadCase->Psi_jkk2) , loadCaseNo);
    }
}

/**
 * @brief With Formula 142 143 144: Sets the relevant PsiMax, Psi0 and PsiMin
 * per flange 1 or 2, unit tested with Calc_PsiMax
 * @param loadCaseNo
 */
void Flange::Calc_PsiMaxMin0(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (getFlangeNumber() == 1) {
        if (loadCase->jS == 1) {
            loadCase->PsiMax1 = loadCase->Psi_jkk1;
        } else if (loadCase->jS == -1) {
            loadCase->PsiMin1 = loadCase->Psi_jkk1;
        } else if (loadCase->jS == 0) {
            loadCase->Psi01 = loadCase->Psi_jkk1;
        } else {
            qDebug("Flange_Integral.Calc_PsiMaxMin0() jS ERROR");
        }
    } else if (getFlangeNumber() == 2) {
        if (loadCase->jS == 1) {
            loadCase->PsiMax2 = loadCase->Psi_jkk2;
        } else if (loadCase->jS == -1) {
            loadCase->PsiMin2 = loadCase->Psi_jkk2;
        } else if (loadCase->jS == 0) {
            loadCase->Psi02 = loadCase->Psi_jkk2;
        } else {
            qDebug("Flange_Integral.Calc_PsiMaxMin0() jS ERROR");
        }
    }
}

/**
 * @brief After Table 2: Assess whether load ratio PsiMax and PsiMin are valid
 * @param loadCaseNo
 * @return true if PsiMaxMin is valid
 */
bool Flange::Is_PsiMaxMin_Valid(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    bool result = false;

    if (getFlangeNumber() == 1) {
        result = loadCase->PsiMax1 >= -1 && loadCase->PsiMin1 <= 1;
        PR->addDetail("After_F. 144 Table 2", "result(" + QN(mFlangeNumber) + ")",
                      "PsiMax1 &gt;= -1 and PsiMin1 &lt;= 1",
                      static_cast<int>(result), "-", QN(loadCase->PsiMax1)
                      + " &gt;= -1 AND " + QN(loadCase->PsiMin1)
                      + " &lt;= 1", loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        result = loadCase->PsiMax2 >= -1 && loadCase->PsiMin2 <= 1;
        PR->addDetail("After_F. 144 Table 2", "result(" + QN(mFlangeNumber) + ")",
                      "PsiMax2 &gt;= -1 and PsiMin2 &lt;= 1",
                      static_cast<int>(result), "-", QN(loadCase->PsiMax2)
                      + " &gt;= -1 AND " + QN(loadCase->PsiMin2)
                      + " &lt;= 1", loadCaseNo);
    }

    return result;
}

/**
 * @brief After After Formula 144 and table 2 note d:
 * Load ratio PsiZ and kM per flange and calculates Wf
 * @param loadCaseNo
 */
void Flange::Calc_PsiZ(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_jM = 0;

    if (getFlangeNumber() == 1) {
        tmp_jM = loadCase->jM1;
    } else if (getFlangeNumber() == 2) {
        tmp_jM = loadCase->jM2;
    }

    if (getFlangeNumber() == 1) {
        if (tmp_jM == 1) {
            if (loadCase->PsiMax1 <= loadCase->PsiOpt1) {
                loadCase->kM1 = 1;
                loadCase->PsiZ1 = loadCase->PsiMax1;
                Calc_WF(loadCaseNo);
            } else if (loadCase->Psi01 <= loadCase->PsiOpt1
                       && loadCase->PsiOpt1 < loadCase->PsiMax1) {
                loadCase->kM1 = 1;
                loadCase->PsiZ1 = loadCase->PsiOpt1;
                Calc_WF(loadCaseNo);
            } else if (loadCase->PsiOpt1 < loadCase->Psi01) {
                // determine kM for maximum of WF
                // note: includes calculation for WF
                Calc_kM_for_WFmax(loadCaseNo);
            }
        } else if (tmp_jM == -1) {
            if (loadCase->PsiOpt1 <= loadCase->PsiMin1) {
                loadCase->kM1 = -1;
                loadCase->PsiZ1 = loadCase->PsiMin1;
                Calc_WF(loadCaseNo);
            } else if (loadCase->PsiMin1 < loadCase->PsiOpt1
                       && loadCase->PsiOpt1 <= loadCase->Psi01) {
                loadCase->kM1 = -1;
                loadCase->PsiZ1 = loadCase->PsiOpt1;
                Calc_WF(loadCaseNo);
            } else if (loadCase->Psi01 < loadCase->PsiOpt1) {
                // determine kM for maximum of WF (note: includes calculation for WF)
                Calc_kM_for_WFmax(loadCaseNo);
            }
        }
    } else if (getFlangeNumber() == 2) {
        if (tmp_jM == 1) {
            if (loadCase->PsiMax2 <= loadCase->PsiOpt2) {
                loadCase->kM2 = 1;
                loadCase->PsiZ2 = loadCase->PsiMax2;
                Calc_WF(loadCaseNo);
            } else if (loadCase->Psi02 <= loadCase->PsiOpt2
                       && loadCase->PsiOpt2 < loadCase->PsiMax2) {
                loadCase->kM2 = 1;
                loadCase->PsiZ2 = loadCase->PsiOpt2;
                Calc_WF(loadCaseNo);
            } else if (loadCase->PsiOpt2 < loadCase->Psi02) {
                // determine kM for maximum of WF
                // note: includes calculation for WF
                Calc_kM_for_WFmax(loadCaseNo);
            }
        } else if (tmp_jM == -1) {
            if (loadCase->PsiOpt2 <= loadCase->PsiMin2) {
                loadCase->kM2 = -1;
                loadCase->PsiZ2 = loadCase->PsiMin2;
                Calc_WF(loadCaseNo);
            } else if (loadCase->PsiMin2 < loadCase->PsiOpt2
                       && loadCase->PsiOpt2 <= loadCase->Psi02) {
                loadCase->kM2 = -1;
                loadCase->PsiZ2 = loadCase->PsiOpt2;
                Calc_WF(loadCaseNo);
            } else if (loadCase->Psi02 < loadCase->PsiOpt2) {
                // determine kM for maximum of WF (note: includes calculation for WF)
                Calc_kM_for_WFmax(loadCaseNo);
            }
        }
    }

    if (getFlangeNumber() == 1) {
        PR->addDetail("Before_F. 140 Table 2", "kM(" + QN(mFlangeNumber) + ")",
                      "para 8.4 note d", loadCase->kM1,
                      "Table 2", QN(loadCase->kM1), loadCaseNo);
        PR->addDetail("Before_F. 140 Table 2", "PsiZ(" + QN(mFlangeNumber) + ")",
                      "para 8.4 note d",
                      loadCase->PsiZ1, "-", QN(loadCase->PsiZ1), loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        PR->addDetail("Before_F. 140 Table 2", "kM(" + QN(mFlangeNumber) + ")",
                      "para 8.4 note d", loadCase->kM2,
                      "Table 2", QN(loadCase->kM2), loadCaseNo);
        PR->addDetail("Before_F. 140 Table 2", "PsiZ(" + QN(mFlangeNumber) + ")",
                      "para 8.4 note d",
                      loadCase->PsiZ2, "-", QN(loadCase->PsiZ2), loadCaseNo);
    }
}

/**
 * @brief Calculate factor kM for maximum of WF
 * @param loadCaseNo
 */
void Flange::Calc_kM_for_WFmax(int loadCaseNo) {
    double xMinInterval = -1.0;
    double xMaxInterval = 1.0;
    double xBeforeMax = -1.0; // associated with yBeforeMax
    double xAfterMax = 1.0; // associated with yAfterMax
    int xInterval = 10;
    double xIncrement = (xMaxInterval - xMinInterval) / xInterval;

    double yBeforeMax = 0.0;
    double yMidMax = 0.0;
    double yAfterMax = 0.0;
    int NumberOfLoops = 4; // accuracy of 0.2 ^ 4 = 0.0016
    int maxInnerLoops = 100;
    int counterInnerLoops = 0;

    double tmp_WF = 0.0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);


    for (int loopNo = 1; loopNo <= NumberOfLoops; loopNo++) {
        counterInnerLoops = 0;

        for (double xRunner = xMinInterval;
             xRunner <= xMaxInterval && counterInnerLoops++ < maxInnerLoops;
             xRunner += xIncrement) { // xIncrement should be positive
            // Calculation
            if (getFlangeNumber() == 1) {
                loadCase->kM1 = xRunner;
            } else if (getFlangeNumber() == 2) {
                loadCase->kM2 = xRunner;
            }

            Calc_Psi_jkk(loadCaseNo);

            if (getFlangeNumber() == 1) {
                loadCase->PsiZ1 = loadCase->Psi_jkk1;
            } else if (getFlangeNumber() == 2) {
                loadCase->PsiZ2 = loadCase->Psi_jkk2;
            }

            Calc_WF(loadCaseNo);

            // Set current y values
            yBeforeMax = yMidMax;
            yMidMax = yAfterMax;

            if (getFlangeNumber() == 1) {
                tmp_WF = loadCase->WF1;
            } else if (getFlangeNumber() == 2) {
                tmp_WF = loadCase->WF2;
            }

            yAfterMax = tmp_WF;

            // Set current x values
            xAfterMax = xRunner;
            xBeforeMax = xAfterMax - 2 * xIncrement;

            // Stop for-loop if passed highest point by setting
            // xRunner larger than xMaxInterval
            if (yMidMax == std::max(yMidMax, std::max(yBeforeMax, yAfterMax))) {
                xRunner = xMaxInterval * 1.1;
            }
        }

        // Set y back to below minimum
        yBeforeMax = std::min(yMidMax, std::min(yBeforeMax, yAfterMax)) - 1;
        yMidMax = yBeforeMax;
        yAfterMax = yBeforeMax;

        // Set reduced x-axis interval
        xMinInterval = xBeforeMax;
        xMaxInterval = xAfterMax;
        xIncrement = (xMaxInterval - xMinInterval) / xInterval;
    }
}

/**
 * @brief Formula 130: Resistance flange
 * @param loadCaseNo
 */
void Flange::Calc_WF(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_fE = loadCase->fE1;
    double tmp_fF = loadCase->fF1;
    double tmp_cM = loadCase->cM1;
    int tmp_jM = loadCase->jM1;
    double tmp_kM = loadCase->kM1;
    double tmp_PsiOpt = loadCase->PsiOpt1;
    double tmp_PsiZ = loadCase->PsiZ1;

    if (getFlangeNumber() == 2) {
        tmp_fE = loadCase->fE2;
        tmp_fF = loadCase->fF2;
        tmp_cM = loadCase->cM2;
        tmp_jM = loadCase->jM2;
        tmp_kM = loadCase->kM2;
        tmp_PsiOpt = loadCase->PsiOpt1;
        tmp_PsiZ = loadCase->PsiZ1;
    }

    double val1 = tmp_fF * 2 * bF *  pow(eF, 2);
    double val2 = 1 + 2 * tmp_PsiOpt * tmp_PsiZ - pow(tmp_PsiZ, 2);
    double val3 = tmp_fE * dE * pow(eD, 2) * tmp_cM * tmp_jM * tmp_kM;
    double tmp_WF = M_PI / 4 * (val1 * val2 + val3);

    QString flNr = QN(getFlangeNumber());
    QString forStr = "pi / 4 * (fF" + flNr + " * 2 * bF" + flNr
            + " *  (eF" + flNr + " ^ 2) * (1 + 2 * PsiOpt" + flNr
            + " * PsiZ" + flNr + " - (PsiZ" + flNr + " ^ 2)) + fE" + flNr
            + " * dE" + flNr + " * (eD" + flNr + " ^ 2) * cM" + flNr
            + " * jM" + flNr + " * kM" + flNr + ")";
    QString valStr = "pi / 4 * (" + QN(tmp_fF) + " * 2 * " + QN(bF)
            + " *  (" + QN(eF) + " ^ 2) * (1 + 2 * " + QN(tmp_PsiOpt)
            + " * " + QN(tmp_PsiZ) + " - (" + QN(tmp_PsiZ) + " ^ 2)) + "
            + QN(tmp_fE) + " * " + QN(dE) + " * (" + QN(eD) + " ^ 2) * "
            + QN(tmp_cM) + " * " + QN(tmp_jM) + " * " + QN(tmp_kM) + ")";

    if (getFlangeNumber() == 1) {
        loadCase->WF1 = tmp_WF;
        PR->addDetail("Formula 130", "WF(" + QN(mFlangeNumber) + ")",
                      forStr, loadCase->WF1, "Nmm", valStr, loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        loadCase->WF2 = tmp_WF;
        PR->addDetail("Formula 130", "WF(" + QN(mFlangeNumber) + ")",
                      forStr, loadCase->WF2, "Nmm", valStr, loadCaseNo);
    }
}

/**
 * @brief Formula 129: Load ratio for an integral flange stub or collar
 * @param loadCaseNo
 */
void Flange::Calc_PhiF(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_WF = loadCase->WF1;

    if (getFlangeNumber() == 2) {
        tmp_WF = loadCase->WF2;
    }

    double tmp_PhiF = (fabs(loadCase->F_G * hG + loadCase->F_Q
                           * (hH - hP) + loadCase->F_R * hH)) / tmp_WF;
    QString str = "(abs(" + QN(loadCase->F_G) + " * " + QN(hG) + " + "
            + QN(loadCase->F_Q) + " * (" + QN(hH) + " - " + QN(hP)
            + ") + " + QN(loadCase->F_R) + " * " + QN(hH) + ")) / "
            + QN(tmp_WF);

    if (getFlangeNumber() == 1) {
        loadCase->PhiF1 = tmp_PhiF;
        PR->addDetail("Formula 129", "PhiF(" + QN(mFlangeNumber) + ")",
                      "(abs(F_G * hG1 + F_Q * (hH1 - hP1) + F_R * hH1)) / WF1",
                      loadCase->PhiF1, "-", str, loadCaseNo);
    } else if (getFlangeNumber() == 2) {
        loadCase->PhiF2 = tmp_PhiF;
        PR->addDetail("Formula 129", "PhiF(" + QN(mFlangeNumber) + ")",
                      "(abs(F_G * hG2 + F_Q * (hH2 - hP2) + F_R * hH2)) / WF2",
                      loadCase->PhiF2, "-", str, loadCaseNo);
    }
}

void Flange::Calc_PhiX(int /*loadCaseNo*/) {
    // Formula 147: does nothing, blind only
}

/**
 * @brief After Formula 147: PhiX assessment
 * \todo: why here in not in blind flange
 * @param loadCaseNo
 * @returns true if PhiX is valid
 */
bool Flange::Is_PhiX_Valid(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    bool result = false;

    if (getFlangeNumber() == 1) {
        result = loadCase->PhiX1 <= 1.0;
        PR->addDetail("After_F. 147", "result(" + QN(mFlangeNumber) + ")",
                      "PhiX1 <= 1.0", static_cast<int>(result), "-",
                      QN(loadCase->PhiX1) + " <= 1.0", loadCaseNo);
    }
    else if (getFlangeNumber() == 2)
    {
        result = loadCase->PhiX2 <= 1.0;
        PR->addDetail("After_F. 147", "result(" + QN(mFlangeNumber) + ")",
                      "PhiX2 <= 1.0", static_cast<int>(result), "-",
                      QN(loadCase->PhiX2) + " <= 1.0", loadCaseNo);
    }

    return result;
}

void Flange::Calc_WX(int /*loadCaseNo*/) {
    // does nothing, blind only
}

void Flange::Calc_kM(int /*loadCaseNo*/) {
    // does nothing, integral, stub or collar only
}

bool Flange::Is_d0_Valid() {
    return false; // only for blind
}

void Flange::Calc_WL(int /*loadCaseNo*/) {
    // does nothing, loose flange only
}

void Flange::Calc_PhiL(int /*loadCaseNo*/) {
    // does nothing, loose flange only
}

bool Flange::Is_PhiL_valid(int /*loadCaseNo*/) {
    return false;
}

void Flange::Calc_WQ(int /*loadCaseNo*/) {
    // does nothing, loose flange only
}

END_NAMESPACE_BILUNA_CALC_EN1591
