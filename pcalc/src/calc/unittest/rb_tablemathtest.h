#ifndef RB_TABLEMATHTEST_H
#define RB_TABLEMATHTEST_H

#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_unittest.h"

NAMESPACE_BILUNA_CALC

class RB_TableMathTest : public RB_UnitTest {

public:
    RB_TableMathTest();
    virtual ~RB_TableMathTest();

    void exec();

private:
    void setupTarget();
    void getBilinearValueTest();
    void getLinearValueTest();
    void updateCornerObjectsTest();
    void getThreeCornerValueTest();
    void getTwoCornerValueTest();
    void getOneCornerValueTest();

    RB_TableMath* target;

};

END_NAMESPACE_BILUNA_CALC

#endif //RB_TABLEMATHTEST_H
