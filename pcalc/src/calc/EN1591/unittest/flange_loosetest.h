#ifndef FLANGE_LOOSETEST_H
#define FLANGE_LOOSETEST_H

#include "flange_loose.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_REDBAG_CALC_EN1591

class Flange_LooseTest : public RB_UnitTest {

public:
    Flange_LooseTest();
    virtual ~Flange_LooseTest();

    void exec();

private:
    void setupTarget();

    void Calc_ZFTest();
    void Calc_ZLTest();
    void Calc_bFTest();
    void Calc_bLTest();
    void Calc_cFTest();
    void Calc_d7Test();
    void Calc_d7maxTest();
    void Calc_d7minTest();
    void Calc_betaTest();
    void Calc_dETest();
    void Calc_dFTest();
    void Calc_dLTest();
    void Calc_eETest();
    void Calc_AFTest();
    void Calc_ALTest();
    void Calc_gammaTest();
    void Calc_hGTest();
    void Calc_hHTest();
    void Calc_hLTest();
    void Calc_hQTest();
    void Calc_hRTest();
    void Calc_hSTest();
    void Calc_hTTest();
    void Calc_kQTest();
    void Calc_kRTest();
    void Calc_chiTest();
    void Calc_lambdaTest();
    void Calc_thetaTest();
    void Calc_WLTest();
    void Calc_PhiLTest();
    void Is_PhiL_validTest();
    void Calc_WQTest();
    void Calc_WFTest();
    void Calc_PhiFTest();

    // '''<summary>
    // '''A test for Calc_ThetaL
    // '''</summary>
    //<TestMethod()> _
    //Public Sub Calc_ThetaLTest()
    //    setupTarget()
    //    Dim loadCaseNo As Integer = 0

    //    With target.LoadCaseList(loadCaseNo)
    //        target.ZL = 7.8
    //        .EL1 = 6.7
    //        .F_B = 4.5
    //        .hL1 = 9.4
    //        target.Calc_ThetaL(loadCaseNo)
    //        Assert.AreEqual(49.244776119402985, .ThetaL1, ACCURACY)
    //    End With

    //End Sub

    void Is_flange_ValidTest();

    Flange_Loose *target;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //FLANGE_LOOSETEST_H
