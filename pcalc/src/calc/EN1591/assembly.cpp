#include "assembly.h"
#include "pcalc_report.h"
#include "table02_15property.h"
NAMESPACE_BILUNA_CALC_EN1591


Assembly_IN::Assembly_IN() : RB_Object() {
    setName("PCALC EN1591 Assembly");

    mBolt = NULL;
    mGasket = NULL;
    mFlange1 = NULL;
    mFlange2 = NULL;
    mLoadCaseList = NULL;
    mNR = 0.0;
    mF_Bspec = 0.0;
}

Assembly_IN::~Assembly_IN() {
    // nothing
}

Assembly_OUT::Assembly_OUT() : Assembly_IN() {
    mPhiMax = 0.0;
}

Assembly::Assembly() : Assembly_OUT() {
    // nothing
}

/**
 * @brief Formula 1 (54): Initial gasket force is specified
 */
void Assembly::Calc_F_GInitial_1() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (loadCase->F_Bspec <= 0) {
        RB_DEBUG->error("Assembly::Calc_F_GInitial_1() F_Bspec <= 0 ERROR");
    }

    loadCase->F_G = loadCase->F_Bspec * (1 - mBolt->eta1minus) - loadCase->F_R;
    PR->addDetail("Formula 54 (1)", "F_G", "F_Bspec * (1 - eta1minus) - F_R",
                  loadCase->F_G, "N", QN(loadCase->F_Bspec) + " * (1 - "
                  + QN(mBolt->eta1minus) + ") - " + QN(loadCase->F_R));
}

/**
 * @brief Formula 54: Initial gasket force
 * Bolt area * allowable design stress bolt / 3 -
 * net equivalent external force. Sets loadcase [-1] F_G.
 * Different from EN13445 Appendix GA
 */
void Assembly::Calc_F_GInitial() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (loadCase->F_Bspec > 0) {
        RB_DEBUG->error("Assembly::Calc_F_GInitial() F_Bspec > 0 ERROR");
    }

    //    loadCase->F_B = mBolt->AB * loadCase->fB / 3;
    loadCase->F_G = mBolt->AB * loadCase->fB / 3 - loadCase->F_R;
    PR->addDetail("Formula 54", "F_G", "AB * fB / 3 - F_R",
                  loadCase->F_G, "N",
                  QN(mBolt->AB) + " * " + QN(loadCase->fB)
                  + " / 3 - " + QN(loadCase->F_R));
}

/**
 * @brief Formula 68 71 73 76 in Table 1: Effective gasket diameter
 * @param loadCaseNo
 * @param gasket
 */
void Assembly::Calc_dGe() {
    if (mGasket->frmType == Gasket::Flat) {
        mGasket->dGe = mGasket->dG2 - mGasket->bGe;
        PR->addDetail("Formula 68", "dGe", "dG2 - bGe", mGasket->dGe, "mm",
                      QN(mGasket->dG2) + " - " + QN(mGasket->bGe));
    } else if (mGasket->frmType == Gasket::CurvedSimpleContact) {
        mGasket->dGe = mGasket->dG0;
        PR->addDetail("Formula 71", "dGe", "dG0", mGasket->dGe, "mm",
                      QN(mGasket->dG0));
    } else if (mGasket->frmType == Gasket::CurvedDoubleContact) {
        mGasket->dGe = mGasket->dGt;
        PR->addDetail("Formula 76", "dGe", "dGt", mGasket->dGe, "mm",
                      QN(mGasket->dGt));
    } else if (mGasket->frmType == Gasket::OctagonalDoubleContact) {
        mGasket->dGe = mGasket->dGt;
        PR->addDetail("Formula 73", "dGe", "dGt", mGasket->dGe, "mm",
                      QN(mGasket->dGt));
    }
}

/**
 * @brief Formula 64 - 76, Table 1: Effective gasket geometry
 * @param isFirstApproximation
 */
void Assembly::Calc_bGi(bool isFirstApproximation) {
    int loadCaseNo = 0;
    LoadCase* loadCase0 = mLoadCaseList->at(loadCaseNo);

    if (isFirstApproximation) {
        if (mGasket->frmType == Gasket::Flat) {
            mGasket->bGi = mGasket->bGt;
            PR->addDetail("Formula 64", "bGi", "bGt", mGasket->bGi, "mm",
                          QN(mGasket->bGt));
        } else if (mGasket->frmType == Gasket::CurvedSimpleContact) {
            mGasket->bGi = pow((6 * mGasket->r2 * cos(mGasket->phiG)
                                * mGasket->bGt * loadCase0->Q_smax
                                / loadCase0->E_G), 0.5);
            PR->addDetail("Formula 69", "bGi",
                          "(6 * r2 * Cos(phiG) * bGt * Q_smax / E_G) ^ 0.5",
                          mGasket->bGi, "mm",
                          "(6 * " + QN(mGasket->r2) + " * cos("
                          + QN(mGasket->phiG) + ") * " + QN(mGasket->bGt)
                          + " * " + QN(loadCase0->Q_smax) + " / "
                          + QN(loadCase0->E_G) + ") ^ 0.5");
        } else if (mGasket->frmType == Gasket::CurvedDoubleContact) {
            mGasket->bGi = pow((12 * mGasket->r2 * cos(mGasket->phiG)
                                * mGasket->bGt * loadCase0->Q_smax
                                / loadCase0->E_G), 0.5);
            PR->addDetail("Formula 74", "bGi",
                          "(12 * r2 * Cos(phiG) * bGt * Q_smax / E_G) ^ 0.5",
                          mGasket->bGi, "mm",
                          "(12 * " + QN(mGasket->r2) + " * cos("
                          + QN(mGasket->phiG) + ") * " + QN(mGasket->bGt)
                          + " * " + QN(loadCase0->Q_smax) + " / "
                          + QN(loadCase0->E_G) + ") ^ 0.5");
        } else if (mGasket->frmType == Gasket::OctagonalDoubleContact) {
            mGasket->bGi = mGasket->bGiOct;
            PR->addDetail("Formula 72", "bGi", "bGiOct", mGasket->bGi, "mm",
                          QN(mGasket->bGiOct));
        }
    } else {
        double bGp = loadCase0->F_G / (M_PI * mGasket->dGe
                                       * loadCase0->Q_smax);
        QString bGpStr = QN(loadCase0->F_G) + " / (PI *" + QN(mGasket->dGe)
                         + " * " + QN(loadCase0->Q_smax) + ")";
        if (mGasket->frmType == Gasket::Flat) {
            if (!mGasket->isMetalic()) {
                loadCase0->EGm = 0.5 * loadCase0->E_G;
            } else {
                loadCase0->EGm = loadCase0->E_G;
            }

            mGasket->bGi =
                    pow((loadCase0->eG
                          / (M_PI * mGasket->dGe * loadCase0->EGm))
                         / (mFlange1->hG * mFlange1->ZF / loadCase0->EF1
                             + mFlange2->hG * mFlange2->ZF / loadCase0->EF2)
                            + pow(bGp, 2), 0.5);
            PR->addDetail("Formula 65", "bGi",
                          "((eG / (PI * dGe * EGm)) / (hG1 * Flange1.ZF "
                          "/ EF1 + hG2 * Flange2.ZF / EF2) + (F_G / (PI * dGe "
                          "* Q_smax)) ^ 2) ^ 0.5", mGasket->bGi, "mm",
                          "((" + QN(loadCase0->eG)
                          + " / (PI * " + QN(mGasket->dGe)
                          + " * " + QN(loadCase0->EGm) + ")) / ("
                          + QN(mFlange1->hG) + " * " + QN(mFlange1->ZF)
                          + " / " + QN(loadCase0->EF1) + " + "
                          + QN(mFlange2->hG) + " * " + QN(mFlange2->ZF)
                          + " / " + QN(loadCase0->EF2) + ") + ("
                          + bGpStr + ") ^ 2) ^ 0.5",
                          loadCaseNo);

        } else if (mGasket->frmType == Gasket::CurvedSimpleContact) {
            //loadCase0->E_G = loadCase0->E0 + loadCase0->K1 * loadCase0->Qsmax

            mGasket->bGi =
                    pow((6 * mGasket->r2 * cos(mGasket->phiG) * loadCase0->F_G)
                         / (M_PI * mGasket->dGe * loadCase0->E_G)
                        + pow(bGp, 2), 0.5);
            PR->addDetail("Formula 70", "bGi",
                          "((6 * r2 * Cos(phiG) * F_G) "
                          "/ (PI * dGe * E_G) + (F_G / (PI * dGe * Q_smax)) "
                          "^ 2) ^ 0.5", mGasket->bGi, "mm",
                          "(6 * " + QN(mGasket->r2) + " * cos("
                          + QN(mGasket->phiG) + ") * " + QN(loadCase0->F_G)
                          + ") / (pi * " + QN(mGasket->dGe) + " * "
                          + QN(loadCase0->E_G) + ") + (" + bGpStr
                          + ") ^ 2) ^ 0.5");
        } else if (mGasket->frmType == Gasket::CurvedDoubleContact) {
            //loadCase0->E_G = loadCase0->E0 + loadCase0->K1 * loadCase0->Qsmax

            mGasket->bGi =
                    pow((12 * mGasket->r2 * cos(mGasket->phiG) * loadCase0->F_G)
                         / (M_PI * mGasket->dGe * loadCase0->E_G)
                         + pow(bGp, 2), 0.5);
            PR->addDetail("Formula 75", "bGi",
                          "((12 * r2 * Cos(phiG) * F_G) / (PI * dGe * E_G) "
                          "+ (F_G / (PI * dGe * Q_smax)) ^ 2) ^ 0.5",
                          mGasket->bGi, "mm",
                          "(12 * " + QN(mGasket->r2) + " * cos("
                          + QN(mGasket->phiG) + ") * " + QN(loadCase0->F_G)
                          + ") / (pi * " + QN(mGasket->dGe) + " * "
                          + QN(loadCase0->E_G) + ") + (" + bGpStr
                          + ") ^ 2) ^ 0.5");
        } else if (mGasket->frmType == Gasket::OctagonalDoubleContact) {
            mGasket->bGi = mGasket->bGiOct;
            PR->addDetail("Formula 72", "bGi", "bGiOct", mGasket->bGi, "mm",
                          QN(mGasket->bGiOct));
        }
    }
}

