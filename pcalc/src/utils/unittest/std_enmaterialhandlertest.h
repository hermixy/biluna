#ifndef STD_ENMATERIALHANDLERTEST_H
#define STD_ENMATERIALHANDLERTEST_H

#include "rb_unittest.h"
#include "std_enmaterialhandler.h"

class STD_EnMaterialHandlerTest : public Biluna::Calc::RB_UnitTest {

public:
    STD_EnMaterialHandlerTest();
    virtual ~STD_EnMaterialHandlerTest();

    void exec();

private:
    void setupTarget();

    void get_Rp02Test();
    void get_Rp10Test();
    void get_RmMinTest();

    QString mTestUuid;
    STD_EnMaterialHandler mTarget;
};

#endif // STD_ENMATERIALHANDLERTEST_H
