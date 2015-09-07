#ifndef RB_UNITTESTFACTORY_H
#define RB_UNITTESTFACTORY_H

#include "rb_namespace.h"
#include "rb_objectcontainer.h"

NAMESPACE_BILUNA_CALC

#define UNITTESTCLASS(className) \
    ut = new className(); \
    ut->exec(); \
    delete ut;

class RB_UnitTestFactory {

public:
    RB_UnitTestFactory();
    virtual ~RB_UnitTestFactory();

    virtual void exec() = 0;

};

END_NAMESPACE_BILUNA_CALC

#endif // RB_UNITTESTFACTORY_H
