#ifndef RB_TABLEMATHTEST_H
#define RB_TABLEMATHTEST_H

#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_unittest.h"

NAMESPACE_REDBAG_CALC

class RB_TableMathTest : public RB_UnitTest {

public:
    RB_TableMathTest();
    virtual ~RB_TableMathTest();

    void exec();

private:
    void setupTarget();
    void getBilinearValueTest();

    RB_TableMath* target;

};

END_NAMESPACE_REDBAG_CALC

#endif //RB_TABLEMATHTEST_H
