#include "tablegproperty.h"
NAMESPACE_REDBAG_CALC_EN1591

Q0min_m_Property::Q0min_m_Property(Gasket::InsFilLayMatType insFillMat,
                                   double Q0min, double m) {
    mInsertFillMaterial = insFillMat;
    mQ0min = Q0min;
    mM = m;
}

TableGProperty::TableGProperty() {
    CreateList();
}

TableGProperty::~TableGProperty() {
    for (std::vector<Q0min_m_Property*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        delete (*it);
    }
}

double TableGProperty::getTableG_Q0min(Gasket::InsFilLayMatType insType) {
    Q0min_m_Property* obj = getPropertyObject(insType);
    if (obj) {
        return obj->mQ0min;
    }

    return -1.0;
}

double TableGProperty::getTableG_m(Gasket::InsFilLayMatType insType) {
    Q0min_m_Property* obj = getPropertyObject(insType);
    if (obj) {
        return obj->mM;
    }

    return -1.0;
}

Q0min_m_Property* TableGProperty::getPropertyObject(Gasket::InsFilLayMatType insType) {

    for (std::vector<Q0min_m_Property*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        Q0min_m_Property* obj = (*it);

        if (obj->mInsertFillMaterial == insType) {
            return obj;
        }
    }

    return NULL;
}

void TableGProperty::CreateList() {
    // Gasket::NonMetalic
    mList.push_back(new Q0min_m_Property(Gasket::Rubber, 0.5, 0.9));
    mList.push_back(new Q0min_m_Property(Gasket::Ptfe, 10, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::ExpPtfe, 12, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::ExpGraphWithoutIns, 10, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::ExpGraphPerfMetal, 15, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::ExpGraphAdhFlat, 10, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::ExpGraphMetSheet, 15, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::NonAsbetosSmaller1, 40, 1.6));
    mList.push_back(new Q0min_m_Property(Gasket::NonAsbetosLargerEqual1, 35, 1.6));
    // Gasket::GroovedSteel
    mList.push_back(new Q0min_m_Property(Gasket::PtfeSoftSteel, 10, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::PtfeStainless, 10, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::GraphSoftSteel, 15, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::GraphLowAlloy, 15, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::GraphStainless, 15, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::SilverLayer, 125, 1.8));
    // Gasket::SpiralWound
    mList.push_back(new Q0min_m_Property(Gasket::SpiralPtfeFillOuterOnly, 20, 1.6));
    mList.push_back(new Q0min_m_Property(Gasket::SpiralPtfeFillOuterInner, 20, 1.6));
    mList.push_back(new Q0min_m_Property(Gasket::SpiralGraphFillOuterOnly, 20, 1.6));
    mList.push_back(new Q0min_m_Property(Gasket::SpiralGraphFillOuterInner, 50, 1.6));
    // Gasket::SolidMetal
    mList.push_back(new Q0min_m_Property(Gasket::AluminumSoft, 50, 2.0));
    mList.push_back(new Q0min_m_Property(Gasket::CopperOrBrassSoft, 100, 2.0));
    mList.push_back(new Q0min_m_Property(Gasket::IronSoft, 175, 2.0));
    mList.push_back(new Q0min_m_Property(Gasket::SteelSoft, 200, 2.0));
    mList.push_back(new Q0min_m_Property(Gasket::SteelLowAlloyHeatRes, 225, 2.0));
    mList.push_back(new Q0min_m_Property(Gasket::StainlessSteel, 250, 2.0));
    mList.push_back(new Q0min_m_Property(Gasket::StainLessSteelHeatRes, 300, 2.0));
    // Gasket::CoveredMetalJacketed
    mList.push_back(new Q0min_m_Property(Gasket::SoftIronOrSteelJackGraphFillCover, 20, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::LowAlloyOrStainlessSteelGraphFillCover, 20, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::StainlessSteelJacketPtfeFillCover, 10, 1.3));
    mList.push_back(new Q0min_m_Property(Gasket::NickelAlloyPtfeFillCover, 10, 1.3));
    // Gasket::MetalJacketed
    mList.push_back(new Q0min_m_Property(Gasket::AluminumSoftGraphFill, 50, 1.6));
    mList.push_back(new Q0min_m_Property(Gasket::CopperOrBrassSoftGraphFill, 60, 1.8));
    mList.push_back(new Q0min_m_Property(Gasket::SoftIronOrSteelGraphFill, 80, 2.0));
    mList.push_back(new Q0min_m_Property(Gasket::LowAlloyOrStainlessSteelGraphFill, 100, 2.2));
}

END_NAMESPACE_REDBAG_CALC_EN1591
