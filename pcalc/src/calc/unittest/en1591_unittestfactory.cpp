#include "en1591_unittestfactory.h"

#include "assemblytest.h"
#include "bolttest.h"
#include "flange_blindtest.h"
#include "flange_integraltest.h"
#include "flange_loosetest.h"
#include "flangetest.h"
#include "forcetest.h"
#include "gaskettest.h"
#include "rb_tablemathtest.h"
#include "tablegsimpletest.h"
#include "WasherTest.h"
NAMESPACE_REDBAG_CALC_EN1591

#define UNITTESTCLASS(className) \
    ut = new className(mInputOutput); \
    ut->exec(); \
    delete ut;

EN1591_UnitTestFactory::EN1591_UnitTestFactory(RB_ObjectContainer* inputOutput)
            : RB_UnitTestFactory(inputOutput) {
    mInputOutput = inputOutput;
}

EN1591_UnitTestFactory::~EN1591_UnitTestFactory() {
    // nothing yet
}

void EN1591_UnitTestFactory::exec() {

    RB_UnitTest* ut;
    UNITTESTCLASS(AssemblyTest);
    UNITTESTCLASS(BoltTest);
    UNITTESTCLASS(Flange_BlindTest);
    UNITTESTCLASS(Flange_IntegralTest);
    UNITTESTCLASS(Flange_LooseTest);
    UNITTESTCLASS(FlangeTest);
    UNITTESTCLASS(ForceTest);
    UNITTESTCLASS(GasketTest);
    UNITTESTCLASS(RB_TableMathTest);
    UNITTESTCLASS(TableGSimpleTest);
    UNITTESTCLASS(WasherTest);
    ut = NULL;

}

END_NAMESPACE_REDBAG_CALC_EN1591
