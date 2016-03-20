#include "gasket.h"
#include "en13445tableg.h"
#include "en13555property.h"
#include "pcalc_report.h"
NAMESPACE_BILUNA_CALC_EN1591


Gasket_IN::Gasket_IN() : RB_Object("PCALC EN1591 Gasket"){
    gasketIdx = "";
    frmType = Flat;
    insType = SpiralGraphFillOuterInner;

    mNR = 0;
    mLeakageRate = 0;

    dG0 = 0;
    dGin = 0;
    dGout = 0;

    dG1_EN13555 = 0;
    dG2_EN13555 = 0;
    eGt = 0;
    muG = 0;

    K = 500000; // usual or 1,000,000 or 1,500,000 [N/mm]
    phiG = 0;
    r2 = 0;

}

Gasket_IN::~Gasket_IN() {
    // nothing
}

bool Gasket_IN::isMetalic() {
    switch (insType) {

    // Non metalic
    case Rubber :
        return false;
    case Ptfe :
        return false;
    case ExpPtfe :
        return false;
    case ExpGraphWithoutIns :
        return false;
    case ExpGraphPerfMetal :
        return false;
    case ExpGraphAdhFlat :
        return false;
    case ExpGraphMetSheet :
        return false;
    case NonAsbetosSmaller1 :
        return false;
    case NonAsbetosLargerEqual1 :
        return false;

    // Grooved steel gaskets with soft layers on both sides
    case PtfeSoftSteel :
        return false;
    case PtfeStainless :
        return false;
    case GraphSoftSteel :
        return false;
    case GraphLowAlloy :
        return false;
    case GraphStainless :
        return false;
    case SilverLayer :
        return false;

    // Spiral wound gaskets with soft filler
    case SpiralPtfeFillOuterOnly :
        return false;
    case SpiralPtfeFillOuterInner :
        return false;
    case SpiralGraphFillOuterOnly :
        return false;
    case SpiralGraphFillOuterInner :
        return false;

    // Solid metal gaskets
    case AluminumSoft :
        return true;
    case CopperOrBrassSoft :
        return true;
    case IronSoft :
        return true;
    case SteelSoft :
        return true;
    case SteelLowAlloyHeatRes :
        return true;
    case StainlessSteel :
        return true;
    case StainLessSteelHeatRes :
        return true;

    // Covered metal jacketed gaskets
    case SoftIronOrSteelJackGraphFillCover :
        return false;
    case LowAlloyOrStainlessSteelGraphFillCover :
        return false;
    case StainlessSteelJacketPtfeFillCover :
        return false;
    case NickelAlloyPtfeFillCover :
        return false;

    // Metal jacketed gaskets
    case AluminumSoftGraphFill :
        return false;
    case CopperOrBrassSoftGraphFill :
        return false;
    case SoftIronOrSteelGraphFill :
        return false;
    case LowAlloyOrStainlessSteelGraphFill :
        return false;
    default:
        return false;
    }
}

Gasket_OUT::Gasket_OUT() : Gasket_IN() {
    bGi = 0.0;
    bGiOct = 0.0;
    bGp = 0.0;
    bGt = 0.0;
    dGt = 0.0;
    AGt = 0.0;
    XG = 0.0;

    AGe = 0.0;
    AQ = 0.0;
    bGe = 0.0;
    dGe = 0.0;
}

Gasket::Gasket() : Gasket_OUT() {
    // nothing
}

Gasket::~Gasket() {
    // nothing
}

/**
 * @brief Formula 51: Theoretical gasket width
 * dG2 should be smaller or equal to Flange::dRF Raised Face
 * dG1 should be larger or equal to Flange::dREC Recess
 */
void Gasket::Calc_bGt() {
    bGt = 0.5 * (dG2 - dG1);
    PR->addDetail("Formula 51", "bGt", "0.5 * (dG2 - dG1)", bGt, "mm",
                  "0.5 * (" + QN(dG2) + " - " + QN(dG1) + ")");
}

/**
 * @brief Formula 52:Theoretical calculated gasket diameter,
 * if dG0 not set, will set dG0 = dGt
 * dG2 should be smaller or equal to Flange::dRF Raised Face
 * dG1 should be larger or equal to Flange::dREC Recess
 */
void Gasket::Calc_dGt() {
    dGt = 0.5 * (dG2 + dG1);
    PR->addDetail("Formula 52", "dGt", "0.5 * (dG2 + dG1)", dGt, "mm",
                  "0.5 * (" + QN(dG2) + " + " + QN(dG1) + ")");

    if (dG0 <= 0.0)     {
        dG0 = dGt;
    }
}

/**
 * @brief Formula 53: Theoretical gasket area
 */
