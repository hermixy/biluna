#include "tablegsimpletest.h"
NAMESPACE_REDBAG_CALC_EN1591


TableGSimpleTest::TableGSimpleTest() : RB_UnitTest() {
    target = NULL;
}

TableGSimpleTest::~TableGSimpleTest() {
    delete target;
    target = NULL;
}

void TableGSimpleTest::exec() {
    getTableG_QmaxTest();
}

void TableGSimpleTest::setupTarget() {
    if (!target) {
        target = TABLEGSIMPLE;
    }
}

void TableGSimpleTest::getTableG_QmaxTest() {
    setupTarget();
    double result = target->getTableG_Qmax(Gasket::PtfeSoftSteel, 150.0);
    areEqual(PR->getLastOutput(), "TableGSimpleTest::getQmaxTest()", 310, result);
    result = target->getTableG_Qmax(Gasket::PtfeSoftSteel, -10.0);
    areEqual(PR->getLastOutput(), "TableGSimpleTest::getQmaxTest()", 0, result);
    result = target->getTableG_Qmax(Gasket::PtfeSoftSteel, 310.0);
    areEqual(PR->getLastOutput(), "TableGSimpleTest::getQmaxTest()", 0, result);
}

END_NAMESPACE_REDBAG_CALC_EN1591
