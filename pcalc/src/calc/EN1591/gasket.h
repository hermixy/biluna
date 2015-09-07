﻿#ifndef GASKET_H
#define GASKET_H

#include "loadcase.h"
#include "loadcaselist.h"
#include "rb_namespace.h"
#include "rb_object.h"

NAMESPACE_BILUNA_CALC_EN1591

class Table16Property;
class TableGSimple;

class Gasket_IN : public RB_Object {

public:
    Gasket_IN();
    virtual ~Gasket_IN();

    bool isMetalic();

    enum FormType { // Figure 4
        Flat,
        CurvedSimpleContact,
        CurvedDoubleContact,
        OctagonalDoubleContact
    };

//    enum BasicMatType { // Table G
//        NonMetalic,
//        GroovedSteel,
//        SpiralWound,
//        SolidMetal,
//        CoveredMetalJacketed,
//        MetalJacketed
//    };

    enum InsFilLayMatType { // Table G
        // Non metalic
        Rubber,
        Ptfe,
        ExpPtfe,
        ExpGraphWithoutIns,
        ExpGraphPerfMetal,
        ExpGraphAdhFlat,
        ExpGraphMetSheet,
        NonAsbetosSmaller1,
        NonAsbetosLargerEqual1,
        // Grooved steel gaskets with soft layers on both sides
        PtfeSoftSteel,
        PtfeStainless,
        GraphSoftSteel,
        GraphLowAlloy,
        GraphStainless,
        SilverLayer,
        // Spiral wound gaskets with soft filler
        SpiralPtfeFillOuterOnly,
        SpiralPtfeFillOuterInner,
        SpiralGraphFillOuterOnly,
        SpiralGraphFillOuterInner,
        // Solid metal gaskets
        AluminumSoft,
        CopperOrBrassSoft,
        IronSoft,
        SteelSoft,
        SteelLowAlloyHeatRes,
        StainlessSteel,
        StainLessSteelHeatRes,
        // Covered metal jacketed gaskets
        SoftIronOrSteelJackGraphFillCover,
        LowAlloyOrStainlessSteelGraphFillCover,
        StainlessSteelJacketPtfeFillCover,
        NickelAlloyPtfeFillCover,
        // Metal jacketed gaskets
        AluminumSoftGraphFill,
        CopperOrBrassSoftGraphFill,
        SoftIronOrSteelGraphFill,
        LowAlloyOrStainlessSteelGraphFill
    };

    RB_String matCode; // Gasket code from database
    FormType frmType;
    InsFilLayMatType insType;
    double dG0;
    double dG1;
    double dG2;
    double dG1_EN13555;
    double dG2_EN13555;
    double eGt;
    double K;
    double phiG;
    double r2;

    // Use eG, E_G(0) and Q_smax from EN13445 tables in appendix G
    bool mIsSimplified;
};


class Gasket_OUT : public Gasket_IN {

public:
    Gasket_OUT();

    double bGi;
    double bGiOct;
    double bGp;
    double bGt;
    double dGt;
    double AGt;

    double AGe;
    double AQ;
    double bGe;
    double dGe;

    double XG;
};


class Gasket : public Gasket_OUT {

public:
    Gasket();
    virtual ~Gasket();

    void Calc_bGt();
    void Calc_dGt();
    void Calc_AGt();
    void Calc_bGe(int loadCaseNo);
    void Calc_AGe();
    void Calc_E_G(int loadCaseNo);
    void Calc_eG(int loadCaseNo);
    void Calc_Q_smax(int loadCaseNo);
    void Calc_XG(int loadCaseNo);
    void Calc_AQ();
    void Calc_P_QR(int loadCaseNo);
    double gasketCompressedElasticity(int loadCaseNo, LoadCase* loadCase);
    double gasketCompressedThickness(LoadCase* loadCase);
    double gasketMaximumLoad(int loadCaseNo, LoadCase* loadCase);
    double gasketCreepFactor(int loadCaseNo, LoadCase* loadCase);

    LoadCaseList* mLoadCaseList;

};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //GASKET_H
