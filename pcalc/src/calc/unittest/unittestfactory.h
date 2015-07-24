#ifndef UNITTESTFACTORY_H
#define UNITTESTFACTORY_H

#include <QString>
#include <QTextEdit>
#include "rb_namespace.h"
#include "rb_objectcontainer.h"

NAMESPACE_REDBAG_CALC_EN1591

class UnitTestFactory {

public:
    UnitTestFactory(RB_ObjectContainer* inputOutput);
    virtual ~UnitTestFactory();

    virtual void exec();

protected:
    RB_ObjectContainer* mInputOutput;
};

END_NAMESPACE_REDBAG_CALC_EN1591

#endif // UNITTESTFACTORY_H
