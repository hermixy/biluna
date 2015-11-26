#include "washer.h"
#include "pcalc_report.h"
NAMESPACE_BILUNA_CALC_EN1591

Washer_IN::Washer_IN() : RB_Object() {
    eW = 0;
    dW1 = 0;
    dW2 = 0;
}

Washer_IN::~Washer_IN() {
    // nothing
}

Washer_OUT::Washer_OUT() : Washer_IN() {
    bW = 0.0;
    dW = 0.0;
    XW = 0.0;
    dK1 = 0.0;
    dK2 = 0.0;
    bKB = 0.0;
}

bool Washer::isPresent() {
    return eW > 0 && dW1 > 0 && dW2 > dW1;
}

/**
 * @brief Formula 44: Effective width of a washer
 */
void Washer::Calc_bW() {
    if (isPresent())     {
        bW = 0.5 * (dW2 - dW1);
        PR->addDetail("Formula 44", "bW(" + QN(mWasherNumber) + ")",
                      "0.5 * (dW2 - dW1)", bW, "mm",
                      "0.5 * (" + QN(dW2) + " - " + QN(dW1) + ")", 0);
    }
}

/**
 * @brief Formula 45: Average diameter of washer
 */
void Washer::Calc_dW() {
    if (isPresent())     {
        dW = 0.5 * (dW2 + dW1);
        PR->addDetail("Formula 45", "dW(" + QN(mWasherNumber) + ")",
                      "0.5 * (dW2 + dW1)", dW, "mm",
                      "0.5 * (" + QN(dW2) + " + " + QN(dW1) + ")", 0);
    }
}

/**
 * @brief Formula 48: Washer contact width bolt side
 */
void Washer::Calc_bKB() {
    if (isPresent())     {
        bKB = (dK2 - dW1) / 2;
        PR->addDetail("Formula 48", "bKB(" + QN(mWasherNumber) + ")",
                      "(dK2 - dW1) / 2", bKB, "mm",
                      "(" + QN(dK2) + " - " + QN(dW1) + ") / 2", 0);
    }
}

END_NAMESPACE_BILUNA_CALC_EN1591
