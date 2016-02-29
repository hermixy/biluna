#ifndef GASKETTEST_H
#define GASKETTEST_H

#include "gasket.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_BILUNA_CALC_EN1591

class GasketTest : public RB_UnitTest {

public:
    GasketTest();
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
    void Calc_delta_eGcTest();

    Gasket* target;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //GASKETTEST_H
