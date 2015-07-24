#ifndef PCALC_INPUTOUTPUT_H
#define PCALC_INPUTOUTPUT_H

#include "rb_objectcontainer.h"

/**
 * @brief Input (including loadcaselist) and output object for calculations
 * of the PCALC perspective.
 */
class PCALC_InputOutput : public RB_ObjectContainer {
public:
    PCALC_InputOutput(const RB_String& id = "", RB_ObjectBase* p = NULL,
                      const RB_String& n = "PCALC_InputOutput",
                      RB_ObjectFactory* f = NULL,
                      bool woMembers = false);
};

#endif // PCALC_INPUTOUTPUT_H
