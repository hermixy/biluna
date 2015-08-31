#include "bolt.h"
#include "pcalc_report.h"
NAMESPACE_REDBAG_CALC_EN1591

Bolt_IN::Bolt_IN() : RB_Object(){
    setName("PCALC EN1591 Bolt");

    bType = Stud;
    tType = ManualStandardRing;
    dB0 = 0;
    dB2 = 0;
    dB4 = 0;
    dBe = 0;
    dBS = 0;
    eN = 0;
    l5t = 0;
    lS = 0;
    mut = 0;
    ruptureElongationA = 0;

    mBoltHole = new BoltHole();
}

Bolt_IN::~Bolt_IN() {
    delete mBoltHole;
    mBoltHole = NULL;
}

Bolt_OUT::Bolt_OUT() : Bolt_IN(){
    AB = 0.0;
    IB = 0.0;
    lB = 0.0;
    kB = 0.0;
    XB = 0.0;
    eta1plus = 0.0;
    etanplus = 0.0;
    eta1minus = 0.0;
    etanminus = 0.0;

}

/**
 * @brief Table B.1: High scatter value of one bolt tensioning
 */
Bolt::Bolt() : Bolt_OUT(){
    // nothing
}

void Bolt::Calc_eta1plus() {
    switch (tType) {
    case ManualOperatorFeel:
        eta1plus = 0.3 + 0.5 * mut;
        PR->addDetail("Table B.1", "eta1plus", "0.3 + 0.5 * mut", eta1plus, "-");
        break;
    case Impact:
        eta1plus = 0.2 + 0.5 * mut;
        PR->addDetail("Table B.1", "eta1plus", "0.2 + 0.5 * mut", eta1plus, "-");
        break;
    case TorqueWrench:
        eta1plus = 0.1 + 0.5 * mut;
        PR->addDetail("Table B.1", "eta1plus", "0.1 + 0.5 * mut", eta1plus, "-");
        break;
    case TensionerMeasureHydraulicPressure:
        eta1plus = 0.4; // only difference
        PR->addDetail("Table B.1", "eta1plus", "0.4", eta1plus, "-");
        break;
    case TensionerMeasureBoltElongation:
        eta1plus = 0.15;
        PR->addDetail("Table B.1", "eta1plus", "0.15", eta1plus, "-");
        break;
    case WrenchMeasureNutTurn:
        eta1plus = 0.1;
        PR->addDetail("Table B.1", "eta1plus", "0.1", eta1plus, "-");
        break;
    case WrenchMeasureTorquePlusNutTurn:
        eta1plus = 0.07;
        PR->addDetail("Table B.1", "eta1plus", "0.07", eta1plus, "-");
        break;
    default:
        // Default and ~ManualStandardRing (not defined as such)
        eta1plus = 0.5;
        PR->addDetail("With B.3, 116", "eta1plus", "0.5", eta1plus, "-");
        break;
    }
}

/**
 * @brief Table B.1: Low scatter value of one bolt tensioning
 */
void Bolt::Calc_eta1minus() {
    switch (tType) {
    case ManualOperatorFeel:
        eta1minus = 0.3 + 0.5 * mut;
        PR->addDetail("Table B.1", "eta1minus", "0.3 + 0.5 * mut", eta1minus, "-");
        break;
    case Impact:
        eta1minus = 0.2 + 0.5 * mut;
        PR->addDetail("Table B.1", "eta1minus", "0.2 + 0.5 * mut", eta1minus, "-");
        break;
    case TorqueWrench:
        eta1minus = 0.1 + 0.5 * mut;
        PR->addDetail("Table B.1", "eta1minus", "0.1 + 0.5 * mut", eta1minus, "-");
        break;
    case TensionerMeasureHydraulicPressure:
        eta1minus = 0.2; // only difference
        PR->addDetail("Table B.1", "eta1minus", "0.4", eta1minus, "-");
        break;
    case TensionerMeasureBoltElongation:
        eta1minus = 0.15;
        PR->addDetail("Table B.1", "eta1minus", "0.15", eta1minus, "-");
        break;
    case WrenchMeasureNutTurn:
        eta1minus = 0.1;
        PR->addDetail("Table B.1", "eta1minus", "0.1", eta1minus, "-");
        break;
    case WrenchMeasureTorquePlusNutTurn:
        eta1minus = 0.07;
        PR->addDetail("Table B.1", "eta1minus", "0.07", eta1minus, "-");
        break;
    default:
        // Default and ManualStandardRing
        eta1minus = 0.5;
        PR->addDetail("With B.3 116", "eta1minus", "0.5", eta1minus, "-");
        break;
    }
}

// Torsional resistance
void Bolt::Calc_IB() {
    IB = (M_PI / 12) * (pow((std::min(dBe, dBS)), 3));
    PR->addDetail("With F.", "IB",
                  "(pi / 12) * (min(dBe, dBS)) ^ 3", IB, "mm3",
                  "(pi / 12) * (min(" + QN(dBe) + ", " + QN(dBS) + ")) ^ 3");
}

/**
 * @brief Annex B, B.7 calculation of friction factor kB
 */
void Bolt::Calc_kB() {
    if (pt <= 0.0) {
        // from table B EN1591-2001 for ISO bolts/thread and Table A
        pt = (dB0 - dBe) / 0.9382;
        PR->addDetail("Before F. B.7", "pt", "(dB0 - dBe) / 0.9382",
                  pt, "Nmm", "(" + QN(dB0) + " - " + QN(dBe)
                      + ") / 0.9382");
    }

    kB = 0.159 * pt + 0.577 * mut * dB2 + 0.5 * mun * dn;
    PR->addDetail("Formula B.7", "kB",
                  "0.159 * pt + 0.577 * mut * dB2 + 0.5 * mun * dn", kB, "-",
                  "0.159 * " + QN(pt) + " + 0.577 * " + QN(mut) + " * "
                  + QN(dB2) + " + 0.5 * " + QN(mun) + " * " + QN(dn));
}

END_NAMESPACE_REDBAG_CALC_EN1591
