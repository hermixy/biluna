/*****************************************************************
 * $Id: acc_bankparser.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Jul 04, 2013 8:04:25 PM - rutger
 *
 * Copyright (C) 2013 Red-Bag. All rights reserved.
 * This file is part of the Biluna ACC project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "acc_bankparser.h"

#include "acc_csvbankparser.h"
#include "acc_mt940abnparser.h"
#include "acc_mt940ingparser.h"
#include "acc_mt940raboparser.h"
#include "acc_modelfactory.h"
#include "acc_objectfactory.h"
#include "rb_objectcontainer.h"


ACC_BankParser::ACC_BankParser (const RB_String& fileName,
                                RB_ObjectContainer* bankImportList) {
    mFileName = fileName;
    mRawData = "";
    mBankImportList = bankImportList;
    mMessage = "";
    mMT940BankParser = NULL;
    mBankMappingList = NULL;
    mBankMapping = NULL;
}

ACC_BankParser::~ACC_BankParser() {
    delete mMT940BankParser;
    delete mBankMappingList;
    // not mBankMapping, because it is an object of mBankMappingList
}

/**
 * Parsing execute by reading the file and creating object list
 */
bool ACC_BankParser::exec() {
    if (!loadFile()) {
        mMessage = "Error loading file";
        return false;
    }

    if (mMT940BankParser) {
        delete mMT940BankParser;
        mMT940BankParser = NULL;
    }

    if (mRawData.contains("ABNA") && mRawData.contains(":25:")
            && mRawData.contains(":60F:")) {
        mMT940BankParser = new ACC_MT940AbnParser();
    } else if (mRawData.contains("INGB") && mRawData.contains(":25:")
               && mRawData.contains(":60F:")) {
        mMT940BankParser = new ACC_MT940IngParser();
    } else if (mRawData.contains(":940:") && mRawData.contains(":25:")
               && mRawData.contains(":60F:")) {
        mMT940BankParser = new ACC_MT940RaboParser();
    } else if (mRawData.contains(":25:") && mRawData.contains(":60F:")) {
        mMT940BankParser = new ACC_MT940BankParser();
        mMessage = "Unknown bank: default mt940 parser used,\n";
    }

    bool success = false;

    if (mMT940BankParser) {
        if (mMT940BankParser->parse(mRawData, mBankImportList)) {
            success = true;
        } else {
            mMessage += "Error parsing MT940 file\n";
        }
    } else if (!mMT940BankParser) {
        // CSV parser
        success = setBankMappingList();
        success = success ? selectMapping() : false;
        success = success ? parseCsvData() : false;

        if (!success) {
            mMessage += "Error parsing CSV file,\n check settings\n";
        }
    }

    return success;
}

/**
 * Get raw data from file content
 * @param content file content
 */
void ACC_BankParser::getRawData(RB_String& rawData) {
    rawData = mRawData;
}

void ACC_BankParser::getMessage(RB_String& message) {
    message = mMessage;
}

/**
 * Load file from disk
 * @param fn file name
 */
bool ACC_BankParser::loadFile() {
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
 * Get bank CSV mapping list from database
 * @param bankMappingList list to be set with mappings
 */
bool ACC_BankParser::setBankMappingList() {
    if (!mBankMappingList) {
        RB_String parentId = ACC_MODELFACTORY->getRootId();
        mBankMappingList = new RB_ObjectContainer("", NULL, "ACC_BankMappingList",
                                                  ACC_OBJECTFACTORY);
        mBankMappingList->setValue("parent", parentId);
    } else {
        mBankMappingList->erase();
    }

    return mBankMappingList->dbReadList(ACC_MODELFACTORY->getDatabase(), RB2::ResolveOne);
}

/**
 * Select the relevant mapping from the mapping list
 */
bool ACC_BankParser::selectMapping() {
    if (!mBankMappingList) {
        return false;
    }

    mBankMapping = NULL; // not delete because object of mBankMappingList
    RB_ObjectIterator* iter = mBankMappingList->createIterator();
    int countOccurance = 0;

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();
        RB_String bankAccount = obj->getValue("bankaccount").toString();

        if (mRawData.contains(bankAccount)) {
            int count = mRawData.count(bankAccount);

            if (count > countOccurance) {
                countOccurance = count;
                mBankMapping = obj;
            }
        }
    }

    delete iter;

    if (mBankMapping) {
        return true;
    }

    return false;
}

/**
 * Parse CSV data file with the settings of selected mapping
 */
bool ACC_BankParser::parseCsvData() {
    if (!mBankMapping) {
        return false;
    }

    ACC_CsvBankParser* parser = new ACC_CsvBankParser(mBankImportList, mRawData);

    parser->setBankAccount(mBankMapping->getValue("bankaccount").toString());
    parser->setDateFormat(mBankMapping->getValue("dateformat").toString());
    parser->setCreditIdentifier(mBankMapping->getValue("creditname").toString()); // Optional required

    int idx = mBankMapping->getValue("separator_id").toInt();
    RB_String sep = "";

    if (idx == 0) {
        sep = ",";
    } else if (idx == 1) {
        sep = ";";
    } else if (idx == 2) {
        sep = "\t";
    }

    parser->setColumnSeparator(sep);
    bool isFirstRowTitle = mBankMapping->getValue("firstrowtitle_id").toInt() == 1 ? true : false;
    parser->setFirstLineIsTitleRow(isFirstRowTitle);
    bool isStripQuotes = mBankMapping->getValue("stripquotes_id").toInt() == 1 ? true : false;
    parser->setRemoveQuotes(isStripQuotes);

    idx = mBankMapping->getValue("decimal_id").toInt();
    sep = "";

    if (idx == 0) {
        sep = ".";
    } else if (idx == 1) {
        sep = ",";
    }

    parser->setDecimalSeparator(sep);
    parser->addColumnFieldMap(mBankMapping->getValue("colnobankaccount").toInt()-1, "statactnumber");
    // parser->addColumnFieldMap(mBankMapping->getValue("colnostattimestamp").toInt()-1, "stattimestamp"); // not used
    parser->addColumnFieldMap(mBankMapping->getValue("colnotransactiondate").toInt()-1, "transentrytimestamp");
    parser->addColumnFieldMap(mBankMapping->getValue("colnocounterbankaccount").toInt()-1, "transactnumber");
    parser->addColumnFieldMap(mBankMapping->getValue("colnocounteraccountholder").toInt()-1, "transaccount");
    parser->addColumnFieldMap(mBankMapping->getValue("colnodebitcredit").toInt()-1, "transdebitcredit"); // Optional/required
    parser->addColumnFieldMap(mBankMapping->getValue("colnoamount").toInt()-1, "transamount");
    parser->addColumnFieldMap(mBankMapping->getValue("colnodescription").toInt()-1, "transdescription");
    parser->addColumnFieldMap(mBankMapping->getValue("colnoseconddescription").toInt()-1, "transseconddescription");
    parser->addColumnFieldMap(mBankMapping->getValue("colnotransactioncode").toInt()-1, "transcode"); // Optional
    parser->addColumnFieldMap(mBankMapping->getValue("colnostatnumber").toInt()-1, "statnumber"); // Optional

    bool success = parser->exec();
    delete parser;
    return success;
}

