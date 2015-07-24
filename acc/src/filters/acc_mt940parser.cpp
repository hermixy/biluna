/*****************************************************************
 * $Id: acc_mt940parser.cpp 1950 2013-07-08 16:26:56Z rutger $
 * Created: Oct 13, 2012 9:04:25 AM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_mt940parser.h"

#include "acc_mt940abnparser.h"
#include "acc_mt940ingparser.h"
#include "acc_mt940raboparser.h"
#include "rb_objectcontainer.h"


ACC_MT940Parser::ACC_MT940Parser (const RB_String& fileName,
                                  RB_ObjectContainer* bankImportList) {
    mFileName = fileName;
    mRawData = "";
    mBankImportList = bankImportList;
    mBank = "";
    mBankParser = NULL;
    mMessage = "";
}


ACC_MT940Parser::~ACC_MT940Parser() {
    delete mBankParser;
}

/**
 * Parsing execute by reading the file and creating object list
 */
bool ACC_MT940Parser::exec() {
    if (!loadFile()) {
        mMessage = "Error loading file";
        return false;
    }

    if (mBankParser) {
        delete mBankParser;
        mBankParser = NULL;
    }

    if (mBank == "ABNA") {
        // force ABNAmro
        mBankParser = new ACC_MT940AbnParser();
    } else if (mBank == "INGB") {
        // force ING
        mBankParser = new ACC_MT940IngParser();
    } else if (mBank == "RABO") {
        // force Rabo
        mBankParser = new ACC_MT940RaboParser();
    } else if (mRawData.contains("ABNA")) {
        mBankParser = new ACC_MT940AbnParser();
    } else if (mRawData.contains("INGB")) {
        mBankParser = new ACC_MT940IngParser();
    } else if (mRawData.contains(":940:")) {
        mBankParser = new ACC_MT940RaboParser();
    } else if (mRawData.contains(":25:") && mRawData.contains(":60F:")) {
        mBankParser = new ACC_MT940BankParser();
        mMessage = "Unknown bank: default mt940 parser used\n";
    }

    if (!mBankParser) {
        mMessage += "No relevant MT940 parser found";
        return false;
    }

    if (!mBankParser->parse(mRawData, mBankImportList)) {
        mMessage += "Error parsing file";
        return false;
    }

    return true;
}

/**
 * Get raw data from file content
 * @param content file content
 */
void ACC_MT940Parser::getRawData(RB_String& rawData) {
    rawData = mRawData;
}

void ACC_MT940Parser::getMessage(RB_String& message) {
    message = mMessage;
}

/**
 * Load file from disk
 * @param fn file name
 */
bool ACC_MT940Parser::loadFile() {
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

