/*****************************************************************
 * $Id: acc_csvbankparser.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Jul 02, 2013 18:04:25 PM - rutger
 *
 * Copyright (C) 2013 Biluna. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_csvbankparser.h"

#include <QFile>
#include <QTextStream>
#include "acc_bankimport.h"
#include "rb_objectcontainer.h"


ACC_CsvBankParser::ACC_CsvBankParser(const QString& fileName,
                                     RB_ObjectContainer* bankImportList) {
    mFileName = fileName;
    mRawData = "";
    mBankImportList = bankImportList;

    mBankAccount = "";
    mIsFirstLineTitleRow = false;
    mColumnSeparator = "";
    mIsRemoveQuotes = false;
    mDateFormat = "";
    mCreditId = "";
    mDecimalSeparator = "";
}

ACC_CsvBankParser::ACC_CsvBankParser(RB_ObjectContainer* bankImportList,
                                     const RB_String& rawData) {
    mFileName = "";
    mRawData = rawData;
    mBankImportList = bankImportList;

    mBankAccount = "";
    mIsFirstLineTitleRow = false;
    mColumnSeparator = "";
    mIsRemoveQuotes = false;
    mDateFormat = "";
    mCreditId = "";
    mDecimalSeparator = "";
}


ACC_CsvBankParser::~ACC_CsvBankParser() {
    // nothing
}

/**
 * Parsing execute by reading the file and creating object list
 */
bool ACC_CsvBankParser::exec() {
    if (!mFileName.isEmpty() && !loadFile()) {
        mMessage = "Error loading file";
        return false;
    }

    if (!parse(mRawData, mBankImportList)) {
        mMessage += "Error parsing file";
        return false;
    }

    return true;
}

void ACC_CsvBankParser::setBankAccount(const RB_String& bankAccount)  {
    RB_String str = bankAccount.simplified();
    mBankAccount = str.remove(" ");
}

void ACC_CsvBankParser::setFirstLineIsTitleRow(bool isFirstTitleRow) {
    mIsFirstLineTitleRow = isFirstTitleRow;
}

void ACC_CsvBankParser::setColumnSeparator(const RB_String& colSep) {
    mColumnSeparator = colSep;
}

void ACC_CsvBankParser::setRemoveQuotes(bool isRemoveQuotes) {
    mIsRemoveQuotes = isRemoveQuotes;
}

void ACC_CsvBankParser::setDateFormat(const RB_String& dateFormat) {
    mDateFormat = dateFormat.trimmed();
}

void ACC_CsvBankParser::setCreditIdentifier(const RB_String& creditId) {
    mCreditId = creditId.trimmed();
}

void ACC_CsvBankParser::setDecimalSeparator(const RB_String& decSep) {
    mDecimalSeparator = decSep;
}

void ACC_CsvBankParser::addColumnFieldMap(int col, const RB_String& fieldName) {
    if (col < 0) {
        return;
    }
    mColFieldMap[col] = fieldName;
}

/**
 * Load file from disk
 * @param fn file name
 */
bool ACC_CsvBankParser::loadFile() {
    if (mFileName.isEmpty()) {
        return false;
    }

    // Check if file can be read
    QFile file(mFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }

    // Read file
    QTextStream in(&file);
    mRawData = in.readAll();
    file.close();

    return true;
}

/**
 * Parse the rawdata to statement list objects
 * @param rawData raw data (CSV text)
 * @param statementList list with bank import objects
 */
bool ACC_CsvBankParser::parse(const RB_String& rawData,
                              RB_ObjectContainer* bankImportList) {
    QString lineSeparator = "";

    if (rawData.contains("\r\n")) {
        // Windows
        lineSeparator = "\r\n";
    } else if (rawData.contains("\n")) {
        // *nix
        lineSeparator = "\n";
    } else if (rawData.contains("\r")) {
        // old mac
        lineSeparator = "\r";
    }

    RB_StringList lines = rawData.split(lineSeparator);
    int rowCount = lines.count();

    if (rowCount < 1) {
        return false;
    }

    QString lineData = lines.at(0);

    if (mIsFirstLineTitleRow) {
        lines.removeFirst();
        lineData = lines.first();
        --rowCount;
    }

    RB_String separator = mColumnSeparator;
    RB_String quote = "";

    if (mIsRemoveQuotes) {
        if (lineData.contains("\"")) {
            quote = "\"";
        } else if (lineData.contains("\'")) {
            quote = "\'";
        }

        separator = quote + mColumnSeparator + quote;
    }

    if (mIsRemoveQuotes) {
        lineData = lineData.trimmed();

        if (lineData.startsWith(quote)) {
            lineData.remove(0, 1);
        }

        if (lineData.endsWith(quote)) {
            int size = lineData.size();
            lineData.remove(size-1, 1);
        }
    }

    RB_StringList lineFields = lineData.split(separator, QString::KeepEmptyParts);
    int colCount = lineFields.count();
    int col;
    RB_String field;
    std::map<int, RB_String>::const_iterator iter;
    RB_ObjectMember* mem;
    RB_String strDbl;
    RB_String str;

    int row = 0;

    while (row < rowCount) {
        if (row > 0) {
            lineData = lines[row];

            // check for empty last row
            if (lineData.count(separator) < colCount - 1) {
                row = rowCount;
                continue;
            }

            if (mIsRemoveQuotes) {
                lineData = lineData.trimmed();

                if (lineData.startsWith(quote)) {
                    lineData.remove(0, 1);
                }

                if (lineData.endsWith(quote)) {
                    int size = lineData.size();
                    lineData.remove(size-1, 1);
                }
            }

            lineFields = lineData.split(separator, QString::KeepEmptyParts);
        }

        ACC_BankImport* obj = new ACC_BankImport("", bankImportList, "ACC_BankImport");
        bankImportList->addObject(obj);

        for (iter = mColFieldMap.begin(); iter != mColFieldMap.end(); ++iter) {
            col = iter->first;
            field = iter->second;

            if (col >= 0 && col < colCount) {
                mem = obj->getMember(iter->second);

                if (mem) {
                    if (mem->getType() == RB2::MemberDouble) {
                        strDbl = lineFields.at(col);
                        if (mDecimalSeparator != ".") {
                            strDbl.replace(mDecimalSeparator, ".");
                        }
                        mem->setValue(strDbl.toDouble());
                    } else if (mem->getType() == RB2::MemberInteger) {
                        mem->setValue(lineFields.at(col).toInt());
                    } else if (mem->getName() == "stattimestamp"
                               || mem->getName() == "transentrytimestamp"
                               || mem->getName() == "transvaluetimestamp") {
                        QDate dt = QDate::fromString(lineFields.at(col), mDateFormat);
                        mem->setValue(dt.toString(Qt::ISODate));
                    } else if (mem->getName() == "statactnumber") {
                        str = lineFields.at(col);
                        str = str.simplified();
                        str.remove(" ");
                        mem->setValue(str);
                    } else if (mem->getName() == "transdescription"
                               || mem->getName() == "transseconddescription") {
                        str = lineFields.at(col);
                        str = str.simplified();
                        mem->setValue(str);
                    } else {
                        mem->setValue(lineFields.at(col));
                    }
                }
            } else {
                RB_DEBUG->print(RB_Debug::D_WARNING,
                                "Column number out of range WARNING");
                return false;
            }
        }

        // Handle credit amount
        RB_String creditId = obj->getValue("transdebitcredit").toString();
        if (!mCreditId.isEmpty() && creditId == mCreditId) {
            obj->setValue("transamount", -obj->getValue("transamount").toDouble());
        }

        ++row;
    }

    return true;
}
