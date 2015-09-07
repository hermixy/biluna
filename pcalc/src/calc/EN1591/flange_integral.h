#ifndef FLANGE_INTEGRAL_H
#define FLANGE_INTEGRAL_H

#include "flange.h"
#include "rb_objectcontainer.h"

NAMESPACE_BILUNA_CALC_EN1591

class Flange_Integral : public Flange {
    //IN
    // nothing

    //OUT
    // nothing

public:
    Flange_Integral(int flangeNo);
    virtual ~Flange_Integral();

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
    virtual void Calc_hQ();
    virtual void Calc_hR();
    virtual void Calc_kQ();
    virtual void Calc_kR();
    virtual void Calc_ZF();
    virtual void Calc_ZL();
    virtual void Calc_hG();
    virtual void Calc_hH();
    virtual void Calc_hL();
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //FLANGE_INTEGRAL_H
