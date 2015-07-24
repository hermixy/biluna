#ifndef FLANGE_LOOSE_H
#define FLANGE_LOOSE_H

#include "flange.h"
#include "rb_namespace.h"

NAMESPACE_REDBAG_CALC_EN1591

class Flange_Loose : public Flange {
    //IN
    // nothing

    //OUT
    // nothing

public:
    Flange_Loose(int flangeNo, RB_ObjectContainer* inputOutput);
    virtual ~Flange_Loose();

    virtual bool Is_flange_Valid();
    virtual void Calc_bF();
    virtual void Calc_dF();
    virtual void Calc_AF();
    virtual void Calc_bL();
    virtual void Calc_dL();
    virtual void Calc_AL();
    virtual void Calc_beta();
    virtual void Calc_eD();
    virtual void Calc_eE();
    virtual void Calc_dE();
    virtual void Calc_gamma();
    virtual void Calc_theta();
    virtual void Calc_lambda();
    virtual void Calc_cF();
    virtual void Calc_hS();
    virtual void Calc_hT();
    virtual void Calc_hQ(int loadCaseNo);
    virtual void Calc_hR();
    virtual void Calc_kQ();
    virtual void Calc_kR();
    virtual void Calc_ZF();
    virtual void Calc_ZL();
    virtual void Calc_d7min();
    virtual void Calc_d7max();
    virtual void Calc_d70(int loadCaseNo);
    virtual void Calc_d7minMax(int loadCaseNo);
    virtual void Calc_chi(int loadCaseNo);
    virtual void Calc_hG(int loadCaseNo);
    virtual void Calc_hH(int loadCaseNo);
    virtual void Calc_hL(int loadCaseNo);
    virtual void Calc_WL(int loadCaseNo);
    virtual void Calc_PhiL(int loadCaseNo);
    virtual bool Is_PhiL_valid(int loadCaseNo);
    virtual void Calc_WQ(int loadCaseNo);
    virtual void Calc_PhiF(int loadCaseNo);

    // ''' <summary>
    // ''' Flange rotation loose ring
    // ''' </summary>
    // ''' <param name="loadCaseNo"></param>
    // ''' <remarks>Formula E.2</remarks>
    //Public Overrides Sub Calc_ThetaL(ByVal loadCaseNo As Integer)
    //    With LoadCaseList(loadCaseNo)
    //        Dim tmp_E_L As Double = .EL1
    //        Dim tmp_hL As Double = .hL1

    //        If (flangeNumber = 2) Then
    //            tmp_E_L = .EL2
    //            tmp_hL = .hL2
    //        End If

    //        Dim tmp_ThetaL As Double = (ZL / tmp_E_L) * (.F_B * tmp_hL)

    //        If (flangeNumber = 1) Then
    //            .ThetaL1 = tmp_ThetaL
    //        ElseIf (flangeNumber = 2) Then
    //            .ThetaL2 = tmp_ThetaL
    //        End If
    //    End With
    //End Sub


};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //FLANGE_LOOSE_H