/**
 * @brief Formula 57: Initial gasket stress at assembly
 * and the gasket stress in case of other loadcases
 */
void Assembly::Calc_Q_G(int loadCaseNo) {
//    int loadCaseNo = 0;
    mLoadCaseList->at(loadCaseNo)->Q_G =
            mLoadCaseList->at(loadCaseNo)->F_G / mGasket->AGe;
    PR->addDetail("Formula 57", "Q_G", "F_G / AGe",
                  mLoadCaseList->at(loadCaseNo)->Q_G, "N/mm^2",
                  QN(mLoadCaseList->at(loadCaseNo)->F_G) + " / "
                  + QN(mGasket->AGe), loadCaseNo);
}

/**
 * @brief Formula 91: Axial pressure force
 * @param loadCaseNo
 */
void Assembly::Calc_F_Q(int loadCaseNo) {
    mLoadCaseList->at(loadCaseNo)->F_Q = mLoadCaseList->at(loadCaseNo)->P
            * mGasket->AQ;
    PR->addDetail("Formula 91", "F_Q", "P * AQ",
              mLoadCaseList->at(loadCaseNo)->F_Q, "N",
              QN(mLoadCaseList->at(loadCaseNo)->P) + " * "
              + QN(mGasket->AQ), loadCaseNo);
}

/**
 * @brief Formula 96: Equivalent resulting net force.
 * On side where moment induces a tensile load, the load limit of flange,
 * bolt or minimum gasket compression may govern.
 * On side where moment induces a compression load, the load limit
 * of the gasket may govern.
 * \todo: effect of compression or tensile not yet further implemented
 * @param loadCaseNo
 */
void Assembly::Calc_F_R(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->F_Rpositive = loadCase->mForce->F_Z + loadCase->mForce->M_AI
            * 4 / mFlange1->d3e;
    loadCase->F_Rnegative = loadCase->mForce->F_Z - loadCase->mForce->M_AI
            * 4 / mFlange1->d3e;

    PR->addDetail("Formula 96",
              "F_Rnegative", "F_Z - M_AI * 4 / d3e",
              loadCase->F_Rnegative, "N",
              QN(loadCase->mForce->F_Z) + " - "
              + QN(loadCase->mForce->M_AI) + " * 4 / " + QN(mFlange1->d3e),
              loadCaseNo);
    PR->addDetail("Formula 96",
              "F_Rpositive", "F_Z + M_AI * 4 / d3e",
              loadCase->F_Rpositive, "N",
              QN(loadCase->mForce->F_Z) + " + "
              + QN(loadCase->mForce->M_AI) + " * 4 / " + QN(mFlange1->d3e),
              loadCaseNo);

    // Set calculation value F_R
    if (fabs(loadCase->F_Rnegative) > fabs(loadCase->F_Rpositive)) {
        loadCase->F_R = loadCase->F_Rnegative;
        PR->addDetail("Formula 96",
                  "F_R", "F_Rnegative",
                  loadCase->F_R, "N",
                  QN(loadCase->F_Rnegative),
                  loadCaseNo);
    } else {
        loadCase->F_R = loadCase->F_Rpositive;
        PR->addDetail("Formula 96",
                  "F_R", "F_Rpositve",
                  loadCase->F_R, "N",
                  QN(loadCase->F_Rpositive),
                  loadCaseNo);
    }
}

/**
 * @brief Formula 97: Difference in thermal expansion bolt
 * and assembly (washers, gaskets, flanges)
 * @param loadCaseNo
 */
void Assembly::Calc_dUI(int loadCaseNo) {
    double tmp_T0 = mLoadCaseList->at(0)->T0;
    double tmp_eG = mLoadCaseList->at(0)->eG;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    loadCase->dUI = mBolt->lB * loadCase->alphaB * (loadCase->TB - tmp_T0)
        - tmp_eG * loadCase->alphaG * (loadCase->TG - tmp_T0)
        - mFlange1->eFt * loadCase->alphaF1 * (loadCase->TF1 - tmp_T0)
        - mFlange1->eL * loadCase->alphaL1 * (loadCase->TL1 - tmp_T0)
        - mFlange1->mWasher->eW * loadCase->alphaW1 * (loadCase->TW1 - tmp_T0)
        - mFlange2->eFt * loadCase->alphaF2 * (loadCase->TF2 - tmp_T0)
        - mFlange2->eL * loadCase->alphaL2 * (loadCase->TL2 - tmp_T0)
        - mFlange2->mWasher->eW * loadCase->alphaW2 * (loadCase->TW2 - tmp_T0);
    PR->addDetail("Formula 97",
              "dUI", "lB * alphaB * (TB - T0) - eG * alphaG * (TG - T0) "
              "- Flange1.eFt * alphaF1 * (TF1 - T0) - Flange1.eL * alphaL1 "
              "* (TL1 - T0) - Flange1.Washer.eW * alphaW1 * (TW1 - T0) "
              "- Flange2.eFt * alphaF2 * (TF2 - T0) - Flange2.eL * alphaL2 "
              "* (TL2 - T0) - Flange2.Washer.eW * alphaW2 * (TW2 - T0)",
              loadCase->dUI, "N",
              QN(mBolt->lB) + " * " + QN(loadCase->alphaB) + " * (" + QN(loadCase->TB) + " - " + QN(tmp_T0)
              + ") - " + QN(tmp_eG) + " * " + QN(loadCase->alphaG) + " * (" + QN(loadCase->TG) + " - " + QN(tmp_T0)
              + ") - " + QN(mFlange1->eFt) + " * " + QN(loadCase->alphaF1) + " * (" + QN(loadCase->TF1) + " - " + QN(tmp_T0)
              + ") - " + QN(mFlange1->eL) + " * " + QN(loadCase->alphaL1) + " * (" + QN(loadCase->TL1) + " - " + QN(tmp_T0)
              + ") - " + QN(mFlange1->mWasher->eW) + " * " + QN(loadCase->alphaW1) + " * (" + QN(loadCase->TW1) + " - " + QN(tmp_T0)
              + ") - " + QN(mFlange2->eFt) + " * " + QN(loadCase->alphaF2) + " * (" + QN(loadCase->TF2) + " - " + QN(tmp_T0)
              + ") - " + QN(mFlange2->eL) + " * " + QN(loadCase->alphaL2) + " * (" + QN(loadCase->TL2) + " - " + QN(tmp_T0)
              + ") - " + QN(mFlange2->mWasher->eW) + " * " + QN(loadCase->alphaW2) + " * (" + QN(loadCase->TW2) + " - " + QN(tmp_T0)
              + ")",
              loadCaseNo);
}

/**
 * @brief Formula 98: Axial bolt length
 */
void Assembly::Calc_lB() {
    mBolt->lB = mGasket->eGt
            + mFlange1->eFt + mFlange2->eFt
            + mFlange1->eL + mFlange2->eL
            + mFlange1->mWasher->eW + mFlange2->mWasher->eW;
    PR->addDetail("Formula 98",
              "lB", "eGt + eFt1 + eFt2 + eL1 + eL2 + eW1 + eW2",
              mBolt->lB, "mm",
              QN(mGasket->eGt)
              + " + " + QN(mFlange1->eFt) + " + " + QN(mFlange2->eFt)
              + " + " + QN(mFlange1->eL) + " + " + QN(mFlange2->eL)
              + " + " + QN(mFlange1->mWasher->eW) + " + "
              + QN(mFlange2->mWasher->eW));
}

/**
 * @brief Formula 99: Axial compliances of the joint (mm/N)
 * corresponding to load F_B
 * @param loadCaseNo
 */
