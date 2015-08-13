#include "gasket.h"
#include "pcalc_report.h"
#include "table16property.h"
#include "tablegsimple.h"
NAMESPACE_REDBAG_CALC_EN1591


Gasket_IN::Gasket_IN() : RB_Object(){
    setName("PCALC EN1591 Gasket");

    dG0 = 0;
    dG1 = 0;
    dG2 = 0;
    dG1_EN13555 = 0;
    dG2_EN13555 = 0;
    eGt = 0;
    matCode = "";
    frmType = Flat;
    insType = SpiralGraphFillOuterInner;
    K = 0;
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
    mTable16Property = new Table16Property();
    mTableGSimple = new TableGSimple();
}

Gasket::~Gasket() {
    delete mTable16Property;
    mTable16Property = NULL;
    delete mTableGSimple;
    mTableGSimple = NULL;
}

/**
 * @brief Formula 51: Theoretical gasket width
 */
void Gasket::Calc_bGt() {
    bGt = 0.5 * (dG2 - dG1);
    PR->addDetail("Formula 51", "bGt", "0.5 * (dG2 - dG1)", bGt, "mm");
}

/**
 * @brief Formula 52:Theoretical calculated gasket diameter,
 * if dG0 not set, will set dG0 = dGt
 */
void Gasket::Calc_dGt() {
    dGt = 0.5 * (dG2 + dG1);
    PR->addDetail("Formula 52", "dGt", "0.5 * (dG2 + dG1)", dGt, "mm");

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
 * @brief Formula 55: Effective gasket width,
 * set for all loadcases because it is not recalculated refer 6.4.3
 * @param i load case number not used
 */
void Gasket::Calc_bGe(int /* loadCaseNo */) {
    bGe = std::min(bGi, bGt);
    PR->addDetail("Formula 55", "bGe", "Min(bGi, bGt)", bGe, "mm",
                  "min(" + QN(bGi) + "; " + QN(bGt) + ")");
}

/**
 * @brief Formula 56: Effective gasket area, loadcase number is only 0
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
    mLoadCaseList->at(loadCaseNo)->E_G
            = gasketCompressedElasticity(mLoadCaseList->at(loadCaseNo));

    // TODO move addDetail() to relevant table
    PR->addDetail("Formula 58",
              "E_G", "gasketCompressedElasticity(loadCase)",
              mLoadCaseList->at(loadCaseNo)->E_G, "N/mm^2");
}

/**
 * @brief With Formula 63: Value of compressed gasket thickness
 * @param loadCaseNo
 */
void Gasket::Calc_eG(int loadCaseNo) {
    mLoadCaseList->at(loadCaseNo)->eG
            =  gasketCompressedThickness(mLoadCaseList->at(loadCaseNo));

    // TODO move addDetail() to relevant table
    PR->addDetail("With Formula 63",
              "eG", "gasketCompressedThickness(loadCase)",
              mLoadCaseList->at(loadCaseNo)->eG, "mm");
}

/**
 * @brief With Formula 65, 69, 70, 74, 75, gasket maximum load
 * @param loadCaseNo
 */
void Gasket::Calc_Q_smax(int loadCaseNo) {
    mLoadCaseList->at(loadCaseNo)->Q_smax
            = gasketMaximumLoad(loadCaseNo, mLoadCaseList->at(loadCaseNo));
    // addDetail moved to table 16
//    PR->addDetail("With Formula 65, 69, 70, 74, 75",
//              "Q_smax", "gasketMaximumLoad(loadCase)",
//              mLoadCaseList->at(loadCaseNo)->Q_smax, "N/mm^2");
}

/**
 * @brief Formula 63: Axial flexibility modulus of gasket
 * @param i load case number
 */
void Gasket::Calc_XG(int /* loadCaseNo */) {
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

void Gasket::Calc_P_QR(int loadCaseNo) {
    LoadCase* loadCase = mLoadCaseList->at(loadCaseNo);
    loadCase->P_QR = gasketCreepFactor(loadCaseNo, loadCase);
}

/**
 * @brief Temporary solution for calculation of EG
 * \todo  refer www.gasketdata.org and EN1591-2 Table 17 - 30
 * @param loadCase
 * @return gasket compressed elasticity
 */
double Gasket::gasketCompressedElasticity(LoadCase* loadCase) {
    if (loadCase->TG > 120) return 5404;
    return 3833.0;

    // TODO: refer www.gasketdata.org and EN1591-2 Table 17 - 30
//    return 50 * loadCase->Q_G;

    // Simple or EN1591 2001 version
    // E_G = E0 + K1 * Q
    double E0 = mTableGSimple->getTableG_E0(insType, loadCase->TG);
    double K1 = mTableGSimple->getTableG_K1(insType, loadCase->TG);
    double Q = loadCase->Q_G;
    return E0 + K1 * Q;
}

/**
 * @brief Temporary solution for the calculation eG
 * @param loadCase
 * @return gasket compressed thickness
 */
double Gasket::gasketCompressedThickness(LoadCase* loadCase) {
    // TODO refer www.gasketdata.org
    double tmpeG = 0;
    tmpeG = eGt;

//    if (loadCase->Q_G < 1) {
//        tmpeG = eGt;
//    } else {
//        tmpeG = (eGt - eGt * 0.4) + eGt * 0.4 / (pow(loadCase->Q_G, 0.25));
//    }

    return tmpeG;
}

/**
 * @brief Temporary solution for the calculation of Q_smax
 * @param loadCase
 * @return gasket maximum load at temperature
 */
double Gasket::gasketMaximumLoad(int loadCaseNo, LoadCase* loadCase) {
    // TODO: fill database with gasketdata.org now only Table 16

    if (mTable16Property->isGasketMaterialCodeExisting(matCode)) {
        loadCase->Q_smax = mTable16Property->getTable16_Q_smax(matCode,
                                                               loadCase->TG);
        if (loadCase->Q_smax > 0) {
            PR->addDetail("Table 16", "Q_smax", "Table value", loadCase->Q_smax,
                          "-", "Table value", loadCaseNo);
        } else {
            PR->addDetail("Table 16", "Q_smax", "Table value", loadCase->Q_smax,
                          "-", "Table value", loadCaseNo, "Out of range");
        }
    } else if (loadCase->Q_smax < 0.001) {
        loadCase->Q_smax = mTableGSimple->getTableG_Qmax(insType, loadCase->TG);
        PR->addDetail("Table 16", "Q_smax", "Table value", loadCase->Q_smax,
                      "-", "Table value", loadCaseNo, "Material not found");
    }

    return loadCase->Q_smax;
}

/**
 * @brief Gasket creep factor P_QR
 * @param loadCaseNo loadcase number
 * @param loadCase
 * @return
 */
double Gasket::gasketCreepFactor(int loadCaseNo, LoadCase* loadCase) {
    if (mTable16Property->isGasketMaterialCodeExisting(matCode)) {
        loadCase->P_QR = mTable16Property->getTable16_P_QR(matCode,
                                                           loadCase->TG);
        if (loadCase->P_QR > 0) {
            PR->addDetail("Table 16", "PQR", "Table value", loadCase->P_QR, "-",
                          "Table value", loadCaseNo);
        } else {
            PR->addDetail("Table 16", "PQR", "Table value", loadCase->P_QR, "-",
                          "Table value", loadCaseNo, "Out of range");
        }
    } else {
        loadCase->P_QR = 0.0;
        PR->addDetail("Table 16", "PQR", "Table value", loadCase->P_QR, "-",
                      "Table value", loadCaseNo, "Material not found");
    }

    return loadCase->P_QR;
}



END_NAMESPACE_REDBAG_CALC_EN1591
