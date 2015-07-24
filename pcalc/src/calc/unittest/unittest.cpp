#include <iostream>
#include "unittest.h"
NAMESPACE_REDBAG_CALC


UnitTest::UnitTest() {

}

UnitTest::~UnitTest() {

}

void UnitTest::areEqual(RB_ObjectBase* output, const QString& functionName,
                        double expected, double test, double accuracy) {
    if (output) {
        if (expected - accuracy <= test && test <= expected + accuracy) {
            output->setValue("note", functionName + " - test OK");
        } else {
            output->setValue("note", functionName
                              + " - test FAILED\n  => EXPECTED: "
                              + QString::number(expected) + "\n  => TEST: "
                              + QString::number(test));
        }
    } else {
        if (expected - accuracy <= test && test <= expected + accuracy) {
            std::cout << functionName.toStdString()
                      << " - test OK" << std::endl;
        } else {
            std::cout << functionName.toStdString()
                      << " - test FAILED" << std::endl
                      << "  => EXPECTED: " << expected << std::endl
                      << "  => TEST: " << test << std::endl;
        }
    }
}

END_NAMESPACE_REDBAG_CALC
