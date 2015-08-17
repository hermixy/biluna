#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "assembly.h"
#include "rb_objectcontainer.h"


NAMESPACE_REDBAG_CALC_EN1591

class Calculator : public RB_Object {

public:
    enum FlangeType {
        Blind,
        Integral,
        Loose
    };

    Calculator(FlangeType flange1Type, FlangeType flange2Type);
    virtual ~Calculator();

    void exec();
    void Loop_F55_to_108(Assembly* assembly);
    void F3_to_24(Assembly* assembly);
    void F25_to_40(Assembly* assembly);
    void F41_to_53(Assembly* assembly);
    void F54_to_54(Assembly* assembly);
    void F55_to_62_table1(Assembly* assembly);
    void F63_to_63(Assembly* assembly, int loadCaseNo);
    void F77_to_89(Assembly* assembly, int loadCaseNo);
    void F90_to_104(Assembly* assembly, int loadCaseNo);
    void F105_to_105(Assembly* assembly, int loadCaseNo);
    void F106_to_108(Assembly* assembly);
    void F111_to_118(Assembly* assembly);
    void F119_to_119(Assembly* assembly);
    void F120_to_122(Assembly* assembly, int loadCaseNo);
    void F123_to_151(Assembly* assembly, int loadCaseNo);
    void FC1_to_C10(Assembly* assembly, int loadCaseNo);

    Assembly* mAssembly;

private:
    void createAssembly(FlangeType flange1Type, FlangeType flange2Type);
    void createAssemblyHelper(Assembly* assembly);
    void deleteAssembly();

    int mIterNo;
    bool mIsFirstApproximation;
    int mLoadCaseCount;

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //CALCULATOR_H
