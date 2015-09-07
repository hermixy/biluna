#ifndef TABLEEPROPERTY_H
#define TABLEEPROPERTY_H

#include <vector>
#include "gasket.h"
#include "rb_namespace.h"

NAMESPACE_BILUNA_CALC_EN1591

/**
 * @brief EN1591-1 MuG property from Annex E table E.1
 */
class MuG_Property {

public:
    MuG_Property(Gasket::InsFilLayMatType insFillMat, double muG);

    Gasket::InsFilLayMatType mInsertFillMaterial;
    double mMuG;
};

/**
 * @brief EN1591-1 Annex E table E.1
 */
class TableEProperty {

public:
    TableEProperty();
    virtual ~TableEProperty();

    double getTableE_muG(Gasket_IN::InsFilLayMatType insType);

private:
    void CreateList();

    std::vector<MuG_Property*> mList;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //TABLEEPROPERTY_H