void Assembly::Calc_YB(int loadCaseNo) {
    double tmpFlange1Val = 0.0;
    double tmpFlange2Val = 0.0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (dynamic_cast<Flange_Loose*>(mFlange1) != NULL) {
        tmpFlange1Val = mFlange1->ZL * (pow(mFlange1->hL, 2))
                / loadCase->EL1;
    }

    if (dynamic_cast<Flange_Loose*>(mFlange2) != NULL) {
        tmpFlange2Val = mFlange2->ZL * (pow(mFlange2->hL, 2))
                / loadCase->EL2;
    }

    loadCase->Y_B = tmpFlange1Val + tmpFlange2Val + mBolt->XB / loadCase->EB;

    if (mFlange1->mWasher->XW > 0 && loadCase->EW1 > 0) {
        loadCase->Y_B += mFlange1->mWasher->XW / loadCase->EW1;
    }

    if (mFlange2->mWasher->XW > 0 && loadCase->EW2 > 0) {
        loadCase->Y_B += mFlange2->mWasher->XW / loadCase->EW2;
    }

    PR->addDetail("Formula 99", "Y_B", "ZL1 * (hL1 ^ 2) "
              "/ EL1 + ZL2 * (hL2 ^ 2) / EL2 "
              "+ XB / EB + XW1 / EW1 + XW2 / EW2",
              loadCase->Y_B, "mm/N",
              QN(mFlange1->ZL) + " * " + QN(mFlange1->hL) + "^2 / "
                  + QN(loadCase->EL1) + " + "
              + QN(mFlange2->ZL) + " * " + QN(mFlange2->hL) + "^2 / "
                  + QN(loadCase->EL2) + " + "
              + QN(mBolt->XB) + " / " + QN(loadCase->EB) + " + "
              + QN(mFlange1->mWasher->XW) + " / " + QN(loadCase->EW1) + " + "
              + QN(mFlange2->mWasher->XW) + " / " + QN(loadCase->EW2),
              loadCaseNo);
}

/**
 * @brief Formula 100: Axial compliances of the joint (mm/N)
 * corresponding to load F_G
 * @param loadCaseNo
 */
void Assembly::Calc_YG(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->Y_G = mFlange1->ZF * (pow(mFlange1->hG, 2)) / loadCase->EF1
            + mFlange2->ZF * (pow(mFlange2->hG, 2)) / loadCase->EF2
            + loadCase->Y_B + mGasket->XG / loadCase->E_G;
    PR->addDetail("Formula 100", "Y_G", "Flange1.ZF * (hG1 ^ 2) / EF1 "
              "+ Flange2.ZF * (hG2 ^ 2) / EF2 + Y_B + XG / E_G",
              loadCase->Y_G, "mm/N",
              QN(mFlange1->ZF) + " * (" + QN(mFlange1->hG) + "^2) / "
                  + QN(loadCase->EF1) + " + "
              + QN(mFlange2->ZF) + " * (" + QN(mFlange2->hG) + "^2) / "
                  + QN(loadCase->EF2) + " + "
              + QN(loadCase->Y_B) + " + " + QN(mGasket->XG) + " / "
                  + QN(loadCase->E_G),
              loadCaseNo);
}

/**
 * @brief Formula 101: Axial compliances of the joint (mm/N)
 * corresponding to load F_Q
 * @param loadCaseNo
 */
void Assembly::Calc_YQ(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->Y_Q = mFlange1->ZF * mFlange1->hG
            * (mFlange1->hH - mFlange1->hP + mFlange1->hQ) / loadCase->EF1
            + mFlange2->ZF * mFlange2->hG
            * (mFlange2->hH - mFlange2->hP + mFlange2->hQ) / loadCase->EF2
            + loadCase->Y_B;
    PR->addDetail("Formula 101", "Y_Q", "Flange1.ZF * hG1 "
              "* (hH1 - Flange1.hP + Flange1.hQ) / EF1 "
              "+ Flange2.ZF * hG2 "
              "* (hH2 - Flange2.hP + Flange2.hQ) / EF2 + Y_B",
              loadCase->Y_Q, "mm/N",
              QN(mFlange1->ZF) + " * " + QN(mFlange1->hG) + " * ("
              + QN(mFlange1->hH) + " - " + QN(mFlange1->hP) + " + "
                  + QN(mFlange1->hQ) + ") / " + QN(loadCase->EF1) + " + "
              + QN(mFlange2->ZF) + " * " + QN(mFlange2->hG) + " * ("
              + QN(mFlange2->hH) + " - " + QN(mFlange2->hP) + " + "
                  + QN(mFlange2->hQ) + ") / " + QN(loadCase->EF2) + " + "
              + QN(loadCase->Y_B), loadCaseNo);
}

/**
 * @brief Formula 102: Axial compliances of the joint (mm/N)
 * corresponding to load F_R
 * @param loadCaseNo
 */
void Assembly::Calc_YR(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->Y_R = mFlange1->ZF * mFlange1->hG
            * (mFlange1->hH + mFlange1->hR) / loadCase->EF1
            + mFlange2->ZF * mFlange2->hG
            * (mFlange2->hH + mFlange2->hR) / loadCase->EF2 + loadCase->Y_B;
    PR->addDetail("Formula 102", "Y_R", "Flange1.ZF * hG1 * (hH1 + Flange1.hR) / EF1"
              " + Flange2.ZF * hG2 * (hH2 + Flange2.hR) / EF2 + Y_B",
              loadCase->Y_R, "mm/N",
              QN(mFlange1->ZF) + " * " + QN(mFlange1->hG) + " * ("
              + QN(mFlange1->hH) + " + " + QN(mFlange1->hR) + ") / " + QN(loadCase->EF1) + " + "
              + QN(mFlange2->ZF) + " * " + QN(mFlange2->hG) + " * ("
              + QN(mFlange2->hH) + " + " + QN(mFlange2->hR) + ") / " + QN(loadCase->EF2) + " + "
                  + QN(loadCase->Y_B), loadCaseNo);
}

/**
 * @brief Before Formula 103 and 104 Q_A or Qsmin,
 * including Annex/Table G: Minimum gasket force in assemblage
 * NOTE 1: if no leakage rate is requested use Q0,min and m (m * |P|)
 * from Annex G instead of resp. QA and Qsmin.
 * NOTE 2: Q_A can be set by user to a different value
 */
void Assembly::Calc_Q_A_Qsmin(int loadCaseNo) {
    LoadCase* loadCase0 = mLoadCaseList->at(0);

    if (loadCaseNo == 0) {
        if (loadCase0->Q_A > 0) {
            // TODO: User entered value, still necessary as comparison?
            return;
        } else if (loadCase0->F_Bspec > 0) {
            // cannot be done in Calc_F_GInitial_1() because AGe is not known
            // F_G0 is set at Formula 1
            loadCase0->Q_A = loadCase0->F_G / mGasket->AGe;
            PR->addDetail("Before_F. 103", "Q_A", "F_G / AGe",
                          loadCase0->Q_A, "N/mm2",
                          QN(loadCase0->F_G) + " / " + QN(mGasket->AGe),
                          loadCaseNo, "User present bolt force");
        } else {
            // original
            loadCase0->Q_A = TABLE02_15PROPERTY->getTableQA(mLeakageRate,
                                                            mGasket->matCode);
            if (loadCase0->Q_A >0) {
                PR->addDetail("Before_F. 103", "Q_A", "Table 2-15 value",
                              loadCase0->Q_A, "N/mm2", "Table value", loadCaseNo);
            } else {
                // Q_A not found
                TableGProperty* table = new TableGProperty(); // TODO: static class
                double Q0min = table->getTableG_Q0min(mGasket->insType);
                delete table;
                loadCase0->Q_A = Q0min;
                PR->addDetail("Before_F. 103", "Q_A", "Q0min (Table G)",
                              loadCase0->Q_A, "N/mm2",
                              QN(Q0min), loadCaseNo);
            }
        }
    } else {
        LoadCase* loadCaseI = mLoadCaseList->at(loadCaseNo);
        loadCaseI->Q_sminL
                = TABLE02_15PROPERTY->getTableQsminL(mLeakageRate,
                                                     mGasket->matCode,
                                                     loadCase0->Q_A);
        if (loadCaseI->Q_sminL > 0) {
            PR->addDetail("Before_F. 104", "Q_sminL", "Table 2-15 value",
                          loadCaseI->Q_sminL, "N/mm2",
                          "Table value", loadCaseNo);
        } else {
            TableGProperty* table = new TableGProperty(); // TODO: static class
            double m = table->getTableG_m(mGasket->insType);
            delete table;
            loadCaseI->Q_sminL = m * abs(loadCaseI->P);
            PR->addDetail("Before_F. 104", "Q_sminL", "m * |P| (Table G)",
                          loadCaseI->Q_sminL, "N/mm2",
                          QN(m) + " * abs(" + QN(loadCaseI->P) + ")",
                          loadCaseNo);
        }
    }
}

/**
 * @brief Formula 103, 104 and Annex/Table E
 * @param loadCaseNo
 */
