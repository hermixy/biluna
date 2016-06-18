#include "std_unittestfactory.h"

#include "std_materialutilitytest.h"

STD_UnitTestFactory::STD_UnitTestFactory() {
    // nothing
}

STD_UnitTestFactory::~STD_UnitTestFactory() {
    // nothing
}

void STD_UnitTestFactory::exec() {

    Biluna::Calc::RB_UnitTest* ut;
    UNITTESTCLASS(STD_MaterialUtilityTest);

    ut = nullptr;

}
