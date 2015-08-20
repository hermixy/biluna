#include "table16property.h"
#include "pcalc_report.h"
NAMESPACE_REDBAG_CALC_EN1591

Q_smax_Pqr_Property::Q_smax_Pqr_Property(double temperature,
                                         double Q_smax,
                                         double P_QR,
                                         const QString& materialCode) {
    mTemperature = temperature;
    mQ_smax = Q_smax;
    mP_QR = P_QR;
    mMaterialCode = materialCode;
}

Table16Property::Table16Property() : RB_TableMath() {
    createList();
}

Table16Property::~Table16Property() {
    for (std::vector<Q_smax_Pqr_Property*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        delete (*it);
    }
}

double Table16Property::getTable16_Q_smax(const RB_String& materialCode,
                                          double temperature) {
    double value = 0.0;
    if (!getUpperLower(materialCode, temperature)) {
        value = 0.0;
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value = mUpper->mQ_smax;
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mQ_smax - mLower->mQ_smax) * perunage + mLower->mQ_smax;
    return value;
}

double Table16Property::getTable16_P_QR(const RB_String& materialCode,
                                        double temperature) {
    double value = 0.0;
    if (!getUpperLower(materialCode, temperature)) {
        value = 0.0;
        return value;
    }

    if (mUpper->mTemperature == mLower->mTemperature) {
        value = mUpper->mP_QR;
        return value;
    }

    double perunage = (temperature - mLower->mTemperature)
            / (mUpper->mTemperature - mLower->mTemperature);
    value = (mUpper->mP_QR - mLower->mP_QR) * perunage + mLower->mP_QR;
    return value;
}

bool Table16Property::isGasketMaterialCodeExisting(
        const RB_String& materialCode) {
    bool existing = false;

    for (std::vector<Q_smax_Pqr_Property*>::iterator it = mList.begin();
                it != mList.end() && !existing; it++) {
        Q_smax_Pqr_Property* tmpObj = (*it);

        if (tmpObj->mMaterialCode == materialCode) {
            existing = true;
        }
    }

    return existing;
}

bool Table16Property::getUpperLower(const RB_String& materialCode,
                                    double temperature) {
    mUpper = NULL;
    mLower = NULL;

    for (std::vector<Q_smax_Pqr_Property*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        Q_smax_Pqr_Property* obj = (*it);
        updateUpperObject(obj, materialCode, temperature);
        updateLowerObject(obj, materialCode, temperature);
    }

    if (mUpper && mLower) {
        return true;
    }

    return false;
}

void Table16Property::updateUpperObject(Q_smax_Pqr_Property* obj,
        const RB_String& materialCode, double temperature) {

    if (obj->mMaterialCode == materialCode
            && obj->mTemperature >= temperature) {
        if (!mUpper || mUpper->mTemperature >= obj->mTemperature) {
            mUpper = obj;
        }
    }
}

void Table16Property::updateLowerObject(Q_smax_Pqr_Property* obj,
        const RB_String& materialCode, double temperature) {

    if (obj->mMaterialCode == materialCode
            && obj->mTemperature <= temperature) {
        if (!mLower || mLower->mTemperature <= obj->mTemperature) {
            mLower = obj;
        }
    }
}

