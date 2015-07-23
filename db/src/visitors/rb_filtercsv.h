/*****************************************************************
 * $Id: rb_filtercsv.h 1489 2011-12-07 18:21:38Z rutger $
 * Created: Jun 4, 2008 3:49:10 PM - rutger
 *
 * Copyright (C) 2008 Red-Bag. All rights reserved.
 * This file is part of the Biluna db project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef RB_FILTERCSV_H_
#define RB_FILTERCSV_H_

#include <fstream>
#include <string>
#include "rb_objectatomic.h"
#include "rb_objectcontainer.h"

/**
 * Filter for import and export of Comma Separate Value (CSV) files. This filter
 * is for general use and is not related to a specific object model
 */
class RB_FilterCsv {
	
public:
    RB_FilterCsv();
    virtual ~RB_FilterCsv();
	
    // Import:
    virtual bool fileImport(RB_ObjectContainer* root, const RB_String& fileName);
    
    // Export:
    virtual bool fileExport(RB_ObjectContainer* root, const RB_String& fileName);
    virtual void setSeparator(RB2::CsvSeparator sep) { mSeparator = sep; }
    
private:
    void writeList(RB_ObjectBase* obj);
    void writeObject(RB_ObjectBase* obj);
	
    /** (CSV) file to be created */
    mutable std::ofstream m_ofile;
    /** CSV file separator */
    RB2::CsvSeparator mSeparator;
    /** Separator character*/
    std::string mSeparatorCharacter;
};

#endif /*RB_FILTERCSV_H_*/
