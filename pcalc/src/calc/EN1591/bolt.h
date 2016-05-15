#ifndef BOLT_H
#define BOLT_H

#include "bolthole.h"
#include "rb_namespace.h"
#include "rb_object.h"

NAMESPACE_BILUNA_CALC_EN1591

class Bolt_IN : public RB_Object {

public:
    Bolt_IN();
    virtual ~Bolt_IN();

    enum BoltType {
        HexagonHeaded,
        Stud,
        WaistedStud
    };

    enum TensionerType {
        ManualStandardRing, // for formula B.3
        ManualOperatorFeel,
        Impact,
        TorqueWrench,
        TensionerMeasureHydraulicPressure,
        TensionerMeasureBoltElongation,
        WrenchMeasureNutTurn,
        WrenchMeasureTorquePlusNutTurn
    };

    // materialIdx, material type  and id from database for material properties
    // same purpose as gasketIdx for gaskets
    RB_String materialIdx;

    BoltType bType;
    TensionerType tType;
    double dB0;
    double dB2;
    double dB4;
    double dBe;
    double dBS;
    double dn; // B.7 calculation
    double eN;
    double l5t;
    double lS;
    double mun;
    double mut;
    double pt;
    double ruptureElongationA;

    BoltHole* mBoltHole;
};


class Bolt_OUT : public Bolt_IN {

public:
    Bolt_OUT();

    double AB;
    double IB;
    double kB; // B.7 calculation
    double kB9; // B.9 calculation
    double lB;
    double XB;
    double eta1plus;
    double etanplus;
    double eta1minus;
    double etanminus;
};


class Bolt : public Bolt_OUT {

public:
    Bolt();

    void setValid_dBS();
    void setValid_dn();
    void setValid_pt();

    void Calc_eta1plus();
    void Calc_eta1minus();
    void Calc_IB();
    void Calc_kB();
    void Calc_kB9();
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //BOLT_H
