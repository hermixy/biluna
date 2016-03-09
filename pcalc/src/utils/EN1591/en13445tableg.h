#ifndef TABLEGSIMPLE_H
#define TABLEGSIMPLE_H

#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_utility.h"

NAMESPACE_BILUNA_CALC_EN1591

/**
 * @brief The EN13445 Table G class property
 * EN13445 Table G, used instead of EN1591
 */
class EN13445TableGProperty {

public:
    EN13445TableGProperty(Gasket::InsFilLayMatType insFillMat,
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

#define EN13445TABLEG EN13445TableG::getInstance()

/**
 * The EN13445 appendix G tables 9 class for gaskets
 * without EN13555 measured data
 */
class EN13445TableG : public RB_TableMath, RB_Utility {

public:
    virtual ~EN13445TableG();
    static EN13445TableG* getInstance();

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
    EN13445TableG();
    bool getUpperLower(Gasket::InsFilLayMatType insType,
                       double temperature);
    void updateUpperObject(EN13445TableGProperty *obj,
                           Gasket::InsFilLayMatType insType,
                           double temperature);
    void updateLowerObject(EN13445TableGProperty *obj,
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

    static EN13445TableG* mActiveUtility;
    EN13445TableGProperty* mUpper;
    EN13445TableGProperty* mLower;

    std::vector<EN13445TableGProperty*> mList;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //TABLEGSIMPLE_H
