#ifndef EN1591HANDLER_H
#define EN1591HANDLER_H

#include "rb_namespace.h"
class RB_ObjectContainer;

#define NULL 0
NAMESPACE_REDBAG_CALC_EN1591

class Calculator;
class RB_AllowanceService;
class RB_MaterialService;
class RB_QualityService;


/**
 * @brief Handles the interface between the data container
 * and the calculator. Sets the input data in the calculator
 */
class EN1591Handler {

public:
    EN1591Handler(RB_AllowanceService* allowService = NULL,
                  RB_MaterialService* matService = NULL,
                  RB_QualityService* qualService = NULL);
    virtual ~EN1591Handler();

    void exec();

protected:
    void setDimensions();
    void setAllowances();
    void setQualityFactors();
    void setLoadCases();
    void setMaterialProperties();

    Calculator* getCalculator() const;

private:
    void setFlangeType(int& flange1Type, int& flange2Type);
    Calculator* mCalc;
    RB_AllowanceService* mAllowService;
    RB_MaterialService* mMatService;
    RB_QualityService* mQualService;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif // EN1591HANDLER_H
