#ifndef BOLTTEST_H
#define BOLTTEST_H

#include "bolt.h"
#include "rb_namespace.h"
#include "rb_unittest.h"

NAMESPACE_REDBAG_CALC_EN1591

class BoltTest : public RB_UnitTest{

public:
    BoltTest(RB_ObjectContainer* inputOutput);
    virtual ~BoltTest();

    void exec();

private:
    void setupTarget();

    void Calc_eta1plusTest();
    void Calc_eta1minusTest();
    void Calc_IBTest();

    Bolt *target;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //BOLTTEST_H
