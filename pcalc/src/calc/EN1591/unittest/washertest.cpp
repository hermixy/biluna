#include "washertest.h"
NAMESPACE_REDBAG_CALC_EN1591


WasherTest::WasherTest() : RB_UnitTest() {
    target = NULL;
}

WasherTest::~WasherTest() {
    delete target;
    target = NULL;
}

void WasherTest::exec() {
    Calc_bWTest();
    Calc_dWTest();
    Calc_bKBTest();
}

void WasherTest::setupTarget() {
    if (!target) {
        target = new Washer();
    }
    target->washerNumber = 1;
}

void WasherTest::Calc_bWTest() {
    setupTarget();
    target->eW = 1.0;
    target->dW2 = 18.3;
    target->dW1 = 7.7;
    target->Calc_bW();
    areEqual(PR->getLastOutput(), "WasherTest::Calc_bWTest()", 5.3, target->bW);
}

void WasherTest::Calc_dWTest() {
    setupTarget();
    target->eW = 1.0;
    target->dW2 = 18.3;
    target->dW1 = 7.7;
    target->Calc_dW();
    areEqual(PR->getLastOutput(), "WasherTest::Calc_dWTest()", 13.0, target->dW);
}

void WasherTest::Calc_bKBTest() {
    setupTarget();
    target->eW = 2;
    target->dW2 = 3;
    target->dK2 = 7.6;
    target->dW1 = 2.7;
    target->Calc_bKB();
    areEqual(PR->getLastOutput(), "WasherTest::Calc_bKBTest()", 2.45, target->bKB);
}

END_NAMESPACE_REDBAG_CALC_EN1591
