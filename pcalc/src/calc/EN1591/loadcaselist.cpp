#include "loadcaselist.h"

#include "force.h"
#include "loadcase.h"
NAMESPACE_REDBAG_CALC_EN1591


LoadCaseList::LoadCaseList(RB_ObjectContainer* inputOutput)
        : std::vector<LoadCase*>() {
    mInputOutput = inputOutput;
}

LoadCaseList::~LoadCaseList() {
    for (std::vector<LoadCase*>::iterator it = begin();
                it != end(); it++) {
        delete (*it);
    }
}

LoadCase *LoadCaseList::createLoadCase() {
    LoadCase* loadCase = new LoadCase(mInputOutput); // first loadcase number: 0
    loadCase->mForce = new Force(mInputOutput);
    this->push_back(loadCase);
    return loadCase;
}

END_NAMESPACE_REDBAG_CALC_EN1591
