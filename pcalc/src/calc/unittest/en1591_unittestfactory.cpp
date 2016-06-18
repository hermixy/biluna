#include "en1591_unittestfactory.h"

#include "assemblytest.h"
#include "bolttest.h"
#include "flange_blindtest.h"
#include "flange_integraltest.h"
#include "flange_loosetest.h"
#include "flangetest.h"
#include "forcetest.h"
#include "gaskettest.h"
#include "washertest.h"
#include "rb_tablemathtest.h"
NAMESPACE_BILUNA_CALC_EN1591


EN1591_UnitTestFactory::EN1591_UnitTestFactory() : RB_UnitTestFactory() {
    // nothing
}

EN1591_UnitTestFactory::~EN1591_UnitTestFactory() {
    // nothing
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
    UNITTESTCLASS(WasherTest);
    UNITTESTCLASS(RB_TableMathTest);
    ut = nullptr;

}

END_NAMESPACE_BILUNA_CALC_EN1591
