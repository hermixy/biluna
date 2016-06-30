#include "pcalc_report.h"
#include "pcalc_inputoutput.h"
#include "pcalc_utilityfactory.h"
#include "rb_objectatomic.h"
NAMESPACE_BILUNA_CALC

PCALC_Report* PCALC_Report::mActiveUtility = 0;

PCALC_Report::PCALC_Report() : RB_Utility() {
    RB_DEBUG->print("PCALC_Report::PCALC_Report()");
    mInOutContainer = NULL;
    mInContainer = NULL;
    mOutContainer = NULL;
    mLastOutput = NULL;
    mReportType = -1;
    mFormulaFrom = 0;
    mFormulaTo = 999;
    createInputOutputObject();
    PCALC_UTILITYFACTORY->addUtility(this);
}

void PCALC_Report::createInputOutputObject() {
    if (mInOutContainer) {
        delete mInOutContainer;
    }
    mInOutContainer = new PCALC_InputOutput();
    mInContainer = mInOutContainer->getContainer("PCALC_InputList");
    mOutContainer = mInOutContainer->getContainer("PCALC_OutputList");
    mLastOutput = NULL;
    mReportType = -1;
    mFormulaFrom = 0;
    mFormulaTo = 999;
}

PCALC_Report::~PCALC_Report() {
    PCALC_UTILITYFACTORY->removeUtility(this);
    delete mInOutContainer;
    mInOutContainer  = NULL;
    mInContainer = NULL;
    mOutContainer = NULL;
    mLastOutput = NULL;
    mActiveUtility = NULL;
    RB_DEBUG->print("PCALC_Report::~PCALC_Report()");
}

PCALC_Report* PCALC_Report::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new PCALC_Report();
        mActiveUtility->refresh();
    }

    return mActiveUtility;
}

void PCALC_Report::setReportSettings(int reportType, int formulaFrom,
                                     int formulaTo) {
    mReportType = reportType;
    mFormulaFrom = formulaFrom;
    mFormulaTo = formulaTo;
}


void PCALC_Report::addDetail(const QString& formulaNumber,
                       const QString& variableName,
                       const QString& formula,
                       double result,
                       const QString& unit,
                       const QString& formulaValues,
                       int loadCaseNo,
                       const QString& note) {

    if (!mInContainer || !mOutContainer) {
        RB_DEBUG->error("PCALC_Report::addDetail() input or output container ERROR");
        return;
    }

    // Report filter settings, based on index of cbCalculationReportType
    // in pcalc_en1591widget.cpp, summary-, detail-, last iteration report
    if (mReportType == 0 || mReportType == 1 || mReportType == 3) {
        // only last iteration result
        RB_ObjectBase* existObj = getObject(mOutContainer, formulaNumber,
                                            variableName, loadCaseNo);
        if (existObj) {
            existObj->setValue("formulanumber", formulaNumber);
            existObj->setValue("variablename", variableName);
            existObj->setValue("formula", formula);
            existObj->setValue("result", result);
            existObj->setValue("unit", unit);
            existObj->setValue("formulavalues", formulaValues);
            existObj->setValue("loadcaseno", loadCaseNo);
            existObj->setValue("note", note);
            return;
        }
    }

    QStringList formulaStrList = formulaNumber.split(" ");
    if (formulaStrList.size() > 1) {
        QString formulaStr = formulaStrList.at(1);
        formulaStr = formulaStr.remove(","); // remove comma if any
        int formulaNo = formulaStr.toInt();

        if ((mReportType == 2 || mReportType == 3)
                && ((formulaNo < mFormulaFrom && mFormulaFrom > 0)
                    || (mFormulaTo < formulaNo && mFormulaTo < 999))) {
            return;
        }
    } else {
        RB_DEBUG->error("PCALC_Report::addDetail() formula number ERROR");
    }

    // Add result to output object and container
    bool withoutMembers = true;
    RB_ObjectAtomic* obj = new RB_ObjectAtomic("", mOutContainer,
                                               "PCALC_Output", NULL,
                                               withoutMembers);
    obj->addMember("formulanumber", "-", formulaNumber,
                   RB2::MemberChar125);
    obj->addMember("variablename", "-", variableName,
                   RB2::MemberChar125);
    obj->addMember("formula", "-", formula,
                   RB2::MemberChar2500);
    obj->addMember("result", unit, result,
                   RB2::MemberDouble);
    obj->addMember("unit", "-", unit,
                   RB2::MemberChar125);
    obj->addMember("formulavalues", "-", formulaValues,
                   RB2::MemberChar2500);
    obj->addMember("loadcaseno", "-", loadCaseNo,
                   RB2::MemberInteger);
    obj->addMember("note", "-", note,
                   RB2::MemberChar165);
    mOutContainer->addObject(obj);
    mLastOutput = obj;
}

RB_ObjectContainer* PCALC_Report::getInOutContainer() {
    return mInOutContainer;
}

/**
 * @brief PCALC_Report::getLastOutput
 * Some methods produce more than one output. The output enables to step
 * back in the list of outputs and test the result.
 * Used for unit test and calculation output.
 * @param reversePosition number of position back in formula output
 * @return last or number before last output object
 */
RB_ObjectBase* PCALC_Report::getLastOutput(int reversePosition) {
    if (reversePosition < 0) {
        RB_DEBUG->error("PCALC_Report::getLastOutput() 1 reversePosition ERROR");
        return nullptr;
    }

    if (reversePosition > 0) {
        int count = mOutContainer->objectCount();
        int row = count - reversePosition - 1;

        if (row < count && row >= 0) {
            RB_ObjectBase* output = mOutContainer->getObject(row);
            return output;
        } else {
            RB_DEBUG->error("PCALC_Report::getLastOutput() 2 reversePosition ERROR");
            return nullptr;
        }
    }

    if (!mLastOutput) {
        // Add new output object for unit test of functions without output
        bool withoutMembers = true;
        RB_ObjectAtomic* obj = new RB_ObjectAtomic("", mOutContainer,
                                                   "PCALC_Output", NULL,
                                                   withoutMembers);
        obj->addMember("formulanumber", "-", "", RB2::MemberChar125);
        obj->addMember("variablename", "-", "", RB2::MemberChar125);
        obj->addMember("formula", "-", "", RB2::MemberChar2500);
        obj->addMember("result", "-", 0.0, RB2::MemberDouble);
        obj->addMember("unit", "-", "", RB2::MemberChar125);
        obj->addMember("formulavalues", "-", "", RB2::MemberChar2500);
        obj->addMember("loadcaseno", "-", -1, RB2::MemberInteger);
        obj->addMember("note", "-", "", RB2::MemberChar165);
        mOutContainer->addObject(obj);
        mLastOutput = obj;
    }

    RB_ObjectBase* output = mLastOutput;
    mLastOutput = NULL;
    return output;
}

void PCALC_Report::clear() {
    createInputOutputObject();
}

RB_ObjectBase* PCALC_Report::getObject(RB_ObjectContainer* outContainer,
                                       const QString& formulaNumber,
                                       const QString& variableName,
                                       int loadCaseNo) {
    RB_ObjectIterator* iter = outContainer->createIterator();

    for (iter->first(); !iter->isDone(); iter->next()) {
        RB_ObjectBase* obj = iter->currentObject();

        if (obj->getValue("formulanumber").toString() == formulaNumber
                && obj->getValue("variablename").toString() == variableName
                && obj->getValue("loadcaseno").toInt() == loadCaseNo) {
            return obj;
        }
    }

    return NULL;
}

END_NAMESPACE_BILUNA_CALC
