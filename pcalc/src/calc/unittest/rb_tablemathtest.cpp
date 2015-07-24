#include "rb_tablemathtest.h"
NAMESPACE_REDBAG_CALC


RB_TableMathTest::RB_TableMathTest(RB_ObjectContainer *inputOutput) {
    target = NULL;
    mInputOutput = inputOutput;
}

RB_TableMathTest::~RB_TableMathTest() {
    delete target;
    target = NULL;
}

void RB_TableMathTest::exec() {
    getBilinearValueTest();
}

void RB_TableMathTest::setupTarget() {
    if (!target) {
        target = new RB_TableMath(mInputOutput);
    }
}

/**
 * @brief Values from http://en.wikipedia.org/wiki/Bilinear_interpolation
 */
void RB_TableMathTest::getBilinearValueTest() {
    setupTarget();
    double res = target->getBilinearValue(14.5, 20.2,
                                          14.0, 20.0, 91.0,
                                          15.0, 20.0, 210,
                                          14.0, 21.0, 162.0,
                                          15.0, 21.0, 95.0);
    areEqual(target->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 146.1, res);
}

END_NAMESPACE_REDBAG_CALC
