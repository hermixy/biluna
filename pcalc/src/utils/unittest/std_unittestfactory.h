#ifndef STD_UNITTESTFACTORY_H
#define STD_UNITTESTFACTORY_H

#include "rb_unittestfactory.h"

class STD_UnitTestFactory : public Biluna::Calc::RB_UnitTestFactory {

public:
    STD_UnitTestFactory();
    virtual ~STD_UnitTestFactory();

    virtual void exec();

};

#endif // STD_UNITTESTFACTORY_H
