#ifndef TABLEGSIMPLETEST_H
#define TABLEGSIMPLETEST_H

#include "rb_namespace.h"
#include "unittest.h"
#include "tablegsimple.h"

NAMESPACE_REDBAG_CALC_EN1591

class TableGSimpleTest : public UnitTest {

public:
    TableGSimpleTest(RB_ObjectContainer* inputOutput);
    virtual ~TableGSimpleTest();

    void exec();

private:
    void setupTarget();
    void getTableG_QmaxTest();

    TableGSimple* target;

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //TABLEGSIMPLETEST_H
