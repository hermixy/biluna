/*****************************************************************
 * $Id: scan_preparejrresult.cpp 1471 2011-10-20 18:42:46Z rutger $
 * Created: Apr 20, 2011 rutger
 *
 * Copyright (C) 2011 Red-Bag. All rights reserved.
 * This file is part of the Biluna SCAN project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "scan_preparejrresult.h"

#include "rb_objectcontainer.h"
#include "scan_modelfactory.h"


/**
 * Constructor
 */
SCAN_PrepareJrResult::SCAN_PrepareJrResult() {
    mQResultList = NULL;

    mJrTotal01 = 0;
    mJrTotal02 = 0;
    mJrTotal03 = 0;
    mJrTotal04 = 0;
    mJrTotal05 = 0;
    mJrTotal06 = 0;
    mJrTotal07 = 0;
    mJrTotal08 = 0;
    mJrTotal09 = 0;
    mJrTotal10 = 0;
    mJrTotal11 = 0;
    mJrTotal12 = 0;
    mJrTotal13 = 0;
    mJrTotal14 = 0;
    mJrTotal15 = 0;
    mJrTotal16 = 0;
    mJrTotal17 = 0;
    mJrTotal18 = 0;
    mJrTotal19 = 0;
    mJrTotal20 = 0;
    mJrTotal21 = 0;
    mJrPercent01 = 0;
    mJrPercent02 = 0;
    mJrPercent03 = 0;
    mJrPercent04 = 0;
    mJrPercent05 = 0;
    mJrPercent06 = 0;
    mJrPercent07 = 0;
    mJrPercent08 = 0;
    mJrPercent09 = 0;
    mJrPercent10 = 0;
    mJrPercent11 = 0;
    mJrPercent12 = 0;
    mJrPercent13 = 0;
    mJrPercent14 = 0;
    mJrPercent15 = 0;
    mJrPercent16 = 0;
    mJrPercent17 = 0;
    mJrPercent18 = 0;
    mJrPercent19 = 0;
    mJrPercent20 = 0;
    mJrPercent21 = 0;
    mJrColorHb = 0;
    mJrColorSb = 0;
    mJrColorHr = 0;
    mJrColorSr = 0;
    mJrColorHg = 0;
    mJrColorSg = 0;
}

/**
 * Destructor
 */
SCAN_PrepareJrResult::~SCAN_PrepareJrResult() {
    // nothing
}

/**
 * Execute operation, prepare Mindcom results
 * @param scanResult scan result (with question result list)
 */
void SCAN_PrepareJrResult::execute(RB_ObjectBase* scanResult) {
    mQResultList = scanResult->getContainer("SCAN_QuestionResultList");
    if (!mQResultList) return;

    setTotals();

    mJrPercent01 = (mJrTotal01 * 100) / 24;
    mJrPercent02 = (mJrTotal02 * 100) / 24;
    mJrPercent03 = (mJrTotal03 * 100) / 24;
    mJrPercent04 = (mJrTotal04 * 100) / 18;
    mJrPercent05 = (mJrTotal05 * 100) / 18;
    mJrPercent06 = (mJrTotal06 * 100) / 18;
    mJrPercent07 = (mJrTotal07 * 100) / 18;
    mJrPercent08 = (mJrTotal08 * 100) / 24;
    mJrPercent09 = (mJrTotal09 * 100) / 24;
    mJrPercent10 = (mJrTotal10 * 100) / 24;
    mJrPercent11 = (mJrTotal11 * 100) / 24;
    mJrPercent12 = (mJrTotal12 * 100) / 24;
    mJrPercent13 = (mJrTotal13 * 100) / 24;
    mJrPercent14 = (mJrTotal14 * 100) / 18;
    mJrPercent15 = (mJrTotal15 * 100) / 18;
    mJrPercent16 = (mJrTotal16 * 100) / 18;
    mJrPercent17 = (mJrTotal17 * 100) / 18;
    mJrPercent18 = (mJrTotal18 * 100) / 18;
    mJrPercent19 = (mJrTotal19 * 100) / 18;
    mJrPercent20 = (mJrTotal20 * 100) / 18;
    mJrPercent21 = (mJrTotal21 * 100) / 18;

    mJrColorHb = mJrTotal01 + mJrTotal02 + mJrTotal03;
    mJrColorSb = mJrTotal04 + mJrTotal05 + mJrTotal06 + mJrTotal07;
    mJrColorHr = mJrTotal08 + mJrTotal09 + mJrTotal10;
    mJrColorSr = mJrTotal11 + mJrTotal12 + mJrTotal13;
    mJrColorHg = mJrTotal14 + mJrTotal15 + mJrTotal16 + mJrTotal17;
    mJrColorSg = mJrTotal18 + mJrTotal19 + mJrTotal20 + mJrTotal21;
}

