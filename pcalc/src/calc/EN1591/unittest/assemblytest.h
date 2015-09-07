#ifndef ASSEMBLYTEST_H
#define ASSEMBLYTEST_H

#include "assembly.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_BILUNA_CALC_EN1591

class AssemblyTest : public RB_UnitTest {

public:
    AssemblyTest();
    virtual ~AssemblyTest();

    void exec();

private:
    void SetupIntegralTarget();
    void SetupLooseTarget();
    void SetupBlindTarget();
    void SetupRemainingTarget();
    void deleteTarget();

    void Calc_dGeTest();
    void Calc_F_GInitialTest();
    void Calc_F_QTest();
    void Calc_F_RTest();
    void Calc_lBTest();
    void Calc_YBTest();
    void Calc_YGTest();
    void Calc_YQTest();
    void Calc_YRTest();
    void Calc_bGiTest();
    void Calc_dUTTest();
    void Calc_F_GminTest();
    void Calc_F_G0deltaTest();
    void Calc_F_G0reqTest();
    void Is_F_G0act_larger_F_G0reqTest();
    void Is_F_G0act_acceptable_to_F_G0reqTest();
    void Calc_F_B0reqTest();
    void Calc_F_B0nomTest();
    void Is_F_B0nom_ValidTest();
    void Calc_F_G0maxTest();
    void Calc_F_G0dTest();
    void Calc_F_GTest();
    void Calc_F_BTest();
    void Calc_cATest();
    void Calc_cBTest();
    void Calc_PhiBTest();
    void Calc_PhiGTest();
    void Is_PhiB_ValidTest();
    void Calc_fETest();
    void Is_PhiG_ValidTest();
    void Calc_MtnomTest();
//    void Calc_MtBTest();
    void Calc_F_BImaxminTest();
    void Calc_F_GImaxminTest();
    void Calc_ThetaFmaxminTest();
    void Calc_ThetaLmaxminTest();
    void Calc_Q_GTest();
    void Calc_delta_eGcTest();
    void Calc_F_B0avTest();

    Assembly* target;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //ASSEMBLYTEST_H
