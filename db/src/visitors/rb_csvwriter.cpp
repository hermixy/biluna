/*****************************************************************
 * $Id: rb_csvwriter.cpp 1491 2011-12-07 21:51:55Z rutger $
 * Created: Dec 7, 2011 3:48:59 PM - rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_csvwriter.h"


/**
 * Constructor
 * @param device device to be written
 */
RB_CsvWriter::RB_CsvWriter(QIODevice* device) {
    mRootName = "";
    mSeparator = RB2::CsvSemicolon;
    mDevice = device;
    mString = NULL;
    mIsFirstRow = false;
}

/**
 * Constructor
 * @param txt string to be written
 */
RB_CsvWriter::RB_CsvWriter(RB_String* str) {
    mRootName = "";
    mSeparator = RB2::CsvSemicolon;
    mDevice = NULL;
    mString = str;
    mIsFirstRow = false;
}

/**
 * Visit object and write data
 * @param obj object
 */
void RB_CsvWriter::visitObject(RB_ObjectBase* obj) {
    if (!obj) return;

    if (mRootName.isEmpty()) {
        mRootName = obj->getName();
        QStringList strL = mRootName.split("_");
        mContextName = strL.at(0).toUpper();

        switch (mSeparator) {
        case RB2::CsvComma:
            mSeparatorCharacter = ",";
            break;
        case RB2::CsvSemicolon:
            mSeparatorCharacter = ";";
            break;
        case RB2::CsvTab:
            mSeparatorCharacter = "\t";
            break;
        default:
            mSeparatorCharacter = ";";
            break;
        }

        // First object contains path and name of database
        RB_String str = "-- DATABASE: " + obj->getName() + "\n";
        writeCharacters(str);
        return;
    }

    if (obj->getName().endsWith("List")) {
        mIsFirstRow = true;
        return;
    } else {
        if (mIsFirstRow) {
            RB_String str = "-- TABLE: " + obj->getName() + "\n";
            writeCharacters(str);
            writeMemberName(obj);
            writeCharacters(RB_String("\n"));
            mIsFirstRow = false;
        }

        writeMemberValue(obj);
    }
}

/**
 * Leave object and write closing end line
 * @param obj object
 */
void RB_CsvWriter::leaveObject(RB_ObjectBase* obj) {
    if (!obj) return;
    writeCharacters(RB_String("\n"));
}

/**
 * Write data
 * @param str write string to device or string
 */
void RB_CsvWriter::writeCharacters(const RB_String& str) {
    if (mDevice) {
        mDevice->write(str.toStdString().c_str());
    } else if (mString) {
        mString->append(str);
    }
}

/**
 * Writes the object (column names) to the file.
 * @param obj is an object from the model
 */
void RB_CsvWriter::writeMemberName(RB_ObjectBase* obj) {
    for (int i = 0; i < obj->countMember(); i++) {
        if (i > 0) {
            writeCharacters(mSeparatorCharacter);
        }

        writeCharacters(obj->getMember(i)->getName());
    }
}

/**
 * Writes the object (row data) to the file.
 * @param obj is an object from the model
 */
void RB_CsvWriter::writeMemberValue(RB_ObjectBase* obj) {
    for (int i = 0; i < obj->countMember(); i++) {
        if (i > 0) {
            writeCharacters(mSeparatorCharacter);
        }
        // simplified() leaves one internal space per occurence
        writeCharacters(obj->getValue(i).toString().simplified());
    }
}

