#include "rb_tablemathtest.h"
NAMESPACE_BILUNA_CALC


RB_TableMathTest::RB_TableMathTest() : RB_UnitTest() {
    target = NULL;
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
        target = new RB_TableMath();
    }
}

/**
 * @brief Values based on http://en.wikipedia.org/wiki/Bilinear_interpolation
 */
void RB_TableMathTest::getBilinearValueTest() {
    setupTarget();
    double res = target->getBilinearValue(14.5, 20.2,
                                          14.0, 20.0, 91.0,
                                          15.0, 20.0, 210,
                                          14.0, 21.0, 162.0,
                                          15.0, 21.0, 95.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 146.1, res);
    res = target->getBilinearValue(1.0, 0.0,
                                   -1.0, 3.0, 1.0,
                                   3.0, 2.0, 2.0,
                                   -2.0, -1.0, 5.0,
                                   2.0, -2.0, 6.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 4.0, res);
    res = target->getBilinearValue(-1.0, 0.0,
                                   -1.0, 3.0, 2.0,
                                   -1.0, 3.0, 2.0,
                                   -1.0, -1.0, 5.0,
                                   -1.0, -1.0, 5.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 4.25, res);
    res = target->getBilinearValue(1.0, 0.0,
                                   -1.0, 0.0, 1.0,
                                   3.0, 0.0, 6.0,
                                   -1.0, 0.0, 1.0,
                                   3.0, 0.0, 6.0);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 3.5, res);
    res = target->getBilinearValue(1.0, 0.0,
                                   1.0, 0.0, 1.3,
                                   1.0, 0.0, 1.3,
                                   1.0, 0.0, 1.3,
                                   1.0, 0.0, 1.3);
    areEqual(PR->getLastOutput(),
             "RB_TableMathTest::Calc_getBilinearValueTest()", 1.3, res);
}

END_NAMESPACE_BILUNA_CALC