/**
 * Get result based on type
 * @param mcType JrType enumerator of this class
 */
int SCAN_PrepareJrResult::getResult(int mcType) {
    switch(mcType) {
        case JrTotal01 : return mJrTotal01;
        case JrTotal02 : return mJrTotal02;
        case JrTotal03 : return mJrTotal03;
        case JrTotal04 : return mJrTotal04;
        case JrTotal05 : return mJrTotal05;
        case JrTotal06 : return mJrTotal06;
        case JrTotal07 : return mJrTotal07;
        case JrTotal08 : return mJrTotal08;
        case JrTotal09 : return mJrTotal09;
        case JrTotal10 : return mJrTotal10;
        case JrTotal11 : return mJrTotal11;
        case JrTotal12 : return mJrTotal12;
        case JrTotal13 : return mJrTotal13;
        case JrTotal14 : return mJrTotal14;
        case JrTotal15 : return mJrTotal15;
        case JrTotal16 : return mJrTotal16;
        case JrTotal17 : return mJrTotal17;
        case JrTotal18 : return mJrTotal18;
        case JrTotal19 : return mJrTotal19;
        case JrTotal20 : return mJrTotal20;
        case JrTotal21 : return mJrTotal21;
        case JrPerc01 : return mJrPercent01;
        case JrPerc02 : return mJrPercent02;
        case JrPerc03 : return mJrPercent03;
        case JrPerc04 : return mJrPercent04;
        case JrPerc05 : return mJrPercent05;
        case JrPerc06 : return mJrPercent06;
        case JrPerc07 : return mJrPercent07;
        case JrPerc08 : return mJrPercent08;
        case JrPerc09 : return mJrPercent09;
        case JrPerc10 : return mJrPercent10;
        case JrPerc11 : return mJrPercent11;
        case JrPerc12 : return mJrPercent12;
        case JrPerc13 : return mJrPercent13;
        case JrPerc14 : return mJrPercent14;
        case JrPerc15 : return mJrPercent15;
        case JrPerc16 : return mJrPercent16;
        case JrPerc17 : return mJrPercent17;
        case JrPerc18 : return mJrPercent18;
        case JrPerc19 : return mJrPercent19;
        case JrPerc20 : return mJrPercent20;
        case JrPerc21 : return mJrPercent21;
        case JrColorHb : return mJrColorHb;
        case JrColorSb : return mJrColorSb;
        case JrColorHr : return mJrColorHr;
        case JrColorSr : return mJrColorSr;
        case JrColorHg : return mJrColorHg;
        case JrColorSg : return mJrColorSg;
        case JrColorBlue : return mJrColorHb + mJrColorSb;
        case JrColorRed : return mJrColorHr + mJrColorSr;
        case JrColorGreen : return mJrColorHg + mJrColorSg;
        default:
            break;
    }

    return -1; // error
}

/**
 * Get result based on type in string format
 * @param jrStr JrType in string format
 */
int SCAN_PrepareJrResult::getResult(const RB_String& jrStr) {
    RB_StringList strL = getValidityList();
    int number = strL.indexOf(jrStr);
    return getResult(number);
}

/**
 * Set totals based on answerlist
 * @param qResultList question list with answers of scan or interview
 */
