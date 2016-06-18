#ifndef STD_MATERIALUTILITYTEST_H
#define STD_MATERIALUTILITYTEST_H

#include "rb_unittest.h"

class STD_MaterialUtilityTest : public Biluna::Calc::RB_UnitTest {

public:
    STD_MaterialUtilityTest();
    virtual ~STD_MaterialUtilityTest();

    void exec();

private:
    void setupTarget();

    void get_Rp02Test();
    void get_Rp10Test();
    void get_RmMinTest();

    QString mTestUuid;
};

#endif // STD_MATERIALUTILITYTEST_H
