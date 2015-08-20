#include "table17_30property.h"
NAMESPACE_REDBAG_CALC_EN1591

E_GProperty::E_GProperty(const QString &materialCode,
                         double temperature,
                         double gasketStress,
                         double E_G) {
    mMaterialCode = materialCode;
    mTemperature = temperature;
    mGasketStress = gasketStress;
    mE_G = E_G;
}

Table17_30Property::Table17_30Property() : RB_TableMath() {
    createList();
}

Table17_30Property::~Table17_30Property() {
    for (std::vector<E_GProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        delete (*it);
    }
}

double Table17_30Property::getTableE_G(const RB_String& materialCode,
                                       double temperature,
                                       double gasketStress) {
    mTargetTemperature = temperature;
    mTargetGasketStress = gasketStress;

    for (std::vector<E_GProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        E_GProperty* tmpObj = (*it);

        if (tmpObj->mMaterialCode == materialCode) {
            updateTopLeft(tmpObj);
            updateTopRight(tmpObj);
            updateBottomLeft(tmpObj);
            updateBottomRight(tmpObj);
        }
    }

    double value = getBilinearValue(
                mTargetTemperature, mTargetGasketStress,
                mTopLeft->mTemperature, mTopLeft->mGasketStress, mTopLeft->mE_G,
                mTopRight->mTemperature, mTopRight->mGasketStress, mTopRight->mE_G,
                mBottomLeft->mTemperature, mBottomLeft->mGasketStress, mBottomLeft->mE_G,
                mBottomRight->mTemperature, mBottomRight->mGasketStress, mBottomRight->mE_G);

    return value;
}

bool Table17_30Property::isGasketMaterialCodeExisting(
        const RB_String& materialCode) {
    bool existing = false;

    for (std::vector<E_GProperty*>::iterator it = mList.begin();
                it != mList.end() && !existing; it++) {
        E_GProperty* tmpObj = (*it);

        if (tmpObj->mMaterialCode == materialCode) {
            existing = true;
        }
    }

    return existing;
}

void Table17_30Property::updateTopLeft(E_GProperty* obj) {
    if (obj->mTemperature <= mTargetTemperature
            && obj->mGasketStress <= mTargetGasketStress) {
        if (!mTopLeft || (obj->mTemperature >= mTopLeft->mTemperature
                && obj->mGasketStress >= mTopLeft->mGasketStress)) {
            mTopLeft = obj;
        }
    }
}

void Table17_30Property::updateTopRight(E_GProperty* obj) {
    if (obj->mTemperature >= mTargetTemperature
            && obj->mGasketStress <= mTargetGasketStress) {
        if (!mTopLeft || (obj->mTemperature <= mTopLeft->mTemperature
                && obj->mGasketStress >= mTopLeft->mGasketStress)) {
            mTopRight = obj;
        }
    }
}

void Table17_30Property::updateBottomLeft(E_GProperty* obj) {
    if (obj->mTemperature <= mTargetTemperature
            && obj->mGasketStress >= mTargetGasketStress) {
        if (!mTopLeft || (obj->mTemperature >= mTopLeft->mTemperature
                && obj->mGasketStress <= mTopLeft->mGasketStress)) {
            mBottomLeft = obj;
        }
    }
}

void Table17_30Property::updateBottomRight(E_GProperty* obj) {
    if (obj->mTemperature <= mTargetTemperature
            && obj->mGasketStress <= mTargetGasketStress) {
        if (!mTopLeft || (obj->mTemperature >= mTopLeft->mTemperature
                && obj->mGasketStress >= mTopLeft->mGasketStress)) {
            mBottomRight = obj;
        }
    }
}

void Table17_30Property::cl(const RB_String& materialCode, double temperature,
                            double gasketStress, double E_G) {
    mList.push_back(new E_GProperty(materialCode, temperature,
                                    gasketStress, E_G));
}