void Gasket::Calc_AGt() {
    AGt = M_PI * bGt * dGt;
    PR->addDetail("Formula 53", "AGt", "Math.PI * bGt * dGt", AGt, "mm^2",
                  "pi * " + QN(bGt) + " + " + QN(dGt));
}

/**
 * @brief Formula 55: Effective gasket width, equal for all loadcases
 * because it is not recalculated refer 6.4.3
 * @param i load case number not used
 */
void Gasket::Calc_bGe() {
    bGe = std::min(bGi, bGt);
    PR->addDetail("Formula 55", "bGe", "Min(bGi, bGt)", bGe, "mm",
                  "min(" + QN(bGi) + "; " + QN(bGt) + ")");
}

/**
 * @brief Formula 56: Effective gasket area
 */
void Gasket::Calc_AGe() {
    AGe = M_PI * bGe * dGe;
    PR->addDetail("Formula 56", "AGe", "PI * bGe * dGe", AGe, "mm^2",
                  "pi * " + QN(bGe) + " * " + QN(dGe));
}

/**
 * @brief Formula 58: Gasket compressed elasticity at initial loading
 * @param loadCaseNo
 */
void Gasket::Calc_E_G(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->E_G = gasketCompressedElasticity(loadCaseNo);
    // report detail at method gasketCompressedElasticity()
}

/**
 * @brief With Formula 63: Value of compressed gasket thickness
 * @param loadCaseNo
 */
void Gasket::Calc_eG(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->eG = gasketCompressedThickness(loadCase);
    // report detail at method gasketCompressedThickness()
}

/**
 * @brief With Formula 65, 69, 70, 74, 75, gasket maximum load
 * @param loadCaseNo
 */
void Gasket::Calc_Q_smax(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->Q_smax = gasketMaximumLoad(loadCase);
}

/**
 * @brief Formula 63: Axial flexibility modulus of gasket
 */
void Gasket::Calc_XG() {
    XG = (mLoadCaseList->at(0)->eG / AGt)
            * ((bGt + mLoadCaseList->at(0)->eG / 2)
               / (bGe + mLoadCaseList->at(0)->eG / 2));
    PR->addDetail("Formula 63",
              "XG", "(eG / AGt) * ((bGt + eG / 2) / (bGe + eG / 2))",
              XG, "N/mm^2", "("+ QN(mLoadCaseList->at(0)->eG) + " / " + QN(AGt)
                  + ") * ((" + QN(bGt) + " + " + QN(mLoadCaseList->at(0)->eG)
                  + " / 2) / (" + QN(bGe) + " + " + QN(mLoadCaseList->at(0)->eG)
                  + " / 2))");
}

/**
 * @brief Formula 90: Effective area for the axial fluid-pressure force
 * @param loadCaseNo
 */
void Gasket::Calc_AQ() {
    AQ = pow(dGe, 2) * M_PI / 4;
    PR->addDetail("Formula 90", "AQ", "dGe ^ 2 * PI / 4", AQ, "mm^2",
                  QN(dGe) + " ^ 2 * pi / 4");
}

/**
 * Set LoadCase values, called by calculator before Calc_delta_eGc()
 * @param loadCase
 */
void Gasket::setLoadCaseValues(int loadCaseNo) {
    bool success = EN13555PROPERTY->setCurrentGasket(gasketIdx);

    if (!success) {
        RB_DEBUG->print("Gasket::setLoadCaseValues() set current gasket FALSE");
    }

    // Gasket Deflection, is method gasketDeflection() still necessary
    // get delta_eGc_EN13555 first
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->delta_eGc_EN13555 = gasketDeflection(loadCase);

    if (loadCase->delta_eGc_EN13555 > 0.0) {
        return;
    }

    // sets dG2_EN13555 and dG1_EN13555 if P_QR from EN13555
    loadCase->P_QR = gasketCreepFactor(loadCase);
}

/**
 * @brief Before Formula 103 and 104 Q_A or Qsmin,
 * including Annex/Table G: Minimum gasket force in assemblage
 * NOTE 1: if no leakage rate is requested use Q0,min and m (m * |P|)
 * from Annex G instead of resp. QA and Qsmin.
 * NOTE 2: Q_A can be set by user indirectly to a different value via F_Bspec
 */
