#ifndef WASHERTEST_H
#define WASHERTEST_H

#include "rb_namespace.h"
#include "rb_unittest.h"
#include "washer.h"

NAMESPACE_REDBAG_CALC_EN1591

class WasherTest : public RB_UnitTest {

public:
    WasherTest();
    virtual ~WasherTest();

    void exec();

private:
    void setupTarget();

    void Calc_bWTest();
    void Calc_dWTest();
    void Calc_bKBTest();

    Washer* target;

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //WASHERTEST_H
