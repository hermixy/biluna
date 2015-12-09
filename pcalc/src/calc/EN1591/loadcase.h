#ifndef LOADCASE_H
#define LOADCASE_H

#include "force.h"
#include "rb_namespace.h"
NAMESPACE_BILUNA_CALC_EN1591

class LoadCase_IN : public RB_Object {

public:
    LoadCase_IN();
    virtual ~LoadCase_IN();

    Force* mForce;
    double alphaB;
    double alphaG;
    double alphaF1;
    double alphaF2;
    double alphaL1;
    double alphaL2;
    double alphaW1;
    double alphaW2;
    double EB;
    double EF1;
    double EF2;
    double EL1;
    double EL2;
    double EW1;
    double EW2;

    // bolt load specified by user
    double F_Bspec;

    double fB;
    double fF1;
    double fF2;
    double fL1;
    double fL2;
    double fN;
    double fS1;
    double fS2;
    double fW1;
    double fW2;
    double muG;
    double P;
    double T0;
    double TB;
    double TG;
    double TF1;
    double TF2;
    double TL1;
    double TL2;
    double TW1;
    double TW2;
};


class LoadCase_OUT : public LoadCase_IN {

public:
    LoadCase_OUT();

    double EGm;
    double E_G;
    double x;
    double F_R;
    double F_Rnegative;
    double F_Rpositive;
    double F_Q;
    double dUI;

    double P_QR;
    // gasket creep delta ecG in Fomula 105, 106 and 120, 121 and C.8, C.9
    double delta_eGc;

    double delta_eGc_EN13555;
    double eG;
    double F_B;
    double F_Bav;
    double F_Bcalc;
    double F_Bmax;
    double F_Bmin;
    double F_Bnom;
    double F_Breq;

    double F_G;
    double F_Gd;
    double F_Gdelta;
    double F_Gmax;
    double F_Gmin;
    double F_Greq;
    double DGm;
    double Y_B;
    double Y_G;
    double Y_Q;
    double Y_R;

    // Bolt and washer
    double cA;
    double cB;
    double PhiB;
    double fCF1;
    double fCF2;
    double PhiC1;
    double PhiC2;
    double fCB1;
    double fCB2;
    double Sw1;
    double Sw2;
    double PhiCF1;
    double PhiCF2;
    double PhiCB1;
    double PhiCB2;

    // Gasket
    double PhiG;
    double cG;

    // Flange
    double fE1;
    double fE2;
    double deltaQ1;
    double deltaQ2;
    double deltaR1;
    double deltaR2;
    double Psi01;
    double Psi02;
    double PsiMax1;
    double PsiMax2;
    double PsiMin1;
    double PsiMin2;
    double PsiOpt1;
    double PsiOpt2;
    double Psi_jkk1;
    double Psi_jkk2;
    double PsiZ1;
    double PsiZ2;
    double cM1;
    double cM2;
    double cS_minus1;
    double cS_minus2;
    double cS_plus1;
    double cS_plus2;
    int jM1;
    int jM2;
    int jS;
    double kM1;
    double kM2;
    int kS;
    double WF1;
    double WF2;
    double PhiF1;
    double PhiF2;
    double WX1;
    double WX2;
    double PhiX1;
    double PhiX2;
    double WL1;
    double WL2;
    double PhiL1;
    double PhiL2;
    double WQ1;
    double WQ2;
    double MtBnom;
    double Mtnom;
    double Q_A;
    double Q_G;
    double Q_Gmin;
    double Q_Gmax;
    double Q_0min;
    double Q_minL;
    double Q_sminL;
    double Q_smax;
    double ThetaF1min;
    double ThetaF1max;
    double ThetaF2min;
    double ThetaF2max;
    double ThetaL1min;
    double ThetaL1max;
    double ThetaL2min;
    double ThetaL2max;
};


class LoadCase : public LoadCase_OUT {

public:
    LoadCase() : LoadCase_OUT() {
        // nothing
    }

    void init();
    void setInputData(LoadCase* loadCaseIn);
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //LOADCASE_H
