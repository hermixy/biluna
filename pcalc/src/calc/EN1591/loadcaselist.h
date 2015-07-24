#ifndef LOADCASELIST_H
#define LOADCASELIST_H

#include "rb_namespace.h"
#include "rb_objectcontainer.h"
#include <vector>

NAMESPACE_REDBAG_CALC_EN1591

class LoadCase;

class LoadCaseList : public std::vector<LoadCase*> {

public:
    LoadCaseList(RB_ObjectContainer* inputOutput);
    virtual ~LoadCaseList();
    LoadCase* createLoadCase();

private:
    RB_ObjectContainer* mInputOutput;

};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //LOADCASELIST_H