void SCAN_PrepareJrResult::setTotals() {
    if (!mQResultList) return;

    int qNumber = 0;
    int aResA = 0;
    int aResB = 0;
    int aResC = 0;

    RB_ObjectIterator* qIter = mQResultList->createIterator();

    for (qIter->first(); !qIter->isDone(); qIter->next()) {
        RB_ObjectBase* qResult = qIter->currentObject();
        qNumber = qResult->getValue("number").toInt();
        RB_ObjectContainer* aResultList = qResult->getContainer("SCAN_AnswerResultList");

        RB_ObjectIterator* aIter = aResultList->createIterator();

        for (aIter->first(); !aIter->isDone(); aIter->next()) {
            RB_ObjectBase* aResult = aIter->currentObject();
            int aNumber = aResult->getValue("number").toInt();

            if (aNumber == 1) {
                aResA = aResult->getValue("mvalue").toInt();
            } else if (aNumber == 2) {
                aResB = aResult->getValue("mvalue").toInt();
            } else if (aNumber == 3) {
                aResC = aResult->getValue("mvalue").toInt();
            }
        }

        delete aIter;

        // add the results to the totals
        switch (qNumber) {
        case 1 :
            mJrTotal13 += aResA;
            mJrTotal19 += aResB;
            mJrTotal03 += aResC;
            break;
        case 2 :
            mJrTotal18 += aResA;
            mJrTotal09 += aResB;
            mJrTotal07 += aResC;
            break;
        case 3 :
            mJrTotal11 += aResA;
            mJrTotal17 += aResB;
            mJrTotal04 += aResC;
            break;
        case 4 :
            mJrTotal14 += aResA;
            mJrTotal07 += aResB;
            mJrTotal10 += aResC;
            break;
        case 5 :
            mJrTotal21 += aResA;
            mJrTotal02 += aResB;
            mJrTotal09 += aResC;
            break;
        case 6 :
            mJrTotal14 += aResA;
            mJrTotal11 += aResB;
            mJrTotal03 += aResC;
            break;
        case 7 :
            mJrTotal13 += aResA;
            mJrTotal06 += aResB;
            mJrTotal18 += aResC;
            break;
        case 8 :
            mJrTotal01 += aResA;
            mJrTotal16 += aResB;
            mJrTotal10 += aResC;
            break;
        case 9 :
            mJrTotal21 += aResA;
            mJrTotal10 += aResB;
            mJrTotal05 += aResC;
            break;
        case 10 :
            mJrTotal04 += aResA;
            mJrTotal12 += aResB;
            mJrTotal19 += aResC;
            break;
        case 11 :
            mJrTotal17 += aResA;
            mJrTotal08 += aResB;
            mJrTotal02 += aResC;
            break;
        case 12 :
            mJrTotal03 += aResA;
            mJrTotal18 += aResB;
            mJrTotal10 += aResC;
            break;
        case 13 :
            mJrTotal13 += aResA;
            mJrTotal01 += aResB;
            mJrTotal14 += aResC;
            break;
        case 14 :
            mJrTotal12 += aResA;
            mJrTotal20 += aResB;
            mJrTotal01 += aResC;
            break;
        case 15 :
            mJrTotal07 += aResA;
            mJrTotal15 += aResB;
            mJrTotal13 += aResC;
            break;
        case 16 :
            mJrTotal17 += aResA;
            mJrTotal06 += aResB;
            mJrTotal09 += aResC;
            break;
        case 17 :
            mJrTotal11 += aResA;
            mJrTotal20 += aResB;
            mJrTotal02 += aResC;
            break;
        case 18 :
            mJrTotal01 += aResA;
            mJrTotal19 += aResB;
            mJrTotal08 += aResC;
            break;
        case 19 :
            mJrTotal16 += aResA;
            mJrTotal04 += aResB;
            mJrTotal08 += aResC;
            break;
        case 20 :
            mJrTotal03 += aResA;
            mJrTotal09 += aResB;
            mJrTotal15 += aResC;
            break;
        case 21 :
            mJrTotal11 += aResA;
            mJrTotal05 += aResB;
            mJrTotal16 += aResC;
            break;
        case 22 :
            mJrTotal21 += aResA;
            mJrTotal06 += aResB;
            mJrTotal12 += aResC;
            break;
        case 23 :
            mJrTotal05 += aResA;
            mJrTotal08 += aResB;
            mJrTotal20 += aResC;
            break;
        case 24 :
            mJrTotal12 += aResA;
            mJrTotal15 += aResB;
            mJrTotal02 += aResC;
            break;
        default :
            break;
        }
    }

    delete qIter;
}