void Table17_30Property::createList() {
    // Table 17, includes interpolated values
    cl("3-3-100-1", 20, 20, 1854);
    cl("3-3-100-1", 300, 20, 2904);
    cl("3-3-100-1", 450, 20, 2299);
    cl("3-3-100-1", 20, 30, 1975);
    cl("3-3-100-1", 300, 30, 3131.5);
    cl("3-3-100-1", 450, 30, 3196.5);
    cl("3-3-100-1", 20, 40, 2158);
    cl("3-3-100-1", 300, 40, 3359);
    cl("3-3-100-1", 450, 40, 4094);
    cl("3-3-100-1", 20, 50, 2563);
    cl("3-3-100-1", 300, 50, 4026.5);
    cl("3-3-100-1", 450, 50, 5087.5);
    cl("3-3-100-1", 20, 60, 2892);
    cl("3-3-100-1", 300, 60, 4694);
    cl("3-3-100-1", 450, 60, 6081);
    cl("3-3-100-1", 20, 80, 3643);
    cl("3-3-100-1", 300, 80, 6874);
    cl("3-3-100-1", 450, 80, 7835);
    cl("3-3-100-1", 20, 100, 4714);
    cl("3-3-100-1", 300, 100, 10291);
    cl("3-3-100-1", 450, 100, 9943);
    cl("3-3-100-1", 20, 120, 6147);
    cl("3-3-100-1", 300, 120, 15117);
    cl("3-3-100-1", 450, 120, 11529);
    // Table 18
    cl("3-4-104-1", 20, 20, 1233);
    cl("3-4-104-1", 300, 20, 1423);
    cl("3-4-104-1", 450, 20, 1489);
    cl("3-4-104-1", 20, 30, 1620);
    cl("3-4-104-1", 300, 30, 2106.5);
    cl("3-4-104-1", 450, 30, 2251);
    cl("3-4-104-1", 20, 40, 1916);
    cl("3-4-104-1", 300, 40, 2790);
    cl("3-4-104-1", 450, 40, 3013);
    cl("3-4-104-1", 20, 50, 2316);
    cl("3-4-104-1", 300, 50, 3997);
    cl("3-4-104-1", 450, 50, 3876);
    cl("3-4-104-1", 20, 60, 2719);
    cl("3-4-104-1", 300, 60, 4203);
    cl("3-4-104-1", 450, 60, 4739);
    cl("3-4-104-1", 20, 80, 3372);
    cl("3-4-104-1", 300, 80, 4291);
    cl("3-4-104-1", 450, 80, 6156);
    cl("3-4-104-1", 20, 100, 3987);
    cl("3-4-104-1", 300, 100, 5205);
    cl("3-4-104-1", 450, 100, 7428);
    cl("3-4-104-1", 20, 120, 4793);
    cl("3-4-104-1", 300, 120, 6111);
    cl("3-4-104-1", 450, 120, 8525);
    cl("3-4-104-1", 20, 140, 5808);
    cl("3-4-104-1", 300, 140, 6972);
    cl("3-4-104-1", 450, 140, 9297);
    cl("3-4-104-1", 20, 160, 7024);
    cl("3-4-104-1", 300, 160, 7938);
    cl("3-4-104-1", 450, 160, 10206);
    cl("3-4-104-1", 20, 180, 8520);
    cl("3-4-104-1", 300, 180, 9661);
    cl("3-4-104-1", 450, 180, 10968);
    cl("3-4-104-1", 20, 240, 15577);
    cl("3-4-104-1", 300, 240, 11638);
    cl("3-4-104-1", 20, 260, 22806.5);
    cl("3-4-104-1", 20, 300, 30036);
    // Table 19
    cl("3-5-102-1", 20, 20, 725);
    cl("3-5-102-1", 200, 20, 843);
    cl("3-5-102-1", 300, 20, 942);
    cl("3-5-102-1", 450, 20, 850);
    cl("3-5-102-1", 20, 30, 996);
    cl("3-5-102-1", 200, 30, 1326);
    cl("3-5-102-1", 300, 30, 1465);
    cl("3-5-102-1", 450, 30, 1554.5);
    cl("3-5-102-1", 20, 40, 1207);
    cl("3-5-102-1", 200, 40, 1809);
    cl("3-5-102-1", 300, 40, 1988);
    cl("3-5-102-1", 450, 40, 2259);
    cl("3-5-102-1", 20, 50, 1703);
    cl("3-5-102-1", 200, 50, 3010);
    cl("3-5-102-1", 300, 50, 2882);
    cl("3-5-102-1", 450, 50, 3049.5);
    cl("3-5-102-1", 20, 60, 2268);
    cl("3-5-102-1", 200, 60, 4211);
    cl("3-5-102-1", 300, 60, 3776);
    cl("3-5-102-1", 450, 60, 3840);
    cl("3-5-102-1", 200, 80, 8537);
    cl("3-5-102-1", 300, 80, 6992);
    cl("3-5-102-1", 450, 80, 4945);
    // Table 20
    cl("G03", 20, 0, 2989);
    cl("G03", 150, 60, 3232);
    cl("G03", 225, 60, 2415);
    cl("G03", 20, 80, 3742);
    cl("G03", 150, 80, 3507);
    cl("G03", 225, 80, 2694);
    cl("G03", 20, 120, 4723);
    cl("G03", 150, 120, 3933);
    cl("G03", 225, 120, 3241);
    cl("G03", 20, 140, 5324);
    cl("G03", 150, 140, 4980);
    cl("G03", 225, 140, 4363);
    cl("G03", 20, 180, 5241);
    cl("G03", 150, 180, 5479);
    cl("G03", 225, 180, 5221);
    cl("G03", 20, 240, 6519);
    cl("G03", 150, 240, 6751);
    cl("G03", 225, 240, 6597);
    cl("G03", 20, 300, 7566);
    cl("G03", 150, 300, 10077);
    cl("G03", 225, 300, 8521);
    cl("G03", 20, 340, 10518);
    cl("G03", 150, 340, 13690);
    cl("G03", 225, 340, 11485);
    cl("G03", 20, 400, 14393);
    cl("G03", 150, 400, 19892);
    cl("G03", 225, 400, 15054);
    cl("G03", 20, 440, 17000);
    cl("G03", 150, 440, 28614);
    cl("G03", 225, 440, 18352);
    cl("G03", 20, 490, 25742);
    cl("G03", 150, 490, 34196);
    cl("G03", 225, 490, 25922);
    // Table 21
    cl("7-01-104-1", 20, 20, 1498);
    cl("7-01-104-1", 300, 20, 3559);
    cl("7-01-104-1", 450, 20, 2933);
    cl("7-01-104-1", 20, 30, 1822);
    cl("7-01-104-1", 300, 30, 4038.5);
    cl("7-01-104-1", 450, 30, 3918);


//    cl("7-01-104-1", 20, 0, );
//    cl("7-01-104-1", 300, 0, );
//    cl("7-01-104-1", 450, 0, );




    // TODO: complete
}


END_NAMESPACE_REDBAG_CALC_EN1591