void Assembly::Calc_F_Gmin(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (loadCaseNo == 0) {
        if (loadCase->Q_A > 0) {
            loadCase->F_Gmin = mGasket->AGe * loadCase->Q_A;
            PR->addDetail("Formula 103", "F_Gmin", "AGe * Q_A",
                          loadCase->F_Gmin, "N",
                          QN(mGasket->AGe) + " * " + QN(loadCase->Q_A),
                          loadCaseNo);
        } else {
            RB_DEBUG->error("Assembly::Calc_F_Gmin() Q_A <= 0 ERROR");
        }
    } else {
        double tmpVal1 = 0;
        QString tmpVal1Str = "";

        if (loadCase->Q_sminL > 0) {
            tmpVal1 = mGasket->AGe * loadCase->Q_sminL;
            tmpVal1Str = QN(mGasket->AGe) + " * " + QN(loadCase->Q_sminL);
        } else {
            RB_DEBUG->error("Assembly::Calc_F_Gmin() Q_sminL <= 0 ERROR");
        }

        double tmpVal2 = 0;
        QString tmpVal2Str = "";

        if (mGasket->frmType == Gasket::Flat) {
            tmpVal2 = -2 * loadCase->mForce->M_AI / mGasket->dGt;
            tmpVal2Str = "-2 * " + QN(loadCase->mForce->M_AI) + " / "
                    + QN(mGasket->dGt) + " ";
        }

        double muG = loadCase->muG;

        if (muG <= 0) {
            TableEProperty* table = new TableEProperty(); // TODO: static class
            muG = table->getTableE_muG(mGasket->insType);
            delete table;
        }

        tmpVal2 += loadCase->mForce->F_LI / muG
                + 2 * loadCase->mForce->M_Z / (muG * mGasket->dGt);
        tmpVal2Str += QN(loadCase->mForce->F_LI) + " / " + QN(muG) + " + 2 * "
                + QN(loadCase->mForce->M_Z) + " / (" + QN(muG) + " * "
                + QN(mGasket->dGt) + ")";

        double tmpVal3 = -(loadCase->F_Q + loadCase->F_R);
        QString tmpVal3Str = "-("+ QN(loadCase->F_Q) + " + "
                + QN(loadCase->F_R) + ")";
        loadCase->F_Gmin = std::max(std::max(tmpVal1, tmpVal2), tmpVal3);
        PR->addDetail("Formula 104", "F_Gmin",
                      "Max(Max(AGe * Q_sminL, "
                      "F_LI / muG + 2 * M_Z / (muG * dGt) - 2 * M_AI / dGt), "
                      "F_Q + F_R)",
                      loadCase->F_Gmin, "N", "max(max(" + tmpVal1Str + ", "
                      + tmpVal2Str + "), " + tmpVal3Str + ")",
                      loadCaseNo);
    }
}

/**
 * @brief Before Formula 105 annex F: Creep of gasket
 * under seating pressure and temperature
 * @param loadCaseNo
 */
void Assembly::Calc_delta_eGc(int loadCaseNo) {
    LoadCase* loadCaseI = mLoadCaseList->at(loadCaseNo);
    LoadCase* loadCase0 = mLoadCaseList->at(0);

    if (mGasket->dG2_EN13555 > 0
               && mGasket->dG1_EN13555 > 0
               && loadCaseI->P_QR > 0) {
        // F.2, text and F.3
        loadCaseI->delta_eGc = loadCaseI->Y_G * (M_PI / 4)
                * (pow(mGasket->dG2_EN13555, 2) - pow(mGasket->dG1_EN13555, 2))
                * loadCase0->Q_A * (1 - loadCaseI->P_QR);
        PR->addDetail("Before_F. 105 an. F",
                      "delta_eGc", "Y_G * (PI / 4) "
                      "* (dG2_EN13555 ^ 2 - dG1_EN13555 ^ 2) * Q_A * (1 - P_QR)",
                      loadCaseI->delta_eGc, "mm",
                      QN(loadCaseI->Y_G) + " * (pi / 4) * ("
                      + QN(mGasket->dG2_EN13555) + " ^ 2 - "
                      + QN(mGasket->dG1_EN13555) + " ^ 2) * "
                      + QN(loadCase0->Q_A) + " * (1 - "
                      + QN(loadCaseI->P_QR) + ")", loadCaseNo);
    } else {
        // no further gasket information available
        loadCaseI->delta_eGc = 0;
        PR->addDetail("Before_F. 105", "delta_eGc", "0",
                  loadCaseI->delta_eGc, "mm", "No data available", loadCaseNo);
    }
}

/**
 * @brief Formula 105: Required gasket force based on load conditions
 * @param loadCaseNo
 */
void Assembly::Calc_F_Gdelta(int loadCaseNo) {
    if (loadCaseNo == 1) {
        // first time formula 105 is called
        mLoadCaseList->at(0)->F_Gdelta = 0.0;
    }

    double tmpF_Gdelta = 0.0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    tmpF_Gdelta = (loadCase->F_Gmin * loadCase->Y_G
                   + loadCase->F_Q * loadCase->Y_Q
                   + (loadCase->F_R * loadCase->Y_R
                      - mLoadCaseList->at(0)->F_R * mLoadCaseList->at(0)->Y_R)
                   + loadCase->dUI + loadCase->delta_eGc)
            / mLoadCaseList->at(0)->Y_G;

    // Tuckmantel suggestion:
    //tmpF_Gdelta = (.F_Gmin * .Y_G + .F_Q * .Y_Q _
    //               + (.F_R * .Y_R - mLoadCaseList->at(0)->F_R _
    //               * mLoadCaseList->at(0)->Y_R) + .delta_eGc + .dUI) _
    //           / (mLoadCaseList->at(0)->Y_G * .Pqr)

    PR->addDetail("Formula 105",
                  "tmpF_Gdelta", "(F_Gmin * Y_G + F_Q * Y_Q "
                  "+ (F_R * Y_R - F_R * Y_R) + dUI + delta_eGc) / Y_G",
                  tmpF_Gdelta, "N",
                  "(" + QN(loadCase->F_Gmin) + " * " + QN(loadCase->Y_G)
                  + " + " + QN(loadCase->F_Q) + " * " + QN(loadCase->Y_Q)
                  +  " + (" + QN(loadCase->F_R) + " * " + QN(loadCase->Y_R)
                  + " - " + QN(mLoadCaseList->at(0)->F_R) + " * "
                  + QN(mLoadCaseList->at(0)->Y_R)
                  + ") + " + QN(loadCase->dUI) + " + " + QN(loadCase->delta_eGc)
                  + ") / " + QN(mLoadCaseList->at(0)->Y_G), loadCaseNo);
    mLoadCaseList->at(0)->F_Gdelta =
            std::max(mLoadCaseList->at(0)->F_Gdelta, tmpF_Gdelta);
    PR->addDetail("Formula 105",
                  "F_Gdelta", "Max(F_Gdelta, tmpF_Gdelta)",
                  mLoadCaseList->at(0)->F_Gdelta, "N",
                  "max(" + QN(mLoadCaseList->at(0)->F_Gdelta) + "; "
                  + QN(tmpF_Gdelta) + ")", loadCaseNo);
}

/**
 * @brief Formula 107: Required gasket force including tightness requirements
 */
void Assembly::Calc_F_G0req() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->F_Greq = std::max(loadCase->F_Gmin, loadCase->F_Gdelta);
    PR->addDetail("Formula 107", "F_Greq", "max(F_Gmin, F_Gdelta))",
                  loadCase->F_Greq, "N",
                  "max(" + QN(loadCase->F_Gmin) + "; "
                  + QN(loadCase->F_Gdelta) + ")", loadCaseNo);
}

/**
 * @brief Formula 108: Required bolt load in assemblage
 */
void Assembly::Calc_F_B0req() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->F_Breq = loadCase->F_Greq + loadCase->F_R;
    PR->addDetail("Formula 108", "F_Breq", "F_Greq + F_R",
                  loadCase->F_Breq, "N",
                  QN(loadCase->F_Greq) + " + " + QN(loadCase->F_R), loadCaseNo);
}

/**
 * @brief Formula 109: Assess F_G0actual versus F_G0required
 * @returns true if F_G0 larger F_G0req
 */
bool Assembly::Is_F_G0_larger_F_G0req() {
    bool result = false;
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    result = loadCase->F_G >= loadCase->F_Greq;
    PR->addDetail("Formula 109", "result109", "F_G >= F_Greq",
                  static_cast<int>(result), "-",
                  QN(loadCase->F_G) + " >= " + QN(loadCase->F_Greq),
                  loadCaseNo);
    return result;
}

/**
 * @brief Formula 110: F_G0actual larger F_G0required but within 0.1%
 * @returns true if F_G0actual larger F_G0required but within 0.1%
 */
bool Assembly::Is_F_G0act_within_0_1_percent_of_F_G0req() {
    bool result = false;
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    result = loadCase->F_Greq <= loadCase->F_G
            && loadCase->F_G <= loadCase->F_Greq * 1.001; // 0.1%
    PR->addDetail("Formula 110",
                  "result110", "F_Greq &lt;= F_G And F_G &lt;= F_Greq * 1.001",
                  static_cast<int>(result), "-",
                  QN(loadCase->F_Greq) + " &lt;= " + QN(loadCase->F_G)
                  + " &amp;&amp; " + QN(loadCase->F_G)
                  + " &lt;= " + QN(loadCase->F_Greq) + " * 1.001", loadCaseNo);
    return result;
}

/**
 * @brief Formula B.3: ManualOperatorFeel uncontrolled tightning
 */
void Assembly::Calc_F_B0av() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (mBolt->tType != Bolt::ManualStandardRing) {
        loadCase->F_Bav = 0.0;
        return;
    }

    loadCase->F_Bav = std::min(mBolt->AB * loadCase->fB,
                               (double)mFlange1->nB * 200000);
    PR->addDetail("Formula B.3",
                  "F_Bav", "Min(AB * fB, Flange1.nB * 200000)",
                  loadCase->F_Bav, "N",
                  "min(" + QN(mBolt->AB) + " * " + QN(loadCase->fB) + "; "
                  + QN(mFlange1->nB) + " * 200000)");
}

/**
 * @brief Formula 112 113 117 118: Required nominal bolt force,
 */