void Gasket::Calc_Q_A_Qsmin(int loadCaseNo) {
    LoadCase* loadCase0 = mLoadCaseList->at(0); // assembly
    LoadCase* loadCase1 = mLoadCaseList->at(1); // test condition

    if (loadCaseNo == 0) {
        if (EN13555PROPERTY->isValid()) {
            loadCase0->Q_minL = EN13555PROPERTY->get_QminL(mLeakageRate,
                                                           loadCase1->P);
            PR->addDetail("Before_F. 103", "Q_minL", "EN13555 value",
                          loadCase0->Q_minL, "N/mm2", "Table value", loadCaseNo);
        }

        if (loadCase0->F_Bspec > 0) {
            // cannot be done in Calc_F_GInitial_1() because AGe is not known
            // F_G0 is set at Formula 1
            loadCase0->Q_A = loadCase0->F_G / AGe;
            PR->addDetail("Before_F. 103", "Q_A", "F_G / AGe",
                          loadCase0->Q_A, "N/mm2",
                          QN(loadCase0->F_G) + " / " + QN(AGe),
                          loadCaseNo, "User specified bolt force");
        } else {
            loadCase0->Q_A = loadCase0->Q_minL;

            if (loadCase0->Q_A > 0) {
                PR->addDetail("Before_F. 103", "Q_A", "Q_minL",
                              loadCase0->Q_A, "N/mm2", "Table value", loadCaseNo);
            } else {
                // Q_A EN13445 not found
                double Q0min = EN13445TABLEG->getTableG_Q0min(insType,
                                                              loadCase0->TG);
                loadCase0->Q_minL = Q0min;
                loadCase0->Q_A = Q0min;
                PR->addDetail("Before_F. 103", "Q_minL", "Q0min (EN13445 Table G)",
                              loadCase0->Q_minL, "N/mm2", "Table value", loadCaseNo);
                PR->addDetail("Before_F. 103", "Q_A", "Q0min (EN13445 Table G)",
                              loadCase0->Q_A, "N/mm2",
                              "Table value", loadCaseNo);
            }
        }
    } else {
        LoadCase* loadCaseI = mLoadCaseList->at(loadCaseNo);

        if (EN13555PROPERTY->isValid()) {
            loadCaseI->Q_sminL = EN13555PROPERTY->get_QsminL(mLeakageRate,
                                                             loadCase0->Q_A,
                                                             loadCase1->P);
        }

        if (loadCaseI->Q_sminL > 0) {
            PR->addDetail("Before_F. 104", "Q_sminL", "EN13555 value",
                          loadCaseI->Q_sminL, "N/mm2",
                          "Table value", loadCaseNo);
        } else {
            double m = EN13445TABLEG->getTableG_mI(insType, loadCaseI->TG);
            loadCaseI->Q_sminL = m * std::fabs(loadCaseI->P);
            PR->addDetail("Before_F. 104", "Q_sminL", "m * |P| (EN13445 Table G)",
                          loadCaseI->Q_sminL, "N/mm2",
                          QN(m) + " * abs(" + QN(loadCaseI->P) + ")",
                          loadCaseNo);
        }
    }
}


/**
 * @brief Before Formula 105 annex F: Creep of gasket
 * under seating pressure and temperature
 * @param loadCaseNo
 */
void Gasket::Calc_delta_eGc(int loadCaseNo) {
    LoadCase* loadCaseI = mLoadCaseList->at(loadCaseNo);
    LoadCase* loadCase0 = mLoadCaseList->at(0);
    // delta_eGc_EN13555 and P_QR set in setLoadCaseValues()

    if (loadCaseI->delta_eGc_EN13555 > 0) {
        // delta_eGc_EN13555 (=test) is available F.3
        loadCaseI->delta_eGc = K * loadCaseI->Y_G
                * loadCaseI->delta_eGc_EN13555;
        PR->addDetail("Before_F. 105 an. F.3",
                      "delta_eGc", "K * Y_G * delta_eGc_test",
                      loadCaseI->delta_eGc, "mm",
                      QN(K) + " * " + QN(loadCaseI->Y_G) + " * "
                      + QN(loadCaseI->delta_eGc_EN13555), loadCaseNo);
    } else if (loadCaseI->P_QR > 0 && dG2_EN13555 > 0 && dG1_EN13555 > 0) {
        // only P_QR is available from EN13555 F.2, text and F.3
        loadCaseI->delta_eGc = loadCaseI->Y_G * (M_PI / 4)
                * (pow(dG2_EN13555, 2) - pow(dG1_EN13555, 2))
                * loadCase0->Q_A * (1 - loadCaseI->P_QR);
        PR->addDetail("Before_F. 105 an. F.2, F.3",
                      "delta_eGc", "Y_G * (PI / 4) "
                      "* (dG2_EN13555 ^ 2 - dG1_EN13555 ^ 2) * Q_A * (1 - P_QR)",
                      loadCaseI->delta_eGc, "mm",
                      QN(loadCaseI->Y_G) + " * (pi / 4) * ("
                      + QN(dG2_EN13555) + " ^ 2 - "
                      + QN(dG1_EN13555) + " ^ 2) * "
                      + QN(loadCase0->Q_A) + " * (1 - "
                      + QN(loadCaseI->P_QR) + ")", loadCaseNo);
    } else {
        // no further gasket information available
        loadCaseI->delta_eGc = 0;
        PR->addDetail("Before_F. 105", "delta_eGc", "0.0",
                      loadCaseI->delta_eGc, "mm",
                      "No data available", loadCaseNo);
    }
}

