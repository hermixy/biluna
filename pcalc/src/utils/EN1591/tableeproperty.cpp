#include "tableeproperty.h"
NAMESPACE_BILUNA_CALC_EN1591

MuG_Property::MuG_Property(Gasket::InsFilLayMatType insFillMat,
                           double muG) {
    mInsertFillMaterial = insFillMat;
    mMuG = muG;
}

TableEProperty::TableEProperty() {
    CreateList();
}

TableEProperty::~TableEProperty() {
    for (std::vector<MuG_Property*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        delete (*it);
    }
}

double TableEProperty::getTableE_muG(Gasket::InsFilLayMatType insType) {
    for (std::vector<MuG_Property*>::iterator it = mList.begin();
                it != mList.end(); it++) {
        MuG_Property* obj = (*it);

        if (obj->mInsertFillMaterial == insType) {
            return obj->mMuG;
        }
    }

    return -1.0;
}

void TableEProperty::CreateList() {
    // Gasket::NonMetalic
    mList.push_back(new MuG_Property(Gasket::Rubber, 0.25));
    mList.push_back(new MuG_Property(Gasket::Ptfe, 0.05));
    mList.push_back(new MuG_Property(Gasket::ExpPtfe, 0.05));
    mList.push_back(new MuG_Property(Gasket::ExpGraphWithoutIns, 0.1));
    mList.push_back(new MuG_Property(Gasket::ExpGraphPerfMetal, 0.1));
    mList.push_back(new MuG_Property(Gasket::ExpGraphAdhFlat, 0.1));
    mList.push_back(new MuG_Property(Gasket::ExpGraphMetSheet, 0.1));
    mList.push_back(new MuG_Property(Gasket::NonAsbetosSmaller1, 0.1));
    mList.push_back(new MuG_Property(Gasket::NonAsbetosLargerEqual1, 0.1));
    // Gasket::GroovedSteel
    mList.push_back(new MuG_Property(Gasket::PtfeSoftSteel, 0.05));
    mList.push_back(new MuG_Property(Gasket::PtfeStainless, 0.05));
    mList.push_back(new MuG_Property(Gasket::GraphSoftSteel, 0.1));
    mList.push_back(new MuG_Property(Gasket::GraphLowAlloy, 0.1));
    mList.push_back(new MuG_Property(Gasket::GraphStainless, 0.1));
    mList.push_back(new MuG_Property(Gasket::SilverLayer, 0.1));
    // Gasket::SpiralWound
    mList.push_back(new MuG_Property(Gasket::SpiralPtfeFillOuterOnly, 0.05));
    mList.push_back(new MuG_Property(Gasket::SpiralPtfeFillOuterInner, 0.05));
    mList.push_back(new MuG_Property(Gasket::SpiralGraphFillOuterOnly, 0.1));
    mList.push_back(new MuG_Property(Gasket::SpiralGraphFillOuterInner, 0.1));
    // Gasket::SolidMetal
    mList.push_back(new MuG_Property(Gasket::AluminumSoft, 0.15));
    mList.push_back(new MuG_Property(Gasket::CopperOrBrassSoft, 0.15));
    mList.push_back(new MuG_Property(Gasket::IronSoft, 0.15));
    mList.push_back(new MuG_Property(Gasket::SteelSoft, 0.15));
    mList.push_back(new MuG_Property(Gasket::SteelLowAlloyHeatRes, 0.15));
    mList.push_back(new MuG_Property(Gasket::StainlessSteel, 0.15));
    mList.push_back(new MuG_Property(Gasket::StainLessSteelHeatRes, 0.15));
    // Gasket::CoveredMetalJacketed
    mList.push_back(new MuG_Property(Gasket::SoftIronOrSteelJackGraphFillCover, 0.1));
    mList.push_back(new MuG_Property(Gasket::LowAlloyOrStainlessSteelGraphFillCover, 0.1));
    mList.push_back(new MuG_Property(Gasket::StainlessSteelJacketPtfeFillCover, 0.05));
    mList.push_back(new MuG_Property(Gasket::NickelAlloyPtfeFillCover, 0.05));
    // Gasket::MetalJacketed
    mList.push_back(new MuG_Property(Gasket::AluminumSoftGraphFill, 0.1));
    mList.push_back(new MuG_Property(Gasket::CopperOrBrassSoftGraphFill, 0.1));
    mList.push_back(new MuG_Property(Gasket::SoftIronOrSteelGraphFill, 0.1));
    mList.push_back(new MuG_Property(Gasket::LowAlloyOrStainlessSteelGraphFill, 0.1));
}

END_NAMESPACE_BILUNA_CALC_EN1591
