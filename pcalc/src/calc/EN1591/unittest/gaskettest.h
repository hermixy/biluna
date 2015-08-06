#ifndef GASKETTEST_H
#define GASKETTEST_H

#include "gasket.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_REDBAG_CALC_EN1591

class GasketTest : public RB_UnitTest {

public:
    GasketTest(RB_ObjectContainer* inputOutput);
    virtual ~GasketTest();

    void exec();

private:
    void setupTarget();

    void Calc_AGtTest();
    void Calc_bGtTest();
    void Calc_dGtTest();
    void Calc_AGeTest();
    void Calc_AQTest();
    void Calc_bGeTest();
    void Calc_XGTest();

    Gasket *target;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //GASKETTEST_H
