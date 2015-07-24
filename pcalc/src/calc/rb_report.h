#ifndef RB_REPORT_H
#define RB_REPORT_H

#include <cmath>
#include "rb_namespace.h"
#include "rb_object.h"
#include "rb_objectcontainer.h"

NAMESPACE_REDBAG_CALC

class RB_Report : public RB_Object {

public:
    RB_Report(RB_ObjectContainer* inputOutput);
    virtual ~RB_Report();

    virtual void addDetail(const QString& formulaNumber,
                           const QString& variableName,
                           const QString& formula,
                           double result,
                           const QString& unit,
                           const QString& formulaValues = "",
                           int loadCaseNo = -1,
                           const QString& note = "");
    RB_ObjectBase* getLastOutput();

protected:
    RB_String qn(double val);
    RB_String fv(const char* format ...);

private:
    RB_ObjectContainer* mInContainer;
    RB_ObjectContainer* mOutContainer;
    RB_ObjectBase* mLastOutput;
};

END_NAMESPACE_REDBAG_CALC

#endif //RB_REPORT_H