void Assembly::Calc_F_B0nom() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (mBolt->tType != Bolt::ManualStandardRing) {
        // Use required bolt as starting point
        loadCase->F_Bmin = loadCase->F_Breq;
        PR->addDetail("Formula 114", "F_Bmin", "F_Breq",
                      loadCase->F_Bmin, "N", QN(loadCase->F_Breq), loadCaseNo);
        loadCase->F_Bnom = loadCase->F_Breq / (1 - mBolt->etanminus);
        PR->addDetail("Formula 115", "F_Bnom", "F_Breq / (1 - etanminus)",
                      loadCase->F_Bnom, "N",
                      QN(loadCase->F_Breq) + " / (1 - "
                      + QN(mBolt->etanminus) + ")", loadCaseNo);
    } else {
        // Use uncontrolled standard ring, F_Bav already calculated
        // Formula B.2, 116, 0.5 is fixed e1+ and e1-
        double etanplusminus = 0.5 *(1 + 3 / (pow(mFlange1->nB, 0.5))) / 4;
        PR->addDetail("Formula B.2",
                      "etanminus", "0.5 *(1 + 3 / (nB ^ 0.5)) / 4",
                      etanplusminus, "-",
                      "0.5 *(1 + 3 / (" + QN(mFlange1->nB) + " ^ 0.5)) / 4");


        loadCase->F_Bnom = loadCase->F_Bav; // already calculated
        PR->addDetail("Formula 116",
                      "F_Bnom", "F_Bav",
                      loadCase->F_Bnom, "N",
                      QN(loadCase->F_Bav), loadCaseNo);


        loadCase->F_Bmin = loadCase->F_Bnom * (1 - etanplusminus);
        PR->addDetail("Formula 112", "F_Bmin", "F_Bnom * (1 - etanminus)",
                      loadCase->F_Bmin, "N",
                      QN(loadCase->F_Bnom) + " * (1 - "
                      + QN(etanplusminus) + ")", loadCaseNo);
    }

    loadCase->F_Bmax = loadCase->F_Bnom * (1 + mBolt->etanplus);
    PR->addDetail("Formula 113 117", "F_Bmax", "F_B * (1 + etanplus)",
                  loadCase->F_Bmax, "N",
                  QN(loadCase->F_B) + " * (1 + "
                  + QN(mBolt->etanplus) + ")", loadCaseNo);
}

/**
 * @brief Formula 111 114 115 116: Is F_B0nom greater or equal
 * to F_Bspec, F_Breq and F_Bav
 * @returns true if OK
 */
bool Assembly::Is_F_B0nom_Valid() {
    LoadCase* loadCase = mLoadCaseList->at(0);
    // if F_Bnom based F_Bav instead of F_Breq test against F_Bspec
    double tmpF_Bnom = loadCase->F_Bspec > 0.0 ?
                loadCase->F_Bspec : loadCase->F_Bnom;
    QString strF_Bnom = loadCase->F_Bspec > 0.0 ? "F_Bspec" : "F_Bnom";

    bool result = false;
    result = loadCase->F_Bmin <= tmpF_Bnom
            && tmpF_Bnom <= loadCase->F_Bmax; // 111
    PR->addDetail("Formula 111", "result111",
                  "F_Bmin =&lt; " + strF_Bnom + " AND " + strF_Bnom + " &lt;= F_Bmax",
                  static_cast<int>(result), "-", QN(loadCase->F_Bmin) + " &lt;= "
                  + QN(tmpF_Bnom) + " AND " + QN(tmpF_Bnom)
                  + " &lt;= " + QN(loadCase->F_Bmax));

    if (loadCase->F_Bspec > 0.0
            || ! (mBolt->tType == Bolt::ManualStandardRing)) {
        result = result
                && loadCase->F_Bnom >= loadCase->F_Breq
                / (1 - mBolt->etanminus); // 115
        PR->addDetail("Formula 115", "result115",
                      "result(111) AND F_Bnom &gt;= F_Breq / (1 - etanminus)",
                      static_cast<int>(result), "-",
                      QN((int)result) + " AND " + QN(loadCase->F_Bnom) + " &gt;= "
                      + QN(loadCase->F_Breq) + " / (1 - "
                      + QN(mBolt->etanminus) + ")");
    } else {
        double etanplusminus = 0.5 *(1 + 3 / (pow(mFlange1->nB, 0.5))) / 4;
        result = result && loadCase->F_Bnom >= loadCase->F_Breq
                / (1 - etanplusminus); // 116
        PR->addDetail("Formula 116", "result116",
                      "result(111) AND F_Bnom &gt;= F_Breq / (1 - 0.5 * (1 + 3 "
                      "/ (nB ^ 0.5)) / 4)",
                      static_cast<int>(result), "-",
                      QN((int)result) + " AND " + QN(loadCase->F_Bnom) + " &gt;= "
                      + QN(loadCase->F_Breq) + " / (1 - 0.5 * (1 + 3 / ("
                      + QN(mFlange1->nB) + " ^ 0.5)) / 4)");
    }

    result = result && loadCase->F_Bmin >= loadCase->F_Breq; // 114
    PR->addDetail("Formula 114", "result114",
                  "111 AND (115 OR 116) And F_Bmin &lt;= F_Breq",
                  static_cast<int>(result), "-",
                  QN((int)result) + " AND " + QN(loadCase->F_Bmin) + " &lt;= "
                  + QN(loadCase->F_Breq));

    return result;
}

/**
 * @brief Formula 118: Maximum gasket load for load limit calculation
 */
void Assembly::Calc_F_G0max() {
    LoadCase* loadCase = mLoadCaseList->at(0);
    loadCase->F_Gmax = loadCase->F_Bmax - loadCase->F_R;
    PR->addDetail("Formula 118", "F_Gmax", "F_Bmax - F_R",
                  loadCase->F_Gmax, "N",
                  QN(loadCase->F_Bmax) + " - " + QN(loadCase->F_R));
}

/**
 * @brief After Formula 122: calculate F_Bmax for load limit calculation
 * @param loadCaseNo
 */
void Assembly::Calc_F_Bmax(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->F_Bmax = loadCase->F_B * (1 + mBolt->etanplus);
    PR->addDetail("After_F. 122", "F_Bmax", "F_B * (1 + etanplus)",
                  loadCase->F_Bmax, "N",
                  QN(loadCase->F_B) + " * (1 + "
                  + QN(mBolt->etanplus) + ")", loadCaseNo);
}

/**
 * @brief After Formula 122: Maximum gasket load for load limit calculation
 * @param loadCaseNo
 */
void Assembly::Calc_F_Gmax(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->F_Gmax = loadCase->F_Bmax - loadCase->F_R;
    PR->addDetail("After_F. 122", "F_Gmax", "F_Bmax - F_R",
                  loadCase->F_Gmax, "N",
                  QN(loadCase->F_Bmax) + " - " + QN(loadCase->F_R), loadCaseNo);
}

/**
 * @brief Formula 2 (119): Increased gasket load due to more
 * than once assemblage (NR) and FBspecified > 0
 */
void Assembly::Calc_F_G0d_2() {
    LoadCase* loadCase = mLoadCaseList->at(0);
    double tmpF_G = loadCase->F_Bmin;
    QString varStr = "F_Bmin";
    QString forStr = "Formula 119 (2)";

    if (loadCase->F_Bspec <= 0.0) {
        // refer also to Formula 1,2,54,119
        RB_DEBUG->error("Assembly::Calc_F_G0d_2() F_Bspec <= 0.0 ERROR");
    }

    // TODO: the second argument does almost nothing
    loadCase->F_Gd = std::max(tmpF_G, (2.0 / 3.0) * (1.0 - 10.0 / mNR)
                              * loadCase->F_Bmax - loadCase->F_R);
    PR->addDetail(forStr, "F_Gd",
                  "max(" + varStr + ", (2 / 3) "
                  "* (1 - 10 / NR) * F_Bmax - F_R)",
                  loadCase->F_Gd, "N",
                  "max(" + QN(tmpF_G) + "; (2 / 3.0) * (1 - 10 / " + QN(mNR)
                  + ") * " + QN(loadCase->F_Bmax) + " - "
                  + QN(loadCase->F_R) + ")",
                  0);
}

/**
 * @brief Formula 119: Increased gasket load due to more
 * than once assemblage (NR) and FBspecified <= 0
 */
void Assembly::Calc_F_G0d() {
    LoadCase* loadCase = mLoadCaseList->at(0);
    double tmpF_G = loadCase->F_Gdelta;
    QString varStr = "F_Gdelta";
    QString forStr = "Formula 119";

    if (loadCase->F_Bspec > 0.0) {
        // refer also to Formula 1,2,54,119
        RB_DEBUG->error("Assembly::Calc_F_G0d() F_Bspec > 0.0 ERROR");
    }

    // TODO: the second argument does almost nothing
    loadCase->F_Gd = std::max(tmpF_G, (2.0 / 3.0) * (1.0 - 10.0 / mNR)
                              * loadCase->F_Bmax - loadCase->F_R);
    PR->addDetail(forStr, "F_Gd",
                  "max(" + varStr + ", (2 / 3) "
                  "* (1 - 10 / NR) * F_Bmax - F_R)",
                  loadCase->F_Gd, "N",
                  "max(" + QN(tmpF_G) + "; (2 / 3.0) * (1 - 10 / " + QN(mNR)
                  + ") * " + QN(loadCase->F_Bmax) + " - "
                  + QN(loadCase->F_R) + ")",
                  0);
}

