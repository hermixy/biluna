#ifndef TABLE16PROPERTY_H
#define TABLE16PROPERTY_H

#include <string>
#include <vector>
#include "gasket.h"
#include "rb_namespace.h"
#include "rb_tablemath.h"

NAMESPACE_REDBAG_CALC_EN1591

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


/**
 * @brief Table EN1591-2 Table 16 used for Q_smax property  and P_QR property.
 * Refer also www.gasketdata.org and www.europeansealing.com
 */
class Table16Property : public RB_TableMath {

public:
    Table16Property(RB_ObjectContainer* inputOutput);
    virtual ~Table16Property();

    double getTable16_Q_smax(const RB_String& materialCode, double temperature);
    double getTable16_P_QR(const RB_String& materialCode, double temperature);
    bool isGasketMaterialCodeExisting(const RB_String& materialCode);

private:
    bool getUpperLower(const RB_String& materialCode,
                       double temperature);
    Q_smax_Pqr_Property* getUpperObject(const RB_String& materialCode,
                                        double temperature);
    Q_smax_Pqr_Property* getLowerObject(const RB_String& materialCode,
                                        double temperature);
    void createList();
    void cl(double temperature,
            double Q_smax,
            double P_QR,
            const RB_String& materialCode);
    Q_smax_Pqr_Property* mUpper;
    Q_smax_Pqr_Property* mLower;

    std::vector<Q_smax_Pqr_Property*> mList;
};

END_NAMESPACE_REDBAG_CALC_EN1591
#endif //TABLE16PROPERTY_H
