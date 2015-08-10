#ifndef TABLE02_15PROPERTY_H
#define TABLE02_15PROPERTY_H

#include <string>
#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_tablemath.h"

NAMESPACE_REDBAG_CALC_EN1591

/**
 * @brief EN1591-2 QminL and QsminL property from table 2 to 15
 */
class QminLQsminLProperty {

public:
    QminLQsminLProperty(double leakageRate,
                        const RB_String& materialCode,
                        double testPressure,
                        double QA,
                        double QminL,
                        double QsminL);
    double mLeakageRate;
    RB_String mMaterialCode;
    double mTestPressure;
    double mQA;
    double mQminL;
    double mQsminL;
};


/**
 * @brief Table EN1591-2 Table 2 to 15 used for QminL and QsminL property.
 * These values are is used if no detail gasket data from,
 * for example, www.gasketdata.org is available.
 * Refer also www.europeansealing.com
 */
class Table02_15Property : public RB_TableMath {

public:
    Table02_15Property(RB_ObjectContainer* inputOutput);
    virtual ~Table02_15Property();

    double getTableQminL(double leakageRate,
                         const RB_String& materialCode,
                         double testPressure = 40.0);
    double getTableQsminL(double leakageRate,
                          const RB_String& materialCode,
                          double QA,
                          double testPressure = 40.0);


private:
    void createList();
    void cl(double leakageRate, const RB_String& materialCode,
            double testPressure, double QA, double QminL, double QsminL);

    bool isGasketMaterialCodeExisting(const RB_String& materialCode);
    void updateLeft(QminLQsminLProperty* obj);
    void updateRight(QminLQsminLProperty* obj);

    std::vector<QminLQsminLProperty*> mList;
    double mTargetQA;
    QminLQsminLProperty* mLeft;
    QminLQsminLProperty* mRight;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //TABLE02_15PROPERTY_H
