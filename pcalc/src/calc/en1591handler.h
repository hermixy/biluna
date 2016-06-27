#ifndef EN1591HANDLER_H
#define EN1591HANDLER_H

#include "rb_namespace.h"
class RB_ObjectContainer;

#define NULL 0
NAMESPACE_BILUNA_CALC_EN1591

class Calculator;


/**
 * @brief Handles the interface between the data container
 * and the calculator. Sets the input data in the calculator
 */
class EN1591Handler {

public:
    EN1591Handler();
    virtual ~EN1591Handler();

    void exec();

protected:
    void setDimMatInput();
    void setLoadCaseInput();

    Calculator* getCalculator() const;

private:
    void setFlangeType(int& flange1Type, int& flange2Type);
    Calculator* mCalc;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif // EN1591HANDLER_H