/**
 * @brief Formula 120: Increased gasket force for load limit calculation
 * @param loadCaseNo
 */
void Assembly::Calc_F_G(int loadCaseNo) {
    LoadCase* loadCase0 = mLoadCaseList->at(0);
    LoadCase* loadCaseI = mLoadCaseList->at(loadCaseNo);
    loadCaseI->F_G = (loadCase0->F_Gd * loadCase0->Y_G
                      - (loadCaseI->F_Q * loadCaseI->Y_Q
                         + (loadCaseI->F_R * loadCaseI->Y_R
                            - loadCase0->F_R * loadCase0->Y_R) + loadCaseI->dUI)
                      - loadCaseI->delta_eGc) / loadCaseI->Y_G;
    PR->addDetail("Formula 120", "F_G",
                  "(F_Gd * Y_G - (F_Q * Y_Q + (F_R * Y_R "
                  "- F_R * Y_R) + dUI) - delta_eGc) / Y_G",
                  loadCaseI->F_G, "N",
                  "(" + QN(loadCase0->F_Gd) + " * " + QN(loadCase0->Y_G)
                  + " - (" + QN(loadCaseI->F_Q) + " * " + QN(loadCaseI->Y_Q)
                  + " + (" + QN(loadCaseI->F_R) + " * " + QN(loadCaseI->Y_R)
                  + " - " + QN(loadCase0->F_R) + " * " + QN(loadCase0->Y_R)
                  + ") + " + QN(loadCaseI->dUI) + ") - "
                  + QN(loadCaseI->delta_eGc)
                  + ") / " + QN(loadCaseI->Y_G), loadCaseNo);
}

/**
 * @brief Formula 122: Bolt force for load limit calculation in subsequent
 * load case numbers 1, 2, etc.
 * @param loadCaseNo
 */
void Assembly::Calc_F_B(int loadCaseNo) {
    if (loadCaseNo < 1) {
        RB_DEBUG->error("Assembly::Calc_F_B() loadCaseNo < 1 ERROR");
    }

    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->F_B = loadCase->F_G + (loadCase->F_Q + loadCase->F_R);
    PR->addDetail("Formula 122", "F_B", "F_G + (F_Q + F_R)", loadCase->F_B, "N",
                  QN(loadCase->F_G) + " + (" + QN(loadCase->F_Q) + " + "
                  + QN(loadCase->F_R) + ")", loadCaseNo);
}

/**
 * @brief Formula B.4
 */
void Assembly::Calc_Mtnom() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->Mtnom = mBolt->kB * mLoadCaseList->at(0)->F_Bnom / mFlange1->nB;
    PR->addDetail("Before_F. 123 B.4", "Mtnom", "kB * F_Bnom / nB",
                  loadCase->Mtnom, "Nmm", QN(mBolt->kB) + " * "
                  + QN(mLoadCaseList->at(loadCaseNo)->F_Bnom) + " / "
                  + QN(mFlange1->nB));
}

/**
 * @brief Formula B.9: Torsional moment during assembly in bolts,
 * only for loadcase = 0 with table B EN1591-2001 for ISO bolts/thread for pt
 */
void Assembly::Calc_MtBnom() {
    int loadCaseNo = 0;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->MtBnom = mBolt->kB9 * mLoadCaseList->at(0)->F_Bnom / mFlange1->nB;
    PR->addDetail("Before_F. 123 B.9", "MtBnom", "kB9 * F_Bnom / nB",
                  loadCase->MtBnom, "Nmm", QN(mBolt->kB9) + " * "
                  + QN(mLoadCaseList->at(loadCaseNo)->F_Bnom) + " / "
                  + QN(mFlange1->nB));
}

/**
 * @brief Formula 123: Load ratio for bolts
 * @param loadCaseNo
 */
void Assembly::Calc_PhiB(int loadCaseNo) {
    double torsionVal = 0.0;
    QString str1 = "";
    QString str2 = "";
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (loadCaseNo == 0) {
        torsionVal =
                3 * pow(loadCase->cA
                        * loadCase->MtBnom * (1 + mBolt->etanplus)
                        / mBolt->IB, 2);
        str1 = " + 3 * (cA * MtBnom * (1 + etanplus) / IB) ^ 2";
        str2 = " + 3 * (" + QN(loadCase->cA)
                + " * " + QN(loadCase->MtBnom)
                + " * (1 + " + QN(mBolt->etanplus)
                + ") / " + QN(mBolt->IB) + ") ^ 2";
    }

    loadCase->PhiB = 1 / (loadCase->fB * loadCase->cB)
            * pow((pow(loadCase->F_Bmax / mBolt->AB, 2) + torsionVal), 0.5);
    PR->addDetail("Formula 123", "PhiB",
                  "1 / (fB * cB) * ((F_Bmax / AB) ^ 2"
                  + str1 + ") ^ 0.5",
                  loadCase->PhiB, "-",
                  "1 / (" + QN(loadCase->fB) + " * " + QN(loadCase->cB)
                  + ") * ((" + QN(loadCase->F_Bmax) + " / " + QN(mBolt->AB)
                  + ") ^ 2" + str2 + ") ^ 0.5", loadCaseNo);
}

/**
 * @brief With Formula 123: Load ratio of bolts acceptance
 * @param loadCaseNo
 * @returns true if OK
 */
bool Assembly::Is_PhiB_Valid(int loadCaseNo) {
    bool result = mLoadCaseList->at(loadCaseNo)->PhiB <= 1.0;
    PR->addDetail("With_F. 123", "result123",
                  "PhiB <= 1.0", static_cast<int>(result), "-",
                  QN(mLoadCaseList->at(loadCaseNo)->PhiB) + " <= 1.0",
                  loadCaseNo);
    return result;
}

/**
 * @brief Formula 124 125 126: Correction factor for bolt tensioning,
 * when calculating load limits
 * @param loadCaseNo
 */
void Assembly::Calc_cA(int loadCaseNo) {
    if (mBolt->tType == Bolt::TensionerMeasureHydraulicPressure
            || mBolt->tType == Bolt::TensionerMeasureBoltElongation) {
        mLoadCaseList->at(loadCaseNo)->cA = 0.0;
        PR->addDetail("Formula 126", "cA", "0.0",
                      mLoadCaseList->at(loadCaseNo)->cA, "-",
                      "No torque on bolt",
                      loadCaseNo);
    } else {
        if (mBolt->ruptureElongationA >= 10) { // percent
            mLoadCaseList->at(loadCaseNo)->cA = 1.0;
            PR->addDetail("Formula 124", "cA", "1.0",
                          mLoadCaseList->at(loadCaseNo)->cA, "-",
                          QN(mBolt->ruptureElongationA) + " >= 10%",
                          loadCaseNo);
        } else {
            mLoadCaseList->at(loadCaseNo)->cA = 4 / 3.0;
            PR->addDetail("Formula 125", "cA", "4 / 3",
                      mLoadCaseList->at(loadCaseNo)->cA, "-",
                          QN(mBolt->ruptureElongationA) + " < 10%",
                          loadCaseNo);
        }
    }
}

/**
 * @brief Formula 127: Correction factor for unusual cases
 * @param loadCaseNo
 */
void Assembly::Calc_cB(int loadCaseNo) {
    double tmp_cB1 = Calc_cB_helper(mFlange1, loadCaseNo);
    double tmp_cB2 = Calc_cB_helper(mFlange2, loadCaseNo);
    mLoadCaseList->at(loadCaseNo)->cB = std::min(tmp_cB1, tmp_cB2);
    PR->addDetail("With_F. 127",
                  "cB", "min(cB1, cB2)",
                  mLoadCaseList->at(loadCaseNo)->cB, "-",
                  "min(" + QN(tmp_cB1) + "; " + QN(tmp_cB2) + ")", loadCaseNo);
}

/**
 * @brief With Formula 127: Helper for correction factor for unusual cases
 * @param flange
 * @param loadCaseNo
 * @return
 */
double Assembly::Calc_cB_helper(Flange* flange, int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    double tmp_fF = loadCase->fF1;

    if (flange->getFlangeNumber() == 2) {
        tmp_fF = loadCase->fF2;
    }

    double denom = 0.8 * mBolt->dB0 * loadCase->fB;
    double val = 0.0;

    if (dynamic_cast<Flange_Blind*>(flange) != NULL) {
        val = std::min(1.0, std::min(mBolt->eN * loadCase->fN / denom,
                                      mBolt->l5t * tmp_fF / denom));
        PR->addDetail("With_F. 127",
                      "cB" + QN(flange->getFlangeNumber()),
                      "min(1.0; eN * fN / (0.8 * dB0 * fB); "
                      "l5t * fF" + QN(flange->getFlangeNumber())
                      + " / (0.8 * dB0 * fB))",
                      val, "-",
                      "min(1.0; min(" + QN(mBolt->eN) + " * " + QN(loadCase->fN)
                      + " / (0.8 * " + QN(mBolt->dB0) + " * " + QN(loadCase->fB)
                      + "); " + QN(mBolt->l5t) + " * " + QN(tmp_fF)
                      + " / (0.8 * " + QN(mBolt->dB0) + " * " + QN(loadCase->fB)
                      + ")))", loadCaseNo);
    } else {
        val = std::min(1.0, mBolt->eN * loadCase->fN / denom);
        PR->addDetail("With_F. 127",
                      "cB" + QN(flange->getFlangeNumber()),
                      "min(1.0; eN * fN / (0.8 * dB0 * fB))",
                      val, "-",
                      "min(1.0; " + QN(mBolt->eN) + " * " + QN(loadCase->fN)
                      + " / (0.8 * " + QN(mBolt->dB0) + " * " + QN(loadCase->fB)
                      + ")) " , loadCaseNo);
    }

    return val;
}

