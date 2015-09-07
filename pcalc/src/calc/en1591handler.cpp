#include "en1591handler.h"

#include "calculator.h"
#include "pcalc_report.h"
#include "rb_allowanceservice.h"
#include "rb_materialservice.h"
#include "rb_qualityservice.h"
#include "rb_objectcontainer.h"

NAMESPACE_BILUNA_CALC_EN1591

EN1591Handler::EN1591Handler(RB_AllowanceService* allowService,
                             RB_MaterialService* matService,
                             RB_QualityService* qualService) {
    mCalc = NULL;
    mAllowService = allowService;
    mMatService = matService;
    mQualService = qualService;
}

EN1591Handler::~EN1591Handler() {
    delete mCalc;
    mCalc = NULL;
}

void EN1591Handler::exec() {
    int flange1Type; // items << "Blind" << "Integral" << "Loose";
    int flange2Type;
    setFlangeType(flange1Type, flange2Type);

    RB_ObjectContainer* inList
            = PR->getInOutContainer()->getContainer("PCALC_InputList");
    RB_ObjectBase* in = inList->getObject("name", "PCALC_Input");

    if (in->getValue("flange2equal").toInt() != 0) {
        // flange 2 is equal flange 1
        flange2Type = flange1Type;
    }

    mCalc = new Calculator((Calculator::FlangeType)flange1Type,
                           (Calculator::FlangeType)flange2Type);

    setDimensions();
    setAllowances();
    setQualityFactors();
    setLoadCases();
    setMaterialProperties();

    mCalc->exec();
}

