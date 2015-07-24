/*****************************************************************
 * $Id: acc_bankparser.h 1950 2013-07-08 16:26:56Z rutger $
 * Created: Jul 04, 2013 8:04:25 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_BANKPARSER_H
#define ACC_BANKPARSER_H

#include "rb_string.h"
#include "rb_stringlist.h"

class RB_ObjectBase;
class RB_ObjectContainer;
class ACC_MT940BankParser;

/**
 * MT940 and CSV file format parser for reading bank statements
 */
class ACC_BankParser {

public:
    ACC_BankParser(const RB_String& fileName, RB_ObjectContainer* bankImportList);
    virtual ~ACC_BankParser();

    virtual bool exec();
    virtual void getRawData(RB_String& rawData);
    virtual void getMessage(RB_String& message);

protected:
    virtual bool loadFile();
    virtual bool setBankMappingList();
    virtual bool selectMapping();
    virtual bool parseCsvData();

private:
    RB_String mFileName;
    RB_String mRawData;
    ACC_MT940BankParser* mMT940BankParser;
    RB_ObjectContainer* mBankImportList;

    RB_String mMessage;

    RB_ObjectContainer* mBankMappingList;
    RB_ObjectBase* mBankMapping;

};

#endif // ACC_BANKPARSER_H
