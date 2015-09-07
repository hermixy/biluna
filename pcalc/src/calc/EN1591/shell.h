#ifndef SHELL_H
#define SHELL_H

#include "rb_namespace.h"
#include "pcalc_report.h"

NAMESPACE_BILUNA_CALC_EN1591

class Shell_IN : public RB_Object {

public:
    Shell_IN();
    virtual ~Shell_IN();

    enum ShellType {
        Conical,
        Cylindrical,
        Spherical
    };

    ShellType sType;
    double dS;
    double eS;
    double phiS;
    int shellNumber;
};


class Shell_OUT : public Shell_IN {

public:
    Shell_OUT() : Shell_IN() {
        // nothing
    }

};


class Shell : public Shell_OUT {

public:
    Shell() : Shell_OUT() {
        // nothing
    }

};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //SHELL_H
