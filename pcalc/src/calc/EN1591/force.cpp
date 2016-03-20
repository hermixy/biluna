#include "force.h"
#include "pcalc_report.h"
NAMESPACE_BILUNA_CALC_EN1591


Force_IN::Force_IN() : RB_Object("PCALC EN1591 Force") {
    F_X = 0;
    F_Y = 0;
    F_Z = 0;
    M_X = 0;
    M_Y = 0;
    M_Z = 0;

    mLoadCaseNo = -1;
}

Force_IN::~Force_IN() {
    // nothing
}

Force_OUT::Force_OUT() : Force_IN() {
    F_LI = 0.0;
    M_AI = 0.0;

}

Force::Force() : Force_OUT() {
    // nothing
}

/**
 * @brief Formula 93: Shear force
 */
void Force::Calc_F_LI() {
    F_LI = pow((pow(F_X, 2) + pow(F_Y, 2)), 0.5);
    PR->addDetail("Formula 93", "F_LI", "(F_X ^ 2 + F_Y ^ 2) ^ 0.5", F_LI, "N",
              "(" + QN(F_X) + " ^ 2 + " + QN(F_Y) + " ^ 2) ^ 0.5", mLoadCaseNo);
}

/**
 * @brief Formula 94: Bending moment
 */
void Force::Calc_M_AI() {
    M_AI = pow((pow(M_X, 2) + pow(M_Y, 2)), 0.5);
    PR->addDetail("Formula 94", "M_AI", "(M_X ^ 2 + M_Y ^ 2) ^ 0.5", M_AI, "Nmm",
              "(" + QN(M_X) + " ^ 2 + " + QN(M_Y) + " ^ 2) ^ 0.5", mLoadCaseNo);
}

END_NAMESPACE_BILUNA_CALC_EN1591
