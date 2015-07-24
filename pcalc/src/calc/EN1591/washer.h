#ifndef WASHER_H
#define WASHER_H

#include "rb_namespace.h"
#include "rb_report.h"

NAMESPACE_REDBAG_CALC_EN1591

class Washer_IN : public RB_Report {

public:
    Washer_IN(RB_ObjectContainer* inputOutput);
    virtual ~Washer_IN();

    double eW;
    double dW1;
    double dW2;
};


class Washer_OUT : public Washer_IN {

public:
    Washer_OUT(RB_ObjectContainer* inputOutput);

    double bW;
    double dW;
    double XW;
    double dK1;
    double dK2;
    double bKB;
};


class Washer : public Washer_OUT {

public:
    Washer(RB_ObjectContainer* inputOutput) : Washer_OUT(inputOutput) {
        // nothing
    }

    bool isPresent();
    void Calc_bW();
    void Calc_dW();
    void Calc_bKB();

    int washerNumber;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //WASHER_H
