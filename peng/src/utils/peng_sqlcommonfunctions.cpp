/*****************************************************************
 * $Id: peng_sqlcommonfunctions.cpp 2232 2015-04-23 16:04:08Z rutger $
 * Created: Mar 19, 2012 8:14:42 PM - rutger
 *
 * Copyright (C) 2012 Red-Bag. All rights reserved.
 * This file is part of the Plant Engineering (PENG) project.
 *
 * See http://www.red-bag.com for further details.
 *****************************************************************/

#include "peng_sqlcommonfunctions.h"

#include "peng_dialogfactory.h"
#include "peng_modelfactory.h"
#include "peng_objectfactory.h"
#include "rb_settings.h"

/**
 * Constructor
 */
PENG_SqlCommonFunctions::PENG_SqlCommonFunctions() {

}

/**
 * Destructor
 */
PENG_SqlCommonFunctions::~PENG_SqlCommonFunctions() {

}

/**
 * Get PED equipment list, used for reporting
 * @param query query to be set
 * @param eqIdList ID's of selected equipment
 */
void PENG_SqlCommonFunctions::getPedEqList(QSqlQuery& query,
                                           const QStringList& eqIdList) {
    RB_String qStr;
    qStr = "SELECT peng_equipment.number as number, "
                "peng_type.typename as typename, "
                "peng_equipment.eqname as eqname, "
                "peng_equipment.designtemp as designtemp, "
                "peng_equipment.designpress as designpress, "
                "peng_equipment.volume as volume, "
                "peng_pedclassif.pedtype_id as pedtype_id, "
                "peng_pedclassif.assessmentby_id as assessment, "
                "peng_pedclassif.fluidstate_id as fluidstate_id, "
                "peng_pedclassif.fluidgroup_id as fluidgroup_id, "
                "peng_pedclassif.additionalinfo_id as additionalinfo_id, "
                "peng_pedclassif.unstablegas as unstablegas, "
                "peng_pedclassif.fluid1 as fluid1, "
                "peng_pedclassif.fluid2 as fluid2, "
                "peng_pedclassif.fluid3 as fluid3, "
                "peng_pedclassif.fluid4 as fluid4 "
            "FROM peng_equipment "
            "INNER JOIN peng_type ON ";
    this->substrIdxId("peng_equipment.type_idx", qStr);
    qStr += "=peng_type.id "
            "INNER JOIN peng_pedclassif ON peng_equipment.id=peng_pedclassif.parent";

    if (eqIdList.isEmpty()) {
        qStr += " WHERE peng_equipment.parent='";
        qStr += PENG_MODELFACTORY->getRootId() + "' ORDER BY number ASC;";
    } else {
        qStr += " WHERE";

        RB_String id = "";
        int count = eqIdList.size();
        bool isFirst = true;

        for (int i = 0; i < count; ++i) {
            id = eqIdList.at(i);

            if (isFirst) {
                qStr += " peng_equipment.id='" + id + "'";
            } else {
                qStr += " OR peng_equipment.id='" + id + "'";
            }

            if (isFirst){
                isFirst = false;
            }
        }

        qStr += " ORDER BY number ASC;";
    }

    if (!query.exec(qStr)) {
        RB_DEBUG->error("PENG_SqlCommonFunctions::getPedEqList() "
                        + query.lastError().text() + " ERROR");
//        TODO:only removed for testing
//        PENG_DIALOGFACTORY->commandMessage("PENG_SqlCommonFunctions::getPedEqList() "
//                                          + query.lastError().text());
    }
}

/**
 * Get PED line list, used for reporting
 * @param query query to be set
 * @param lineIdList ID's of selected pipe lines
 */
