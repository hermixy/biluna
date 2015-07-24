#ifndef SHELL_H
#define SHELL_H

#include "rb_namespace.h"
#include "rb_report.h"

NAMESPACE_REDBAG_CALC_EN1591

class Shell_IN : public RB_Report {

public:
    Shell_IN(RB_ObjectContainer* inputOutput);
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
    Shell_OUT(RB_ObjectContainer* inputOutput)
        : Shell_IN(inputOutput) {
        // nothing
    }

};


class Shell : public Shell_OUT {

public:
    Shell(RB_ObjectContainer* inputOutput)
        : Shell_OUT(inputOutput) {
        // nothing
    }

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //SHELL_H
