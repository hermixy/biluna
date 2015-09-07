#ifndef TABLE16PROPERTY_H
#define TABLE16PROPERTY_H

#include <string>
#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_tablemath.h"
#include "rb_utility.h"

NAMESPACE_BILUNA_CALC_EN1591

/**
 * EN1591-2 table 16 properties Q_smax and P_QR
 */
class Q_smax_Pqr_Property {

public:
    Q_smax_Pqr_Property(double temperature,
                        double Q_smax,
                        double P_QR,
                        const RB_String& materialCode);
    double mTemperature;
    double mQ_smax;
    double mP_QR;
    RB_String mMaterialCode;
};

#define TABLE16PROPERTY Table16Property::getInstance()

/**
 * @brief Table EN1591-2 Table 16 used for Q_smax property  and P_QR property.
 * Refer also www.gasketdata.org and www.europeansealing.com
 */
class Table16Property : public RB_TableMath, RB_Utility {

public:
    virtual ~Table16Property();
    static Table16Property* getInstance();

    void refresh() {}

    double getTable16_Q_smax(const RB_String& materialCode, double temperature);
    double getTable16_P_QR(const RB_String& materialCode, double temperature);
    bool isGasketMaterialCodeExisting(const RB_String& materialCode);

private:
    Table16Property();
    bool getUpperLower(const RB_String& materialCode,
                       double temperature);
    void updateUpperObject(Q_smax_Pqr_Property* obj,
                           const RB_String& materialCode, double temperature);
    void updateLowerObject(Q_smax_Pqr_Property* obj,
                           const RB_String& materialCode, double temperature);
    void createList();
    void cl(double temperature,
            double Q_smax,
            double P_QR,
            const RB_String& materialCode);

    static Table16Property* mActiveUtility;
    Q_smax_Pqr_Property* mUpper;
    Q_smax_Pqr_Property* mLower;

    std::vector<Q_smax_Pqr_Property*> mList;
};

END_NAMESPACE_BILUNA_CALC_EN1591
#endif //TABLE16PROPERTY_H
