#include "bolt.h"
NAMESPACE_REDBAG_CALC_EN1591

Bolt_IN::Bolt_IN(RB_ObjectContainer *inputOutput) : RB_Report(inputOutput){
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

    mBoltHole = new BoltHole(inputOutput);
}

Bolt_IN::~Bolt_IN() {
    delete mBoltHole;
    mBoltHole = NULL;
}

/**
 * @brief Table B.1: High scatter value of one bolt tensioning
 */
Bolt::Bolt(RB_ObjectContainer *inputOutput) : Bolt_OUT(inputOutput){

}

void Bolt::Calc_eta1plus() {
    switch (tType) {
    case ManualOperatorFeel:
        eta1plus = 0.3 + 0.5 * mut;
        addDetail("Table B.1", "eta1plus", "0.3 + 0.5 * mut", eta1plus, "-");
        break;
    case Impact:
        eta1plus = 0.2 + 0.5 * mut;
        addDetail("Table B.1", "eta1plus", "0.2 + 0.5 * mut", eta1plus, "-");
        break;
    case TorqueWrench:
        eta1plus = 0.1 + 0.5 * mut;
        addDetail("Table B.1", "eta1plus", "0.1 + 0.5 * mut", eta1plus, "-");
        break;
    case TensionerMeasureHydraulicPressure:
        eta1plus = 0.4; // only difference
        addDetail("Table B.1", "eta1plus", "0.4", eta1plus, "-");
        break;
    case TensionerMeasureBoltElongation:
        eta1plus = 0.15;
        addDetail("Table B.1", "eta1plus", "0.15", eta1plus, "-");
        break;
    case WrenchMeasureNutTurn:
        eta1plus = 0.1;
        addDetail("Table B.1", "eta1plus", "0.1", eta1plus, "-");
        break;
    case WrenchMeasureTorquePlusNutTurn:
        eta1plus = 0.07;
        addDetail("Table B.1", "eta1plus", "0.07", eta1plus, "-");
        break;
    default:
        // Default and ~ManualStandardRing (not defined as such)
        eta1plus = 0.5;
        addDetail("With B.3, 116", "eta1plus", "0.5", eta1plus, "-");
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
        addDetail("Table B.1", "eta1minus", "0.3 + 0.5 * mut", eta1minus, "-");
        break;
    case Impact:
        eta1minus = 0.2 + 0.5 * mut;
        addDetail("Table B.1", "eta1minus", "0.2 + 0.5 * mut", eta1minus, "-");
        break;
    case TorqueWrench:
        eta1minus = 0.1 + 0.5 * mut;
        addDetail("Table B.1", "eta1minus", "0.1 + 0.5 * mut", eta1minus, "-");
        break;
    case TensionerMeasureHydraulicPressure:
        eta1minus = 0.2; // only difference
        addDetail("Table B.1", "eta1minus", "0.4", eta1minus, "-");
        break;
    case TensionerMeasureBoltElongation:
        eta1minus = 0.15;
        addDetail("Table B.1", "eta1minus", "0.15", eta1minus, "-");
        break;
    case WrenchMeasureNutTurn:
        eta1minus = 0.1;
        addDetail("Table B.1", "eta1minus", "0.1", eta1minus, "-");
        break;
    case WrenchMeasureTorquePlusNutTurn:
        eta1minus = 0.07;
        addDetail("Table B.1", "eta1minus", "0.07", eta1minus, "-");
        break;
    default:
        // Default and ManualStandardRing
        eta1minus = 0.5;
        addDetail("With B.3 116", "eta1minus", "0.5", eta1minus, "-");
        break;
    }
}

// Torsional resistance
void Bolt::Calc_IB() {
    IB = (M_PI / 12) * (pow((std::min(dBe, dBS)), 3));
    addDetail("With Formula 123", "IB", "(Math.PI / 12) * (Math.Min(dBe, dBS)) ^ 3", IB, "-");
}

Bolt_OUT::Bolt_OUT(RB_ObjectContainer* inputOutput) : Bolt_IN(inputOutput){
    AB = 0.0;
    IB = 0.0;
    lB = 0.0;
    XB = 0.0;
    eta1plus = 0.0;
    etanplus = 0.0;
    eta1minus = 0.0;
    etanminus = 0.0;

}


END_NAMESPACE_REDBAG_CALC_EN1591
