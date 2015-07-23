/*****************************************************************
 * $Id: rb_filtercsv.cpp 1489 2011-12-07 18:21:38Z rutger $
 * Created: Jun 4, 2008 3:48:59 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "rb_filtercsv.h"

RB_FilterCsv::RB_FilterCsv() {
	mSeparator = RB2::CsvSemicolon;
}

RB_FilterCsv::~RB_FilterCsv() {
	
}

/**
 * Import CSV file
 * TODO: all
 */
bool RB_FilterCsv::fileImport(RB_ObjectContainer* /*root*/, const RB_String& /*fileName*/) {
	return false;
}

/**
 * Export CSV file
 * @param root root of object model written to CSV file
 * @param fileName file name
 */
bool RB_FilterCsv::fileExport(RB_ObjectContainer* root, const RB_String& fileName) {
    RB_DEBUG->print("RB_FilterCsv::writeObject() %s",
                        root->getName().toStdString().c_str());
    m_ofile.open(fileName.toStdString().c_str());
    m_ofile << "-- DATABASE: "
            << root->getName().toStdString().c_str()
            << std::endl;

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
	
    writeObject(root);

    return true;
}

/**
 * Writes the object (row) to the file.
 * @param obj is an object from the model
 */
void RB_FilterCsv::writeObject(RB_ObjectBase* obj) {
    // RB_DEBUG->print("RB_FilterCsv::writeObject() %s",
    //					obj->getName().toStdString().c_str());
	

    // Function is recursive, in case of obj is a table
    if (obj->isContainer()) {
        RB_ObjectContainer* objC = dynamic_cast<RB_ObjectContainer*>(obj);
        RB_ObjectIterator* iter = objC->createIterator();
        bool isFirstRow = true;

        for (iter->first(); !iter->isDone(); iter->next()) {
            // if current object is atomic write the first time to column names
            RB_ObjectBase* curObj = iter->currentObject();

            if (isFirstRow && curObj->isAtomic()) {
                // obj is table from database
                RB_String str = obj->getName();
                m_ofile << "-- TABLE: "
                        << str.remove("List").toStdString().c_str()
                        << std::endl;

                bool isStart = true;
                for (int i = 0; i < curObj->countMember(); i++) {
                    if (!isStart) m_ofile << mSeparatorCharacter;
                    m_ofile << curObj->getMember(i)->getName().toStdString();
                    isStart = false;
                }
                m_ofile << std::endl;
                isFirstRow = false;
            }

            writeObject(curObj);
		}
		delete iter;
    } else if (obj->isAtomic()) {
        // row from table, write member value (no attributes)
        bool isStart = true;
        for (int i = 0; i < obj->countMember(); i++) {
            if (!isStart) m_ofile << mSeparatorCharacter;
            m_ofile << obj->getMember(i)->getValue().toString().toStdString();
            isStart = false;
        }

        m_ofile << std::endl;
        // synchronize the output with the output (file) device
        m_ofile.flush();
    }
}

