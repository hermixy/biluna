#ifndef RB_REPORT_H
#define RB_REPORT_H

#include <cmath>
#include "rb_namespace.h"
#include "rb_objectcontainer.h"
#include "rb_utility.h"

#define PR RedBag::Calc::PCALC_Report::getInstance()
#define QN(value) RB_String::number(value)

NAMESPACE_REDBAG_CALC

class PCALC_Report : public RB_Utility {

public:
    virtual ~PCALC_Report();
    static PCALC_Report* getInstance();

    void refresh() {}

    virtual void addDetail(const QString& formulaNumber,
                           const QString& variableName,
                           const QString& formula,
                           double result,
                           const QString& unit,
                           const QString& formulaValues = "",
                           int loadCaseNo = -1,
                           const QString& note = "");
    RB_ObjectContainer* getInOutContainer();
    RB_ObjectBase* getLastOutput();
    virtual void clear();

private:
    PCALC_Report();
    void createInputOutputObject();
    RB_ObjectBase* getObject(RB_ObjectContainer* outContainer,
                             const QString& variableName,
                             int loadCaseNo);

    static PCALC_Report* mActiveUtility;
    RB_ObjectContainer* mInOutContainer;
    RB_ObjectContainer* mInContainer;
    RB_ObjectContainer* mOutContainer;
    RB_ObjectBase* mLastOutput;
};

END_NAMESPACE_REDBAG_CALC

#endif //RB_REPORT_H


