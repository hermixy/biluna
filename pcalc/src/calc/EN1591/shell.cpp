#include "shell.h"
NAMESPACE_REDBAG_CALC_EN1591

Shell_IN::Shell_IN() : RB_Object() {
    setName("PCALC EN1591 Shell");

    shellNumber = 0;
    phiS = 0;
    sType = Cylindrical;
}

Shell_IN::~Shell_IN() {
    // nothing
}

END_NAMESPACE_REDBAG_CALC_EN1591
