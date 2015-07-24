#include "tablegsimple.h"
NAMESPACE_REDBAG_CALC_EN1591

/**
 * Constructor
 * @param baseMat
 * @param insFillMat
 * @param temperature
 * @param Q0min
 * @param Qmax
 * @param E0
 * @param K1
 * @param mI
 * @param gC
 */
TableGSimpleProperty::TableGSimpleProperty(Gasket::InsFilLayMatType insFillMat,
                                           double temperature,
                                           double Q0min,
                                           double Qmax,
                                           double E0,
                                           double K1,
                                           double mI,
                                           double gC) {
    mInsertFillMaterial = insFillMat;
    mTemperature = temperature;
    mQ0min = Q0min;
    mQmax = Qmax;
    mE0 = E0;
    mK1 = K1;
    mmI = mI;
    mgC = gC;
}

/**
 * Constructor
 */
TableGSimple::TableGSimple(RB_ObjectContainer* inputOutput)
                : RB_Report(inputOutput) {
    mUpper = NULL;
    mLower = NULL;
    createList();
}

TableGSimple::~TableGSimple() {
    for (std::vector<TableGSimpleProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        delete (*it);
    }
}

double TableGSimple::getTableG_Q0min(Gasket::InsFilLayMatType insType,
                                     double temperature) {
    double value = 0.0;
    if (!getUpperLower(insType, temperature)) {
        value = 0.0;
        addDetail("Table G", "Q0min", "", value, "N/mmm2", "", -1, "Out of range");
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value =  mUpper->mQ0min;
        addDetail("Table G", "Q0min", "", value, "N/mmm2");
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mQ0min - mLower->mQ0min) * perunage + mLower->mQ0min;
    addDetail("Table G", "Q0min", "", value, "N/mmm2");
    return value;
}

double TableGSimple::getTableG_Qmax(Gasket::InsFilLayMatType insType,
                                    double temperature) {
    double value = 0.0;
    if (!getUpperLower(insType, temperature)) {
        value = 0.0;
        addDetail("Table G", "Qmax", "", value, "N/mmm2", "", -1, "Out of range");
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value = mUpper->mQmax;
        addDetail("Table G", "Qmax", "", value, "N/mmm2");
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mQmax - mLower->mQmax) * perunage + mLower->mQmax;
    addDetail("Table G", "Qmax", "", value, "N/mmm2");
    return value;
}

double TableGSimple::getTableG_E0(Gasket::InsFilLayMatType insType,
                                  double temperature) {
    double value = 0.0;
    if (!getUpperLower(insType, temperature)) {
        value = 0.0;
        addDetail("Table G", "E0", "", value, "N/mmm2", "", -1, "Out of range");
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value = mUpper->mE0;
        addDetail("Table G", "E0", "", value, "N/mmm2");
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mE0 - mLower->mE0) * perunage + mLower->mE0;
    addDetail("Table G", "E0", "", value, "N/mmm2");
    return value;
}

double TableGSimple::getTableG_K1(Gasket::InsFilLayMatType insType,
                                  double temperature) {
    double value = 0.0;
    if (!getUpperLower(insType, temperature)) {
        value = 0.0;
        addDetail("Table G", "K1", "", value, "-", "", -1, "Out of range");
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value = mUpper->mK1;
        addDetail("Table G", "K1", "", value, "-");
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mK1 - mLower->mK1) * perunage + mLower->mK1;
    addDetail("Table G", "K1", "", value, "-");
    return value;
}

double TableGSimple::getTableG_mI(Gasket::InsFilLayMatType insType,
                                  double temperature) {
    double value = 0.0;
    if (!getUpperLower(insType, temperature)) {
        value = 0.0;
        addDetail("Table G", "mI", "", value, "-", "", -1, "Out of range");
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value = mUpper->mmI;
        addDetail("Table G", "mI", "", value, "-");
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mmI - mLower->mmI) * perunage + mLower->mmI;
    addDetail("Table G", "mI", "", value, "-");
    return value;
}

