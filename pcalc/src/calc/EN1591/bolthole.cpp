#include "bolthole.h"
NAMESPACE_REDBAG_CALC_EN1591


BoltHole_IN::BoltHole_IN(RB_ObjectContainer* inputOutput)
            : RB_Report(inputOutput) {
    setName("PCALC EN1591 BoltHole");

    d5 = 0;
    isBlindHole = false;
    d5t = 0;
    eFb = 0;
}

BoltHole_IN::~BoltHole_IN() {
    // nothing
}

BoltHole_OUT::BoltHole_OUT(RB_ObjectContainer* inputOutput)
            : BoltHole_IN(inputOutput) {

}

BoltHole::BoltHole(RB_ObjectContainer* inputOutput)
            : BoltHole_OUT(inputOutput){

}


END_NAMESPACE_REDBAG_CALC_EN1591
