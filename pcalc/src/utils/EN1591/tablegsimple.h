#ifndef TABLEGSIMPLE_H
#define TABLEGSIMPLE_H

#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_objectcontainer.h"

NAMESPACE_REDBAG_CALC_EN1591

class TableGSimpleProperty {

public:
    TableGSimpleProperty(Gasket::InsFilLayMatType insFillMat,
                         double temperature,
                         double Q0min,
                         double Qmax,
                         double E0,
                         double K1,
                         double mI,
                         double gC);

    Gasket::InsFilLayMatType mInsertFillMaterial;
    double mTemperature;
    double mQ0min;
    double mQmax;
    double mE0;
    double mK1;
    double mmI;
    double mgC;
};

/**
 * The EN13445 appendix G tables 9 class for gaskets
 * without EN13555 measured data
 */
class TableGSimple : public RB_Object {

public:
    TableGSimple();
    virtual ~TableGSimple();

    double getTableG_Q0min(Gasket::InsFilLayMatType insType,
                           double temperature);
    double getTableG_Qmax(Gasket::InsFilLayMatType insType,
                          double temperature);
    double getTableG_E0(Gasket::InsFilLayMatType insType,
                        double temperature);
    double getTableG_K1(Gasket::InsFilLayMatType insType,
                        double temperature);
    double getTableG_mI(Gasket::InsFilLayMatType insType,
                        double temperature);
    double getTableG_gC(Gasket::InsFilLayMatType insType,
                        double temperature);

private:
    bool getUpperLower(Gasket::InsFilLayMatType insType,
                       double temperature);
    TableGSimpleProperty* getUpperObject(Gasket::InsFilLayMatType insType,
                                         double temperature);
    TableGSimpleProperty* getLowerObject(Gasket::InsFilLayMatType insType,
                                         double temperature);
    void createList();
    void cl(Gasket::InsFilLayMatType insFillMat,
            double temperature,
            double Q0min,
            double Qmax,
            double E0,
            double K1,
            double mI,
            double gC);
    TableGSimpleProperty* mUpper;
    TableGSimpleProperty* mLower;

    std::vector<TableGSimpleProperty*> mList;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //TABLEGSIMPLE_H