double TableGSimple::getTableG_gC(Gasket::InsFilLayMatType insType,
                                  double temperature) {
    double value = 0.0;
    if (!getUpperLower(insType, temperature)) {
        value = 0.0;
        addDetail("Table G", "gC", "", value, "-", "", -1, "Out of range");
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value = mUpper->mgC;
        addDetail("Table G", "gC", "", value, "-");
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mgC - mLower->mgC) * perunage + mLower->mgC;
    addDetail("Table G", "gC", "", value, "-");
    return value;
}

bool TableGSimple::getUpperLower(Gasket::InsFilLayMatType insType,
                                 double temperature) {
    mUpper = getUpperObject(insType, temperature);
    mLower = getLowerObject(insType, temperature);

    if (mUpper && mLower) {
        return true;
    }

    return false;
}

TableGSimpleProperty* TableGSimple::getUpperObject(
        Gasket::InsFilLayMatType insType,
        double temperature) {

    TableGSimpleProperty* obj = NULL;

    for (std::vector<TableGSimpleProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        TableGSimpleProperty* tmpObj = (*it);

        if (tmpObj->mInsertFillMaterial == insType
                && tmpObj->mTemperature >= temperature) {

            if (!obj || (obj && obj->mTemperature >= tmpObj->mTemperature)) {
                obj = tmpObj;
            }
        }
    }

    return obj;
}

TableGSimpleProperty *TableGSimple::getLowerObject(
        Gasket::InsFilLayMatType insType,
        double temperature) {

    TableGSimpleProperty* obj = NULL;

    for (std::vector<TableGSimpleProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        TableGSimpleProperty* tmpObj = (*it);

        if (tmpObj->mInsertFillMaterial == insType
                && tmpObj->mTemperature <= temperature) {

            if (!obj || (obj && obj->mTemperature <= tmpObj->mTemperature)) {
                obj = tmpObj;
            }
        }
    }

    return obj;
}

