#ifndef BOLTHOLE_H
#define BOLTHOLE_H

#include "rb_namespace.h"
#include "rb_object.h"

NAMESPACE_REDBAG_CALC_EN1591

class BoltHole_IN : public RB_Object {

public:
    BoltHole_IN();
    virtual ~BoltHole_IN();

    double d5;
    double d5t;
    double eFb;
    bool isBlindHole;

};


class BoltHole_OUT : public BoltHole_IN {

public:
    BoltHole_OUT();
};


class BoltHole : public BoltHole_OUT {

public:
    BoltHole();
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //BOLTHOLE_H