/**
 * @brief Formula 128: Load ratio for the gasket
 * @param loadCaseNo
 */
void Assembly::Calc_PhiG(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->PhiG = loadCase->F_Gmax / (mGasket->AGt * loadCase->Q_smax);
    PR->addDetail("Formula 128", "PhiG", "F_Gmax / (AGt * Q_smax)",
                  loadCase->PhiG, "-",
                  QN(loadCase->F_Gmax) + " / (" + QN(mGasket->AGt)
                  + " * " + QN(loadCase->Q_smax) + ")", loadCaseNo);
}

/**
 * @brief With Formula 128: Load ratio of gasket acceptance
 * @param loadCaseNo
 * @returns true if OK
 */
bool Assembly::Is_PhiG_Valid(int loadCaseNo) {
    bool result = mLoadCaseList->at(loadCaseNo)->PhiG <= 1.0;
    PR->addDetail("With_F. 128", "result128", "PhiG &lt;= 1.0",
                  static_cast<int>(result), "-",
                  QN(mLoadCaseList->at(loadCaseNo)->PhiG) + " &lt;= 1.0",
                  loadCaseNo);
    return result;
}

/**
 * @brief Formula 131: Minimum allowable stress of flange and shell
 * @param flange
 * @param loadCaseNo
 */
void Assembly::Calc_fE(Flange *flange, int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (flange->getFlangeNumber() == 1)     {
        if (flange->mShell != NULL && flange->mShell->eS > 0)         {
            loadCase->fE1 = std::min(loadCase->fF1, loadCase->fS1);
            PR->addDetail("Formula 131", "fE(1)", "Min(fF1, fS1)",
                          loadCase->fE1, "N/mm2",
                          "min(" + QN(loadCase->fF1) + ", "
                          + QN(loadCase->fS1) + ")", loadCaseNo);
        } else {
            loadCase->fE1 = loadCase->fF1;
            PR->addDetail("Formula 131", "fE(1)", "fF1", loadCase->fE1, "N/mm2",
                          QN(loadCase->fF1), loadCaseNo);
        }
    } else if (flange->getFlangeNumber() == 2) {
        if (flange->mShell != NULL && flange->mShell->eS > 0) {
            loadCase->fE2 = std::min(loadCase->fF2, loadCase->fS2);
            PR->addDetail("Formula 131", "fE(2)", "Min(fF2, fS2)",
                          loadCase->fE2, "N/mm2",
                          "min(" + QN(loadCase->fF2) + ", "
                          + QN(loadCase->fS2) + ")", loadCaseNo);
        } else {
            loadCase->fE2 = loadCase->fF2;
            PR->addDetail("Formula 131", "fE(2)", "fF2", loadCase->fE2, "N/mm2",
                          QN(loadCase->fF2), loadCaseNo);
        }
    }
}

/**
 * @brief Formula C.3 C.4 C.9 C.10: Maximum bolt load for load limit calculation
 * @param loadCaseNo
 */
void Assembly::Calc_F_BImaxmin(int loadCaseNo) {
    LoadCase* loadCase0 = mLoadCaseList->at(0);
    LoadCase* loadCaseI = mLoadCaseList->at(loadCaseNo);

    if (loadCaseNo == 0) {
        loadCase0->F_BminA = loadCase0->F_Bnom * (1 - mBolt->etanminus);
        loadCase0->F_BmaxA = loadCase0->F_Bnom * (1 + mBolt->etanplus);
        PR->addDetail("After_F. 151 C.3", "F_BminA", "F_B0nom * (1 - etanminus)",
                      loadCase0->F_BminA, "N",
                      QN(loadCase0->F_Bnom) + " * (1 - "
                      + QN(mBolt->etanminus) + ")", loadCaseNo);
        PR->addDetail("After_F. 151 C.4", "F_BmaxA", "F_B0nom * (1 + etanplus)",
                      loadCase0->F_BmaxA, "N",
                      QN(loadCase0->F_Bnom) + " * (1 + "
                      + QN(mBolt->etanplus) + ")", loadCaseNo);
    } else {
        loadCaseI->F_BminA = loadCaseI->F_GminA
                + (loadCaseI->F_Q + loadCaseI->F_R);
        loadCaseI->F_BmaxA = loadCaseI->F_GmaxA
                + (loadCaseI->F_Q + loadCaseI->F_R);
        PR->addDetail("After_F. 151 C.9", "F_BminA", "F_GminA + (F_QI + F_RI)",
                      loadCaseI->F_BminA, "N",
                      QN(loadCaseI->F_GminA) + " + (" + QN(loadCaseI->F_Q)
                      + " + " + QN(loadCaseI->F_R) + ")", loadCaseNo);
        PR->addDetail("After_F. 151 C.10", "F_BmaxA", "F_GmaxA + (F_QI + F_RI)",
                      loadCaseI->F_BmaxA, "N",
                      QN(loadCaseI->F_GmaxA) + " + (" + QN(loadCaseI->F_Q)
                      + " + " + QN(loadCaseI->F_R) + ")", loadCaseNo);
    }

}

/**
 * @brief Formula C.5 C.6 C.7 C.8: Maximum gasket load
 * for load limit calculation
 * @param loadCaseNo
 */
void Assembly::Calc_F_GImaxmin(int loadCaseNo) {
    LoadCase* loadCase0 = mLoadCaseList->at(0);
    LoadCase* loadCaseI = mLoadCaseList->at(loadCaseNo);

    if (loadCaseNo == 0) {
        loadCaseI->F_GminA = loadCaseI->F_BminA - loadCaseI->F_R;
        loadCaseI->F_GmaxA = loadCaseI->F_BmaxA - loadCaseI->F_R;
        PR->addDetail("After_F. 151 C.5", "F_GminA", "F_B0minA - F_R",
                      loadCaseI->F_GminA, "N",
                      QN(loadCaseI->F_BminA) + " - " + QN(loadCaseI->F_R),
                      loadCaseNo);
        PR->addDetail("After_F. 151 C.6", "F_GmaxA", "F_B0maxA - F_R",
                      loadCaseI->F_GmaxA, "N",
                      QN(loadCaseI->F_BmaxA) + " - " + QN(loadCaseI->F_R),
                      loadCaseNo);
        loadCaseI->Q_GminA = loadCaseI->F_GminA / mGasket->AGe;
        loadCaseI->Q_GmaxA = loadCaseI->F_GmaxA / mGasket->AGe;
        PR->addDetail("After_F. 151 C.5", "Q_GminA", "F_G0minA / AGe",
                      loadCaseI->Q_GminA, "N/mm2",
                      QN(loadCaseI->F_GminA) + " / " + QN(mGasket->AGe),
                      loadCaseNo);
        PR->addDetail("After_F. 151 C.6", "Q_GmaxA", "F_G0maxA / AGe",
                      loadCaseI->Q_GmaxA, "N",
                      QN(loadCaseI->F_GmaxA) + " / " + QN(mGasket->AGe),
                      loadCaseNo);
    } else {
        double tmpF_G = loadCaseI->F_Q * loadCaseI->Y_Q +
                (loadCaseI->F_R * loadCaseI->Y_R - loadCase0->F_R
                 * loadCase0->Y_R) + loadCaseI->dUI;
        loadCaseI->F_GminA = (loadCase0->F_GminA * loadCase0->Y_G - tmpF_G
                              - loadCaseI->delta_eGc) / loadCaseI->Y_G;
        loadCaseI->F_GmaxA = (loadCase0->F_GmaxA * loadCase0->Y_G - tmpF_G
                              - loadCaseI->delta_eGc) / loadCaseI->Y_G;
        QString str = " - (" + QN(loadCaseI->F_Q) + " * " + QN(loadCaseI->Y_Q)
                + " + (" + QN(loadCaseI->F_R) + " * " + QN(loadCaseI->Y_R)
                + " - " + QN(loadCase0->F_R) + " * " + QN(loadCase0->Y_R)
                + ") + " + QN(loadCaseI->dUI) + ") - " + QN(loadCaseI->delta_eGc)
                + ") / " + QN(loadCaseI->Y_G);
        PR->addDetail("After_F. 151 C.7", "F_GminA", "(F_G0minA * Y_G0 - (F_QI * Y_QI "
                      "+ (F_RI * Y_RI - F_R0 * Y_R0) + dUI) - delta_eGcI) / Y_GI",
                      loadCaseI->F_GminA, "N", "(" + QN(loadCase0->F_GminA)
                      + " * " + QN(loadCase0->Y_G) + str, loadCaseNo);
        PR->addDetail("After_F. 151 C.8", "F_GmaxA", "(F_G0maxA * Y_G0 - (F_QI * Y_QI "
                      "+ (F_RI * Y_RI - F_R0 * Y_R0) + dUI) - delta_eGcI) / Y_GI",
                      loadCaseI->F_GmaxA, "N", "(" + QN(loadCase0->F_GmaxA)
                      + " * " + QN(loadCase0->Y_G) + str, loadCaseNo);
        loadCaseI->Q_GminA = loadCaseI->F_GminA / mGasket->AGe;
        loadCaseI->Q_GmaxA = loadCaseI->F_GmaxA / mGasket->AGe;
        PR->addDetail("After_F. 151 C.7", "Q_GminA", "F_GminA / AGe",
                      loadCaseI->Q_GminA, "N/mm2",
                      QN(loadCaseI->F_GminA) + " / " + QN(mGasket->AGe),
                      loadCaseNo);
        PR->addDetail("After_F. 151 C.8", "Q_GmaxA", "F_GmaxA / AGe",
                      loadCaseI->Q_GmaxA, "N",
                      QN(loadCaseI->F_GmaxA) + " / " + QN(mGasket->AGe),
                      loadCaseNo);
    }
}

