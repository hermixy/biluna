#ifndef RB_TABLEMATHTEST_H
#define RB_TABLEMATHTEST_H

#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "unittest.h"

NAMESPACE_REDBAG_CALC

class RB_TableMathTest : public UnitTest {

public:
    RB_TableMathTest(RB_ObjectContainer* inputOutput);
    virtual ~RB_TableMathTest();

    void exec();

private:
    void setupTarget();

    void getBilinearValueTest();

    RB_TableMath* target;

};

END_NAMESPACE_REDBAG_CALC

#endif //RB_TABLEMATHTEST_H
