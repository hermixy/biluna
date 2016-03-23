#ifndef FLANGE_H
#define FLANGE_H

#include "bolt.h"
#include "gasket.h"
#include "rb_namespace.h"
#include "shell.h"
#include "washer.h"

NAMESPACE_BILUNA_CALC_EN1591

class Flange_IN : public RB_Object {

public:
    Flange_IN(int flangeNo);
    virtual ~Flange_IN();

    int getFlangeNumber() { return mFlangeNumber; }
    virtual void addDetail(const QString& formulaNumber,
                           const QString& variableName,
                           const QString& formula,
                           double result,
                           const QString& unit,
                           const QString& formulaValues = "",
                           int loadCaseNo = -1,
                           const QString &note = "");

    //! for unittest only
    void setFlangeNumber(int flangeNumber) {
        mFlangeNumber = flangeNumber;
    }

    static double ACCURACY; //1.0e-6 required by integral a.o.

    // materialIdx, material type  and id from database for material properties
    // same purpose as gasketIdx for gaskets
    RB_String materialIdx;
    RB_String materialLooseIdx;

    int nB;
    double d0;
    double d3;
    double d4;
    double dREC;
    double dRF;
    double dX; // in case of groove
    double eFb;
    double eREC;
    double eRF;
    double eQ;
    double eX; // in case of groove
    double temp_d7;

    // Blind only
    double d9;
    double e0;

    // Integral only
    double d1;
    double d2;

    // Integral loose only
    double e1;
    double e2;
    double lH;

    // Loose only
    double b0;
    double d6;
    double d8;
    double eL;

    Shell* mShell;
    Washer* mWasher;

    // for quick reference
    Bolt* mBolt;
    Gasket* mGasket;
    LoadCaseList* mLoadCaseList;

protected:
    int mFlangeNumber;

};


class Flange_OUT : public Flange_IN {

public:
    Flange_OUT(int flangeNo);
    virtual ~Flange_OUT() { }

    double pB;
    double d5e;
    double d3e;
    double eF;
    double eFt;
    double eP;
    double bF;
    double dF;
    double AF;
    double bL;
    double dL;
    double AL;
    double beta;
    double eD;
    double dE;
    double eE;
    double rho;
    double gamma;
    double theta;
    double lambda;
    double cF;

    double d7;
    double hG;
    double hH;
    double hL;

    double hS;
    double hT;
    double kQ;
    double kR;
    double hQ;
    double hR;
    double hP;
    double ZF;
    double ZL;
    double d7min;
    double d7max;
    double d70;
    double chi;
};


class Flange : public Flange_OUT {

public:
    Flange(int flangeNo) : Flange_OUT(flangeNo) { }
    virtual ~Flange() { }

    /**
     * @brief according para 4.2 eF/bF, eL/bL and cos(phiS)
     * @returns true if flange is valid
     */
    virtual bool Is_flange_Valid() = 0;
    virtual void Calc_eP();
    virtual void Calc_eFt();
    virtual void Calc_pB();
    virtual void Calc_d5e();
    virtual void Calc_d3e();
    virtual void Calc_AB();
    virtual void Calc_XB();
    virtual void Calc_dK1();
    virtual void Calc_dK2();
    virtual void Calc_XW();
    virtual void Calc_hP();
    virtual void Calc_etanplus();
    virtual void Calc_etanminus();

    virtual void Calc_bF() = 0;
    virtual void Calc_dF() = 0;
    virtual void Calc_AF();
    virtual void Calc_eF();

    virtual void Calc_bL() = 0;
    virtual void Calc_dL() = 0;
    virtual void Calc_AL() = 0;
    virtual void Calc_eL();

    virtual void Calc_beta();

    virtual void Calc_eD();

    virtual void Calc_eE() = 0;
    virtual void Calc_dE() = 0;

    virtual void Calc_gamma();
    virtual void Calc_theta();
    virtual void Calc_lambda();

    virtual void Calc_cF();
    virtual void Calc_hS();
    virtual void Calc_hT();

    virtual void Calc_hQ() = 0;
    virtual void Calc_hR() = 0;

    virtual void Calc_kQ();
    virtual void Calc_kR();

    virtual void Calc_ZF() = 0;
    virtual void Calc_ZL() = 0;

    virtual void Calc_rho();

    virtual void Calc_hG() = 0;
    virtual void Calc_hH() = 0;
    virtual void Calc_hL() = 0;

    virtual void Calc_d7min();
    virtual void Calc_d7max();
    virtual void Calc_d70();
    virtual void Calc_d7minMax();
    virtual void Calc_chi();

    virtual bool Is_PhiF_valid(int loadCaseNo);

    virtual void Calc_deltaQ(int loadCaseNo);
    virtual void Calc_deltaR(int loadCaseNo);

    virtual void Calc_cM(int loadCaseNo);
    virtual bool Is_cM_valid(int loadCaseNo);
    virtual void Calc_cS(int loadCaseNo);
    virtual bool Is_cS_valid(int loadCaseNo);
    virtual void Calc_jM(int loadCaseNo);

private:
    void Calc_Psi_jkk(int loadCaseNo);

public:
    virtual void Calc_PsiOpt(int loadCaseNo);
    virtual void Calc_PsiMax(int loadCaseNo);
    virtual void Calc_Psi0(int loadCaseNo);
    virtual void Calc_PsiMin(int loadCaseNo);

private:
    void Calc_PsiMaxMin0(int loadCaseNo);

public:
    virtual bool Is_PsiMaxMin_Valid(int loadCaseNo);
    virtual void Calc_PsiZ(int loadCaseNo);

    virtual void Calc_kM_for_WFmax(int loadCaseNo);
    virtual void Calc_WF(int loadCaseNo);
    virtual void Calc_PhiF(int loadCaseNo);
    virtual void Calc_PhiX(int loadCaseNo);
    virtual bool Is_PhiX_Valid(int loadCaseNo);
    virtual void Calc_WX(int loadCaseNo);
    virtual void Calc_kM(int loadCaseNo);

    // loose only
    virtual bool Is_d0_Valid();
    virtual void Calc_WL(int loadCaseNo);
    virtual void Calc_PhiL(int loadCaseNo);
    virtual bool Is_PhiL_valid(int loadCaseNo);
    virtual void Calc_WQ(int loadCaseNo);

};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //FLANGE_H
