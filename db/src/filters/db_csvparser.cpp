/*****************************************************************
 * $Id: db_csvparser.cpp 2201 2015-01-08 14:30:23Z rutger $
 * Created: Dec 18, 2014 18:04:25 PM - rutger
 *
 * Copyright (C) 2014 Biluna. All rights reserved.
 * This file is part of the Biluna DB project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "db_csvparser.h"

#include <QFile>
#include <QTextStream>
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"


DB_CsvParser::DB_CsvParser(RB_ObjectContainer* csvObjectList,
                           const QString& fileName) {
    mFileName = fileName;
    mRawData = "";
    mCsvObjectList = csvObjectList;

    mIsFirstLineTitleRow = false;
    mRemoveTitleRow = false;
    mSkipFirstNumberOfLines = -1;
    mReadMaximumNumberOfLines = -1;

    mFieldSeparator = "";
    mIsRemoveQuotes = false;
}

DB_CsvParser::DB_CsvParser(const RB_String& rawData,
                           RB_ObjectContainer* csvObjectList) {
    mFileName = "";
    mRawData = rawData;
    mCsvObjectList = csvObjectList;

    mIsFirstLineTitleRow = false;
    mRemoveTitleRow = false;
    mSkipFirstNumberOfLines = -1;
    mReadMaximumNumberOfLines = -1;

    mFieldSeparator = "";
    mIsRemoveQuotes = false;
}


DB_CsvParser::~DB_CsvParser() {
    // nothing
}

/**
 * Parsing execute by reading the file and creating object list
 */
bool DB_CsvParser::exec() {
    /*
    TODO: line by line and progress bar
    void MainWindow::processCSVFile(QString filename)
    {
        QFile file(filename);
        ui->statusBar->showMessage(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                processLineFromCSV(line);
                ui->progressBar->setValue(++CurrentLine);
            }
            file.close();
        }
    }
    */
    if (!mFileName.isEmpty() && !loadFile()) {
        mMessage = "Error loading file";
        return false;
    }

    if (!parse(mRawData)) {
        mMessage += "Error parsing file";
        return false;
    }

    return true;
}

void DB_CsvParser::setFirstLineIsTitleRow(bool isFirstTitleRow) {
    mIsFirstLineTitleRow = isFirstTitleRow;
}

void DB_CsvParser::setRemoveTitleRow(bool removeTitleRow) {
    mRemoveTitleRow = removeTitleRow;
}

void DB_CsvParser::setSkipFirstNumberOfLines(int skipLines) {
    mSkipFirstNumberOfLines = skipLines;
}

void DB_CsvParser::setReadMaximumNumberOfLines(int maxNumberLines) {
    mReadMaximumNumberOfLines = maxNumberLines;
}

void DB_CsvParser::setFieldSeparator(const RB_String& sep) {
    mFieldSeparator = sep;
}

void DB_CsvParser::setRemoveQuotes(bool isRemoveQuotes) {
    mIsRemoveQuotes = isRemoveQuotes;
}

/**
 * Load file from disk
 * @param fn file name
 */
bool DB_CsvParser::loadFile() {
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
bool DB_CsvParser::parse(const RB_String& rawData) {
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

    if (mIsFirstLineTitleRow && mRemoveTitleRow) {
        lines.removeFirst();
        lineData = lines.first();
        --rowCount;
    }

    RB_String fieldSeparator = mFieldSeparator;
    RB_String quote = "";

    if (mIsRemoveQuotes) {
        if (lineData.contains("\"")) {
            quote = "\"";
        } else if (lineData.contains("\'")) {
            quote = "\'";
        }

        fieldSeparator = quote + mFieldSeparator + quote;
    }

    trimLine(lineData, quote);

    RB_StringList lineFields = lineData.split(fieldSeparator, QString::KeepEmptyParts);
    int colCount = lineFields.count();
    int row = 0;

    while (row < rowCount) {
        if (row > 0) { // first line was already done above
            lineData = lines[row];

            if (row < mSkipFirstNumberOfLines && mSkipFirstNumberOfLines > 0) {
                continue;
            }

            if ((row >= mReadMaximumNumberOfLines && mReadMaximumNumberOfLines > 0)
                    || (row >= mReadMaximumNumberOfLines + mSkipFirstNumberOfLines
                        && mReadMaximumNumberOfLines > 0
                        && mSkipFirstNumberOfLines > 0)) {
                row = rowCount;
            }

            // check for empty last row due to ending lineSeparator
            if (lineData.count(fieldSeparator) < colCount - 1) {
                row = rowCount;
                continue;
            }

            trimLine(lineData, quote);
            lineFields = lineData.split(fieldSeparator, QString::KeepEmptyParts);
        }

        bool withoutMembers = true;
        RB_ObjectAtomic* csvObj = new RB_ObjectAtomic("", mCsvObjectList, "",
                                                      NULL, withoutMembers);
        mCsvObjectList->addObject(csvObj);

        for (int col = 0; col < colCount; ++col) {
            csvObj->addMember(csvObj, "val" + RB_String::number(col),
                              "-", lineFields.at(col));
        }

        ++row;
    }

    return true;
}

void DB_CsvParser::trimLine(RB_String& lineData, const RB_String& quote) {
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
}
