#include "bolthole.h"
#include "pcalc_report.h"
NAMESPACE_BILUNA_CALC_EN1591


BoltHole_IN::BoltHole_IN() : RB_Object("PCALC EN1591 BoltHole") {
    d5 = 0;
    isBlindHole = false;
    d5t = 0;
    eFb = 0;
}

BoltHole_IN::~BoltHole_IN() {
    // nothing
}

BoltHole_OUT::BoltHole_OUT() : BoltHole_IN() {

}

BoltHole::BoltHole() : BoltHole_OUT(){

}


END_NAMESPACE_BILUNA_CALC_EN1591
