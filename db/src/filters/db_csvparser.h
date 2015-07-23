/*****************************************************************
 * $Id: db_csvparser.h 2248 2015-06-21 09:13:00Z rutger $
 * Created: Dec 18, 2014 18:04:25 PM - rutger
 *
 * Copyright (C) 2014 Biluna. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef DB_CSVPARSER_H
#define DB_CSVPARSER_H

#include <map>
#include "db_global.h"
#include "rb_string.h"
#include "rb_stringlist.h"

class RB_ObjectContainer;

/**
 * CSV file format parser for bank statements
 */
class DB_EXPORT DB_CsvParser {

public:
    DB_CsvParser(RB_ObjectContainer* csvObjectList,
                 const RB_String& fileName);
    DB_CsvParser(const RB_String& rawData,
                 RB_ObjectContainer* csvObjectList);
    virtual ~DB_CsvParser();

    virtual bool exec();

    virtual void setFirstLineIsTitleRow(bool isFirstTitleRow);
    virtual void setRemoveTitleRow(bool removeTitleRow);
    virtual void setSkipFirstNumberOfLines(int skipLines);
    virtual void setReadMaximumNumberOfLines(int maxNumberLines);

    virtual void setFieldSeparator(const RB_String& sep);
    virtual void setRemoveQuotes(bool isRemoveQuotes);

protected:
    virtual bool loadFile();

private:
    bool parse(const RB_String& rawData);
    void trimLine(RB_String& lineData, const RB_String& quote);

    RB_String mFileName;
    RB_String mRawData;
    RB_ObjectContainer* mCsvObjectList;

    RB_String mMessage;

    bool mIsFirstLineTitleRow;
    bool mRemoveTitleRow;
    int mSkipFirstNumberOfLines;
    int mReadMaximumNumberOfLines;

    RB_String mFieldSeparator;
    bool mIsRemoveQuotes;
};

#endif // DB_CSVPARSER_H
