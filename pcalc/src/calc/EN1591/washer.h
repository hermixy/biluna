#ifndef WASHER_H
#define WASHER_H

#include "rb_namespace.h"
#include "rb_object.h"
NAMESPACE_BILUNA_CALC_EN1591

class Washer_IN : public RB_Object {

public:
    Washer_IN();
    virtual ~Washer_IN();

    double eW;
    double dW1;
    double dW2;
};


class Washer_OUT : public Washer_IN {

public:
    Washer_OUT();

    double bW;
    double dW;
    double XW;
    double dK1;
    double dK2;
    double bKB;
};


class Washer : public Washer_OUT {

public:
    Washer() : Washer_OUT() {
        // nothing
    }

    bool isPresent();
    void Calc_bW();
    void Calc_dW();
    void Calc_bKB();

    int mWasherNumber;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //WASHER_H
