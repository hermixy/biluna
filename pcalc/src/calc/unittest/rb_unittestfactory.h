#ifndef RB_UNITTESTFACTORY_H
#define RB_UNITTESTFACTORY_H

#include "rb_namespace.h"
#include "rb_objectcontainer.h"

NAMESPACE_REDBAG_CALC

class RB_UnitTestFactory {

public:
    RB_UnitTestFactory(RB_ObjectContainer* inputOutput);
    virtual ~RB_UnitTestFactory();

    virtual void exec() = 0;

protected:
    RB_ObjectContainer* mInputOutput;
};

END_NAMESPACE_REDBAG_CALC

#endif // RB_UNITTESTFACTORY_H
