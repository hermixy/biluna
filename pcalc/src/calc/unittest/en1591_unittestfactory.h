#ifndef EN1591_UNITTESTFACTORY_H
#define EN1591_UNITTESTFACTORY_H

#include <QString>
#include <QTextEdit>
#include "rb_namespace.h"
#include "rb_objectcontainer.h"
#include "rb_unittestfactory.h"

NAMESPACE_REDBAG_CALC_EN1591

class EN1591_UnitTestFactory : public RB_UnitTestFactory {

public:
    EN1591_UnitTestFactory(RB_ObjectContainer* inputOutput);
    virtual ~EN1591_UnitTestFactory();

    virtual void exec();

};

END_NAMESPACE_REDBAG_CALC_EN1591

#endif // EN1591_UNITTESTFACTORY_H
