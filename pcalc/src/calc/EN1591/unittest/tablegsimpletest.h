#ifndef TABLEGSIMPLETEST_H
#define TABLEGSIMPLETEST_H

#include "rb_namespace.h"
#include "rb_unittest.h"
#include "tablegsimple.h"

NAMESPACE_BILUNA_CALC_EN1591

class TableGSimpleTest : public RB_UnitTest {

public:
    TableGSimpleTest();
    virtual ~TableGSimpleTest();

    void exec();

private:
    void setupTarget();
    void getTableG_QmaxTest();

    TableGSimple* target;

};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //TABLEGSIMPLETEST_H