void Table16Property::createList() {
    // Table 16
    // Non-asbestos fibre based sheet
    cl(20, 120, 1.0, "G01");
    cl(20, 120, 1.0, "E01");
    cl(20, 120, 1.0, "D01");
    cl(20, 120, 1.0, "B01");
    cl(20, 150, 0.98, "1-9-101-1");
    cl(175, 120, 0.9, "G01");
    cl(175, 120, 0.8, "E01");
    cl(175, 80, 0.7, "D01");
    cl(175, 80, 0.8, "B01");
    cl(200, 60, 0.81, "1-9-101-1");
    cl(250, 120, 0.9, "G01");
    cl(250, 100, 0.8, "E01");
    cl(250, 80, 0.5, "D01");
    cl(250, 80, 0.6, "B01");
    cl(250, 50, 0.77, "1-9-101-1");
    // Modified PTFE sheet
    cl(20, 60, 0.9, "1-10-102-1");
    cl(20, 50, 0.84, "1-10-100-1");
    cl(20, 120, 1.0, "G02");
    cl(20, 80, 0.9, "D02");
    cl(20, 120, 0.9, "A02");
    cl(20, 40, 0.9, "K02");
    cl(175, 60, 0.5, "1-10-102-1");
    cl(175, 40, 0.41, "1-10-100-1");
    cl(150, 120, 1.0, "G02");
    cl(150, 40, 0.6, "D02");
    cl(150, 120, 0.6, "A02");
    cl(150, 40, 0.5, "K02");
    cl(225, 60, 0.42, "1-10-102-1");
    cl(225, 25, 0.36, "1-10-100-1");
    cl(225, 120, 0.8, "G02");
    cl(225, 40, 0.4, "D02");
    cl(225, 120, 0.5, "A02");
    cl(225, 40, 0.4, "K02");
    // Serrated metal core [Kammprofile] with graphite facing
    cl(20, 600, 1.0, "2-5-104-1");
    cl(20, 328, 1.0, "F01");
    cl(300, 450, 0.94, "2-5-104-1");
    cl(300, 328, 0.98, "F01");
    cl(450, 400, 0.8, "2-5-104-1");
    cl(450, 328, 0.96, "F01");
    // Graphite sheet with tanged (perforated) stainless steel core
    cl(20, 200, 1.0, "E02");
    cl(20, 200, 1.0, "1-5-101-1");
    cl(300, 120, 1.0, "E02");
    cl(300, 140, 0.78, "1-5-101-1");
    cl(400, 120, 1.0, "E02");
    cl(450, 120, 0.62, "1-5-101-1");
    // Graphite sheet with multiple thin metal insertions
    cl(20, 120, 1.0, "A01");
    cl(300, 120, 1.0, "A01");
    cl(400, 120, 0.98, "A01");
    // Graphite covered metal jacketed with with graphite filler and outer ring
    cl(20, 120, 1.0, "H01");
    cl(20, 400, 0.98, "5-5-103-1");
    cl(300, 120, 1.0, "H01");
    cl(400, 120, 1.0, "H01");
    // Metal jacketed with graphite filler
    cl(20, 400, 1.0, "6-4-103-1");
    cl(20, 120, 1.0, "H02");
    cl(300, 400, 0.93, "6-4-103-1");
    cl(300, 120, 1.0, "H02");
    cl(450, 400, 0.87, "6-4-103-1");
    cl(400, 120, 1.0, "H02");
    // Corrugated metal core with graphite facing
    cl(20, 400, 1.0, "7-01-104-1");
    cl(20, 120, 0.9, "K04");
    cl(300, 200, 0.72, "7-01-104-1");
    cl(300, 120, 0.5, "K04");
    cl(450, 180, 0.52, "7-01-104-1");
    cl(400, 120, 0.4, "K04");
    // PTFE filled spiral wound gasket with both inner and outer rings
    cl(20, 360, 0.98, "G03");
    cl(150, 360, 0.98, "G03");
    cl(225, 360, 0.98, "G03");
    // Low Stress, graphite filled, spiral wound gasket with both inner & outer rings
    cl(20, 297, 0.99, "3-5-102-1");
    // Graphite filled spiral wound gasket with outer ring only
    cl(20, 125, 0.99, "3-3-100-1");
    // Graphite filled spiral wound gasket with both inner and outer rings
    cl(20, 311, 1.0, "3-4-104-1");
    cl(300, 250, 0.94, "3-4-104-1");
    cl(450, 220, 0.92, "3-4-104-1");
    // Proprietary PTFE/Graphite gasket with metal eyelet
    cl(20, 120, 1.0, "K01");
    cl(300, 120, 0.9, "K01");
}

void Table16Property::cl(double temperature,
                         double Q_smax,
                         double P_QR,
                         const RB_String& materialCode) {
    mList.push_back(new Q_smax_Pqr_Property(temperature, Q_smax,
                                            P_QR, materialCode));
}

END_NAMESPACE_REDBAG_CALC_EN1591
