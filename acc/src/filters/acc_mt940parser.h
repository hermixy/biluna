/*****************************************************************
 * $Id: acc_mt940parser.h 1950 2013-07-08 16:26:56Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_MT940PARSER_H
#define ACC_MT940PARSER_H

#include "rb_string.h"
#include "rb_stringlist.h"

class RB_ObjectContainer;
class ACC_MT940BankParser;

/**
 * MT940 file format parser for reading bank statements
 */
class ACC_MT940Parser {

public:
    ACC_MT940Parser(const RB_String& fileName, RB_ObjectContainer* bankImportList);
    virtual ~ACC_MT940Parser();

    virtual bool exec();
    virtual void getRawData(RB_String& rawData);
    virtual void getMessage(RB_String& message);

protected:
    virtual bool loadFile();

private:
    RB_String mFileName;
    RB_String mRawData;
    RB_ObjectContainer* mBankImportList;

    RB_String mBank; // bank name to force correct parser
    ACC_MT940BankParser* mBankParser;
    RB_String mMessage;

};

#endif // ACC_MT940PARSER_H