void TableGSimple::createList() {
    // Gasket::BasicMatType matType = Gasket::NonMetalic;
    Gasket::InsFilLayMatType insType = Gasket::Rubber;
    cl(insType, 0.0, 0.5, 28, 200, 10, 0.9, 0.9);
    cl(insType, 20, 0.5, 28, 200, 10, 0.9, 0.9);
    cl(insType, 100, 0.5, 18, 200, 10, 0.9, 0.9);
    cl(insType, 150, 0.5, 12, 200, 10, 0.9, 0.9);
    insType = Gasket::Ptfe;
    cl(insType, 0.0, 10, 50, 600, 20, 1.3, 0.9);
    cl(insType, 20, 10, 50, 600, 20, 1.3, 0.9);
    cl(insType, 100, 10, 35, 500, 20, 1.3, 0.7);
    cl(insType, 200, 10, 20, 400, 20, 1.3, 0.5);
    insType = Gasket::ExpPtfe;
    cl(insType, 0, 12, 150, 500, 40, 1.3, 1);
    cl(insType, 20, 12, 150, 500, 40, 1.3, 1);
    cl(insType, 100, 12, 150, 1500, 35, 1.3, 0.9);
    cl(insType, 200, 12, 150, 2500, 30, 1.3, 0.8);
    insType = Gasket::ExpGraphWithoutIns;
    cl(insType, 0, 10, 100, 1, 26, 1.3, 1);
    cl(insType, 20, 10, 100, 1, 26, 1.3, 1);
    cl(insType, 100, 10, 100, 1, 26, 1.3, 1);
    cl(insType, 200, 10, 95, 1, 26, 1.3, 1);
    cl(insType, 300, 10, 90, 1, 26, 1.3, 1);
    insType = Gasket::ExpGraphPerfMetal;
    cl(insType, 0, 15, 150, 1, 31, 1.3, 1);
    cl(insType, 20, 15, 150, 1, 31, 1.3, 1);
    cl(insType, 100, 15, 145, 1, 31, 1.3, 1);
    cl(insType, 200, 15, 140, 1, 31, 1.3, 1);
    cl(insType, 300, 15, 130, 1, 31, 1.3, 1);
    insType = Gasket::ExpGraphAdhFlat;
    cl(insType, 0, 10, 100, 1, 28, 1.3, 0.9);
    cl(insType, 20, 10, 100, 1, 28, 1.3, 0.9);
    cl(insType, 100, 10, 90, 1, 28, 1.3, 0.9);
    cl(insType, 200, 10, 80, 1, 28, 1.3, 0.9);
    cl(insType, 300, 10, 70, 1, 28, 1.3, 0.9);
    insType = Gasket::ExpGraphMetSheet;
    cl(insType, 0, 15, 270, 1, 33, 1.3, 1);
    cl(insType, 20, 15, 270, 1, 33, 1.3, 1);
    cl(insType, 100, 15, 250, 1, 33, 1.3, 1);
    cl(insType, 200, 15, 230, 1, 33, 1.3, 1);
    cl(insType, 300, 15, 210, 1, 33, 1.3, 1);
    insType = Gasket::NonAsbetosSmaller1;
    cl(insType, 0, 40, 100, 500, 20, 1.6, 0);
    cl(insType, 20, 40, 100, 500, 20, 1.6, 0);
    cl(insType, 100, 40, 90, 500, 20, 1.6, 0);
    cl(insType, 200, 40, 70, 500, 20, 1.6, 0);
    insType = Gasket::NonAsbetosLargerEqual1;
    cl(insType, 0, 35, 80, 500, 20, 1.6, 0);
    cl(insType, 20, 35, 80, 500, 20, 1.6, 0);
    cl(insType, 100, 35, 70, 500, 20, 1.6, 0);
    cl(insType, 200, 35, 60, 500, 20, 1.6, 0);

    // matType = Gasket::GroovedSteel;
    insType = Gasket::PtfeSoftSteel;
    cl(insType, 0, 10, 350, 16000, 0, 1.3, 0.9);
    cl(insType, 20, 10, 350, 16000, 0, 1.3, 0.9);
    cl(insType, 100, 10, 330, 16000, 0, 1.3, 0.8);
    cl(insType, 200, 10, 290, 16000, 0, 1.3, 0.7);
    cl(insType, 300, 10, 250, 16000, 0, 1.3, 0.6);
    insType = Gasket::PtfeStainless;
    cl(insType, 0, 10, 500, 16000, 0, 1.3, 0.9);
    cl(insType, 20, 10, 500, 16000, 0, 1.3, 0.9);
    cl(insType, 100, 10, 480, 16000, 0, 1.3, 0.8);
    cl(insType, 200, 10, 450, 16000, 0, 1.3, 0.7);
    cl(insType, 300, 10, 420, 16000, 0, 1.3, 0.6);
    insType = Gasket::GraphSoftSteel;
    cl(insType, 0, 15, 350, 16000, 0, 1.3, 1);
    cl(insType, 20, 15, 350, 16000, 0, 1.3, 1);
    cl(insType, 100, 15, 330, 16000, 0, 1.3, 1);
    cl(insType, 200, 15, 290, 16000, 0, 1.3, 1);
    cl(insType, 300, 15, 250, 16000, 0, 1.3, 1);
    insType = Gasket::GraphLowAlloy;
    cl(insType, 0, 15, 400, 16000, 0, 1.3, 1);
    cl(insType, 20, 15, 400, 16000, 0, 1.3, 1);
    cl(insType, 100, 15, 390, 16000, 0, 1.3, 1);
    cl(insType, 200, 15, 360, 16000, 0, 1.3, 1);
    cl(insType, 300, 15, 320, 16000, 0, 1.3, 1);
    cl(insType, 400, 15, 270, 16000, 0, 1.3, 0.9);
    cl(insType, 500, 15, 220, 16000, 0, 1.3, 0.8);
    insType = Gasket::GraphStainless;
    cl(insType, 0, 15, 500, 16000, 0, 1.3, 1);
    cl(insType, 20, 15, 500, 16000, 0, 1.3, 1);
    cl(insType, 100, 15, 480, 16000, 0, 1.3, 1);
    cl(insType, 200, 15, 450, 16000, 0, 1.3, 1);
    cl(insType, 300, 15, 420, 16000, 0, 1.3, 1);
    cl(insType, 400, 15, 390, 16000, 0, 1.3, 0.9);
    cl(insType, 500, 15, 350, 16000, 0, 1.3, 0.8);
    insType = Gasket::SilverLayer;
    cl(insType, 0, 125, 600, 20000, 0, 1.8, 1);
    cl(insType, 20, 125, 600, 20000, 0, 1.8, 1);
    cl(insType, 100, 125, 570, 20000, 0, 1.8, 1);
    cl(insType, 200, 125, 540, 20000, 0, 1.8, 1);
    cl(insType, 300, 125, 500, 20000, 0, 1.8, 1);
    cl(insType, 400, 125, 460, 20000, 0, 1.8, 1);
    cl(insType, 500, 125, 400, 20000, 0, 1.8, 0.9);
    cl(insType, 600, 125, 250, 20000, 0, 1.8, 0.8);

    // matType = Gasket::SpiralWound;
    insType = Gasket::SpiralPtfeFillOuterOnly;
    cl(insType, 0, 20, 110, 6000, 0, 1.6, 0.9);
    cl(insType, 20, 20, 110, 6000, 0, 1.6, 0.9);
    cl(insType, 100, 20, 100, 6000, 0, 1.6, 0.8);
    cl(insType, 200, 20, 90, 6000, 0, 1.6, 0.7);
    cl(insType, 300, 20, 80, 6000, 0, 1.6, 0.6);
    insType = Gasket::SpiralPtfeFillOuterInner;
    cl(insType, 0, 20, 180, 6000, 0, 1.6, 0.9);
    cl(insType, 20, 20, 180, 6000, 0, 1.6, 0.9);
    cl(insType, 100, 20, 170, 6000, 0, 1.6, 0.8);
    cl(insType, 200, 20, 160, 6000, 0, 1.6, 0.7);
    cl(insType, 300, 20, 150, 6000, 0, 1.6, 0.6);
    insType = Gasket::SpiralGraphFillOuterOnly;
    cl(insType, 0, 20, 110, 8000, 0, 1.6, 1);
    cl(insType, 20, 20, 110, 8000, 0, 1.6, 1);
    cl(insType, 100, 20, 110, 8000, 0, 1.6, 1);
    cl(insType, 200, 20, 100, 8000, 0, 1.6, 1);
    cl(insType, 300, 20, 90, 8000, 0, 1.6, 1);
    cl(insType, 400, 20, 80, 8000, 0, 1.6, 0.9);
    insType = Gasket::SpiralGraphFillOuterInner;
    cl(insType, 0, 50, 300, 10000, 0, 1.6, 1);
    cl(insType, 20, 50, 300, 10000, 0, 1.6, 1);
    cl(insType, 100, 50, 280, 10000, 0, 1.6, 1);
    cl(insType, 200, 50, 250, 10000, 0, 1.6, 1);
    cl(insType, 300, 50, 220, 10000, 0, 1.6, 1);
    cl(insType, 400, 50, 180, 10000, 0, 1.6, 0.9);

    // matType = Gasket::SolidMetal;
    insType = Gasket::AluminumSoft;
    cl(insType, 0, 50, 100, 70000, 0, 2, 1);
    cl(insType, 20, 50, 100, 70000, 0, 2, 1);
    cl(insType, 100, 50, 85, 65000, 0, 2, 0.9);
    cl(insType, 200, 50, 60, 60000, 0, 2, 0.8);
    cl(insType, 300, 50, 20, 50000, 0, 2, 0.7);
    insType = Gasket::CopperOrBrassSoft;
    cl(insType, 0, 100, 210, 115000, 0, 2, 1);
    cl(insType, 20, 100, 210, 115000, 0, 2, 1);
    cl(insType, 100, 100, 190, 110000, 0, 2, 1);
    cl(insType, 200, 100, 155, 105000, 0, 2, 1);
    cl(insType, 300, 100, 110, 95000, 0, 2, 0.9);
    cl(insType, 400, 100, 50, 85000, 0, 2, 0.7);
    insType = Gasket::IronSoft;
    cl(insType, 0, 175, 380, 210000, 0, 2, 1);
    cl(insType, 20, 175, 380, 210000, 0, 2, 1);
    cl(insType, 100, 175, 340, 205000, 0, 2, 1);
    cl(insType, 200, 175, 280, 195000, 0, 2, 1);
    cl(insType, 300, 175, 220, 185000, 0, 2, 1);
    cl(insType, 400, 175, 160, 175000, 0, 2, 0.9);
    cl(insType, 500, 175, 100, 165000, 0, 2, 0.7);
    insType = Gasket::SteelSoft;
    cl(insType, 0, 200, 440, 210000, 0, 2, 1);
    cl(insType, 20, 200, 440, 210000, 0, 2, 1);
    cl(insType, 100, 200, 410, 205000, 0, 2, 1);
    cl(insType, 200, 200, 360, 195000, 0, 2, 1);
    cl(insType, 300, 200, 300, 185000, 0, 2, 1);
    cl(insType, 400, 200, 220, 175000, 0, 2, 0.9);
    cl(insType, 500, 200, 140, 165000, 0, 2, 0.7);
    insType = Gasket::SteelLowAlloyHeatRes;
    cl(insType, 0, 225, 495, 210000, 0, 2, 1);
    cl(insType, 20, 225, 495, 210000, 0, 2, 1);
    cl(insType, 100, 225, 490, 205000, 0, 2, 1);
    cl(insType, 200, 225, 460, 195000, 0, 2, 1);
    cl(insType, 300, 225, 420, 185000, 0, 2, 1);
    cl(insType, 400, 225, 370, 175000, 0, 2, 1);
    cl(insType, 500, 225, 310, 165000, 0, 2, 0.9);
    insType = Gasket::StainlessSteel;
    cl(insType, 0, 250, 550, 200000, 0, 2, 1);
    cl(insType, 20, 250, 550, 200000, 0, 2, 1);
    cl(insType, 100, 250, 525, 195000, 0, 2, 1);
    cl(insType, 200, 250, 495, 188000, 0, 2, 1);
    cl(insType, 300, 250, 460, 180000, 0, 2, 1);
    cl(insType, 400, 250, 425, 170000, 0, 2, 0.9);
    cl(insType, 500, 250, 370, 160000, 0, 2, 0.8);
    cl(insType, 600, 250, 300, 150000, 0, 2, 0.7);
    insType = Gasket::StainLessSteelHeatRes;
    cl(insType, 0, 300, 660, 210000, 0, 2, 1);
    cl(insType, 20, 300, 660, 210000, 0, 2, 1);
    cl(insType, 100, 300, 630, 205000, 0, 2, 1);
    cl(insType, 200, 300, 600, 200000, 0, 2, 1);
    cl(insType, 300, 300, 560, 194000, 0, 2, 1);
    cl(insType, 400, 300, 510, 188000, 0, 2, 1);
    cl(insType, 500, 300, 445, 180000, 0, 2, 0.9);
    cl(insType, 600, 300, 360, 170000, 0, 2, 0.8);

    // matType = Gasket::CoveredMetalJacketed;
    insType = Gasket::SoftIronOrSteelJackGraphFillCover;
    cl(insType, 0, 10, 150, 1, 69, 1.3, 1);
    cl(insType, 20, 10, 150, 1, 69, 1.3, 1);
    cl(insType, 100, 10, 150, 1, 69, 1.3, 0.9);
    cl(insType, 200, 10, 150, 1, 69, 1.3, 0.8);
    cl(insType, 300, 10, 150, 1, 69, 1.3, 0.7);
    insType = Gasket::LowAlloyOrStainlessSteelGraphFillCover;
    cl(insType, 0, 10, 150, 1, 69, 1.3, 1);
    cl(insType, 20, 10, 150, 1, 69, 1.3, 1);
    cl(insType, 100, 10, 150, 1, 69, 1.3, 0.9);
    cl(insType, 200, 10, 150, 1, 69, 1.3, 0.8);
    cl(insType, 300, 10, 150, 1, 69, 1.3, 0.7);
    insType = Gasket::StainlessSteelJacketPtfeFillCover;
    cl(insType, 0, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 20, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 100, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 200, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 300, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 400, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 500, 20, 300, 1, 48, 1.3, 1);
    insType = Gasket::NickelAlloyPtfeFillCover;
    cl(insType, 0, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 20, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 100, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 200, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 300, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 400, 20, 300, 1, 48, 1.3, 1);
    cl(insType, 500, 20, 300, 1, 48, 1.3, 1);

    // matType = Gasket::MetalJacketed;
    insType = Gasket::AluminumSoftGraphFill;
    cl(insType, 0, 50, 135, 500, 25, 1.6, 1);
    cl(insType, 20, 50, 135, 500, 25, 1.6, 1);
    cl(insType, 100, 50, 120, 800, 25, 1.6, 1);
    cl(insType, 200, 50, 90, 1100, 25, 1.6, 1);
    cl(insType, 300, 50, 60, 1400, 25, 1.6, 1);
    insType = Gasket::CopperOrBrassSoftGraphFill;
    cl(insType, 0, 60, 150, 600, 25, 1.8, 1);
    cl(insType, 20, 60, 150, 600, 25, 1.8, 1);
    cl(insType, 100, 60, 140, 900, 25, 1.8, 1);
    cl(insType, 200, 60, 130, 1200, 25, 1.8, 1);
    cl(insType, 300, 60, 120, 1500, 25, 1.8, 1);
    cl(insType, 400, 60, 100, 1800, 25, 1.8, 1);
    insType = Gasket::SoftIronOrSteelGraphFill;
    cl(insType, 0, 80, 180, 800, 25, 2, 1);
    cl(insType, 20, 80, 180, 800, 25, 2, 1);
    cl(insType, 100, 80, 170, 1100, 25, 2, 1);
    cl(insType, 200, 80, 160, 1400, 25, 2, 1);
    cl(insType, 300, 80, 150, 1700, 25, 2, 1);
    cl(insType, 400, 80, 140, 2000, 25, 2, 1);
    cl(insType, 500, 80, 120, 2300, 25, 2, 1);
    insType = Gasket::LowAlloyOrStainlessSteelGraphFill;
    cl(insType, 0, 100, 250, 800, 25, 2.2, 1);
    cl(insType, 20, 100, 250, 800, 25, 2.2, 1);
    cl(insType, 100, 100, 240, 1100, 25, 2.2, 1);
    cl(insType, 200, 100, 220, 1400, 25, 2.2, 1);
    cl(insType, 300, 10, 200, 1700, 25, 2.2, 1);
    cl(insType, 400, 100, 180, 2000, 25, 2.2, 1);
    cl(insType, 500, 100, 140, 2300, 25, 2.2, 1);
}

/**
 * @brief Create list item
 * @param baseMat
 * @param insFillMat
 * @param temperature
 * @param Q0min
 * @param Qmax
 * @param E0
 * @param K1
 * @param mI
 * @param gC
 */
void TableGSimple::cl(Gasket::InsFilLayMatType insFillMat,
                      double temperature, double Q0min, double Qmax,
                      double E0, double K1, double mI, double gC) {
    mList.push_back(new TableGSimpleProperty(insFillMat, temperature,
                                             Q0min, Qmax, E0, K1, mI, gC));
}

END_NAMESPACE_REDBAG_CALC_EN1591
