#ifndef FLANGE_INTEGRALTEST_H
#define FLANGE_INTEGRALTEST_H

#include "flange_integral.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_BILUNA_CALC_EN1591

class Flange_IntegralTest : public RB_UnitTest {

public:
    Flange_IntegralTest();
    virtual ~Flange_IntegralTest();

    void exec();

private:
    void setupTarget();

    void Calc_thetaTest();
    void Calc_lambdaTest();
    void Calc_kRTest();
    void Calc_kQTest();
    void Calc_hTTest();
    void Calc_hSTest();
    void Calc_hRTest();
    void Calc_hQTest();
    void Calc_hLTest();
    void Calc_hHTest();
    void Calc_hGTest();
    void Calc_gammaTest();
    void Calc_ALTest();
    void Calc_eETest();
    void Calc_eDTest();
    void Calc_dLTest();
    void Calc_dFTest();
    void Calc_betaTest();
    void Calc_dETest();
    void Calc_cFTest();
    void Calc_bLTest();
    void Calc_bFTest();
    void Calc_ZLTest();
    void Calc_ZFTest();
    void Is_flange_ValidTest();

    Flange_Integral *target;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //FLANGE_INTEGRALTEST_H
