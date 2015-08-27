#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <cmath>
#include "bolt.h"
#include "bolthole.h"
#include "flange_blind.h"
#include "flange_integral.h"
#include "flange_loose.h"
#include "gasket.h"
#include "loadcase.h"
#include "loadcaselist.h"
#include "rb_namespace.h"
#include "rb_object.h"
#include "shell.h"
#include "tableeproperty.h"
#include "tablegproperty.h"
#include "washer.h"

NAMESPACE_REDBAG_CALC_EN1591

class Assembly_IN : public RB_Object {

public:
    Assembly_IN();
    virtual ~Assembly_IN();

    Bolt* mBolt;
    Gasket* mGasket;
    Flange* mFlange1;
    Flange* mFlange2;
    LoadCaseList* mLoadCaseList;
    double mNR;
    double mLeakageRate;
    double mF_Bspec; // transferred to loadcase(0)
    double mQ_Aspec; // transferred to loadcase(0)
};


class Assembly_OUT : public Assembly_IN {

public :
    Assembly_OUT();
    virtual ~Assembly_OUT() { }

    double mPhiMax;
};


class Assembly : public Assembly_OUT {

public:
    Assembly();

    void Calc_F_GInitial();
    void Calc_dGe();
    void Calc_bGi(int loadCaseNo, bool isFirstApproximation = false);
    void Calc_Q_G();
    void Calc_F_Q(int loadCaseNo);
    void Calc_F_R(int loadCaseNo);
    void Calc_dUI(int loadCaseNo);
    void Calc_lB();
    void Calc_YB(int loadCaseNo);
    void Calc_YG(int loadCaseNo);
    void Calc_YQ(int loadCaseNo);
    void Calc_YR(int loadCaseNo);
    void Calc_Q_A_Qsmin(int loadCaseNo);
    void Calc_F_Gmin(int loadCaseNo);
    void Calc_delta_eGc(int loadCaseNo);
    void Calc_F_Gdelta(int loadCaseNo);
    void Calc_F_G0req();
    void Calc_F_B0req();

    bool Is_F_G0_larger_F_G0req();
    bool Is_F_G0act_within_0_1_percent_of_F_G0req();

    void Calc_F_B0av();
    void Calc_F_B0nom();

    bool Is_F_B0nom_Valid();

    void Calc_F_G0max();
    void Calc_F_G0d();
    void Calc_F_G(int loadCaseNo);
    void Calc_F_B(int loadCaseNo);
    void Calc_Mtnom();
    void Calc_MtB(); // not used any more
    void Calc_PhiB(int loadCaseNo);

    bool Is_PhiB_Valid(int loadCaseNo);

    void Calc_cA(int loadCaseNo);
    void Calc_cB(int loadCaseNo);
    void Calc_PhiG(int loadCaseNo);

    bool Is_PhiG_Valid(int loadCaseNo);

    void Calc_fE(Flange *flange, int loadCaseNo);
    void Calc_F_BImaxmin(int loadCaseNo);
    void Calc_F_GImaxmin(int loadCaseNo);
    void Calc_ThetaFmaxmin(int loadCaseNo);
    void Calc_ThetaLmaxmin(int loadCaseNo);

private:
    double Calc_cB_helper(Flange *flange, int loadCaseNo);
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //ASSEMBLY_H
