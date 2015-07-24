/*****************************************************************
 * $Id: scan_preparejrresult.h 1470 2011-10-19 21:06:10Z rutger $
 * Created: April 20, 2011 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna LDT project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#ifndef SCAN_PREPAREJRRESULT_H
#define SCAN_PREPAREJRRESULT_H

#include <QObject>
#include "rb_string.h"
#include "rb_stringlist.h"

class RB_ObjectBase;


/**
 * Operation class prepare the Mindcom scan or interview result. This
 * operation is specific for mindcom.nl, based on method of Jerry Rhodes.
 * TODO: This operation should be
 * replaced by a script and scripting tool in Biluna inorder to
 * be generic for all scan or interview types.
 */
class SCAN_PrepareJrResult : public QObject {

    Q_OBJECT // translation only

public:
    SCAN_PrepareJrResult();
    virtual ~SCAN_PrepareJrResult();

    void execute(RB_ObjectBase* scanResult);
    int getResult(int mcType);
    int getResult(const RB_String& jrStr);

    RB_String getResultHtml();
    RB_StringList getContentTypeList();
    RB_StringList getValidityList();

    /** Identification of the content type such as text and charts */
    enum ContentType {
        // Note: list order should be the same as getContentTypeList()
        ContentText = 0,
        ContentTextValue,
        ContentChart
    };

    /** Identification of the result in Mindcom format */
    enum JrType {
        // Note: list order should be the same as getValidityList()
        JrTotal01 = 1, JrTotal02, JrTotal03, JrTotal04, JrTotal05,
        JrTotal06, JrTotal07, JrTotal08, JrTotal09, JrTotal10,
        JrTotal11, JrTotal12, JrTotal13, JrTotal14, JrTotal15,
        JrTotal16, JrTotal17, JrTotal18, JrTotal19, JrTotal20, JrTotal21,
        JrPerc01, JrPerc02, JrPerc03, JrPerc04, JrPerc05,
        JrPerc06, JrPerc07, JrPerc08, JrPerc09, JrPerc10,
        JrPerc11, JrPerc12, JrPerc13, JrPerc14, JrPerc15,
        JrPerc16, JrPerc17, JrPerc18, JrPerc19, JrPerc20, JrPerc21,
        JrColorHb, JrColorSb, JrColorHr, JrColorSr, JrColorHg, JrColorSg,
        JrColorBlue, JrColorRed, JrColorGreen
    };

private:
    void setTotals();
    RB_String get(int questionNo, int answerNo);
    RB_String getRS(int mcType);

    RB_ObjectBase* mQResultList;

    int mJrTotal01, mJrTotal02, mJrTotal03, mJrTotal04, mJrTotal05;
    int mJrTotal06, mJrTotal07, mJrTotal08, mJrTotal09, mJrTotal10;
    int mJrTotal11, mJrTotal12, mJrTotal13, mJrTotal14, mJrTotal15;
    int mJrTotal16, mJrTotal17, mJrTotal18, mJrTotal19, mJrTotal20;
    int mJrTotal21;
    int mJrPercent01, mJrPercent02, mJrPercent03, mJrPercent04, mJrPercent05;
    int mJrPercent06, mJrPercent07, mJrPercent08, mJrPercent09, mJrPercent10;
    int mJrPercent11, mJrPercent12, mJrPercent13, mJrPercent14, mJrPercent15;
    int mJrPercent16, mJrPercent17, mJrPercent18, mJrPercent19, mJrPercent20;
    int mJrPercent21;
    int mJrColorHb, mJrColorSb, mJrColorHr, mJrColorSr, mJrColorHg, mJrColorSg;

    RB_StringList mValidityList;
};

#endif /*SCAN_PREPAREJRRESULT_H*/
