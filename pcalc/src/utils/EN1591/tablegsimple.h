#ifndef TABLEGSIMPLE_H
#define TABLEGSIMPLE_H

#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_utility.h"

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

#define TABLEGSIMPLE TableGSimple::getInstance()

/**
 * The EN13445 appendix G tables 9 class for gaskets
 * without EN13555 measured data
 */
class TableGSimple : public RB_TableMath, RB_Utility {

public:
    virtual ~TableGSimple();
    static TableGSimple* getInstance();

    void refresh() {}

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
    TableGSimple();
    bool getUpperLower(Gasket::InsFilLayMatType insType,
                       double temperature);
    void updateUpperObject(TableGSimpleProperty *obj,
                           Gasket::InsFilLayMatType insType,
                           double temperature);
    void updateLowerObject(TableGSimpleProperty *obj,
                           Gasket::InsFilLayMatType insType,
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

    static TableGSimple* mActiveUtility;
    TableGSimpleProperty* mUpper;
    TableGSimpleProperty* mLower;

    std::vector<TableGSimpleProperty*> mList;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //TABLEGSIMPLE_H
