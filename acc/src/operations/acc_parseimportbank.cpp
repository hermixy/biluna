/*****************************************************************
 * $Id: acc_parseimportbank.cpp 1809 2012-12-03 12:26:40Z rutger $
 * Created: Dec 3, 2012 12:1:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Accounting (ACC) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/


#include "acc_parseimportbank.h"

/**
 * Constructor
 */
ACC_ParseImportBank::ACC_ParseImportBank() {
    mObject = NULL;

}

/**
 * Destructor
 */
ACC_ParseImportBank::~ACC_ParseImportBank() {

}

/**
 * Execute parse operation
 * @param input imported bank statements
 * @param output ACC_ChartMaster object to be completed with
 * ACC_TransDoc and ACC_BankTrans transactions
 * @return true on success, default false
 */
bool ACC_ParseImportBank::execute(RB_ObjectBase* /*input*/, RB_ObjectBase* /*output*/) {
    RB_DEBUG->print(RB_Debug::D_ERROR, "ACC_ParseImportBank::execute() "
                    "not implemented ERROR");
    return false;
}
