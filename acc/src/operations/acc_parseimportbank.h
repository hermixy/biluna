/*****************************************************************
 * $Id: acc_parseimportbank.h 1809 2012-12-03 12:26:40Z rutger $
 * Created: Dec 3, 2012 12:1:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_PARSEIMPORTBANK_H
#define ACC_PARSEIMPORTBANK_H

#include "acc_operation.h"


/**
 * Base class for operations (none-gui actions)
 * TODO: consider moving this class to DB as RB_Operation
 */
class ACC_ParseImportBank : public ACC_Operation  {

public:
    ACC_ParseImportBank();
    virtual ~ACC_ParseImportBank();

    virtual bool execute(RB_ObjectBase* input,
                         RB_ObjectBase* output);

protected:
    RB_ObjectBase* mObject;

};

#endif // ACC_PARSEIMPORTBANK_H
