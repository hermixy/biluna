#ifndef FLANGE_BLINDTEST_H
#define FLANGE_BLINDTEST_H

#include "flange_blind.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_BILUNA_CALC_EN1591

class Flange_BlindTest : public RB_UnitTest {

public:
    Flange_BlindTest();
    virtual ~Flange_BlindTest();

    void exec();

private:
    void setupTarget();

    void Calc_ZFTest();
    void Calc_ZLTest();
    void Calc_bFTest();
    void Calc_dETest();
    void Calc_dFTest();
    void Calc_dLTest();
    void Calc_eETest();
    void Calc_AFTest();
    void Calc_eLTest();
    void Calc_hGTest();
    void Calc_hHTest();
    void Calc_hLTest();
    void Calc_hPTest();
    void Calc_hQTest();
    void Calc_hRTest();
    void Calc_rhoTest();
    void Is_d0_ValidTest();
    void Calc_WFTest();
    void Calc_PhiFTest();
    void Is_flange_ValidTest();
    void Calc_WXTest();
    void Calc_PhiXTest();

    Flange_Blind *target;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //FLANGE_BLINDTEST_H