void EN1591Handler::setDimensions() {
    RB_ObjectContainer* inList
            = PR->getInOutContainer()->getContainer("PCALC_InputList");
    RB_ObjectBase* in = inList->getObject("name", "PCALC_Input");

    mCalc->mAssembly->mNR = in->getValue("nr").toInt();
    mCalc->mAssembly->mLeakageRate
            = pow(10, -(in->getValue("leakagerate").toInt()));
    // TODO: now calculation value from loadCase 0, change to assembly value only
    mCalc->mAssembly->mF_Bspec = in->getValue("f_bspecified").toDouble();
    mCalc->mAssembly->mQ_Aspec = in->getValue("q_aspecified").toDouble();

    Flange* fl1 = mCalc->mAssembly->mFlange1;
    fl1->nB = in->getValue("nb").toInt();
    // typeflange1_id already used
    fl1->d0 = in->getValue("d01").toDouble();
    fl1->d3 = in->getValue("d31").toDouble();
    fl1->d4 = in->getValue("d41").toDouble();
    fl1->dX = in->getValue("dx1").toDouble();
    fl1->eF = in->getValue("ef1").toDouble();
    fl1->eFt = in->getValue("eft1").toDouble();
    fl1->eP = in->getValue("ep1").toDouble();
    fl1->eRF = in->getValue("erf1").toDouble();
    fl1->eX = in->getValue("ex1").toDouble();

    fl1->d9 = in->getValue("d91").toDouble();
    fl1->e0 = in->getValue("e01").toDouble();

    fl1->d1 = in->getValue("d11").toDouble();
    fl1->d2 = in->getValue("d21").toDouble();

    fl1->e1 = in->getValue("e11").toDouble();
    fl1->e2 = in->getValue("e21").toDouble();
    fl1->lH = in->getValue("lh1").toDouble();

    fl1->b0 = in->getValue("b01").toDouble();
    fl1->d6 = in->getValue("d61").toDouble();
    fl1->d8 = in->getValue("d81").toDouble();
    fl1->eL = in->getValue("el1").toDouble();
    // TODO: materialflange1_idx
    // TODO: materialloosering1_idx

    Flange* fl2 = mCalc->mAssembly->mFlange2;
    fl2->nB = fl1->nB; // always same number of bolts

    if (in->getValue("flange2equal").toInt() != 0) {
        // flange 2 is equal flange 1
        fl2->d0 = fl1->d0;
        fl2->d3 = fl1->d3;
        fl2->d4 = fl1->d4;
        fl2->dX = fl1->dX;
        fl2->eF = fl1->eF;
        fl2->eFt = fl1->eFt;
        fl2->eP = fl1->eP;
        fl2->eRF = fl1->eRF;
        fl2->eX = fl1->eX;

        fl2->d9 = fl1->d9;
        fl2->e0 = fl1->e0;

        fl2->d1 = fl1->d1;
        fl2->d2 = fl1->d2;

        fl2->e1 = fl1->e1;
        fl2->e2 = fl1->e2;
        fl2->lH = fl1->lH;

        fl2->b0 = fl1->b0;
        fl2->d6 = fl1->d6;
        fl2->d8 = fl1->d8;
        fl2->eL = fl1->eL;
        // TODO: materialflange1_idx
        // TODO: materialloosering1_idx
    } else {
        fl2->d0 = in->getValue("d02").toDouble();
        fl2->d3 = in->getValue("d32").toDouble();
        fl2->d4 = in->getValue("d42").toDouble();
        fl2->dX = in->getValue("dx2").toDouble();
        fl2->eF = in->getValue("ef2").toDouble();
        fl2->eFt = in->getValue("eft2").toDouble();
        fl2->eP = in->getValue("ep2").toDouble();
        fl1->eRF = in->getValue("erf2").toDouble();
        fl2->eX = in->getValue("ex2").toDouble();

        fl2->d9 = in->getValue("d92").toDouble();
        fl2->e0 = in->getValue("e02").toDouble();

        fl2->d1 = in->getValue("d12").toDouble();
        fl2->d2 = in->getValue("d22").toDouble();

        fl2->e1 = in->getValue("e12").toDouble();
        fl2->e2 = in->getValue("e22").toDouble();
        fl2->lH = in->getValue("lh2").toDouble();

        fl2->b0 = in->getValue("b02").toDouble();
        fl2->d6 = in->getValue("d62").toDouble();
        fl2->d8 = in->getValue("d82").toDouble();
        fl2->eL = in->getValue("el2").toDouble();
        // TODO: materialflange2_idx
        // TODO: materialloosering2_idx
    }

    // gasket
    Gasket* gkt = mCalc->mAssembly->mGasket;
    gkt->matCode = in->getValue("materialcode_ix").toString();
    gkt->frmType = (Gasket::FormType)in->getValue("formtype_id").toInt();
    gkt->insType =
            (Gasket::InsFilLayMatType)in->getValue("insertfilltype_id").toInt();
    gkt->dG0 = in->getValue("dg0").toDouble();
    gkt->dG1 = in->getValue("dg1").toDouble();
    gkt->dG2 = in->getValue("dg2").toDouble();
    gkt->dG1_EN13555 = in->getValue("dg1en13555").toDouble();
    gkt->dG2_EN13555 = in->getValue("dg2en13555").toDouble();
    gkt->eGt = in->getValue("egt").toDouble();
    gkt->K = in->getValue("k").toDouble();
    gkt->phiG = in->getValue("phig").toDouble();
    gkt->r2 = in->getValue("r2").toDouble();

    // bolt
    Bolt* blt = mCalc->mAssembly->mBolt;
    // TODO size_idx
    blt->bType = (Bolt::BoltType)in->getValue("bolttype_id").toInt();
    blt->tType = (Bolt::TensionerType)in->getValue("tensionertype_id").toInt();
    blt->dB0 = in->getValue("db0").toDouble();
    blt->dB2 = in->getValue("db2").toDouble();
    blt->dB4 = in->getValue("db4").toDouble();
    blt->dBe = in->getValue("dbe").toDouble();
    blt->dBS = in->getValue("dbs").toDouble();
    blt->dn = in->getValue("dn").toDouble();
    blt->eN = in->getValue("en").toDouble();
    blt->l5t = in->getValue("l5t").toDouble();
    blt->lS = in->getValue("ls").toDouble();
    blt->mun = in->getValue("mun").toDouble();
    blt->mut = in->getValue("mut").toDouble();
    blt->ruptureElongationA = in->getValue("ruptureelongationa").toDouble();
    // TODO materialbolt_idx
    // bolthole
    BoltHole* bhl = blt->mBoltHole;
    bhl->d5 = in->getValue("d5").toDouble();
    bhl->d5t = in->getValue("d5t").toDouble();
    bhl->eFb = in->getValue("efb").toDouble();
    bhl->isBlindHole = (bool)in->getValue("blindhole").toInt();
    // washer
    Washer* wr = mCalc->mAssembly->mFlange1->mWasher;
    wr->eW = in->getValue("ew").toDouble();
    wr->dW1 = in->getValue("dw1").toDouble();
    wr->dW2 = in->getValue("dw2").toDouble();
    // TODO materialwasher_idx
    // input only allows one type of washer could be change to two different
    wr = mCalc->mAssembly->mFlange2->mWasher;
    wr->eW = in->getValue("ew").toDouble();
    wr->dW1 = in->getValue("dw1").toDouble();
    wr->dW2 = in->getValue("dw2").toDouble();
    // TODO materialwasher_idx

    // shell
    Shell* sh1 = mCalc->mAssembly->mFlange1->mShell;
    sh1->sType = (Shell::ShellType)in->getValue("typeshell1_id").toInt();
    sh1->dS = in->getValue("ds1").toDouble();
    sh1->eS = in->getValue("es1").toDouble();
    sh1->phiS = in->getValue("phis1").toDouble();
    // TODO materialshell1_idx
    Shell* sh2 = mCalc->mAssembly->mFlange2->mShell;

    if (in->getValue("shell2equal").toInt() != 0) {
        // shell 2 is equal shell 1
        sh2->sType = sh1->sType;
        sh2->dS = sh1->dS;
        sh2->eS = sh1->eS;
        sh2->phiS = sh1->phiS;
        // TODO materialshell2_idx
    } else {
        sh2->sType = (Shell::ShellType)in->getValue("typeshell2_id").toInt();
        sh2->dS = in->getValue("ds2").toDouble();
        sh2->eS = in->getValue("es2").toDouble();
        sh2->phiS = in->getValue("phis2").toDouble();
        // TODO materialshell2_idx
    }
}