double Gasket::gasketDeflection(LoadCase* loadCase) {
    double value = 0.0;

    if (EN13555PROPERTY->isValid()) {
        value = EN13555PROPERTY->get_deltaeGc(loadCase->Q_G, loadCase->TG);
        PR->addDetail("With_F. 105",
                      "delta_eGc_EN13555", "gasketDeflection(loadCase) EN13555",
                      value, "mm",
                      "Table value", loadCase->number);
    } else {
        PR->addDetail("With_F. 105",
                      "delta_eGc_EN13555", "0.0 gasketDeflection(loadCase)",
                      value, "mm",
                      "No data available", loadCase->number);
    }
    return value;
}

double Gasket::gasketCreepFactor(LoadCase* loadCase) {
    double value = 0.0;

    if (EN13555PROPERTY->isValid()) {
        value = EN13555PROPERTY->get_PQR(loadCase->Q_G, loadCase->TG);
        PR->addDetail("With_F. 105",
                      "P_QR", "gasketCreepFactor(loadCase) EN13555",
                      value, "-",
                      "Table value", loadCase->number);
    } else {
        PR->addDetail("With_F. 105",
                      "P_QR", "0.0 gasketCreepFactor(loadCase)",
                      value, "-",
                      "No data available", loadCase->number);
    }

    return value;
}

/**
 * @brief Gasket::gasketCompressedElasticity
 * Gasket modulus of elasticity
 * @param loadCaseNo loadcase number
 * @return gasket compressed elasticity
 */
double Gasket::gasketCompressedElasticity(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    LoadCase* loadCase0 = mLoadCaseList->at(0);
    double EG = 0.0;

    if (EN13555PROPERTY->isValid()) {
        // according code
        EG = EN13555PROPERTY->get_EG(loadCase0->Q_G, loadCase->TG);
        // TODO: better would be the following line but is not according code
        // double EG = EN13555PROPERTY->get_EG(loadCase->Q_G, loadCase->TG);
        PR->addDetail("Formula 58", "E_G", "EN13555 Table value",
                      EG, "N/mm2", "Table value", loadCaseNo);
    } else {
        // Simple EN13445 or EN1591 2001 version
        // E_G = E0 + K1 * Q(max) (Q from assembly, ref EN13445 after G.9-1)
        double E0 = EN13445TABLEG->getTableG_E0(insType, loadCase->TG);
        double K1 = EN13445TABLEG->getTableG_K1(insType, loadCase->TG);
        double Q = loadCase0->Q_G;
        EG = E0 + K1 * Q;

        PR->addDetail("F. 58 Table G", "E_G",
                      "E0 + K1 * Q (EN13445 Table G)",
                      EG, "N/mmm2", "Table value", loadCaseNo);
    }

    return EG;
}

/**
 * @brief Temporary solution for the calculation eG
 * @param loadCase
 * @return gasket compressed thickness
 */
double Gasket::gasketCompressedThickness(LoadCase* loadCase) {
    double eG = eGt;

    if (EN13555PROPERTY->isValid()) {
        eG = EN13555PROPERTY->get_eG(loadCase->Q_G, loadCase->TG);
        PR->addDetail("With_F. 63",
                      "eG", "gasketCompressedThickness(loadCase) EN13555",
                      eG, "mm", "Table value", loadCase->number);
    } else {
        PR->addDetail("With_F. 63",
                      "eG", "eGt gasketCompressedThickness(loadCase)",
                      eG, "mm", "No data available", loadCase->number);
    }

    return eG;
}

/**
 * @brief Maximum gasket load Q_smax EN13555 or Table 16
 * @param loadCase
 * @return gasket maximum load at temperature
 */
double Gasket::gasketMaximumLoad(LoadCase* loadCase) {
    double Qsmax = 0.0;

    if (EN13555PROPERTY->isValid()) {
        Qsmax = EN13555PROPERTY->get_Qsmax(loadCase->TG);
    }

    if (Qsmax > 0.0) {
        PR->addDetail("Before_F. 65 etc.", "Q_smax", "EN13555 Table value",
                      Qsmax, "N/mm2", "Table value", loadCase->number);
    } else {
        Qsmax = EN13445TABLEG->getTableG_Qmax(insType, loadCase->TG);
        PR->addDetail("Before_F. 65 etc.", "Q_smax", "EN13445 Table G",
                      Qsmax, "N/mm2", "Table value", loadCase->number);
    }

    return Qsmax;
}



END_NAMESPACE_BILUNA_CALC_EN1591
