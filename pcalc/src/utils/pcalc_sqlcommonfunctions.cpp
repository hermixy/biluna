/*****************************************************************
 * $Id: pcalc_sqlcommonfunctions.cpp 2232 2015-04-23 16:04:08Z rutger $
 * Created: Mar 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Plant Engineering (PENG) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "pcalc_sqlcommonfunctions.h"

#include "pcalc_dialogfactory.h"
#include "pcalc_modelfactory.h"
#include "pcalc_objectfactory.h"
#include "pcalc_utilityfactory.h"
#include "rb_settings.h"

PCALC_SqlCommonFunctions* PCALC_SqlCommonFunctions::mActiveUtility = 0;

/**
 * Constructor
 */
PCALC_SqlCommonFunctions::PCALC_SqlCommonFunctions() {
    RB_DEBUG->print("PCALC_SqlCommonFunctions::PCALC_SqlCommonFunctions()");
    PCALC_UTILITYFACTORY->addUtility(this);
}

/**
 * Destructor
 */
PCALC_SqlCommonFunctions::~PCALC_SqlCommonFunctions() {
    PCALC_UTILITYFACTORY->removeUtility(this);
    mActiveUtility = NULL;
    RB_DEBUG->print("PCALC_SqlCommonFunctions::~PCALC_SqlCommonFunctions() OK");
}

PCALC_SqlCommonFunctions *PCALC_SqlCommonFunctions::getInstance() {
    if (!mActiveUtility) {
        mActiveUtility = new PCALC_SqlCommonFunctions();
    }

    return mActiveUtility;
}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE 'whereStr'
 * @param field field name
 * @param table table name
 * @param whereStr where string
 * @return variant result value
 */
RB_Variant PCALC_SqlCommonFunctions::selectFromWhere(const RB_String& field,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    if (!PCALC_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PCALC_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE " + whereStr + ";";
    QSqlQuery query(PCALC_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->error("PCALC_SqlCommonFunctionsFunction::selectFromWhereId() "
                        + query.lastError().text() + " ERROR");
    }

    return RB_Variant();
}

