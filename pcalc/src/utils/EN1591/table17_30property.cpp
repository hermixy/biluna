#include "table17_30property.h"
#include "pcalc_utilityfactory.h"
NAMESPACE_BILUNA_CALC_EN1591

E_GProperty::E_GProperty(const QString &materialCode,
                         double gasketStress,
                         double temperature,
                         double E_G) {
    mMaterialCode = materialCode;
    mGasketStress = gasketStress;
    mTemperature = temperature;
    mE_G = E_G;
}

Table17_30Property* Table17_30Property::mActiveUtility = 0;

Table17_30Property::Table17_30Property() : RB_TableMath(), RB_Utility() {
    RB_DEBUG->print("Table02_15Property::Table02_15Property()");
    mTargetTemperature = 0.0;
    mTargetGasketStress = 0.0;
    mTopLeft = NULL;
    mTopRight = NULL;
    mBottomLeft = NULL;
    mBottomRight = NULL;
    createList();
    PCALC_UTILITYFACTORY->addUtility(this);
}

Table17_30Property::~Table17_30Property() {
    for (std::vector<E_GProperty*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        delete (*it);
    }

    PCALC_UTILITYFACTORY->removeUtility(this);
    mActiveUtility = NULL;
    RB_DEBUG->print("Table17_30Property::~Table17_30Property()");
}

Table17_30Property* Table17_30Property::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new Table17_30Property();
        mActiveUtility->refresh();
    }
    return mActiveUtility;
}

double Table17_30Property::getTableE_G(const RB_String& materialCode,
                                       double temperature,
                                       double gasketStress) {
    mTargetTemperature = temperature;
    mTargetGasketStress = gasketStress;
    mTopLeft = NULL;
    mTopRight = NULL;
    mBottomLeft = NULL;
    mBottomRight = NULL;

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

    double value = 0.0;

    if (mTopLeft && mTopRight && mBottomLeft && mBottomRight) {
        value = getBilinearValue(
            mTargetTemperature, mTargetGasketStress,
            mTopLeft->mTemperature, mTopLeft->mGasketStress, mTopLeft->mE_G,
            mTopRight->mTemperature, mTopRight->mGasketStress, mTopRight->mE_G,
            mBottomLeft->mTemperature, mBottomLeft->mGasketStress,
                    mBottomLeft->mE_G,
            mBottomRight->mTemperature, mBottomRight->mGasketStress,
                    mBottomRight->mE_G);
    }

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
        if (!mTopRight || (obj->mTemperature <= mTopRight->mTemperature
                && obj->mGasketStress >= mTopRight->mGasketStress)) {
            mTopRight = obj;
        }
    }
}

void Table17_30Property::updateBottomLeft(E_GProperty* obj) {
    if (obj->mTemperature <= mTargetTemperature
            && obj->mGasketStress >= mTargetGasketStress) {
        if (!mBottomLeft || (obj->mTemperature >= mBottomLeft->mTemperature
                && obj->mGasketStress <= mBottomLeft->mGasketStress)) {
            mBottomLeft = obj;
        }
    }
}