void EN1591Handler::setAllowances() {
    // nothing
}

void EN1591Handler::setQualityFactors() {
    // nothing
}

void EN1591Handler::setLoadCases() {
    RB_ObjectContainer* loadList
            = PR->getInOutContainer()->getContainer("PCALC_LoadCaseList");
    loadList->sort(0, RB2::SortOrderAscending, RB2::MemberInteger);
    RB_ObjectIterator* iter = loadList->createIterator();
    int loadCaseNoMemory = 0;

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* lcIn = iter->currentObject();
        LoadCase* lc = mCalc->mAssembly->mLoadCaseList->createLoadCase();

        if (loadCaseNoMemory == 0) {
            lc->F_Bspec = mCalc->mAssembly->mF_Bspec; // TODO: change in calculations
            lc->Q_A = mCalc->mAssembly->mQ_Aspec; // TODO: change in calculations
            ++loadCaseNoMemory;
        }

        // pressure temperature
        lc->P = lcIn->getValue("p").toDouble();
        lc->T0 = lcIn->getValue("t0").toDouble();
        lc->TB = lcIn->getValue("tb").toDouble();
        lc->TG = lcIn->getValue("tg").toDouble();
        // flange 1
        lc->TF1 = lcIn->getValue("tf1").toDouble();
        lc->TL1 = lcIn->getValue("tl1").toDouble();
        lc->TW1 = lcIn->getValue("tw1").toDouble();
        // force
        lc->mForce->F_X = lcIn->getValue("fx").toDouble();
        lc->mForce->F_Y = lcIn->getValue("fy").toDouble();
        lc->mForce->F_Z = lcIn->getValue("fz").toDouble();
        lc->mForce->M_X = lcIn->getValue("mx").toDouble();
        lc->mForce->M_Y = lcIn->getValue("my").toDouble();
        lc->mForce->M_Z = lcIn->getValue("mz").toDouble();
        // stress expansion elasticity
        lc->fB = lcIn->getValue("fb").toDouble();
        lc->fN = lcIn->getValue("fn").toDouble();

        lc->fF1 = lcIn->getValue("ff1").toDouble();
        lc->fL1 = lcIn->getValue("fl1").toDouble();
        lc->fS1 = lcIn->getValue("fs1").toDouble();
        lc->fW1 = lcIn->getValue("fw1").toDouble();

        lc->alphaB = lcIn->getValue("alphab").toDouble();
        lc->alphaG = lcIn->getValue("alphag").toDouble();

        lc->alphaF1 = lcIn->getValue("alphaf1").toDouble();
        lc->alphaL1 = lcIn->getValue("alphal1").toDouble();
        lc->alphaW1 = lcIn->getValue("alphaw1").toDouble();

        lc->EB = lcIn->getValue("eb").toDouble();

        lc->EF1 = lcIn->getValue("ef1").toDouble();
        lc->EL1 = lcIn->getValue("el1").toDouble();
        lc->EW1 = lcIn->getValue("ew1").toDouble();

        // flange 2
        RB_ObjectContainer* inList
                = PR->getInOutContainer()->getContainer("PCALC_InputList");
        RB_ObjectBase* in = inList->getObject("name", "PCALC_Input");

        if (in->getValue("flange2equal").toInt() != 0) {
            // flange 2 is equal flange 1
            lc->TF2 = lc->TF1;
            lc->TL2 = lc->TL1;
            lc->TW2 = lc->TW1;
            lc->fF2 = lc->fF1;
            lc->fL2 = lc->fL1;
            lc->fS2 = lc->fS1;
            lc->fW2 = lc->fW1;
            lc->alphaF2 = lc->alphaF1;
            lc->alphaL2 = lc->alphaL1;
            lc->alphaW2 = lc->alphaW1;
            lc->EF2 = lc->EF1;
            lc->EL2 = lc->EL1;
            lc->EW2 = lc->EW1;
        } else {
            lc->TF2 = lcIn->getValue("tf2").toDouble();
            lc->TL2 = lcIn->getValue("tl2").toDouble();
            lc->TW2 = lcIn->getValue("tw2").toDouble();
            lc->fF2 = lcIn->getValue("ff2").toDouble();
            lc->fL2 = lcIn->getValue("fl2").toDouble();
            lc->fS2 = lcIn->getValue("fs2").toDouble();
            lc->fW2 = lcIn->getValue("fw2").toDouble();
            lc->alphaF2 = lcIn->getValue("alphaf2").toDouble();
            lc->alphaL2 = lcIn->getValue("alphal2").toDouble();
            lc->alphaW2 = lcIn->getValue("alphaw2").toDouble();
            lc->EF2 = lcIn->getValue("ef2").toDouble();
            lc->EL2 = lcIn->getValue("el2").toDouble();
            lc->EW2 = lcIn->getValue("ew2").toDouble();
        }
    }
}

void EN1591Handler::setMaterialProperties() {
    // nothing yet, to be set in loadcases
}

Calculator* EN1591Handler::getCalculator() const {
    return mCalc;
}

void EN1591Handler::setFlangeType(int& flangeType1, int& flangeType2) {
    RB_ObjectContainer* inList
            = PR->getInOutContainer()->getContainer("PCALC_InputList");
    RB_ObjectBase* objIn = inList->getObject("name", "PCALC_Input");
    flangeType1 = objIn->getValue("typeflange1_id").toInt();
    flangeType2 = objIn->getValue("typeflange2_id").toInt();
}

END_NAMESPACE_BILUNA_CALC_EN1591
