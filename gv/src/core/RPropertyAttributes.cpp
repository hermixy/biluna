#include "RPropertyAttributes.h"

void RPropertyAttributes::mixWith(const RPropertyAttributes & other) {
    setAffectsOtherProperties(affectsOtherProperties() || other.affectsOtherProperties());
    setInvisible(isInvisible() || other.isInvisible());
	choices.intersect(other.choices);
}


void RPropertyAttributes::setOption(RPropertyAttributes::Option option, bool on) {
	if (on) {
		options |= option;
	} else {
		options &= ~option;
	}
}
