/*****************************************************************
 * $Id: scan_preparejrreport.h 1470 2011-10-19 21:06:10Z rutger $
 * Created: May 17, 2011 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_PREPAREJRREPORT_H
#define SCAN_PREPAREJRREPORT_H

#include "rb_string.h"

class RB_ObjectBase;
class SCAN_PrepareJrResult;

/**
 * Operation class to prepare the Mindcom report.
 * This operation is specific for mindcom.nl,
 * based on method of Jerry Rhodes (Jr).
 * TODO: This operation should be replaced
 * by a script and scripting tool in Biluna inorder to
 * be generic for all scan or interview types.
 */
class SCAN_PrepareJrReport {

public:
    SCAN_PrepareJrReport();

    RB_String getReport(RB_ObjectBase* textBlock, SCAN_PrepareJrResult* result);

    enum AndOrType {
        AndOrNone= 0,
        AndOrAnd,
        AndOrOr
    };

private:
    void addReportText(RB_ObjectBase* textBlock, SCAN_PrepareJrResult* result);
    void updateContent(RB_String& content, int type, SCAN_PrepareJrResult* result);
    void setValue(RB_String& content, SCAN_PrepareJrResult* result);
    void setChart(RB_String& content, SCAN_PrepareJrResult* result);

    RB_String mReportText;

};

#endif // SCAN_PREPAREJRREPORT_H
