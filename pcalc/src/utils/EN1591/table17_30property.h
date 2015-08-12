#ifndef TABLE17_30PROPERTY_H
#define TABLE17_30PROPERTY_H

#include <string>
#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_tablemath.h"

NAMESPACE_REDBAG_CALC_EN1591

/**
 * @brief EN1591-2 E_G property from table 17 to 30
 */
class E_GProperty {

public:
    E_GProperty(const RB_String& materialCode,
                double temperature,
                double gasketStress,
                double E_G);

    RB_String mMaterialCode;
    double mTemperature;
    double mGasketStress;
    double mE_G;

};


/**
 * @brief Table EN1591-2 Table 17 to 30 used for E_G property. The E_G value
 * is used if no detail gasket data from for example www.gasketdata.org
 * is available. Refer also www.europeansealing.com
 */
class Table17_30Property : public RB_TableMath {

public:
    Table17_30Property();
    virtual ~Table17_30Property();

    double getTableE_G(const RB_String& materialCode, double temperature,
                       double gasketStress);

private:
    void createList();
    void cl(const RB_String& materialCode,
            double temperature,
            double gasketStress,
            double E_G);

    bool isGasketMaterialCodeExisting(const RB_String& materialCode);
    void updateTopLeft(E_GProperty* obj);
    void updateTopRight(E_GProperty* obj);
    void updateBottomLeft(E_GProperty* obj);
    void updateBottomRight(E_GProperty* obj);

    std::vector<E_GProperty*> mList;
    double mTargetTemperature;
    double mTargetGasketStress;
    E_GProperty* mTopLeft;
    E_GProperty* mTopRight;
    E_GProperty* mBottomLeft;
    E_GProperty* mBottomRight;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //TABLE17_30PROPERTY_H
