#ifndef RB_UNITTEST_H
#define RB_UNITTEST_H

#include <QString>
#include <QTextEdit>
#include "rb_namespace.h"
#include "rb_objectbase.h"

NAMESPACE_REDBAG_CALC

class RB_UnitTest {

public:
    RB_UnitTest();
    virtual ~RB_UnitTest();

    virtual void exec() = 0;

protected:
    void areEqual(RB_ObjectBase* output, const QString& functionName,
                  double expected, double test, double accuracy = 0.000001);
    RB_ObjectContainer* mInputOutput;
};

END_NAMESPACE_REDBAG_CALC

#endif // RB_UNITTEST_H