void Table17_30Property::updateBottomRight(E_GProperty* obj) {
    if (obj->mTemperature >= mTargetTemperature
            && obj->mGasketStress >= mTargetGasketStress) {
        if (!mBottomRight || (obj->mTemperature <= mBottomRight->mTemperature
                && obj->mGasketStress <= mBottomRight->mGasketStress)) {
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
    cl("3-3-100-1", 20, 300, 2904);
    cl("3-3-100-1", 20, 450, 2299);
    cl("3-3-100-1", 30, 20, 1975);
    cl("3-3-100-1", 30, 300, 3131.5);
    cl("3-3-100-1", 30, 450, 3196.5);
    cl("3-3-100-1", 40, 20, 2158);
    cl("3-3-100-1", 40, 300, 3359);
    cl("3-3-100-1", 40, 450, 4094);
    cl("3-3-100-1", 50, 20, 2563);
    cl("3-3-100-1", 50, 300, 4026.5);
    cl("3-3-100-1", 50, 450, 5087.5);
    cl("3-3-100-1", 60, 20, 2892);
    cl("3-3-100-1", 60, 300, 4694);
    cl("3-3-100-1", 60, 450, 6081);
    cl("3-3-100-1", 80, 20, 3643);
    cl("3-3-100-1", 80, 300, 6874);
    cl("3-3-100-1", 80, 450, 7835);
    cl("3-3-100-1", 100, 20, 4714);
    cl("3-3-100-1", 100, 300, 10291);
    cl("3-3-100-1", 100, 450, 9943);
    cl("3-3-100-1", 120, 20, 6147);
    cl("3-3-100-1", 120, 300, 15117);
    cl("3-3-100-1", 120, 450, 11529);
    // Table 18
    cl("3-4-104-1", 20, 20, 1233);
    cl("3-4-104-1", 20, 300, 1423);
    cl("3-4-104-1", 20, 450, 1489);
    cl("3-4-104-1", 30, 20, 1620);
    cl("3-4-104-1", 30, 300, 2106.5);
    cl("3-4-104-1", 30, 450, 2251);
    cl("3-4-104-1", 40, 20, 1916);
    cl("3-4-104-1", 40, 300, 2790);
    cl("3-4-104-1", 40, 450, 3013);
    cl("3-4-104-1", 50, 20, 2316);
    cl("3-4-104-1", 50, 300, 3997);
    cl("3-4-104-1", 50, 450, 3876);
    cl("3-4-104-1", 60, 20, 2719);
    cl("3-4-104-1", 60, 300, 4203);
    cl("3-4-104-1", 60, 450, 4739);
    cl("3-4-104-1", 80, 20, 3372);
    cl("3-4-104-1", 80, 300, 4291);
    cl("3-4-104-1", 80, 450, 6156);
    cl("3-4-104-1", 100, 20, 3987);
    cl("3-4-104-1", 100, 300, 5205);
    cl("3-4-104-1", 100, 450, 7428);
    cl("3-4-104-1", 120, 20, 4793);
    cl("3-4-104-1", 120, 300, 6111);
    cl("3-4-104-1", 120, 450, 8525);
    cl("3-4-104-1", 140, 20, 5808);
    cl("3-4-104-1", 140, 300, 6972);
    cl("3-4-104-1", 140, 450, 9297);
    cl("3-4-104-1", 160, 20, 7024);
    cl("3-4-104-1", 160, 300, 7938);
    cl("3-4-104-1", 160, 450, 10206);
    cl("3-4-104-1", 180, 20, 8520);
    cl("3-4-104-1", 180, 300, 9661);
    cl("3-4-104-1", 180, 450, 10968);
    cl("3-4-104-1", 240, 20, 15577);
    cl("3-4-104-1", 240, 300, 11638);
    cl("3-4-104-1", 260, 20, 22806.5);
    cl("3-4-104-1", 300, 20, 30036);
    // Table 19
    cl("3-5-102-1", 20, 20, 725);
    cl("3-5-102-1", 20, 200, 843);
    cl("3-5-102-1", 20, 300, 942);
    cl("3-5-102-1", 20, 450, 850);
    cl("3-5-102-1", 30, 20, 996);
    cl("3-5-102-1", 30, 200, 1326);
    cl("3-5-102-1", 30, 300, 1465);
    cl("3-5-102-1", 30, 450, 1554.5);
    cl("3-5-102-1", 40, 20, 1207);
    cl("3-5-102-1", 40, 200, 1809);
    cl("3-5-102-1", 40, 300, 1988);
    cl("3-5-102-1", 40, 450, 2259);
    cl("3-5-102-1", 50, 20, 1703);
    cl("3-5-102-1", 50, 200, 3010);
    cl("3-5-102-1", 50, 300, 2882);
    cl("3-5-102-1", 50, 450, 3049.5);
    cl("3-5-102-1", 60, 20, 2268);
    cl("3-5-102-1", 60, 200, 4211);
    cl("3-5-102-1", 60, 300, 3776);
    cl("3-5-102-1", 60, 450, 3840);
    cl("3-5-102-1", 80, 200, 8537);
    cl("3-5-102-1", 80, 300, 6992);
    cl("3-5-102-1", 80, 450, 4945);
    // Table 20
    cl("G03", 60, 20, 2989);
    cl("G03", 60, 150, 3232);
    cl("G03", 60, 225, 2415);
    cl("G03", 80, 20, 3742);
    cl("G03", 80, 150, 3507);
    cl("G03", 80, 225, 2694);
    cl("G03", 120, 20, 4723);
    cl("G03", 120, 150, 3933);
    cl("G03", 120, 225, 3241);
    cl("G03", 140, 20, 5324);
    cl("G03", 140, 150, 4980);
    cl("G03", 140, 225, 4363);
    cl("G03", 180, 20, 5241);
    cl("G03", 180, 150, 5479);
    cl("G03", 180, 225, 5221);
    cl("G03", 240, 20, 6519);
    cl("G03", 240, 150, 6751);
    cl("G03", 240, 225, 6597);
    cl("G03", 300, 20, 7566);
    cl("G03", 300, 150, 10077);
    cl("G03", 300, 225, 8521);
    cl("G03", 340, 20, 10518);
    cl("G03", 340, 150, 13690);
    cl("G03", 340, 225, 11485);
    cl("G03", 400, 20, 14393);
    cl("G03", 400, 150, 19892);
    cl("G03", 400, 225, 15054);
    cl("G03", 440, 20, 17000);
    cl("G03", 440, 150, 28614);
    cl("G03", 440, 225, 18352);
    cl("G03", 490, 20, 25742);
    cl("G03", 490, 150, 34196);
    cl("G03", 490, 225, 25922);
    // Table 21
    cl("7-01-104-1", 20, 20, 1498);
    cl("7-01-104-1", 20, 300, 3559);
    cl("7-01-104-1", 20, 450, 2933);
    cl("7-01-104-1", 30, 20, 1822);
    cl("7-01-104-1", 30, 300, 4038.5);
    cl("7-01-104-1", 30, 450, 3918);
    cl("7-01-104-1", 40, 20, 2134);
    cl("7-01-104-1", 40, 300, 4518);
    cl("7-01-104-1", 40, 450, 4903);
    cl("7-01-104-1", 50, 20, 2221);
    cl("7-01-104-1", 50, 300, 4670.5);
    cl("7-01-104-1", 50, 450, 5008);
    cl("7-01-104-1", 60, 20, 1968);
    cl("7-01-104-1", 60, 300, 4823);
    cl("7-01-104-1", 60, 450, 5113);


//    cl("7-01-104-1", 0, 20, );
//    cl("7-01-104-1", 0, 300, );
//    cl("7-01-104-1", 0, 450, );

    // TODO: complete
}


END_NAMESPACE_BILUNA_CALC_EN1591
