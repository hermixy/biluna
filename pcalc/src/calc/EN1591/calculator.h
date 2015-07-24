#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "assembly.h"
#include "rb_objectcontainer.h"
#include "rb_report.h"

NAMESPACE_REDBAG_CALC_EN1591

class Calculator : public RB_Report {

public:
    enum FlangeType {
        Blind,
        Integral,
        Loose
    };

    Calculator(FlangeType flange1Type, FlangeType flange2Type,
               RB_ObjectContainer* inputOutput);
    virtual ~Calculator();

    void SetInputData_Ohmtech(Assembly* assembly);
    void SetLoadCases_Ohmtech(Assembly* assembly);
    void SetInputData_CR13642_B4(Assembly* assembly);
    void SetLoadCases_CR13642_B4(Assembly* assembly);
    void SetInputData_Ambrosio(Assembly* assembly);
    void SetLoadCases_Ambrosio(Assembly* assembly);
    void SetInputData_Plantware(Assembly* assembly);
    void SetLoadCases_Plantware(Assembly* assembly);

    void exec();
    void Loop_F55_to_108(Assembly* assembly, int loadCaseNo);
    void F3_to_24(Assembly* assembly);
    void F25_to_40(Assembly* assembly);
    void F41_to_53(Assembly* assembly);
    void F54_to_54(Assembly* assembly);
    void F55_to_62_table1(Assembly* assembly, int loadCaseNo);
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
    void createAssembly(FlangeType flange1Type, FlangeType flange2Type,
                        RB_ObjectContainer *inputOutput);
    void CreateAssemblyHelper(Assembly* assembly,
                              RB_ObjectContainer* inputOutput);

    int mWriteCalcFromNumber;
    int mIterNo;
    bool mIsFirstApproximation;
    int mLoadCaseCount;

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //CALCULATOR_H
