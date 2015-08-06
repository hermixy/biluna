#include "bolttest.h"
NAMESPACE_REDBAG_CALC_EN1591


BoltTest::BoltTest(RB_ObjectContainer* inputOutput) : RB_UnitTest() {
    target = NULL;
    mInputOutput = inputOutput;
}

BoltTest::~BoltTest() {
    delete target;
    target = NULL;
}

void BoltTest::exec() {
    Calc_eta1plusTest();
    Calc_eta1minusTest();
    Calc_IBTest();
}

void BoltTest::setupTarget() {
    if (!target) {
        target = new Bolt(mInputOutput);
    }

    target->bType = Bolt::Stud;

    target->dB0 = 15.2;
    target->dBe = 5.4;
    target->dBS = 3.2;

    //target.dB4 = 8.9

    target->lB = 1.3;
    target->lS = 5.4;

    target->eta1plus = 0.25;
    target->eta1minus = 0.12;
}

void BoltTest::Calc_eta1plusTest() {
    setupTarget();
    target->mut = 0.21;
    target->tType = Bolt::ManualOperatorFeel;
    target->Calc_eta1plus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1plusTest()", 0.405, target->eta1plus);

    target->tType = Bolt::Impact;
    target->Calc_eta1plus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1plusTest()", 0.305, target->eta1plus);

    target->tType = Bolt::TorqueWrench;
    target->Calc_eta1plus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1plusTest()", 0.205, target->eta1plus);

    target->tType = Bolt::TensionerMeasureHydraulicPressure;
    target->Calc_eta1plus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1plusTest()", 0.4, target->eta1plus);

    target->tType = Bolt::TensionerMeasureBoltElongation;
    target->Calc_eta1plus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1plusTest()", 0.15, target->eta1plus);

    target->tType = Bolt::WrenchMeasureNutTurn;
    target->Calc_eta1plus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1plusTest()", 0.1, target->eta1plus);

    target->tType = Bolt::WrenchMeasureTorquePlusNutTurn;
    target->Calc_eta1plus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1plusTest()", 0.07, target->eta1plus);
}

void BoltTest::Calc_eta1minusTest() {
    setupTarget();
    target->mut = 0.22;
    target->tType = Bolt::ManualOperatorFeel;
    target->Calc_eta1minus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1minusTest()", 0.41, target->eta1minus);

    target->tType = Bolt::Impact;
    target->Calc_eta1minus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1minusTest()", 0.31, target->eta1minus);

    target->tType = Bolt::TorqueWrench;
    target->Calc_eta1minus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1minusTest()", 0.21, target->eta1minus);

    target->tType = Bolt::TensionerMeasureHydraulicPressure;
    target->Calc_eta1minus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1minusTest()", 0.2, target->eta1minus);

    target->tType = Bolt::TensionerMeasureBoltElongation;
    target->Calc_eta1minus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1minusTest()", 0.15, target->eta1minus);

    target->tType = Bolt::WrenchMeasureNutTurn;
    target->Calc_eta1minus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1minusTest()", 0.1, target->eta1minus);

    target->tType = Bolt::WrenchMeasureTorquePlusNutTurn;
    target->Calc_eta1minus();
    areEqual(target->getLastOutput(), "BoltTest::Calc_eta1minusTest()", 0.07, target->eta1minus);
}

void BoltTest::Calc_IBTest() {
    setupTarget();
    target->dBe = 4.2;
    target->dBS = 1.3;
    target->Calc_IB();
    areEqual(target->getLastOutput(), "BoltTest::Calc_IBTest()", 0.57517325499473126, target->IB);
    target->dBS = 77.5;
    target->Calc_IB();
    areEqual(target->getLastOutput(), "BoltTest::Calc_IBTest()", 19.396193043263384, target->IB);
}

END_NAMESPACE_REDBAG_CALC_EN1591