/**
 * @brief Formula C.1: Rotation flange or stub/collar
 * @param loadCaseNo
 */
void Assembly::Calc_ThetaFmaxmin(int loadCaseNo) {
    double radToDeg = 180 / M_PI;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    double tmpFlange1 =
            loadCase->F_Q *(mFlange1->hH - mFlange1->hP + mFlange1->hQ)
            + loadCase->F_R *(mFlange1->hH + mFlange1->hR);
    double tmpFlange2 =
            loadCase->F_Q *(mFlange2->hH - mFlange2->hP + mFlange2->hQ)
            + loadCase->F_R *(mFlange2->hH + mFlange2->hR);
    loadCase->ThetaF1min = mFlange1->ZF / loadCase->EF1
            * (loadCase->F_GminA * mFlange1->hG + tmpFlange1);
    loadCase->ThetaF1max = mFlange1->ZF / loadCase->EF1
            * (loadCase->F_GmaxA * mFlange1->hG + tmpFlange1);
    loadCase->ThetaF2min = mFlange2->ZF / loadCase->EF2
            * (loadCase->F_GminA * mFlange2->hG + tmpFlange2);
    loadCase->ThetaF2max = mFlange2->ZF / loadCase->EF2
            * (loadCase->F_GmaxA * mFlange2->hG + tmpFlange2);
    QString str1 = QN(loadCase->F_Q) + " *(" + QN(mFlange1->hH) + " - "
            + QN(mFlange1->hP) + " + " + QN(mFlange1->hQ) + ") + "
            + QN(loadCase->F_R) + " *(" + QN(mFlange1->hH) + " + "
            + QN(mFlange1->hR) + ")";
    QString str2 = QN(loadCase->F_Q) + " *(" + QN(mFlange2->hH) + " - "
            + QN(mFlange2->hP) + " + " + QN(mFlange2->hQ) + ") + "
            + QN(loadCase->F_R) + " *(" + QN(mFlange2->hH) + " + "
            + QN(mFlange2->hR) + ")";
    PR->addDetail("After_F. 151 C.1", "ThetaF1min",
                  "radToDeg * ZF1 / EF1 * (F_GminA * hG1 "
                  "+ F_Q * (hH1 - hP1 + hQ1) + F_R * (hH1 + hR1))",
                  radToDeg * loadCase->ThetaF1min, "degree",
                  QN(radToDeg) + " * " + QN(mFlange1->ZF) + " / "
                  + QN(loadCase->EF1) + " * (" + QN(loadCase->F_GminA) + " * "
                  + QN(mFlange1->hG) + " + " + str1 + ")", loadCaseNo);
    PR->addDetail("After_F. 151 C.1", "ThetaF1max",
                  "radToDeg * ZF1 / EF1 * (F_GmaxA * hG1 "
                  "+ F_Q * (hH1 - hP1 + hQ1) + F_R * (hH1 + hR1))",
                  radToDeg * loadCase->ThetaF1max, "degree",
                  QN(radToDeg) + " * " + QN(mFlange1->ZF) + " / "
                  + QN(loadCase->EF1) + " * (" + QN(loadCase->F_GmaxA) + " * "
                  + QN(mFlange1->hG) + " + " + str1 + ")", loadCaseNo);
    PR->addDetail("After_F. 151 C.2", "ThetaF2min",
                  "radToDeg * ZF2 / EF2 * (F_GminA * hG2 "
                  "+ F_Q * (hH2 - hP2 + hQ2) + F_R * (hH2 + hR2))",
                  radToDeg * loadCase->ThetaF2min, "degree",
                  QN(radToDeg) + " * " + QN(mFlange2->ZF) + " / "
                  + QN(loadCase->EF2) + " * (" + QN(loadCase->F_GminA) + " * "
                  + QN(mFlange2->hG) + " + " + str2 + ")", loadCaseNo);
    PR->addDetail("After_F. 151 C.2", "ThetaF2max",
                  "radToDeg * ZF2 / EF2 * (F_GmaxA * hG2 "
                  "+ F_Q * (hH2 - hP2 + hQ2) + F_R * (hH2 + hR2))",
                  radToDeg * loadCase->ThetaF2max, "degree",
                  QN(radToDeg) + " * " + QN(mFlange2->ZF) + " / "
                  + QN(loadCase->EF2) + " * (" + QN(loadCase->F_GmaxA) + " * "
                  + QN(mFlange2->hG) + " + " + str2 + ")", loadCaseNo);
}

/**
 * @brief Formula C.2: Rotation loose flange
 * @param loadCaseNo
 */
void Assembly::Calc_ThetaLmaxmin(int loadCaseNo) {
    double radToDeg = 180 / M_PI;
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);

    if (dynamic_cast<Flange_Loose*>(mFlange1) != NULL)     {
        loadCase->ThetaL1min = mFlange1->ZL / loadCase->EL1
                * (loadCase->F_BminA * mFlange1->hL);
        loadCase->ThetaL1max = mFlange1->ZL / loadCase->EL1
                * (loadCase->F_BmaxA * mFlange1->hL);
        PR->addDetail("After_F. 151 C.2", "ThetaL1min",
                      "radToDeg * ZL1 / EL1 * (F_BminA * hL1)",
                      radToDeg * loadCase->ThetaL1min, "degree",
                      QN(radToDeg) + " * " + QN(mFlange1->ZL) + " / "
                      + QN(loadCase->EL1) + " * (" + QN(loadCase->F_BminA)
                      + " * " + QN(mFlange1->hL) + ")", loadCaseNo);
        PR->addDetail("After_F. 151 C.2", "ThetaL1max",
                      "radToDeg * ZL1 / EL1 * (F_BmaxA * hL1)",
                  radToDeg * loadCase->ThetaL1max, "degree",
                      QN(radToDeg) + " * " + QN(mFlange1->ZL) + " / "
                      + QN(loadCase->EL1) + " * (" + QN(loadCase->F_BmaxA)
                      + " * " + QN(mFlange1->hL) + ")", loadCaseNo);
    } else {
        loadCase->ThetaL1min = 0.0;
        loadCase->ThetaL1max = 0.0;
        PR->addDetail("After_F. 151 C.2", "ThetaL1min", "0.0",
                  radToDeg * loadCase->ThetaL1min, "degree", "0.0", loadCaseNo);
        PR->addDetail("After_F. 151 C.2", "ThetaL1max", "0.0",
                  radToDeg * loadCase->ThetaL1max, "degree", "0.0", loadCaseNo);
    }

    if (dynamic_cast<Flange_Loose*>(mFlange2) != NULL)     {
        loadCase->ThetaL2min = mFlange2->ZL / loadCase->EL2
                * (loadCase->F_BminA * mFlange2->hL);
        loadCase->ThetaL2max = mFlange2->ZL / loadCase->EL2
                * (loadCase->F_BmaxA * mFlange2->hL);
        PR->addDetail("After_F. 151 C.2",
                  "ThetaL2min", "radToDeg * ZL2 / EL2 * (F_BminA * hL2)",
                  radToDeg * loadCase->ThetaL2min, "degree",
                      QN(radToDeg) + " * " + QN(mFlange2->ZL) + " / "
                      + QN(loadCase->EL2) + " * (" + QN(loadCase->F_BminA)
                      + " * " + QN(mFlange2->hL) + ")", loadCaseNo);
        PR->addDetail("After_F. 151 C.2",
                  "ThetaL2max", "radToDeg * ZL2 / EL2 * (F_BmaxA * hL2)",
                  radToDeg * loadCase->ThetaL2max, "degree",
                      QN(radToDeg) + " * " + QN(mFlange2->ZL) + " / "
                      + QN(loadCase->EL2) + " * (" + QN(loadCase->F_BmaxA)
                      + " * " + QN(mFlange2->hL) + ")", loadCaseNo);
    } else {
        loadCase->ThetaL2min = 0.0;
        loadCase->ThetaL2max = 0.0;
        PR->addDetail("After_F. 151 C.2", "ThetaL2min", "0.0",
                  radToDeg * loadCase->ThetaL2min, "degree", "0.0", loadCaseNo);
        PR->addDetail("After_F. 151 C.2", "ThetaL2max", "0.0",
                  radToDeg * loadCase->ThetaL2max, "degree", "0.0", loadCaseNo);
    }
}

END_NAMESPACE_BILUNA_CALC_EN1591
