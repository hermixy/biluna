#ifndef FORCETEST_H
#define FORCETEST_H

#include "force.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_REDBAG_CALC_EN1591

class ForceTest : public RB_UnitTest {

public:
    ForceTest(RB_ObjectContainer* inputOutput);
    virtual ~ForceTest();

    void exec();

private:
    void setupTarget();

    void Calc_F_STest();
    void Calc_M_BTest();

    Force *target;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //FORCETEST_H