/**
 * @returns score result in HTML table format
 */
RB_String SCAN_PrepareJrResult::getResultHtml() {
    RB_MmProxy* m = SCAN_MODELFACTORY->getModel(SCAN_ModelFactory::ModelScanResult);
    RB_String title = "";
    if (m) {
        title = m->getCurrentValue("title").toString();
    }

    // <tr><td>1</td><td>2</td><td>3</td><td>4</td><td>5</td><td>6</td><td>7</td><td>8</td><td>9</td><td>10</td><td>11</td><td>12</td><td>13</td><td>14</td><td>15</td><td>16</td><td>17</td><td>18</td><td>19</td><td>20</td><td>21</td><td>22</td><td>23</td><td>24</td><td>25</td></tr>
    RB_String html;
    html = " \
<html><head> \
</head><body style=\"leftmargin=\"5px\" topmargin=\"5px\"> \
<table width=\"100%\" style=\" font-size: small; text-align: center; border-collapse: collapse; border: 1px solid black;\" border=\"1\"> \
<tbody> \
<tr><td colspan=\"19\" style=\"text-align: left;\">THUNKS ANALYSIS</td><td colspan=\"6\" style=\"text-align: right;\">Rhodes TIP</td></tr> \
<tr><td colspan=\"19\" style=\"text-align: left;\">Name: "+title+"</td><td colspan=\"6\" style=\"text-align: left;\">Date:</td></tr> \
<tr><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>_</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>_</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td><td>___</td></tr> \
<tr><td>&nbsp;</td><td>Comp</td><td>Dist</td><td>Test</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>Spec</td><td>Cat</td><td>Look</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>Chal</td><td>Red</td><td>Esc</td><td>Purs</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr> \
<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>Val</td><td>Int</td><td>Pred</td><td>Com</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>Obs</td><td>Cod</td><td>Set</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>Unf</td><td>Pret</td><td>Sym</td><td>Fee</td><td>&nbsp;</td></tr> \
<tr><td>1</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(1,3)+"</td><td>&nbsp;</td><td>&nbsp;</td> <td>&nbsp;</td> <td>&nbsp;</td> <td>&nbsp;</td> <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(1,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(1,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>1</td></tr> \
<tr><td>2</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(2,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(2,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(2,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>2</td></tr> \
<tr><td>3</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(3,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(3,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(3,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>3</td></tr> \
<tr><td>4</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(4,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(4,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(4,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>4</td></tr> \
<tr><td>5</td><td>&nbsp;</td><td><sup>B</sup>"+get(5,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(5,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(5,1)+"</td><td>5</td></tr> \
<tr><td>6</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(6,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(6,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(6,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>6</td></tr> \
<tr><td>7</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(7,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(7,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(7,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>7</td></tr> \
<tr><td>8</td><td><sup>A</sup>"+get(8,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(8,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(8,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>8</td></tr> \
<tr><td>9</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(9,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(9,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(9,1)+"</td><td>9</td></tr> \
<tr><td>10</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(10,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(10,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(10,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>10</td></tr> \
<tr><td>11</td><td>&nbsp;</td><td><sup>C</sup>"+get(11,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(11,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(11,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>11</td></tr> \
<tr><td>12</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(12,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(12,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(12,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>12</td></tr> \
<tr><td>13</td><td><sup>B</sup>"+get(13,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(13,1)+"</td><td>&nbsp;</td><td><sup>C</sup>"+get(13,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>13</td></tr> \
<tr><td>14</td><td><sup>C</sup>"+get(14,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(14,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(14,2)+"</td><td>&nbsp;</td><td>14</td></tr> \
<tr><td>15</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(15,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(15,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(15,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>15</td></tr> \
<tr><td>16</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(16,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(16,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(16,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>16</td></tr> \
<tr><td>17</td><td>&nbsp;</td><td><sup>C</sup>"+get(17,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(17,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(17,2)+"</td><td>&nbsp;</td><td>17</td></tr> \
<tr><td>18</td><td><sup>A</sup>"+get(18,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(18,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(18,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>18</td></tr> \
<tr><td>19</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(19,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(19,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(19,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>19</td></tr> \
<tr><td>20</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(20,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(20,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(20,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>20</td></tr> \
<tr><td>21</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(21,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(21,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(21,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>21</td></tr> \
<tr><td>22</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(22,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(22,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(22,1)+"</td><td>22</td></tr> \
<tr><td>23</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(23,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(23,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>C</sup>"+get(23,3)+"</td><td>&nbsp;</td><td>23</td></tr> \
<tr><td>24</td><td>&nbsp;</td><td><sup>C</sup>"+get(24,3)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>A</sup>"+get(24,1)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td><sup>B</sup>"+get(24,2)+"</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>24&nbsp;</td></tr> \
<tr><td>Tot.</td><td>"+getRS(JrTotal01)+"</td><td>"+getRS(JrTotal02)+"</td><td>"+getRS(JrTotal03)+"</td><td>"+getRS(JrTotal04)+"</td><td>"+getRS(JrTotal05)+"</td><td>"+getRS(JrTotal06)+"</td><td>"+getRS(JrTotal07)+"</td><td>&nbsp;</td><td>"+getRS(JrTotal08)+"</td><td>"+getRS(JrTotal09)+"</td><td>"+getRS(JrTotal10)+"</td><td>"+getRS(JrTotal11)+"</td> \
<td>"+getRS(JrTotal12)+"</td><td>"+getRS(JrTotal13)+"</td><td>&nbsp;</td><td>"+getRS(JrTotal14)+"</td><td>"+getRS(JrTotal15)+"</td><td>"+getRS(JrTotal16)+"</td><td>"+getRS(JrTotal17)+"</td><td>"+getRS(JrTotal18)+"</td><td>"+getRS(JrTotal19)+"</td><td>"+getRS(JrTotal20)+"</td><td>"+getRS(JrTotal21)+"</td><td>&nbsp;</td></tr> \
<tr><td>Max.</td><td colspan=\"3\">24</td><td colspan=\"4\">18</td><td>&nbsp;</td><td colspan=\"3\">24</td><td colspan=\"3\">24</td><td>&nbsp;</td><td colspan=\"4\">18</td><td colspan=\"4\">18</td><td>&nbsp;</td></tr> \
<tr><td>Perc.</td><td>"+getRS(JrPerc01)+"</td><td>"+getRS(JrPerc02)+"</td><td>"+getRS(JrPerc03)+"</td><td>"+getRS(JrPerc04)+"</td><td>"+getRS(JrPerc05)+"</td><td>"+getRS(JrPerc06)+"</td><td>"+getRS(JrPerc07)+"</td><td>&nbsp;</td><td>"+getRS(JrPerc08)+"</td><td>"+getRS(JrPerc09)+"</td><td>"+getRS(JrPerc10)+"</td><td>"+getRS(JrPerc11)+"</td> \
<td>"+getRS(JrPerc12)+"</td><td>"+getRS(JrPerc13)+"</td><td>&nbsp;</td><td>"+getRS(JrPerc14)+"</td><td>"+getRS(JrPerc15)+"</td><td>"+getRS(JrPerc16)+"</td><td>"+getRS(JrPerc17)+"</td><td>"+getRS(JrPerc18)+"</td><td>"+getRS(JrPerc19)+"</td><td>"+getRS(JrPerc20)+"</td><td>"+getRS(JrPerc21)+"</td><td>&nbsp;</td></tr> \
<tr><td colspan=\"25\">&nbsp;</td></tr> \
<tr><td>Col.</td><td>HB</td><td colspan=\"2\">"+getRS(JrColorHb)+"</td><td>SB</td><td colspan=\"3\">"+getRS(JrColorSb)+"</td><td>&nbsp;</td><td>HR</td><td colspan=\"2\">"+getRS(JrColorHr)+"</td><td>SR</td><td colspan=\"2\">"+getRS(JrColorSr)+"</td><td>&nbsp;</td><td>HG</td><td colspan=\"3\">"+getRS(JrColorHg)+"</td><td>SG</td><td colspan=\"3\">"+getRS(JrColorSg)+"</td><td>&nbsp;</td></tr> \
<tr><td>&nbsp;</td><td colspan=\"24\" style=\"text-align: left;\">J.D. (Jerry) Rhodes 1997</td></tr> \
</tbody></table> \
</body></html> \
           ";
    return html;
}

/**
 * Get answer value in string format.
 * Quick access function for getHtml().
 * @param questNo question number
 * @param answerNo answer number
 * @return answer value in string format
 */
RB_String SCAN_PrepareJrResult::get(int questionNo, int answerNo) {
    if (!mQResultList) return RB_String("Error");
    RB_ObjectIterator* qIter = mQResultList->createIterator();

    for (qIter->first(); !qIter->isDone(); qIter->next()) {
        RB_ObjectBase* qResult = qIter->currentObject();
        int qNumber = qResult->getValue("number").toInt();

        if (qNumber == questionNo) {
            RB_ObjectContainer* aResultList = qResult->getContainer("SCAN_AnswerResultList");
            RB_ObjectIterator* aIter = aResultList->createIterator();

            for (aIter->first(); !aIter->isDone(); aIter->next()) {
                RB_ObjectBase* aResult = aIter->currentObject();
                int aNumber = aResult->getValue("number").toInt();

                if (aNumber == answerNo) {
                    delete qIter;
                    delete aIter;
                    return aResult->getValue("mvalue").toString();
                }
            }

            delete aIter;
        }
    }

    delete qIter;
    return RB_String("E");
}

/**
 * Get Mindcom result value in string format.
 * Quick access function for getHtml().
 * @param mcType Mindcom result type
 * @return result value in string format
 */
RB_String SCAN_PrepareJrResult::getRS(int mcType) {
    return RB_String::number(getResult(mcType));
}

/**
 * @returns list of available graphs
 */
RB_StringList SCAN_PrepareJrResult::getContentTypeList() {
    // Note list order should be the same as enumerator
    RB_StringList gl;
    gl << tr("Text only") << tr("Text value") << tr("Chart");
    return gl;
}

/**
 * @returns list of available scan metrics
 */
RB_StringList SCAN_PrepareJrResult::getValidityList() {
    if (mValidityList.count() < 1) {
        mValidityList << "None"
            << "JrTotal01" << "JrTotal02" << "JrTotal03" << "JrTotal04"
            << "JrTotal05" << "JrTotal06" << "JrTotal07" << "JrTotal08"
            << "JrTotal09" << "JrTotal10" << "JrTotal11" << "JrTotal12"
            << "JrTotal13" << "JrTotal14" << "JrTotal15" << "JrTotal16"
            << "JrTotal17" << "JrTotal18" << "JrTotal19" << "JrTotal20"
            << "JrTotal21" << "JrPerc01" << "JrPerc02" << "JrPerc03"
            << "JrPerc04" << "JrPerc05" << "JrPerc06" << "JrPerc07"
            << "JrPerc08" << "JrPerc09" << "JrPerc10" << "JrPerc11"
            << "JrPerc12" << "JrPerc13" << "JrPerc14" << "JrPerc15"
            << "JrPerc16" << "JrPerc17" << "JrPerc18" << "JrPerc19"
            << "JrPerc20" << "JrPerc21" << "JrColorHb" << "JrColorSb"
            << "JrColorHr" << "JrColorSr" << "JrColorHg" << "JrColorSg"
            << "JrColorBlue" << "JrColorRed" << "JrColorGreen";
    }
    return mValidityList;
}
