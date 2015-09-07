#ifndef FLANGE_BLIND_H
#define FLANGE_BLIND_H

#include "flange.h"
#include "rb_namespace.h"

NAMESPACE_BILUNA_CALC_EN1591

class Flange_Blind : public Flange {
    //IN
    // nothing


    //OUT
    // nothing

public:
    Flange_Blind(int flangeNo);
    virtual ~Flange_Blind();

    virtual bool Is_flange_Valid();
    virtual void Calc_bF();
    virtual void Calc_dF();
    virtual void Calc_AF();
    virtual void Calc_bL();
    virtual void Calc_dL();
    virtual void Calc_AL();
    virtual void Calc_eE();
    virtual void Calc_dE();
    virtual void Calc_rho();
    virtual void Calc_hQ();
    virtual void Calc_hR();
    virtual void Calc_ZF();
    virtual void Calc_ZL();
    virtual void Calc_hP();
    virtual void Calc_hG();
    virtual void Calc_hH();
    virtual void Calc_hL();
    virtual bool Is_d0_Valid();
    virtual void Calc_WF(int loadCaseNo);
    virtual void Calc_PhiF(int loadCaseNo);
    virtual void Calc_PhiX(int loadCaseNo);
    virtual bool Is_PhiX_Valid(int loadCaseNo);
    virtual void Calc_WX(int loadCaseNo);

    virtual void Calc_cM(int loadCaseNo);
    virtual bool Is_cM_valid(int loadCaseNo);
    virtual void Calc_cS(int loadCaseNo);
    virtual bool Is_cS_valid(int loadCaseNo);
    virtual void Calc_jM(int loadCaseNo);
    virtual void Calc_PsiOpt(int loadCaseNo);
    virtual void Calc_Psi0(int loadCaseNo);
    virtual void Calc_PsiMax(int loadCaseNo);
    virtual void Calc_PsiMin(int loadCaseNo);
    virtual void Calc_PsiZ(int loadCaseNo);
    virtual bool Is_PsiMaxMin_Valid(int loadCaseNo);
    virtual void Calc_kM_for_WFmax(int loadCaseNo);
    virtual void Calc_deltaQ(int loadCaseNo);
    virtual void Calc_deltaR(int loadCaseNo);
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //FLANGE_BLIND_H
