#ifndef FLANGETEST_H
#define FLANGETEST_H

#include "flange.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_REDBAG_CALC_EN1591

class FlangeTest : public RB_UnitTest {

public:
    FlangeTest();
    virtual ~FlangeTest();

    void exec();

private:
    void setupTarget();

    void Calc_d3eTest();
    void Calc_d5eTest();
    void Calc_pBTest();
    void Calc_hPTest();
    void Calc_eQTest();
    void Calc_ABTest();
    void Calc_XBTest();
    void Calc_etanminusTest();
    void Calc_etanplusTest();
    void Calc_deltaQTest();
    void Calc_deltaRTest();
    void Calc_cMTest();
    void Is_cM_validTest();
    void Calc_cSTest();
    void Is_cS_validTest();
    void Calc_jMTest();
    void Calc_PsiOptTest();
    void Calc_Psi0Test();
    void Calc_PsiMaxTest();
    void Is_PsiMaxMin_ValidTest();
    void Calc_PsiZTest();
    void Calc_WFTest();
    void Calc_PhiFTest();
    void Is_PhiF_validTest();
    void Calc_dK1Test();
    void Calc_dK2Test();
    void Calc_XWTest();

    Flange *target;

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //WASHERTEST_H
