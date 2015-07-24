/*****************************************************************
 * $Id: acc_csvbankparser.h 2201 2015-01-08 14:30:23Z rutger $
 * Created: Jul 02, 2013 18:04:25 PM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef ACC_CSVBANKPARSER_H
#define ACC_CSVBANKPARSER_H

#include <map>
#include "rb_string.h"
#include "rb_stringlist.h"
class RB_ObjectContainer;

/**
 * CSV file format parser for bank statements
 */
class ACC_CsvBankParser {

public:
    ACC_CsvBankParser(const RB_String& fileName,
                      RB_ObjectContainer* bankImportList);
    ACC_CsvBankParser(RB_ObjectContainer* bankImportList,
                      const RB_String& rawData);
    virtual ~ACC_CsvBankParser();

    virtual bool exec();

    virtual void setBankAccount(const RB_String& bankAccount);
    virtual void setFirstLineIsTitleRow(bool isFirstTitleRow);
    virtual void setColumnSeparator(const RB_String& colSep);
    virtual void setRemoveQuotes(bool isRemoveQuotes);
    virtual void setDateFormat(const RB_String& dateFormat);
    virtual void setCreditIdentifier(const RB_String& creditId);
    virtual void setDecimalSeparator(const RB_String& decSep);
    virtual void addColumnFieldMap(int col, const RB_String& fieldName);

protected:
    virtual bool loadFile();

private:
    bool parse(const RB_String& rawData,
               RB_ObjectContainer* bankImportList);

    RB_String mFileName;
    RB_String mRawData;
    RB_ObjectContainer* mBankImportList;

    RB_String mMessage;

    RB_String mBankAccount;
    bool mIsFirstLineTitleRow;
    RB_String mColumnSeparator;
    bool mIsRemoveQuotes;
    RB_String mDateFormat;
    RB_String mCreditId;
    RB_String mDecimalSeparator;

    std::map<int, RB_String> mColFieldMap;

};

#endif // ACC_CSVBANKPARSER_H
