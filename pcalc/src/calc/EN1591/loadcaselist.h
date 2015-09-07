#ifndef LOADCASELIST_H
#define LOADCASELIST_H

#include "rb_namespace.h"
#include "rb_objectcontainer.h"
#include <vector>

NAMESPACE_BILUNA_CALC_EN1591

class LoadCase;

class LoadCaseList : public std::vector<LoadCase*> {

public:
    LoadCaseList();
    virtual ~LoadCaseList();
    LoadCase* createLoadCase();


};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //LOADCASELIST_H
