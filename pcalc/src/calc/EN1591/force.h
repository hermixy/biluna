#ifndef FORCE_H
#define FORCE_H

#include "rb_namespace.h"
#include "rb_report.h"

NAMESPACE_REDBAG_CALC_EN1591

class Force_IN : public RB_Report {

public:
    Force_IN(RB_ObjectContainer* inputOutput);
    virtual ~Force_IN();

    double F_X;
    double F_Y;
    double F_Z;
    double M_X;
    double M_Y;
    double M_Z;
};


class Force_OUT : public Force_IN {

public:
    Force_OUT(RB_ObjectContainer* inputOutput);

    double F_LI;
    double M_AI;
};


class Force : public Force_OUT {

public:
    Force(RB_ObjectContainer* inputOutput);

    void Calc_F_LI();
    void Calc_M_AI();

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //FORCE_H
