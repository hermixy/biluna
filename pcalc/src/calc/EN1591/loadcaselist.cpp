#include "loadcaselist.h"

#include "force.h"
#include "loadcase.h"
NAMESPACE_BILUNA_CALC_EN1591


LoadCaseList::LoadCaseList()
        : std::vector<LoadCase*>() {
}

LoadCaseList::~LoadCaseList() {
    for (std::vector<LoadCase*>::iterator it = begin();
                it != end(); it++) {
        delete (*it);
    }
}

LoadCase *LoadCaseList::createLoadCase() {
    int loadCaseNo = (int)this->size();
    LoadCase* loadCase = new LoadCase(); // first loadcase number: 0
    loadCase->mForce = new Force();
    loadCase->mForce->mLoadCaseNo = loadCaseNo; // otherwise force does not know
    this->push_back(loadCase);
    return loadCase;
}

END_NAMESPACE_BILUNA_CALC_EN1591
