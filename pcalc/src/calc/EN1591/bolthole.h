#ifndef BOLTHOLE_H
#define BOLTHOLE_H

#include "rb_namespace.h"
#include "rb_report.h"

NAMESPACE_REDBAG_CALC_EN1591

class BoltHole_IN : public RB_Report {

public:
    BoltHole_IN(RB_ObjectContainer* inputOutput);
    virtual ~BoltHole_IN();

    double d5;
    double d5t;
    double eFb;
    bool isBlindHole;

};


class BoltHole_OUT : public BoltHole_IN {

public:
    BoltHole_OUT(RB_ObjectContainer* inputOutput);
};


class BoltHole : public BoltHole_OUT {

public:
    BoltHole(RB_ObjectContainer* inputOutput);
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //BOLTHOLE_H
