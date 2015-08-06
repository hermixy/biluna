#include "RB_unittestfactory.h"

NAMESPACE_REDBAG_CALC

// Example:
//#define UNITTESTCLASS(className) \
//    ut = new className(mInputOutput); \
//    ut->exec(); \
//    delete ut;

RB_UnitTestFactory::RB_UnitTestFactory(RB_ObjectContainer* inputOutput) {
    mInputOutput = inputOutput;
}

RB_UnitTestFactory::~RB_UnitTestFactory() {
    // nothing yet
}

// Example:
//void RB_UnitTestFactory::exec() {
//    RB_UnitTest* ut;
//    UNITTESTCLASS(AssemblyTest);
//    UNITTESTCLASS(BoltTest);
//    UNITTESTCLASS(Flange_BlindTest);
//    UNITTESTCLASS(Flange_IntegralTest);
//    UNITTESTCLASS(Flange_LooseTest);
//    UNITTESTCLASS(FlangeTest);
//    UNITTESTCLASS(ForceTest);
//    UNITTESTCLASS(GasketTest);
//    UNITTESTCLASS(RB_TableMathTest);
//    UNITTESTCLASS(TableGSimpleTest);
//    UNITTESTCLASS(WasherTest);
//    ut = NULL;
//}

END_NAMESPACE_REDBAG_CALC
