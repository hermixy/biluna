#include "pcalc_inputoutput.h"

PCALC_InputOutput::PCALC_InputOutput(const RB_String& id, RB_ObjectBase* p,
                                     const RB_String& n, RB_ObjectFactory* f,
                                     bool woMembers)
                    : RB_ObjectContainer (id, p, n, f, woMembers) {
    // Input list
    RB_ObjectContainer* inList;
    inList = new RB_ObjectContainer("", this, "PCALC_InputList");
    this->addObject(inList);

    // Load case list
    RB_ObjectContainer* loadCaseList;
    loadCaseList = new RB_ObjectContainer("", this, "PCALC_LoadCaseList");
    this->addObject(loadCaseList);

    // Output list
    RB_ObjectContainer* outList;
    outList = new RB_ObjectContainer("", this, "PCALC_OutputList");
    this->addObject(outList);
}

