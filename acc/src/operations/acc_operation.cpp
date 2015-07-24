/*****************************************************************
 * $Id: acc_operation.cpp 1809 2012-12-03 12:26:40Z rutger $
 * Created: Mar 8, 2010 8:14:42 PM - rutger
 *
 * Copyright (C) 2010 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "acc_operation.h"

/**
 * Constructor
 */
ACC_Operation::ACC_Operation() {
    mObject = NULL;

}

/**
 * Destructor
 */
ACC_Operation::~ACC_Operation() {

}

/**
 * Execute operation
 * @param obj result object, if applicable
 * @return true on success, default false
 */
bool ACC_Operation::execute(RB_ObjectBase* /* obj */) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "ACC_Operation::execute() "
                    "not implemented ERROR");
    return false;
}

/**
 * Execute operation
 * @param input input object, if applicable
 * @param output result/output object, if applicable
 * @return true on success, default false
 */
bool ACC_Operation::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "ACC_Operation::execute() "
                    "not implemented ERROR");
    return false;
}