void PENG_SqlCommonFunctions::getPedLineList(QSqlQuery& query,
                                             const QStringList& lineIdList) {
    RB_String qStr;
    qStr = "SELECT peng_line.number as number, "
                "peng_line.designtemp as designtemp, "
                "peng_line.designpress as designpress, "
                "peng_line.sizedn as sizedn, "
                "peng_pedclassif.pedtype_id as pedtype_id, "
                "peng_pedclassif.assessmentby_id as assessment, "
                "peng_pedclassif.fluidstate_id as fluidstate_id, "
                "peng_pedclassif.fluidgroup_id as fluidgroup_id, "
                "peng_pedclassif.additionalinfo_id as additionalinfo_id, "
                "peng_pedclassif.unstablegas as unstablegas, "
                "peng_pedclassif.fluid1 as fluid1, "
                "peng_pedclassif.fluid2 as fluid2, "
                "peng_pedclassif.fluid3 as fluid3, "
                "peng_pedclassif.fluid4 as fluid4 "
            "FROM peng_line "
            "INNER JOIN peng_pedclassif ON peng_line.id=peng_pedclassif.parent";

    if (lineIdList.isEmpty()) {
        qStr += " WHERE peng_line.parent='";
        qStr += PENG_MODELFACTORY->getRootId() + "' ORDER BY number ASC;";
    } else {
        qStr += " WHERE";

        RB_String id = "";
        int count = lineIdList.size();
        bool isFirst = true;

        for (int i = 0; i < count; ++i) {
            id = lineIdList.at(i);

            if (isFirst) {
                qStr += " peng_line.id='" + id + "'";
            } else {
                qStr += " OR peng_line.id='" + id + "'";
            }

            if (isFirst){
                isFirst = false;
            }
        }

        qStr += " ORDER BY number ASC;";
    }

    if (!query.exec(qStr)) {
        RB_DEBUG->error("PENG_SqlCommonFunctions::getPedLineList() "
                        + query.lastError().text() + " ERROR");
//        TODO:only removed for testing
//        PENG_DIALOGFACTORY->commandMessage("PENG_SqlCommonFunctions::getPedLineList() "
//                                          + query.lastError().text());
    }
}

/**
 * Check if default rows are set when opening this project.
 * Used in PENG_ActionSelectProject
 * @returns true if default rows are set for this project
 */
bool PENG_SqlCommonFunctions::isDefaultRow() {
    if (!PENG_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PENG_SqlCommonFunctions::isDefaultRow()"
                        " database not open ERROR");
        return false;
    }

    RB_String qStr = "";
    qStr = "SELECT id FROM `peng_type` WHERE `parent`";
    qStr += " = '" + PENG_MODELFACTORY->getRootId() + "' LIMIT 0, 2;";

    QSqlQuery query(PENG_MODELFACTORY->getDatabase());

    if (!query.exec(qStr)) {
        RB_DEBUG->error("PENG_SqlCommonFunctionsFunction::isDefaultRow() "
                        + query.lastError().text() + " ERROR");
        return false;
    }

    if (!query.first()) {
        // a record exists
        return false;
    }

    return true;
}

/**
 * Simple query execution SELECT 'field' FROM 'table' WHERE 'whereStr'
 * @param field field name
 * @param table table name
 * @param whereStr where string
 * @return variant result value
 */
RB_Variant PENG_SqlCommonFunctions::selectFromWhere(const RB_String& field,
                                                   const RB_String& table,
                                                   const RB_String& whereStr) {
    if (!PENG_MODELFACTORY->getDatabase().isOpen()) {
        RB_DEBUG->print(RB_Debug::D_ERROR,
                        "PENG_SqlCommonFunctions::selectFromWhereId()"
                        " database not open ERROR");
        return -1;
    }

    RB_String qStr = "";
    qStr = "SELECT " + field + " FROM " + table.toLower() + " WHERE " + whereStr + ";";
    QSqlQuery query(PENG_MODELFACTORY->getDatabase());

    if (query.exec(qStr)) {
        if (!query.first()) return RB_Variant();
        return query.value(0);
    } else {
        RB_DEBUG->error("PENG_SqlCommonFunctionsFunction::selectFromWhereId() "
                        + query.lastError().text() + " ERROR");
    }

    return RB_Variant();
}

