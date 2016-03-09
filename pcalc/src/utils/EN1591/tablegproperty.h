#ifndef TABLEGPROPERTY_H
#define TABLEGPROPERTY_H

#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_utility.h"
NAMESPACE_BILUNA_CALC_EN1591

class Q0min_m_Property {

public:
    Q0min_m_Property(Gasket::InsFilLayMatType insFillMat,
                     double Q0min,
                     double m);

    Gasket::InsFilLayMatType mInsertFillMaterial;
    double mQ0min;
    double mM;
};

#define TABLEGPROPERTY TableGProperty::getInstance()

/**
 * The EN1591 table G.1 from Annex G with sealing parameters when
 * no leakage rate is specified.
 * - Q0min [MPa]
 * - m
 */
class TableGProperty : public RB_Utility {

public:
    virtual ~TableGProperty();
    static TableGProperty* getInstance();

    void refresh() {}

    double getTableG_Q0min(Gasket::InsFilLayMatType insType);
    double getTableG_m(Gasket::InsFilLayMatType insType);
private:
    TableGProperty();

    void createList();
    Q0min_m_Property* getPropertyObject(Gasket::InsFilLayMatType insType);

    static TableGProperty* mActiveUtility;
    std::vector<Q0min_m_Property*> mList;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //TABLEGPROPERTY_H
