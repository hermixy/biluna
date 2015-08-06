#include "forcetest.h"
NAMESPACE_REDBAG_CALC_EN1591


ForceTest::ForceTest(RB_ObjectContainer *inputOutput) : RB_UnitTest() {
    target = NULL;
    mInputOutput = inputOutput;
}

ForceTest::~ForceTest() {
    delete target;
    target = NULL;
}

void ForceTest::exec() {
    Calc_F_STest();
    Calc_M_BTest();
}

void ForceTest::setupTarget() {
    if (!target) {
        target = new Force(mInputOutput);
    }
}

void ForceTest::Calc_F_STest() {
    setupTarget();
    target->F_X = 12.3;
    target->F_Y = 345.6;
    target->Calc_F_LI();
    areEqual(target->getLastOutput(), "ForceTest::Calc_F_STest",
             345.8188109400644, target->F_LI);
}

void ForceTest::Calc_M_BTest() {
    setupTarget();
    target->M_X = 2.3;
    target->M_Y = 45.6;
    target->Calc_M_AI();
    areEqual(target->getLastOutput(), "ForceTest::Calc_M_BTest()",
             45.657967541273671, target->M_AI);
}

END_NAMESPACE_REDBAG_CALC_EN1591
